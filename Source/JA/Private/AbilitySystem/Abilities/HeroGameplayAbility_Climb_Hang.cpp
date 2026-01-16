// JhLim All Rights Reserved


#include "AbilitySystem/Abilities/HeroGameplayAbility_Climb_Hang.h"
#include "Characters/JAHeroCharacter.h"
#include "Components/Movement/JACustomMovementComponent.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "JAGameplayTags.h"
#include "JAFunctionLibrary.h"

#include "JADebugHelper.h"

void UHeroGameplayAbility_Climb_Hang::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

    if (CMC->IsClimbing() || !CMC->CanStartClimbing())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    CMC->StartClimbing();
}

void UHeroGameplayAbility_Climb_Hang::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

    if (CMC->IsClimbing()) 
    {
        CMC->StopClimbing();
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
