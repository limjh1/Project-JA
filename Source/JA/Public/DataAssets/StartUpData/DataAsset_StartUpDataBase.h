// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UJAGameplayAbility;
class UJAAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class JA_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UJAAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
		
protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UJAGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UJAGameplayAbility>> ReactiveAbilities;

protected:
	void GrantAbilities(const TArray<TSubclassOf<UJAGameplayAbility>>& InAbilitiesToGive, UJAAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
