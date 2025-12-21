// JhLim All Rights Reserved

#include "AbilitySystem/JAAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "JAFunctionLibrary.h"
#include "JAGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/HeroUIComponent.h"

#include "JADebugHelper.h"

UJAAttributeSet::UJAAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
    InitDamageTaken(1.f);
    InitCurrentStamina(1.f);
    InitMaxStamina(1.f);
}

void UJAAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (!CachedPawnUIInterface.IsValid())
    {
        CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor()); // Cast<IPawnUIInterface>(Data.Target.GetAvatarActor());
    }

    checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

    UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();

    checkf(PawnUIComponent, TEXT("Couldn't extract a PawnUIComponent from %s"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

    if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
    {
        const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
        SetCurrentHealth(NewCurrentHealth);

        PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
    }

    if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
    {
        const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
        SetCurrentRage(NewCurrentRage);

        if (GetCurrentRage() == GetMaxRage())
        {
            UJAFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), JAGameplayTags::Player_Status_Rage_Full);
        }
        else if (GetCurrentRage() == 0.f)
        {
            UJAFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), JAGameplayTags::Player_Status_Rage_None);
        }
        else
        {
            UJAFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), JAGameplayTags::Player_Status_Rage_Full);
            UJAFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), JAGameplayTags::Player_Status_Rage_None);
        }

        if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
        {
            HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
        }       
    }

    if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
    {
        const float OldHealth = GetCurrentHealth();
        const float DamageDone = GetDamageTaken();

        const float NewCurrentHealth = FMath::Clamp((OldHealth - DamageDone), 0.f, GetMaxHealth());
        SetCurrentHealth(NewCurrentHealth);

        //const FString DebugString = FString::Printf(
        //    TEXT("OldHealth: %f, Damage Done: %f, NewCurrentHealth: %f"),
        //    OldHealth,
        //    DamageDone,
        //    NewCurrentHealth
        //);
        //Debug::Print(DebugString, FColor::Green);

        PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

        if (0.f >= GetCurrentHealth())
        {
            UJAFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), JAGameplayTags::Shared_Status_Dead);            
        }
    }

    if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
    {
        const float NewCurrentStamina = FMath::Clamp(GetCurrentStamina(), 0.f, GetMaxStamina());
        SetCurrentRage(NewCurrentStamina);

        if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
        {
            HeroUIComponent->OnCurrentStaminaChanged.Broadcast(GetCurrentStamina() / GetMaxStamina());
        }
    }
}

void UJAAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, CurrentHealth, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, MaxHealth, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, CurrentRage, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, MaxRage, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, CurrentStamina, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, MaxStamina, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, AttackPower, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, DefensePower, COND_OwnerOnly, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UJAAttributeSet, DamageTaken, COND_OwnerOnly, REPNOTIFY_Always);
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

void UJAAttributeSet::OnRep_CurrentStamina(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UJAAttributeSet, CurrentStamina, OldValue);
}

void UJAAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UJAAttributeSet, MaxStamina, OldValue);
}

void UJAAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UJAAttributeSet, AttackPower, OldValue);
}

void UJAAttributeSet::OnRep_DefensePower(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UJAAttributeSet, DefensePower, OldValue);
}

void UJAAttributeSet::OnRep_DamageTaken(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UJAAttributeSet, DamageTaken, OldValue);
}
