// JhLim All Rights Reserved


#include "AnimInstances/Hero/JAHeroAnimInstance.h"
#include "Characters/JAHeroCharacter.h"
#include "Components/Movement/JACustomMovementComponent.h"
#include "JAFunctionLibrary.h"
#include "JAGameplayTags.h"

void UJAHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningHeroCharacter = Cast<AJAHeroCharacter>(OwningCharacter);		
	}

	if (OwningMovementComponent)
	{
		OwningHeroCustomMovementComponent = Cast<UJACustomMovementComponent>(OwningMovementComponent);
	}
}

void UJAHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwningHeroCharacter)
	{
		bIsClimbing = UJAFunctionLibrary::NativeDoesActorHaveTag(OwningHeroCharacter, JAGameplayTags::Player_Status_Climbing);

		ClimbVelocity = OwningHeroCustomMovementComponent->GetUnrotatedClimbVelocity();
	}
}

void UJAHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bShouldMove)
	{
		IdleElpasedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElpasedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxStateThreshold);
	}
}
