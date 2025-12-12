// JhLim All Rights Reserved


#include "AbilitySystem/Abilities/JAEnemyGameplayAbility.h"
#include "Characters/JAEnemyCharacter.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "JAGameplayTags.h"

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

FGameplayEffectSpecHandle UJAEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
    check(EffectClass);

    FGameplayEffectContextHandle ContextHandle = GetJAAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle EffectSpecHandle = GetJAAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass,
        GetAbilityLevel(),
        ContextHandle
    );

    EffectSpecHandle.Data->SetSetByCallerMagnitude(
        JAGameplayTags::Shared_SetByCaller_BaseDamage,
        InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
    );

    return EffectSpecHandle;
}
