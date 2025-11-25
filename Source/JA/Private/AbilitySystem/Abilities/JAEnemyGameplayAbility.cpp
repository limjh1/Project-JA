// JhLim All Rights Reserved


#include "AbilitySystem/Abilities/JAEnemyGameplayAbility.h"
#include "Characters/JAEnemyCharacter.h"

AJAEnemyCharacter* UJAEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
    if (!CachedJAEnemyCharacter.IsValid())
    {
        CachedJAEnemyCharacter = Cast<AJAEnemyCharacter>(CurrentActorInfo->AvatarActor);
    }

    return CachedJAEnemyCharacter.IsValid() ? CachedJAEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UJAEnemyGameplayAbility::GetEnmeyCombatComponentFromActorInfo()
{
    return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}
