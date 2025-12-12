// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/JAGameplayAbility.h"
#include "JAEnemyGameplayAbility.generated.h"

class AJAEnemyCharacter;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class JA_API UJAEnemyGameplayAbility : public UJAGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "JA|Ability")
	AJAEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "JA|Ability")
	UEnemyCombatComponent* GetEnmeyCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "JA|Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat);

private:
	TWeakObjectPtr<AJAEnemyCharacter> CachedJAEnemyCharacter;
};
