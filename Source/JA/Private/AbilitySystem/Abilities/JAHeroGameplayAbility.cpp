// JhLim All Rights Reserved


#include "AbilitySystem/Abilities/JAHeroGameplayAbility.h"
#include "Characters/JAHeroCharacter.h"
#include "Controllers/JAHeroController.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "JAGameplayTags.h"

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

FGameplayEffectSpecHandle UJAHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount)
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

    // Damage
    EffectSpecHandle.Data->SetSetByCallerMagnitude(
        JAGameplayTags::Shared_SetByCaller_BaseDamage,
        InWeaponBaseDamage
    );

    // AttackType, Count
    if (InCurrentAttackTypeTag.IsValid())
    {
        EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InCurrentComboCount);
    }

    return EffectSpecHandle;
}
