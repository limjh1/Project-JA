// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Frontend/Widget_ActivatableBase.h"
#include "JATypes/JAEnumTypes.h"
#include "Widget_OptionsScreen.generated.h"

class UOptionsDataRegistry;
class UJAFrontendTabListWidgetBase;
class UJAFrontendCommonListView;
class UWidget_OptionsDetailsView;
class UListDataObject_Base;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class JA_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	//~ Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface

private:
	UOptionsDataRegistry* GetOrCreateDataRegistry();

	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggerd();

	UFUNCTION()
	void OnOptionsTabSelected(FName TabId);

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);
	void OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionListDataModifyReason ModifyReason);

	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;

private:
	// ~Bound Widgets
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UJAFrontendTabListWidgetBase> TabListWidget_OptionsTabs;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UJAFrontendCommonListView> CommonListView_OptionsList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget_OptionsDetailsView> DetailsView_ListEntryInfo;
	// ~Bound Widgets

private:
	// Handle the creation of data in the options screen. Direct access to this variable is forbidden.
	UPROPERTY(Transient)
	TObjectPtr<UOptionsDataRegistry> CreatedOwningDataRegistry;

	UPROPERTY(EditDefaultsOnly, Category = "FrontendOptionsScreen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UListDataObject_Base>> ResettableDataArray;

	bool bIsResettingData = false;
};
