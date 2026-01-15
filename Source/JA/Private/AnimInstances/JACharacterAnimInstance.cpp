// JhLim All Rights Reserved

#include "AnimInstances/JACharacterAnimInstance.h"
#include "Characters/JABaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UJACharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<AJABaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = Cast<UCharacterMovementComponent>(OwningCharacter->GetMovementComponent());
	}
}

void UJACharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (nullptr == OwningCharacter || nullptr == OwningMovementComponent)
	{
		return;
	}

	// Game Thread에서 안전하게 포인터에서 데이터를 복사만 한다.
	Velocity = OwningCharacter->GetVelocity();
	Acceleration = OwningMovementComponent->GetCurrentAcceleration();
	bIsFallingSafe = OwningMovementComponent->IsFalling();
	ActorRotation = OwningCharacter->GetActorRotation();
}

void UJACharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	// Worker Thread, OwningCharacter 포인터 건드리지 않는다.
	// 복사된 데이터를 사용하므로 스레드 경합이 발생하지 않음

	GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
	AirSpeed = Velocity.Z;

	bShouldMove = (Acceleration.SizeSquared() > 0.f && GroundSpeed > 5.f && !bIsFallingSafe);
	bIsFalling = bIsFallingSafe;

	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, ActorRotation);
}
