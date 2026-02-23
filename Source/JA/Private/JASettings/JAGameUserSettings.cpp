// JhLim All Rights Reserved


#include "JASettings/JAGameUserSettings.h"

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

void UJAGameUserSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;

	//the actual logic for controlling the volume goes here
}

void UJAGameUserSettings::SetBGMVolume(float InVolume)
{
	BGMVolume = InVolume;
}

void UJAGameUserSettings::SetSoundFXVolume(float InVolume)
{
	SoundFXVolume = InVolume;
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
