// JhLim All Rights Reserved


#include "JATypes/JAStructTypes.h"
#include "AbilitySystem/Abilities/JAHeroGameplayAbility.h"

bool FJAHeroAbilitySet::IsValid() const
{
	return (InputTag.IsValid() && AbilityToGrant);
}
