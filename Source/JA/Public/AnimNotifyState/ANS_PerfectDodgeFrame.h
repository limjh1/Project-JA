// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_PerfectDodgeFrame.generated.h"

/**
 *  
 */
UCLASS(meta = (DisplayName = "Perfect Dodge Frame"))
class JA_API UANS_PerfectDodgeFrame : public UAnimNotifyState
{
	GENERATED_BODY()	

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
