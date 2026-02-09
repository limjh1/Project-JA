// JhLim All Rights Reserved


#include "JASettings/JAGameUserSettings.h"

UJAGameUserSettings::UJAGameUserSettings()
	: OverallVolume(1.f)
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
