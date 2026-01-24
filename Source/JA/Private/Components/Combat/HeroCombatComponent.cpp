// JhLim All Rights Reserved


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/JAHeroWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "JAGameplayTags.h"
#include "JAFunctionLibrary.h"

#include "JADebugHelper.h"

AJAHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<AJAHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

AJAHeroWeapon* UHeroCombatComponent::GetHeroCurrentEquippedWeapon() const
{
    return Cast<AJAHeroWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UHeroCombatComponent::GetHeroCurrentEquippedWeaponDamageAtLevel(float InLevel) const
{
    return GetHeroCurrentEquippedWeapon()->HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    if (OverlappedActors.Contains(HitActor))
    {
        return;
    }

    OverlappedActors.AddUnique(HitActor);

    UJAFunctionLibrary::SendGameplayEventToActor(JAGameplayTags::Shared_Event_MeleeHit, GetOwningPawn(), GetOwningPawn(), HitActor);
    UJAFunctionLibrary::SendGameplayEventToActor(JAGameplayTags::Player_Event_HitPause, GetOwningPawn(), GetOwningPawn(), HitActor);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
    UJAFunctionLibrary::SendGameplayEventToActor(JAGameplayTags::Player_Event_HitPause, GetOwningPawn());
}
