// JhLim All Rights Reserved


#include "AnimNotify/AnimNotify_StartRagdoll.h"
#include "Characters/JABaseCharacter.h"

#include "JADebugHelper.h"

UAnimNotify_StartRagdoll::UAnimNotify_StartRagdoll()
{
	NotifyColor = FColor::Red;
}

void UAnimNotify_StartRagdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	//Debug::Print(TEXT("Notify ActivateRagdoll"));

	if (MeshComp && MeshComp->GetOwner())
	{
		if (AJABaseCharacter* Character = Cast<AJABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->ActivateRagdoll();
		}
	}
}
