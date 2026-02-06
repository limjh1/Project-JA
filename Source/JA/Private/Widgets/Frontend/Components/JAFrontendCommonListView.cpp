// JhLim All Rights Reserved


#include "Widgets/Frontend/Components/JAFrontendCommonListView.h"
#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Frontend/Options/DataAsset_DataListEntryMapping.h"
#include "Widgets/Frontend/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widgets/Frontend/Options/DataObjects/ListDataObject_Base.h"

UUserWidget& UJAFrontendCommonListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}

	if (TSubclassOf<UWidget_ListEntry_Base> FoundWidgetClass = DataListEntryMapping->FindEntryWidgetClassByDataObject(CastChecked<UListDataObject_Base>(Item)))
	{
		return GenerateTypedEntry<UWidget_ListEntry_Base>(FoundWidgetClass, OwnerTable);
	}

	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

#if WITH_EDITOR	
void UJAFrontendCommonListView::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!DataListEntryMapping)
	{
		CompileLog.Error(
				FText::FromString(TEXT("The variable DataListEntryMapping has no valid data asset assgine ") +
				GetClass()->GetName() +
				TEXT(" needs a valid data asset to function properly")
			));
	}
}
#endif
