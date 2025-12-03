// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "AttributeSet.h"
#include "JAAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class JA_API UJAAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UJAAttributeSet();

    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:

    // Health
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_CurrentHealth)
    FGameplayAttributeData CurrentHealth;
    ATTRIBUTE_ACCESSORS(UJAAttributeSet, CurrentHealth)

    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UJAAttributeSet, MaxHealth)

    // Rage
    UPROPERTY(BlueprintReadOnly, Category = "Rage", ReplicatedUsing = OnRep_CurrentRage)
    FGameplayAttributeData CurrentRage;
    ATTRIBUTE_ACCESSORS(UJAAttributeSet, CurrentRage)

    UPROPERTY(BlueprintReadOnly, Category = "Rage", ReplicatedUsing = OnRep_MaxRage)
    FGameplayAttributeData MaxRage;
    ATTRIBUTE_ACCESSORS(UJAAttributeSet, MaxRage)

    // Damage
    UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_AttackPower)
    FGameplayAttributeData AttackPower;
    ATTRIBUTE_ACCESSORS(UJAAttributeSet, AttackPower)

    UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_DefensePower)
    FGameplayAttributeData DefensePower;
    ATTRIBUTE_ACCESSORS(UJAAttributeSet, DefensePower)

    UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_DamageTaken)
    FGameplayAttributeData DamageTaken;
    ATTRIBUTE_ACCESSORS(UJAAttributeSet, DamageTaken)

    // === RepNotify 함수들 ===
    UFUNCTION()
    void OnRep_CurrentHealth(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_CurrentRage(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_MaxRage(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_AttackPower(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_DefensePower(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_DamageTaken(const FGameplayAttributeData& OldValue);

    // === Replication 설정 ===
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
