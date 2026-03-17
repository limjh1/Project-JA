// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "JAFrontendCommonListView.generated.h"

class UDataAsset_DataListEntryMapping;

/**
 * 
 */
UCLASS()
class JA_API UJAFrontendCommonListView : public UCommonListView
{
	GENERATED_BODY()

protected:
	//~ Begin UCommonListView Interface
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;
	//~ End UCommonListView Interface

private:
	//~ Begin UWidget Interface
#if WITH_EDITOR	
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface

private:
	UPROPERTY(EditAnywhere, Category = "Frontend List View Settings")
	TObjectPtr<UDataAsset_DataListEntryMapping> DataListEntryMapping;
};
