// JhLim All Rights Reserved


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/JAWeaponBase.h"
#include "Components/BoxComponent.h"

#include "JADebugHelper.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AJAWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
    checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("A named %s has already been added as carried weapon"), *InWeaponTagToRegister.ToString());;
    check(InWeaponToRegister);

    CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

    InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
    InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

    if (bRegisterAsEquippedWeapon)
    {
        CurrentEquippedWeaponTag = InWeaponTagToRegister;
    }
}

AJAWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
    if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
    {
        if (AJAWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
        {
            return *FoundWeapon;
        }
    }

    return nullptr;
}

AJAWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
    if (!CurrentEquippedWeaponTag.IsValid())
    {
        return nullptr;
    }

    return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
    if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
    {
        AJAWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();
        check(WeaponToToggle);

        if (true == bShouldEnable)
        {
            WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        }
        else
        {
            WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            
            OverlappedActors.Empty(); // clear
        }

        // TODO: Handle Body Collision Boxes
    }
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}
