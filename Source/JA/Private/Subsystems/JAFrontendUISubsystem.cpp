// JhLim All Rights Reserved


#include "Subsystems/JAFrontendUISubsystem.h"

#include "JADebugHelper.h"

UJAFrontendUISubsystem* UJAFrontendUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UJAFrontendUISubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UJAFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UJAFrontendUISubsystem::RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget)
{
	check(InCreatedWidget);

	CreatedPrimaryLayout = InCreatedWidget;

	Debug::Print(TEXT("Primary Layout Widget Stored"));
}
