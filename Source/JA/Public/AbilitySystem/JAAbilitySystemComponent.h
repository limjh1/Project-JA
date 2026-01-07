// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "JATypes/JAStructTypes.h"
#include "GameplayAbilitySpec.h"
#include "JAAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJAAbilityEndedSignature, FGameplayTag, AbilityTag);

/**
 * 
 */
UCLASS()
class JA_API UJAAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	// 엔진의 네이티브 OnAbilityEnded를 받을 내부 함수
	void OnInternalAbilityEnded(const FAbilityEndedData& AbilityEndedData);

	virtual void BeginPlay() override;
	
public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category = "JA|Ability", meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbilities(const TArray<FJAHeroAbilitySet>& InDefaultWeaponAbilities, const TArray<FJAHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "JA|Ability")
	void RemoveGrantedHeroWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "JA|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);

	UFUNCTION(BlueprintCallable, Category = "JA|Ability")
	bool TryCancelAbilityByTag(FGameplayTag AbilityTagToActivate);

public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Abilities")
	FJAAbilityEndedSignature OnJAAbilityEnded;

};
