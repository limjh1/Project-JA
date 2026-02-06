// JhLim All Rights Reserved


#include "Widgets/Frontend/Options/ListEntries/Widget_ListEntry_String.h"
#include "Widgets/Frontend/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Frontend/Components/JAFrontendCommonRotator.h"

void UWidget_ListEntry_String::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UListDataObject_String>(InOwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
}
