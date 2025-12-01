// JhLim All Rights Reserved

#include "AbilitySystem/JAAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"

UJAAttributeSet::UJAAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UJAAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, CurrentHealth, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, MaxHealth, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, CurrentRage, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, MaxRage, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, AttackPower, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, DefensePower, COND_OwnerOnly, REPNOTIFY_Always);
}

void UJAAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UJAAttributeSet, CurrentHealth, OldValue);
}

void UJAAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UJAAttributeSet, MaxHealth, OldValue);
}

void UJAAttributeSet::OnRep_CurrentRage(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UJAAttributeSet, CurrentRage, OldValue);
}

void UJAAttributeSet::OnRep_MaxRage(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UJAAttributeSet, MaxRage, OldValue);
}

void UJAAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UJAAttributeSet, AttackPower, OldValue);
}

void UJAAttributeSet::OnRep_DefensePower(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UJAAttributeSet, DefensePower, OldValue);
}