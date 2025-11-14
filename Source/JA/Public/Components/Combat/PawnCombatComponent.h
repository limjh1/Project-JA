// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AJAWeaponBase;
/**
 * 
 */
UCLASS()
class JA_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "JA|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AJAWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "JA|Combat")
	AJAWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UFUNCTION(BlueprintCallable, Category = "JA|Combat")
	AJAWeaponBase* GetCharacterCurrentEquippedWeapon() const;

private:
	TMap<FGameplayTag, AJAWeaponBase*> CharacterCarriedWeaponMap;

public:
	UPROPERTY(BlueprintReadWrite, Category = "JA|Combat")
	FGameplayTag CurrentEquippedWeaponTag;
	
};
