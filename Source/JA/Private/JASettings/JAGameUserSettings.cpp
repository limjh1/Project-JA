// JhLim All Rights Reserved


#include "JASettings/JAGameUserSettings.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "JASettings/JADeveloperSettings.h" 

UJAGameUserSettings::UJAGameUserSettings()
	: OverallVolume(1.f), BGMVolume(1.f), SoundFXVolume(1.f), bAllowBackgroundAudio(false), bUseHDRAudioMode(false)
{
}

UJAGameUserSettings* UJAGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UJAGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void UJAGameUserSettings::InitializeAudioResources()
{
	if (bIsAudioInitialized)
	{
		return;
	}

	const UJADeveloperSettings* DevSettings = GetDefault<UJADeveloperSettings>();
	if (!DevSettings)
	{
		return;
	}

	// 동기 로드는 게임 초기화 시점에 딱 한 번만 수행
	CachedMasterSoundClass = Cast<USoundClass>(DevSettings->MasterSoundClass.TryLoad());
	CachedBGMSoundClass = Cast<USoundClass>(DevSettings->MusicSoundClass.TryLoad());
	CachedSFXSoundClass = Cast<USoundClass>(DevSettings->SoundFXSoundClass.TryLoad());
	CachedDefaultSoundMix = Cast<USoundMix>(DevSettings->DefaultSoundMix.TryLoad());

	// 사운드 믹스를 전역(WorldContextObject 생략 가능)으로 Push 해둔다.
	if (CachedDefaultSoundMix)
	{
		UGameplayStatics::PushSoundMixModifier(this, CachedDefaultSoundMix);
	}

	bIsAudioInitialized = true;
}

void UJAGameUserSettings::ApplyAudioSettings()
{
	if (!bIsAudioInitialized)
	{
		InitializeAudioResources();
	}

	// 저장되어 있던 볼륨값으로 실제 사운드 믹스 오버라이드를 최초 1회 실행
	SetOverallVolume(OverallVolume);
	SetBGMVolume(BGMVolume);
	SetSoundFXVolume(SoundFXVolume);
}

void UJAGameUserSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;

	UWorld* CurrentWorld = nullptr;
	if (GEngine)
	{
		CurrentWorld = GEngine->GetCurrentPlayWorld();
	}

	if (!CurrentWorld)
	{
		return;
	}

	if (CachedDefaultSoundMix && CachedMasterSoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(
			CurrentWorld,
			CachedDefaultSoundMix,
			CachedMasterSoundClass,
			OverallVolume,
			1.f,   // Pitch 보정 여부
			0.1f,  // FadeIn Time (슬라이더 조작 시 틱틱거리는 팝 노이즈 방지)
			true   // bApplyToChildren (Master를 줄이면 BGM, SFX도 같이 줄어들게 함)
		);

		UGameplayStatics::PushSoundMixModifier(CurrentWorld, CachedDefaultSoundMix);
	}
}

void UJAGameUserSettings::SetBGMVolume(float InVolume)
{
	BGMVolume = InVolume;

	UWorld* CurrentWorld = nullptr;
	if (GEngine)
	{
		CurrentWorld = GEngine->GetCurrentPlayWorld();
	}

	if (!CurrentWorld)
	{
		return;
	}

	if (CachedDefaultSoundMix && CachedMasterSoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(
			CurrentWorld,
			CachedDefaultSoundMix,
			CachedBGMSoundClass,
			BGMVolume,
			1.f,
			0.1f,
			true
		);

		UGameplayStatics::PushSoundMixModifier(CurrentWorld, CachedDefaultSoundMix);
	}
}

void UJAGameUserSettings::SetSoundFXVolume(float InVolume)
{
	SoundFXVolume = InVolume;

	UWorld* CurrentWorld = nullptr;
	if (GEngine)
	{
		CurrentWorld = GEngine->GetCurrentPlayWorld();
	}

	if (!CurrentWorld)
	{
		return;
	}

	if (CachedDefaultSoundMix && CachedSFXSoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(
			CurrentWorld,
			CachedDefaultSoundMix,
			CachedSFXSoundClass,
			SoundFXVolume,
			1.f,
			0.1f,
			true
		);

		UGameplayStatics::PushSoundMixModifier(CurrentWorld, CachedDefaultSoundMix);
	}
}

void UJAGameUserSettings::SetAllowBackgroundAudio(bool bIsAllowed)
{
	bAllowBackgroundAudio = bIsAllowed;
}

void UJAGameUserSettings::SetUseHDRAudioMode(bool bIsAllowed)
{
	bUseHDRAudioMode = bIsAllowed;
}

float UJAGameUserSettings::GetCurrentDisplayGamma() const
{
	if (GEngine)
	{
		return GEngine->GetDisplayGamma();
	}

	return 0.0f;
}

void UJAGameUserSettings::SetCurrentDisplayGamma(float InNewGamma)
{
	if (GEngine)
	{
		GEngine->DisplayGamma = InNewGamma;
	}
}

FString UJAGameUserSettings::GetCurrentLanguage() const
{	
	const FCultureRef CurrentCulture = FInternationalization::Get().GetCurrentCulture();
	return CurrentCulture->GetName();
}

void UJAGameUserSettings::SetCurrentLanguage(const FString& InNewLanguageCode)
{
	FInternationalization::Get().SetCurrentCulture(InNewLanguageCode);

	//FString CurrentCulture = FInternationalization::Get().GetCurrentCulture()->GetName();
	//UE_LOG(LogTemp, Warning, TEXT("Current Culture is set to: %s"), *CurrentCulture);
}