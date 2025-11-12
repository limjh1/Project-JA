// JhLim All Rights Reserved

#include "AnimInstances/JACharacterAnimInstance.h"
#include "Characters/JABaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UJACharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<AJABaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UJACharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (nullptr == OwningCharacter || nullptr == OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = OwningCharacter->GetVelocity().Size2D();
	bHasAcceleration = (0.f < OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D());
}
