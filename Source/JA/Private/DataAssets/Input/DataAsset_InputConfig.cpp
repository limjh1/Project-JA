// JhLim All Rights Reserved


#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "EnhancedInputSubsystems.h"

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