// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_StartRagdoll.generated.h"

/**
 * 
 */
UCLASS()
class JA_API UAnimNotify_StartRagdoll : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_StartRagdoll();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
	// 에디터에서 보일 이름 설정
	virtual FString GetNotifyName_Implementation() const override { return TEXT("Start Ragdoll"); }
};
