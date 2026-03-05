// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;
class ULocalPlayer;

USTRUCT(BlueprintType)
struct FJAInputActionConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction = nullptr;

	bool IsValid() const
	{
		return (InputTag.IsValid() && InputAction);
	}
};

/**
 * 
 */
UCLASS()
class JA_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;

	void AddInputMappingContext(ULocalPlayer* LocalPlayer, UInputMappingContext* ContextToAdd, int32 InPriority);
	void RemoveInputMappingContext(ULocalPlayer* LocalPlayer, UInputMappingContext* ContextToRemove);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* ClimbMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FJAInputActionConfig> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FJAInputActionConfig> AbilityInputActions;
};
