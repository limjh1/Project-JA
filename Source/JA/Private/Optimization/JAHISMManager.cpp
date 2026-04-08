// JhLim All Rights Reserved

#include "Optimization/JAHISMManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

AJAHISMManager::AJAHISMManager()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void AJAHISMManager::AddInstance(UStaticMesh* InMesh, const FTransform& InTransform)
{
    if (!InMesh)
    {
        return;
    }

    UHierarchicalInstancedStaticMeshComponent* TargetHISM = GetOrCreateHISMComponent(InMesh);
    if (TargetHISM)
    {
        TargetHISM->AddInstance(InTransform);
    }
}

UHierarchicalInstancedStaticMeshComponent* AJAHISMManager::GetOrCreateHISMComponent(UStaticMesh* InMesh)
{
    if (HISMMap.Contains(InMesh))
    {
        return HISMMap[InMesh];
    }

    // HISM 컴포넌트 생성 및 등록
    UHierarchicalInstancedStaticMeshComponent* NewHISM = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
    NewHISM->RegisterComponent();
    NewHISM->SetStaticMesh(InMesh);
    NewHISM->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    // 기본 LOD 세팅 등 필요 시 여기서 수행
    NewHISM->SetCullDistances(0, 5000);

    HISMMap.Add(InMesh, NewHISM);

    return NewHISM;
}