// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "JAGameUserSettings.generated.h"

class USoundClass;
class USoundMix;

/**
 * 
 */
UCLASS(BlueprintType)
class JA_API UJAGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UJAGameUserSettings();

	static UJAGameUserSettings* Get();

	// Audio Init
	// 게임 인스턴스 초기화 시점에 단 한 번 호출하여 사운드 에셋을 캐싱
	void InitializeAudioResources();

	UFUNCTION(BlueprintCallable)
	void ApplyAudioSettings();

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

	// System Collection Tab
	UFUNCTION()
	FString GetCurrentLanguage() const;

	UFUNCTION()
	void SetCurrentLanguage(const FString& InNewLanguageCode);
	// System Collection Tab	

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

	// System Collection Tab
	UPROPERTY(Config)
	FString CurrentLanguageCode;
	// System Collection Tab

private:
	// 런타임 전용 캐싱
	UPROPERTY(Transient)
	TObjectPtr<USoundClass> CachedMasterSoundClass;

	UPROPERTY(Transient)
	TObjectPtr<USoundClass> CachedBGMSoundClass;

	UPROPERTY(Transient)
	TObjectPtr<USoundClass> CachedSFXSoundClass;

	UPROPERTY(Transient)
	TObjectPtr<USoundMix> CachedDefaultSoundMix;

	bool bIsAudioInitialized = false;
};
