// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "JABaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class JA_API UJABaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHaveTag(FGameplayTag TagToCheck) const;
};
