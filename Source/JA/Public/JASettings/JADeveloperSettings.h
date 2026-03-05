// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "JADeveloperSettings.generated.h"

class UWidget_ActivatableBase;

/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "JAFrontend UI Settings"))
class JA_API UJADeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta = (ForceInlineRow, Categories = "Frontend.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UWidget_ActivatableBase>> JAFrontendWidgetMap;

	UPROPERTY(Config, EditAnywhere, Category = "Options Image Reference", meta = (ForceInlineRow, Categories = "Frontend.Image"))
	TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> OptionsScreenSoftImageMap;

public:
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath MasterSoundClass;

	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath MusicSoundClass;

	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath SoundFXSoundClass;

	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundMix"))
	FSoftObjectPath DefaultSoundMix;
};
