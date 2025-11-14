// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "JAGameplayAbility.generated.h"

class UPawnCombatComponent;
class UJAAbilitySystemComponent;

UENUM(BlueprintType)
enum class EJAAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven,
};

/**
 * 
 */
UCLASS()
class JA_API UJAGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	UFUNCTION(BlueprintPure, Category = "JA|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "JA|Ability")
	UJAAbilitySystemComponent* GetJAAbilitySystemComponentFromActorInfo() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "JAAbility")
	EJAAbilityActivationPolicy AbilityActivationPolicy = EJAAbilityActivationPolicy::OnTriggered;

};
