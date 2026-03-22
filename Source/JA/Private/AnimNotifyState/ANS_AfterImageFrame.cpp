// JhLim All Rights Reserved


#include "AnimNotifyState/ANS_AfterImageFrame.h"
#include "JAFunctionLibrary.h"
#include "JAGameplayTags.h"
#include "Characters/JAHeroCharacter.h"

void UANS_AfterImageFrame::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (!MeshComp || !MeshComp->GetOwner())
    {
        return;
    }

    if (AJAHeroCharacter* HeroCharacter = Cast<AJAHeroCharacter>(MeshComp->GetOwner()))
    {
        UJAFunctionLibrary::AddGameplayTagToActorIfNone(HeroCharacter, JAGameplayTags::Player_Event_AfterImage);
    }
}

void UANS_AfterImageFrame::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !MeshComp->GetOwner())
    {
        return;
    }

    if (AJAHeroCharacter* HeroCharacter = Cast<AJAHeroCharacter>(MeshComp->GetOwner()))
    {
        UJAFunctionLibrary::RemoveGameplayTagFromActorIfFound(MeshComp->GetOwner(), JAGameplayTags::Player_Event_AfterImage);
    }
}
