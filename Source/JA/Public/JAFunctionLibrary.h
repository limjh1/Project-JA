// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JAFunctionLibrary.generated.h"

class UJAAbilitySystemComponent;

UENUM()
enum class EJAConfirmType : uint8
{
	Yes,
	No
};

/**
 * 
 */
UCLASS()
class JA_API UJAFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UJAAbilitySystemComponent* NativeGetJAASCFromAcotr(AActor* InActor);
	
	UFUNCTION(BlueprintCallable, Category = "JA|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "JA|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "JA|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EJAConfirmType& OutConfirmType);
};
