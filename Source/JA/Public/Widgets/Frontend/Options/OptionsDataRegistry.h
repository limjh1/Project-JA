// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OptionsDataRegistry.generated.h"

class UListDataObject_Collection;
class UListDataObject_Base;

/**
 * 
 */
UCLASS()
class JA_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()
	
public:
	// Gets called by options screen right after the object of type UOptionsDataRegistry is created
	void InitOptionsDataRegistry(ULocalPlayer* InOwiningLocalPlayer);

	const TArray<TObjectPtr<UListDataObject_Collection>>& GetRegisteredOptionsTabCollections() const { return RegisteredOptionsTabCollections; }

	TArray<UListDataObject_Base*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;

private:
	void FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const;

	void InitGameplayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab(ULocalPlayer* InOwiningLocalPlayer);
	void InitSystemCollectionTab();

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UListDataObject_Collection>> RegisteredOptionsTabCollections;

};
