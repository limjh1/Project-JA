// JhLim All Rights Reserved


#include "AnimNotifyState/ANS_InvincibilityFrame.h"
#include "JAFunctionLibrary.h"
#include "JAGameplayTags.h"
#include "Characters/JAHeroCharacter.h"

void UANS_InvincibilityFrame::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (!MeshComp || !MeshComp->GetOwner())
    {
        return;
    }

    if (AJAHeroCharacter* HeroCharacter = Cast<AJAHeroCharacter>(MeshComp->GetOwner()))
    {
        UJAFunctionLibrary::AddGameplayTagToActorIfNone(MeshComp->GetOwner(), JAGameplayTags::Shared_Status_Invincible);
    }
}

void UANS_InvincibilityFrame::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !MeshComp->GetOwner())
    {
        return;
    }

    if (AJAHeroCharacter* HeroCharacter = Cast<AJAHeroCharacter>(MeshComp->GetOwner()))
    {
        UJAFunctionLibrary::RemoveGameplayTagFromActorIfFound(MeshComp->GetOwner(), JAGameplayTags::Shared_Status_Invincible);
    }
}
