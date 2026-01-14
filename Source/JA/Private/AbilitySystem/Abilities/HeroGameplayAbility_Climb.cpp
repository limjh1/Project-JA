// JhLim All Rights Reserved


#include "AbilitySystem/Abilities/HeroGameplayAbility_Climb.h"
#include "Characters/JAHeroCharacter.h"
#include "Components/Movement/JACustomMovementComponent.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "JAGameplayTags.h"

#include "JADebugHelper.h"

void UHeroGameplayAbility_Climb::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AJAHeroCharacter* HeroCharacter = Cast<AJAHeroCharacter>(ActorInfo->AvatarActor);
    if (!HeroCharacter)
    {
        return;
    }

    UJACustomMovementComponent* CMC = HeroCharacter->GetJACustomMovementComponent();
    UJAAbilitySystemComponent* ASC = HeroCharacter->GetJAAbilitySystemComponent();
    if (!CMC || !ASC)
    {
        return;
    }

    // 토글 처리: 이미 클라이밍 중이라면 기존, 현재 모두 종료
    if (CMC->IsClimbing())
    {
        ASC->TryCancelAbilityByTag(JAGameplayTags::Player_Ability_Climb);
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    // 시작 조건 확인: 조건 안 맞으면 현재 종료
    if (!CMC->CanStartClimbing())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    CMC->StartClimbing();
}

void UHeroGameplayAbility_Climb::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    AJAHeroCharacter* HeroCharacter = Cast<AJAHeroCharacter>(ActorInfo->AvatarActor);
    if (!HeroCharacter)
    {
        return;
    }

    UJACustomMovementComponent* CMC = HeroCharacter->GetJACustomMovementComponent();
    if (!CMC)
    {
        return;
    }

    CMC->StopClimbing();

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
