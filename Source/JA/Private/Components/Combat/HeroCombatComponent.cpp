// JhLim All Rights Reserved


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/JAHeroWeapon.h"

AJAHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<AJAHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}
