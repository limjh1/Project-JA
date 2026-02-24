// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "JAFrontendCommonButtonBase.generated.h"

class UCommonTextBlock;
class UJAFrontendUISubsystem;
class UCommonLazyImage;

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

	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImage(const FSlateBrush& InBrush);

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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonLazyImage* CommonLazyImage_ButtonImage;
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
