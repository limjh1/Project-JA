// JhLim All Rights Reserved


#include "JAFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"

UJAAbilitySystemComponent* UJAFunctionLibrary::NativeGetJAASCFromAcotr(AActor* InActor)
{
    check(InActor);

    return CastChecked<UJAAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
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

