// JhLim All Rights Reserved


#include "AbilitySystem/Abilities/JAHeroGameplayAbility.h"
#include "Characters/JAHeroCharacter.h"
#include "Controllers/JAHeroController.h"

AJAHeroCharacter* UJAHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
    if (!CachedJAHeroCharacter.IsValid())
    {
        CachedJAHeroCharacter = Cast<AJAHeroCharacter>(CurrentActorInfo->AvatarActor);
    }
   
    return CachedJAHeroCharacter.IsValid() ? CachedJAHeroCharacter.Get() : nullptr;
}

AJAHeroController* UJAHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
    if (!CachedJAHeroController.IsValid())
    {
        CachedJAHeroController = Cast<AJAHeroController>(CurrentActorInfo->PlayerController);
    }

    return CachedJAHeroController.IsValid() ? CachedJAHeroController.Get() : nullptr;
}

UHeroCombatComponent* UJAHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
    return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}
