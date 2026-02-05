// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Frontend/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Collection.generated.h"

/**
 * 
 */
UCLASS()
class JA_API UListDataObject_Collection : public UListDataObject_Base
{
	GENERATED_BODY()
	
public:
	//~ Begin UListDataObject_Base Interface
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const override;
	virtual bool HasAnyChildListData() const override;
	//~ End UListDataObject_Base Interface

public:
	void AddChildListData(UListDataObject_Base* InChildListData);

private:
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ChildListDataArray;
};
