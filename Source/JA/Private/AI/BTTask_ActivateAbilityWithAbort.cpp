// JhLim All Rights Reserved


#include "AI/BTTask_ActivateAbilityWithAbort.h"

#include "AIController.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"

UBTTask_ActivateAbilityWithAbort::UBTTask_ActivateAbilityWithAbort()
{
	NodeName = "Activate Ability With Abort";

    // 각 AI마다 독립적인 인스턴스를 생성하도록
    bCreateNodeInstance = true; 
}

EBTNodeResult::Type UBTTask_ActivateAbilityWithAbort::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
    UJAAbilitySystemComponent* ASC = Pawn ? Pawn->FindComponentByClass<UJAAbilitySystemComponent>() : nullptr;

    if (ASC && AbilityTagToActivate.IsValid())
    {
        CachedOwnerComp = &OwnerComp;

        if (!ASC->OnJAAbilityEnded.IsAlreadyBound(this, &UBTTask_ActivateAbilityWithAbort::OnAbilityEnded))
        {
            ASC->OnJAAbilityEnded.AddDynamic(this, &UBTTask_ActivateAbilityWithAbort::OnAbilityEnded);
        }

        if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTagToActivate)))
        {
            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_ActivateAbilityWithAbort::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
    UJAAbilitySystemComponent* ASC = Pawn ? Pawn->FindComponentByClass<UJAAbilitySystemComponent>() : nullptr;

    if (ASC)
    {
        ASC->CurrentMontageStop(0.2f);
        ASC->TryCancelAbilityByTag(AbilityTagToActivate);
        ASC->OnJAAbilityEnded.RemoveDynamic(this, &ThisClass::OnAbilityEnded);
    }

    return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_ActivateAbilityWithAbort::OnAbilityEnded(FGameplayTag EndedTag)
{
    if (EndedTag.MatchesTag(AbilityTagToActivate) && CachedOwnerComp.IsValid())
    {
        if (auto* Pawn = CachedOwnerComp->GetAIOwner()->GetPawn())
        {
            if (auto* ASC = Pawn->FindComponentByClass<UJAAbilitySystemComponent>())
            {
                ASC->OnJAAbilityEnded.RemoveDynamic(this, &ThisClass::OnAbilityEnded);
            }
        }
        // 태스크 정상 종료 후 다음 노드로 이동 처리
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}
