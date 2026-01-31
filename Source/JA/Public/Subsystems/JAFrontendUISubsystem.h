// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "JAFrontendUISubsystem.generated.h"

class UWidget_PrimaryLayout;

/**
 * 
 */
UCLASS()
class JA_API UJAFrontendUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UJAFrontendUISubsystem* Get(const UObject* WorldContextObject);

	//~ Begin USubSystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End USubSystem Interface

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget);
	

private:
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayout;

};
