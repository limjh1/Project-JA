// JhLim All Rights Reserved


#include "Subsystems/JALoadingScreenSubsystem.h"
#include "PreLoadScreenManager.h"
#include "JASettings/JALoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/JALoadingScreenInterface.h"

#include "JADebugHelper.h"

bool UJALoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UJALoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
}

void UJALoadingScreenSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

UWorld* UJALoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGameInstance = GetGameInstance())
	{
		return OwningGameInstance->GetWorld();
	}

	return nullptr;
}

void UJALoadingScreenSubsystem::Tick(float DeltaTime)
{
	TryUpdateLoadingScreen();
}

ETickableTickType UJALoadingScreenSubsystem::GetTickableTickType() const
{
	if (IsTemplate())
	{
		return ETickableTickType::Never;
	}

	return ETickableTickType::Conditional;
}

bool UJALoadingScreenSubsystem::IsTickable() const
{
	return (GetGameInstance() && GetGameInstance()->GetGameViewportClient());
}

TStatId UJALoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UJALoadingScreenSubsystem, STATGROUP_Tickables);
}

void UJALoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance())
	{
		return;
	}

	SetTickableTickType(ETickableTickType::Conditional);

	bIsCurrentlyLoadingMap = true;

	TryUpdateLoadingScreen();
}

void UJALoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
	{
		bIsCurrentlyLoadingMap = false;
	}
}

void UJALoadingScreenSubsystem::TryUpdateLoadingScreen()
{
	if (IsPreLoadScreenActive())
	{
		return;
	}
	
	if (ShouldShowLoadingScreen())
	{
		TryDisplayLoadingScreenIfNone();

		OnLoadingReasonUpdated.Broadcast(CurrentLoadingReason);
	}
	else
	{
		TryRemoveLoadingScreen();

		HoldLoadingScreenStartUpTime = -1.f;

		// Notify Loading is Complete
		NotifyLoadingScreenVisibilityChanged(false);

		// Disable
		SetTickableTickType(ETickableTickType::Never);
	}
}

bool UJALoadingScreenSubsystem::IsPreLoadScreenActive() const
{
	if (FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get())
	{
		return PreLoadScreenManager->HasValidActivePreLoadScreen();
	}

	return false;
}

bool UJALoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const UJALoadingScreenSettings* LoadingScreenSettings = GetDefault<UJALoadingScreenSettings>();

	if (GIsEditor && !LoadingScreenSettings->bShouldLoadingScreenInEditor)
	{
		return false;
	}

	// Check if the objects in the world need a loading screen
	if (CheckTheNeedToShowLoadingScreen())
	{
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;

		return true;
	}

	CurrentLoadingReason = TEXT("Waiting for Texture Streaming");

	// There is no need to show the loading screen. allow the world to be rendered to out viewport here
	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;

	const float CurrentTime = FPlatformTime::Seconds();

	if (HoldLoadingScreenStartUpTime < 0.f)
	{
		HoldLoadingScreenStartUpTime = CurrentTime;
	}

	const float ElapsedTime = (CurrentTime - HoldLoadingScreenStartUpTime);

	if (ElapsedTime < LoadingScreenSettings->HoldLoadingScreenExtraSeconds)
	{
		return true;
	}

	return false;
}

bool UJALoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
{
	if (bIsCurrentlyLoadingMap)
	{
		CurrentLoadingReason = TEXT("Loading Level");
		return true;
	}

	UWorld* OwningWorld = GetGameInstance()->GetWorld();
	if (!OwningWorld)
	{
		CurrentLoadingReason = TEXT("Initializing World");
		return true;
	}

	if (!OwningWorld->HasBegunPlay())
	{
		CurrentLoadingReason = TEXT("World hasn't begun play yet");
		return true;
	}

	if (!OwningWorld->GetFirstPlayerController())
	{
		CurrentLoadingReason = TEXT("Player Controller is not valid yet");
		return true;
	}

	// #TODO
	// Check if the game states, player states, or player character, actor component are ready

	return false;
}

void UJALoadingScreenSubsystem::TryDisplayLoadingScreenIfNone()
{
	// 이미 있다면 X
	if (CachedCreatedLoadingScreenWidget)
	{
		return;
	}

	const UJALoadingScreenSettings* LoadingScreenSettings = GetDefault<UJALoadingScreenSettings>();

	TSubclassOf<UUserWidget> LoadedWidgetClass = LoadingScreenSettings->GetLoadingScreenWidgetClassChecked();

	UUserWidget* CreatedWidget = UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadedWidgetClass, NAME_None);

	check(CreatedWidget);

	CachedCreatedLoadingScreenWidget = CreatedWidget->TakeWidget();

	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(
		CachedCreatedLoadingScreenWidget.ToSharedRef(),
		1000 // z order
	);

	NotifyLoadingScreenVisibilityChanged(true);
}

void UJALoadingScreenSubsystem::TryRemoveLoadingScreen()
{
	if (!CachedCreatedLoadingScreenWidget)
	{
		return;
	}

	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCreatedLoadingScreenWidget.ToSharedRef());

	CachedCreatedLoadingScreenWidget.Reset(); // Free Memory Alloc
}

void UJALoadingScreenSubsystem::NotifyLoadingScreenVisibilityChanged(bool bIsVisible)
{
	for (ULocalPlayer* ExistingLocalPlayer : GetGameInstance()->GetLocalPlayers())
	{
		if (!ExistingLocalPlayer)
		{
			continue;
		}

		if (APlayerController* PC = ExistingLocalPlayer->GetPlayerController(GetGameInstance()->GetWorld()))
		{
			// Query if the player controller implements the interface. Call the function through interface to notify the loading status if yes
			if (PC->Implements<UJALoadingScreenInterface>())
			{
				if (bIsVisible)
				{
					IJALoadingScreenInterface::Execute_OnLoadingScreenActivated(PC);
				}
				else
				{
					IJALoadingScreenInterface::Execute_OnLoadingScreenDeactivated(PC);
				}				
			}

			if (APawn* OwningPawn = PC->GetPawn())
			{
				if (OwningPawn->Implements<UJALoadingScreenInterface>())
				{
					if (bIsVisible)
					{
						IJALoadingScreenInterface::Execute_OnLoadingScreenActivated(OwningPawn);
					}
					else
					{
						IJALoadingScreenInterface::Execute_OnLoadingScreenDeactivated(OwningPawn);
					}
				}
			}

			// #TODO The code for notifing other objects in the world goes here
		}
	}
}
