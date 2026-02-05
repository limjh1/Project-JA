// JhLim All Rights Reserved


#include "Widgets/Frontend/Options/Widget_OptionsScreen.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"

#include "JADebugHelper.h"

void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction,
				true,
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
			)
		);
	}

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggerd)
		)
	);
}

void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	Debug::Print(TEXT("Reset Bound Action Tiggered"));
}

void UWidget_OptionsScreen::OnBackBoundActionTriggerd()
{
	DeactivateWidget();
}