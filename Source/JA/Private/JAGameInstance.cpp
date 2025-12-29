// JhLim All Rights Reserved


#include "JAGameInstance.h"

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
