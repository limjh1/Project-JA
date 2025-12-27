// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/JAGameplayAbility.h"
#include "JAHeroGameplayAbility.generated.h"

class AJAHeroCharacter;
class AJAHeroController;
class UHeroCombatComponent;

/**
 * 
 */
UCLASS()
class JA_API UJAHeroGameplayAbility : public UJAGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "JA|Ability")
	AJAHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "JA|Ability")
	AJAHeroController* GetHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "JA|Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "JA|Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);

	UFUNCTION(BlueprintCallable, Category = "JA|Ability")
	bool GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime);

private:
	TWeakObjectPtr<AJAHeroCharacter>	CachedJAHeroCharacter;
	TWeakObjectPtr<AJAHeroController>	CachedJAHeroController;
};
