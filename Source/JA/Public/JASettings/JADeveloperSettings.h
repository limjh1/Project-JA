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

};
