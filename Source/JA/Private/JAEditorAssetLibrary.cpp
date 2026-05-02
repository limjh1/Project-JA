// JhLim All Rights Reserved


#include "JAEditorAssetLibrary.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "InputAction.h"

#if WITH_EDITOR
#include "AssetToolsModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#endif

#include "JADebugHelper.h"

UInputAction* UJAEditorAssetLibrary::CreateInputActionAndRegister(FString ActionName, FGameplayTag InputTag, UDataAsset_InputConfig* TargetConfig)
{
#if WITH_EDITOR
    if (!TargetConfig)
    {
        Debug::Print("Target Config is NULL.");
        return nullptr;
    }

    // 에셋 생성 경로 설정 (강제)
    FString PackagePath = FString::Printf(TEXT("/Game/PlayerCharacter/Input/Actions/IA_%s"), *ActionName);

    UPackage* AssetPackage = CreatePackage(*PackagePath);
    AssetPackage->FullyLoad();

    // 완전히 새로운 메모리 할당 (새로운 GUID 발급)
    UInputAction* NewInputAction = NewObject<UInputAction>(
        AssetPackage,
        *FString::Printf(TEXT("IA_%s"), *ActionName),
        RF_Public | RF_Standalone
    );

    if (NewInputAction)
    {
        // 에셋 레지스트리에 새로 만든 에셋을 알림 (GC에 의해 날아가는 것 방지)
        FAssetRegistryModule::AssetCreated(NewInputAction);

        // 변경사항이 있음을 엔진에 알림 (저장 버튼 활성화)
        AssetPackage->MarkPackageDirty();

        // DataAsset_InputConfig 에 자동 등록
        FJAInputActionConfig NewConfigEntry;
        NewConfigEntry.InputTag = InputTag;
        NewConfigEntry.InputAction = NewInputAction;
        TargetConfig->AbilityInputActions.Add(NewConfigEntry);
        TargetConfig->MarkPackageDirty();

        Debug::Print("Successfully created and registered: " + PackagePath);
        return NewInputAction;
    }
#endif

    return nullptr;
}
