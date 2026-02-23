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

	UFUNCTION()
	bool GetAllowBackgroundAudio() const { return bAllowBackgroundAudio; }

	UFUNCTION()
	void SetAllowBackgroundAudio(bool bIsAllowed);

	UFUNCTION()
	bool GetUseHDRAudioMode() const { return bUseHDRAudioMode; }

	UFUNCTION()
	void SetUseHDRAudioMode(bool bIsAllowed);
	// Audio Collection Tab

	// Video Collection Tab
	UFUNCTION()
	float GetCurrentDisplayGamma() const;

	UFUNCTION()
	void SetCurrentDisplayGamma(float InNewGamma);

	// Video Collection Tab

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

	UPROPERTY(Config)
	bool bAllowBackgroundAudio;

	UPROPERTY(Config)
	bool bUseHDRAudioMode;	
	// Audio Collection Tab
};
