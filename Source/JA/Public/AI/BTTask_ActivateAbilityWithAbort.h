// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "BTTask_ActivateAbilityWithAbort.generated.h"

/**
 * 
 */
UCLASS()
class JA_API UBTTask_ActivateAbilityWithAbort : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTTask_ActivateAbilityWithAbort();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    
    UFUNCTION()
    void OnAbilityEnded(FGameplayTag EndedTag);

    UPROPERTY(EditAnywhere, Category = "GAS")
    FGameplayTag AbilityTagToActivate;

private:
    // 나중에 종료 처리를 위해 트리 컴포넌트를 저장
    TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
};
