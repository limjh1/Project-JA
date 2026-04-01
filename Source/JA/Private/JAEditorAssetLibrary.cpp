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

    // 1. 에셋이 생성 경로 설정 (강제)
    FString PackagePath = FString::Printf(TEXT("/Game/PlayerCharacter/Input/Actions/IA_%s"), *ActionName);

    // 2. 패키지(UPackage) 생성: 언리얼의 리플렉션 시스템과 에셋 레지스트리에 등록될 컨테이너
    UPackage* AssetPackage = CreatePackage(*PackagePath);
    AssetPackage->FullyLoad();

    // 3. 완전히 새로운 UObject 메모리 할당 (여기서 새로운 GUID가 발급됨)
    UInputAction* NewInputAction = NewObject<UInputAction>(
        AssetPackage, 
        *FString::Printf(TEXT("IA_%s"), *ActionName), 
        RF_Public | RF_Standalone
    );

    if (NewInputAction)
    {
        // 4. 에셋 레지스트리에 새로 만든 에셋을 알림 (GC에 의해 날아가는 것 방지)
        FAssetRegistryModule::AssetCreated(NewInputAction);

        // 5. 변경사항이 있음을 엔진에 알림 (저장 버튼 활성화)
        AssetPackage->MarkPackageDirty();

        // 6. DataAsset_InputConfig 에 자동 등록
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
