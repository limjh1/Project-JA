// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/JABaseAnimInstance.h"
#include "JAHeroLinkedAnimLayer.generated.h"

class UJAHeroAnimInstance;
/**
 * 
 */
UCLASS()
class JA_API UJAHeroLinkedAnimLayer : public UJABaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UJAHeroAnimInstance* GetHeroAnimInstance() const;
};
