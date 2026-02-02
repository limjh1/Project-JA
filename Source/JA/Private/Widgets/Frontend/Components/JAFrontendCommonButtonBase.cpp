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

	if (!ButtonDescriptionText.IsEmpty())
	{
		UJAFrontendUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
	}
}

void UJAFrontendCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	UJAFrontendUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
}
