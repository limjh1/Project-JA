// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"

class AJAFrontendPlayerController;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class JA_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure)
	AJAFrontendPlayerController* GetOwningJAFrontendPlayerController();

private:
	TWeakObjectPtr<AJAFrontendPlayerController> CachedOwningJAFrontendPC;

};
