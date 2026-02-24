// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Frontend/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_KeyRemap.generated.h"

class UJAFrontendCommonButtonBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class JA_API UWidget_ListEntry_KeyRemap : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UJAFrontendCommonButtonBase* CommonButton_RemapKey;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UJAFrontendCommonButtonBase* CommonButton_ResetKeyBinding;
};
