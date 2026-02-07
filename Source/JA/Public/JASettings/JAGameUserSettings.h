// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "JAGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class JA_API UJAGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	static UJAGameUserSettings* Get();

	// Gameplay Collection Tab
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; }
	// Gameplay Collection Tab

private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
};
