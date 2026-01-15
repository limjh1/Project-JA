// JhLim All Rights Reserved


#include "AnimInstances/Hero/JAHeroAnimInstance.h"
#include "Characters/JAHeroCharacter.h"
#include "Components/Movement/JACustomMovementComponent.h"

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
