// JhLim All Rights Reserved


#include "JAFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/PawnCombatInterface.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "JAGameplayTags.h"

#include "JADebugHelper.h"

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

float UJAFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalalbeFloat, float InLevel)
{
    return InScalalbeFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UJAFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{
    check(InAttacker && InVictim);

    const FVector VictimForward = InVictim->GetActorForwardVector();
    const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

    const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
    OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

    const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);
    if (0.f > CrossResult.Z)
    {
        OutAngleDifference *= -1.f;
    }

    if (-45.f <= OutAngleDifference && 45 >= OutAngleDifference)
    {
        return JAGameplayTags::Shared_Status_HitReact_Front;
    }
    else if (-45.f > OutAngleDifference && -135.f <= OutAngleDifference)
    {
        return JAGameplayTags::Shared_Status_HitReact_Left;
    }
    else if (-135.f > OutAngleDifference || 135.f < OutAngleDifference)
    {
        return JAGameplayTags::Shared_Status_HitReact_Back;
    }
    else if (45.f < OutAngleDifference && 135.f >= OutAngleDifference)
    {
        return JAGameplayTags::Shared_Status_HitReact_Right;
    }

    return JAGameplayTags::Shared_Status_HitReact_Front;
}

bool UJAFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
    check(InAttacker && InDefender);

    const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

    /*const FString DebugString = FString::Printf(TEXT("Dot Result: %f %s"), DotResult, (-0.1f > DotResult ? TEXT("Valid Block"): TEXT("Invalid Block")));
    Debug::Print(DebugString, (-0.1f > DotResult ? FColor::Green : FColor::Red));*/

    return (-0.1f > DotResult);
}

bool UJAFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
    UJAAbilitySystemComponent* SourceASC = NativeGetJAASCFromAcotr(InInstigator);
    UJAAbilitySystemComponent* TargetASC = NativeGetJAASCFromAcotr(InTargetActor);

    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

    return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}
