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
	UJAGameUserSettings();

	static UJAGameUserSettings* Get();

	// Gameplay Collection Tab
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; }
	// Gameplay Collection Tab

	// Audio Collection Tab
	UFUNCTION()
	float GetOverallVolume()const { return OverallVolume; }

	UFUNCTION()
	void SetOverallVolume(float InVolume);

	UFUNCTION()
	float GetBGMVolume()const { return BGMVolume; }

	UFUNCTION()
	void SetBGMVolume(float InVolume);

	UFUNCTION()
	float GetSoundFXVolume()const { return SoundFXVolume; }

	UFUNCTION()
	void SetSoundFXVolume(float InVolume);
	// Audio Collection Tab

private:
	// Gameplay Collection Tab
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	// Gameplay Collection Tab

	// Audio Collection Tab
	UPROPERTY(Config)
	float OverallVolume;

	UPROPERTY(Config)
	float BGMVolume;

	UPROPERTY(Config)
	float SoundFXVolume;
	// Audio Collection Tab
};
