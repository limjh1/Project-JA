// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/JAGameplayAbility.h"
#include "JATypes/JAEnumTypes.h"
#include "HeroGameplayAbility_Equipments.generated.h"

class AJAEquipmentBase;

/**
 * 
 */
UCLASS()
class JA_API UHeroGameplayAbility_Equipments : public UJAGameplayAbility
{
	GENERATED_BODY()
	
public:
	void SetArmourType(EJAEquipmentType InEquipmentType) { EquipmentType = InEquipmentType; }

protected:
	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	EJAEquipmentType EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	AJAEquipmentBase* EquipmentObject;
};
