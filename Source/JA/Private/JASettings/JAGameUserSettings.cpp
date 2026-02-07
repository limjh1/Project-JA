// JhLim All Rights Reserved


#include "JASettings/JAGameUserSettings.h"

UJAGameUserSettings* UJAGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UJAGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}