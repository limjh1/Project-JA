// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "JAGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FJAGameLevelSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Category = "GameData.Level"))
	FGameplayTag LevelTag;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	bool IsValid() const 
	{
		return (LevelTag.IsValid() && !Level.IsNull());
	}
};

/**
 * 
 */
UCLASS()
class JA_API UJAGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

public:
	UFUNCTION(BlueprintPure, meta = (GameplayTagFilter = "GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag InTag) const;

protected:
	virtual void OnPreLoadMap(const FString& MapName);
	virtual void OnDestinationWorldLoaded(UWorld* LoadedWorld);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FJAGameLevelSet> GameLevelSets;

};
