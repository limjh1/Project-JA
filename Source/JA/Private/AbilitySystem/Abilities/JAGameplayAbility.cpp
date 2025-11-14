// JhLim All Rights Reserved


#include "AbilitySystem/Abilities/JAGameplayAbility.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"

void UJAGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (EJAAbilityActivationPolicy::OnGiven == AbilityActivationPolicy)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UJAGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (EJAAbilityActivationPolicy::OnGiven == AbilityActivationPolicy)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UJAGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass< UPawnCombatComponent>();
}

UJAAbilitySystemComponent* UJAGameplayAbility::GetJAAbilitySystemComponentFromActorInfo() const
{
	return Cast<UJAAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}
