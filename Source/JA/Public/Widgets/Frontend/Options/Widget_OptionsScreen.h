// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Frontend/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class JA_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

private:
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggerd();

private:
	UPROPERTY(EditDefaultsOnly, Category = "FrontendOptionsScreen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;
};
