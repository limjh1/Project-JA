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
	UJAFrontendUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		JAGameplayTags::Frontend_WidgetStack_Modal,
		UJAFunctionLibrary::GetFrontendSoftWidgetClassByTag(JAGameplayTags::Frontend_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState, UWidget_ActivatableBase* PushedWidget)
		{
			if (PushState == EAsyncPushWidgetState::OnCraetedBeforePush)
			{
				UWidget_KeyRemapScreen* CreatedKeyRemapScreen =CastChecked<UWidget_KeyRemapScreen>(PushedWidget);

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
	Debug::Print(TEXT("Reset Key Binding Button Clicked"));
}
