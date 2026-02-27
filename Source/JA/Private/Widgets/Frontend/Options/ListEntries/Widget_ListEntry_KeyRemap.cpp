// JhLim All Rights Reserved


#include "Widgets/Frontend/Options/ListEntries/Widget_ListEntry_KeyRemap.h"
#include "Widgets/Frontend/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "Widgets/Frontend/Components/JAFrontendCommonButtonBase.h"
#include "Subsystems/JAFrontendUISubsystem.h"
#include "JAGameplayTags.h"
#include "JAFunctionLibrary.h"
#include "Widgets/Frontend/Options/Widget_KeyRemapScreen.h"

#include "JADebugHelper.h"

void UWidget_ListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_RemapKey->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningKeyRemapDataObject = CastChecked<UListDataObject_KeyRemap>(InOwningListDataObject);

	CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionListDataModifyReason ModifyReason)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
	}
}

void UWidget_ListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();

	UJAFrontendUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		JAGameplayTags::Frontend_WidgetStack_Modal,
		UJAFunctionLibrary::GetFrontendSoftWidgetClassByTag(JAGameplayTags::Frontend_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState, UWidget_ActivatableBase* PushedWidget)
		{
			if (PushState == EAsyncPushWidgetState::OnCraetedBeforePush)
			{
				UWidget_KeyRemapScreen* CreatedKeyRemapScreen =CastChecked<UWidget_KeyRemapScreen>(PushedWidget);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyRemapCanceled);

				if (CachedOwningKeyRemapDataObject)
				{
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(CachedOwningKeyRemapDataObject->GetDesiredInputKeyType());
				}
			}
		}
	);
}

void UWidget_ListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();

	if (nullptr == CachedOwningKeyRemapDataObject)
	{
		return;
	}

	// 이미 기본값인경우
	if (false == CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UJAFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAysnc(
			EConfirmScreenType::Ok,
			FText::FromString(TEXT("Reset Key Mapping")),
			FText::FromString(TEXT("The Key binding for ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT(" 는 이미 기본값입니다.")),
			[](EConfirmScreenButtonType ClickedButton) {}
		);

		return;
	}

	UJAFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAysnc(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Reset Key Mapping")),
		FText::FromString(TEXT("정말 ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT(" 키를 초기화 하시겠습니까?")),
		[this](EConfirmScreenButtonType ClickedButton)
		{
			if (ClickedButton == EConfirmScreenButtonType::Confirmed)
			{
				CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
			}
		}
	);
}

void UWidget_ListEntry_KeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
	}
}

void UWidget_ListEntry_KeyRemap::OnKeyRemapCanceled(const FString& CanceledReason)
{
	UJAFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAysnc(
		EConfirmScreenType::Ok,
		FText::FromString(TEXT("Key Remap")),
		FText::FromString(CanceledReason),
		[](EConfirmScreenButtonType ClickedButton)
		{

		}
	);
}
