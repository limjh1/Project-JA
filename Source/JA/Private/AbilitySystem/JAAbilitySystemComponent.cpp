// JhLim All Rights Reserved


#include "AbilitySystem/JAAbilitySystemComponent.h"

void UJAAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
		{
			continue;
		}

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UJAAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
}