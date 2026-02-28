// JhLim All Rights Reserved


#include "JAGameInstance.h"
#include "MoviePlayer.h"

void UJAGameInstance::Init()
{
	Super::Init();

	// 임시 주석 처리. 이후 로딩 방향성 정해지면 수정 필요
	//// Loading Screen
	//FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreLoadMap);
	//FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnDestinationWorldLoaded);
}

TSoftObjectPtr<UWorld> UJAGameInstance::GetGameLevelByTag(FGameplayTag InTag) const
{
	for (const FJAGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if (!GameLevelSet.IsValid())
		{
			continue;
		}

		if (GameLevelSet.LevelTag == InTag)
		{
			return GameLevelSet.Level;
		}
	}

	return TSoftObjectPtr<UWorld>();
}

void UJAGameInstance::OnPreLoadMap(const FString& MapName)
{
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;

	// video, 다 가능 etc... 현재는 심플 로딩 스크린
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget(); 

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void UJAGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
	GetMoviePlayer()->StopMovie();
}
