// JhLim All Rights Reserved


#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "EnhancedInputSubsystems.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
    for (const FJAInputActionConfig& InputActionConfig : NativeInputActions)
    {
        if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
        {
            return InputActionConfig.InputAction;
        }
    }

    return nullptr;
}

void UDataAsset_InputConfig::AddInputMappingContext(ULocalPlayer* LocalPlayer, UInputMappingContext* ContextToAdd, int32 InPriority)
{
    if (nullptr == ContextToAdd)
    {
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
    check(SubSystem);

    SubSystem->AddMappingContext(ContextToAdd, InPriority);
}

void UDataAsset_InputConfig::RemoveInputMappingContext(ULocalPlayer* LocalPlayer, UInputMappingContext* ContextToRemove)
{
    if (nullptr == ContextToRemove)
    {
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
    check(SubSystem);

    SubSystem->RemoveMappingContext(ContextToRemove);
}

EDataValidationResult UDataAsset_InputConfig::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    TSet<const UInputAction*> UniqueActions;
    TSet<FGameplayTag> UniqueTags;

    auto ValidateActionConfig = [&](const TArray<FJAInputActionConfig>& ConfigArray, const FString& ArrayName)
        {
            for (const FJAInputActionConfig& Config : ConfigArray)
            {
                if (!Config.InputAction)
                {
                    Context.AddError(FText::FromString(FString::Printf(TEXT("%s: InputAction is null!"), *ArrayName)));
                    Result = EDataValidationResult::Invalid;
                    continue;
                }

                // GameplayTag 유효성 체크
                if (!Config.InputTag.IsValid())
                {
                    Context.AddError(FText::FromString(FString::Printf(
                        TEXT("%s: InputTag is invalid for Action %s"), 
                        *ArrayName, 
                        *Config.InputAction->GetName()
                    )));
                    Result = EDataValidationResult::Invalid;
                }

                // 중복 InputAction 체크 
                if (UniqueActions.Contains(Config.InputAction))
                {
                    Context.AddError(FText::FromString(FString::Printf(
                        TEXT("[%s] Duplicate InputAction reference: '%s'. The same asset is assigned to multiple slots!"),
                        *ArrayName,
                        *Config.InputAction->GetName()
                    )));
                    Result = EDataValidationResult::Invalid;
                }
                UniqueActions.Add(Config.InputAction);

                // 중복 Tag 체크
                if (UniqueTags.Contains(Config.InputTag))
                {
                    Context.AddError(FText::FromString(FString::Printf(
                        TEXT("[%s] Duplicate InputTag detected: '%s'. Each action mapping must have a unique GameplayTag."),
                        *ArrayName,
                        *Config.InputTag.ToString()
                    )));
                    Result = EDataValidationResult::Invalid;
                }
                UniqueTags.Add(Config.InputTag);
            }
        };

    ValidateActionConfig(NativeInputActions, TEXT("NativeInputActions"));
    ValidateActionConfig(AbilityInputActions, TEXT("AbilityInputActions"));

    return Result;
}