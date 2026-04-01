// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JAGameplayTags.h"
#include "JAEditorAssetLibrary.generated.h"

class UDataAsset_InputConfig;
class UInputAction;

/**
 * 
 */
UCLASS()
class JA_API UJAEditorAssetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// DevelopmentOnly 메타데이터: 이 노드는 빌드(Cook) 시 블루프린트에서 자동으로 제거되어 패키징 에러를 막아줌
	UFUNCTION(BlueprintCallable, Category = "JA|Editor Pipeline", meta = (DevelopmentOnly))
	static UInputAction* CreateInputActionAndRegister(FString ActionName, FGameplayTag InputTag, UDataAsset_InputConfig* TargetConfig);
};
