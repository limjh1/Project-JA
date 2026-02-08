// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "JAFrontendCommonButtonBase.generated.h"

class UCommonTextBlock;
class UJAFrontendUISubsystem;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class JA_API UJAFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);

	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;

private:
	// ~Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	// ~End UUserWidget Interface

	// ~Begin UCommonButtonBase Interface
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	// ~End UCommonButtonBase Interface

	// Bound Widgets
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;
	// Bound Widgets

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JAFrontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JAFrontend Button", meta = (AllowPrivateAccess = "true"))
	bool bUseUpperCaseForButtonText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JAFrontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDescriptionText;

private:
	// dangling 방지를 위해 약참조
	TWeakObjectPtr<UJAFrontendUISubsystem> CachedFrontendSubsystem;

};
