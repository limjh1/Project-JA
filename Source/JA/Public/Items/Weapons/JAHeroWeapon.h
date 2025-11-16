// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/JAWeaponBase.h"
#include "JATypes/JAStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "JAHeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class JA_API AJAHeroWeapon : public AJAWeaponBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FJAHeroWeaponData HeroWeaponData;
	
private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
