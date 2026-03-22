// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AfterImageFrame.generated.h"

/**
 *  
 */
UCLASS(meta = (DisplayName = "After Image Frame"))
class JA_API UANS_AfterImageFrame : public UAnimNotifyState
{
	GENERATED_BODY()	

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
