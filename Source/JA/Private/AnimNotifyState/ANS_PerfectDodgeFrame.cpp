// JhLim All Rights Reserved


#include "AnimNotifyState/ANS_PerfectDodgeFrame.h"
#include "JAFunctionLibrary.h"
#include "JAGameplayTags.h"

void UANS_PerfectDodgeFrame::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
    if (!Owner)
    {
        return;
    }

    UJAFunctionLibrary::AddGameplayTagToActorIfNone(Owner, JAGameplayTags::Player_Status_PerfectDodge);
    Owner->CustomTimeDilation = 0.5f; // #TODO TEST
}

void UANS_PerfectDodgeFrame::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
    if (!Owner)
    {
        return;
    }

    UJAFunctionLibrary::RemoveGameplayTagFromActorIfFound(MeshComp->GetOwner(), JAGameplayTags::Player_Status_PerfectDodge);
    Owner->CustomTimeDilation = 1.0f; // #TODO TEST
}
