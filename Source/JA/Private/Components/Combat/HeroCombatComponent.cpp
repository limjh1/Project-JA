// JhLim All Rights Reserved


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/JAHeroWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "JAGameplayTags.h"

#include "JADebugHelper.h"

AJAHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<AJAHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    if (OverlappedActors.Contains(HitActor))
    {
        return;
    }

    OverlappedActors.AddUnique(HitActor);

    FGameplayEventData Data;
    Data.Instigator = GetOwningPawn();
    Data.Target = HitActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwningPawn(),
        JAGameplayTags::Shared_Event_MeleeHit,
        Data
    );
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
    
}
