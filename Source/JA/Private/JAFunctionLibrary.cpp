// JhLim All Rights Reserved


#include "JAFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/PawnCombatInterface.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"

UJAAbilitySystemComponent* UJAFunctionLibrary::NativeGetJAASCFromAcotr(AActor* InActor)
{
    check(InActor);

    return CastChecked<UJAAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

UPawnCombatComponent* UJAFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
    check(InActor);

    if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
    {
        return PawnCombatInterface->GetPawnCombatComponent();
    }

    return nullptr;
}

void UJAFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
    UJAAbilitySystemComponent* ASC = NativeGetJAASCFromAcotr(InActor);
    
    if (!ASC->HasMatchingGameplayTag(TagToAdd))
    {
        ASC->AddLooseGameplayTag(TagToAdd);
    }
}

void UJAFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
    UJAAbilitySystemComponent* ASC = NativeGetJAASCFromAcotr(InActor);

    if (ASC->HasMatchingGameplayTag(TagToRemove))
    {
        ASC->RemoveLooseGameplayTag(TagToRemove);
    }
}

bool UJAFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
    UJAAbilitySystemComponent* ASC = NativeGetJAASCFromAcotr(InActor);
    return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UJAFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EJAConfirmType& OutConfirmType)
{
    OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EJAConfirmType::Yes : EJAConfirmType::No;
}

UPawnCombatComponent* UJAFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EJAValidType& OutValidType)
{
    UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

    OutValidType = CombatComponent ? EJAValidType::Valid : EJAValidType::Invalid;
    
    return CombatComponent;
}

bool UJAFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
    check(QueryPawn && TargetPawn);

    IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
    IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

    if (QueryTeamAgent && TargetTeamAgent)
    {
        return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
    }

    return false;
}

