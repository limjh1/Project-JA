// JhLim All Rights Reserved
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JAHISMManager.generated.h"

class UHierarchicalInstancedStaticMeshComponent;

UCLASS()
class JA_API AJAHISMManager : public AActor
{
    GENERATED_BODY()

public:
    AJAHISMManager();

    // 메시와 트랜스폼을 전달받아 인스턴스를 추가하는 핵심 함수
    void AddInstance(UStaticMesh* InMesh, const FTransform& InTransform);

private:
    // 메시별로 HISM 컴포넌트를 캐싱하여 드로우콜을 1개로 유지
    UPROPERTY()
    TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*> HISMMap;

    UHierarchicalInstancedStaticMeshComponent* GetOrCreateHISMComponent(UStaticMesh* InMesh);
};