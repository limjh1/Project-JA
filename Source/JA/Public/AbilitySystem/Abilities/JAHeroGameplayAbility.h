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

private:
	TWeakObjectPtr<AJAHeroCharacter>	CachedJAHeroCharacter;
	TWeakObjectPtr<AJAHeroController>	CachedJAHeroController;
};
