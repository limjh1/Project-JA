// JhLim All Rights Reserved


#include "Widgets/Frontend/Components/JAFrontendCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Subsystems/JAFrontendUISubsystem.h"

void UJAFrontendCommonButtonBase::SetButtonText(FText InText)
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUseUpperCaseForButtonText ? InText.ToUpper() : InText);
	}
}

FText UJAFrontendCommonButtonBase::GetButtonDisplayText() const
{
	if (CommonTextBlock_ButtonText)
	{
		return CommonTextBlock_ButtonText->GetText();
	}

	return FText();
}

void UJAFrontendCommonButtonBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CachedFrontendSubsystem = UJAFrontendUISubsystem::Get(this);
}

void UJAFrontendCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);
}

void UJAFrontendCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (CommonTextBlock_ButtonText && GetCurrentTextStyleClass())
	{
		CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UJAFrontendCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (CachedFrontendSubsystem.IsValid() && !ButtonDescriptionText.IsEmpty())
	{
		CachedFrontendSubsystem->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
	}
}

void UJAFrontendCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	if (CachedFrontendSubsystem.IsValid() && !ButtonDescriptionText.IsEmpty())
	{
		CachedFrontendSubsystem->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
	}
}
