// JhLim All Rights Reserved


#include "JATypes/JAStructTypes.h"
#include "AbilitySystem/Abilities/JAGameplayAbility.h"

bool FJAHeroAbilitySet::IsValid() const
{
	return (InputTag.IsValid() && AbilityToGrant);
}
