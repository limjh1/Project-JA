// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/JABaseAnimInstance.h"
#include "JACharacterAnimInstance.generated.h"

class AJABaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class JA_API UJACharacterAnimInstance : public UJABaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	AJABaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;
};
