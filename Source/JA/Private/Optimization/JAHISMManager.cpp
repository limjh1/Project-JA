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

    // 메시 크기에 비례해서 컬링
    float MeshRadius = InMesh->GetBounds().SphereRadius;
    float MaxDistance = MeshRadius * CullDistanceMultiplier;

    MaxDistance = FMath::Clamp(MaxDistance, MinClampDist, MaxClampDist);

    NewHISM->SetCullDistances(0, MaxDistance);

    HISMMap.Add(InMesh, NewHISM);

    return NewHISM;
}