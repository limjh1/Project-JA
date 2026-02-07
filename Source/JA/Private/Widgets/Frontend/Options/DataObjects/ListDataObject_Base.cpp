// JhLim All Rights Reserved


#include "Widgets/Frontend/Options/DataObjects/ListDataObject_Base.h"
#include "JASettings/JAGameUserSettings.h"


void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObject_Base::OnDataObjectInitialized()
{
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData, EOptionListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);

	if (bShouldApplyChangeImmedialty)
	{
		UJAGameUserSettings::Get()->ApplySettings(true);
	}
}
