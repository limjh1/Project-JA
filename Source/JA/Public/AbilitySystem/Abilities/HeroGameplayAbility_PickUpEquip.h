// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/JAHeroGameplayAbility.h"
#include "HeroGameplayAbility_PickUpEquip.generated.h"

class AActor;
class UObject;
class AJAPickUpEquipmentBase;

/**
 * 
 */
UCLASS()
class JA_API UHeroGameplayAbility_PickUpEquip : public UJAHeroGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

protected:
	UFUNCTION(BlueprintCallable)
	void InteractEquipment();

	UFUNCTION(BlueprintCallable)
	void CollectEquipments();

private:
	AActor* CachedActor;
	UObject* CachedObject;

private:
	UPROPERTY(EditDefaultsOnly)
	float BoxTraceDistance = 50.f;

	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxSize = FVector(100.f);

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> EquipmentTraceChannel;

	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebugSphere = false;

	UPROPERTY()
	TArray<AJAPickUpEquipmentBase*> CollectedEquipments;
};
