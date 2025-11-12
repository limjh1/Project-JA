// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/JACharacterAnimInstance.h"
#include "JAHeroAnimInstance.generated.h"

class AJAHeroCharacter;
/**
 * 
 */
UCLASS()
class JA_API UJAHeroAnimInstance : public UJACharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
	AJAHeroCharacter* OwningHeroCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 2.5f;

	float IdleElpasedTime;
};
