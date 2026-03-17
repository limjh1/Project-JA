// JhLim All Rights Reserved


#include "Components/Equipment/JAEquipmentComponent.h"
#include "DataAssets/Equipment/DataAsset_Equipment.h"
#include "Characters/JABaseCharacter.h"
#include "Components/TimelineComponent.h"

UJAEquipmentComponent::UJAEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimeline"));
}

void UJAEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	DissolveCurve = NewObject<UCurveFloat>(this, TEXT("DynamicDissolveCurve"));
	
	// FloatCurve(FRichCurve)에 키프레임 추가
	DissolveCurve->FloatCurve.AddKey(0.0f, 1.0f); // 0초일 때 디졸브 1.0
	DissolveCurve->FloatCurve.AddKey(1.0f, 0.0f); // 1초일 때 디졸브 0.0

	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("OnDissolveUpdate"));
	DissolveTimeline->AddInterpFloat(DissolveCurve, TimelineProgress);

	ActiveMeshMap.Empty();
}

void UJAEquipmentComponent::InitializeEquipmentMeshes(USkeletalMeshComponent* InChestMesh, USkeletalMeshComponent* InPantsMesh, USkeletalMeshComponent* InGlovesMesh, USkeletalMeshComponent* InBootsMesh)
{
	if (InChestMesh) 
	{
		ArmorMeshMap.Add(JAGameplayTags::Player_Status_Equipment_Chest, InChestMesh);
	}
	if (InPantsMesh)
	{
		ArmorMeshMap.Add(JAGameplayTags::Player_Status_Equipment_Pants, InPantsMesh);
	}
	if (InGlovesMesh)
	{
		ArmorMeshMap.Add(JAGameplayTags::Player_Status_Equipment_Gloves, InGlovesMesh);
	}
	if (InBootsMesh)
	{
		ArmorMeshMap.Add(JAGameplayTags::Player_Status_Equipment_Boots, InBootsMesh);
	}
}

void UJAEquipmentComponent::EquipItem(const UDataAsset_Equipment* InData)
{
	if (!InData)
	{
		return;
	}

	// Soft Pointer 비동기 로드 (동기 로드로 예시 작성, 실무에선 FStreamableManager 사용)
	USkeletalMesh* LoadedMesh = InData->ArmorMesh.LoadSynchronous();
	if (!LoadedMesh)
	{
		return;
	}

	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	USkeletalMeshComponent* MainMesh = OwnerChar->GetMesh();

	if (TObjectPtr<USkeletalMeshComponent>* FoundMeshPtr = ArmorMeshMap.Find(InData->SlotTag))
	{
		USkeletalMeshComponent* TargetComp = *FoundMeshPtr;

		if (TargetComp)
		{
			TargetComp->SetSkeletalMesh(LoadedMesh);

			if (DissolveTimeline)
			{
				CurrentDissolvingMesh = TargetComp;
				ActiveMeshMap.Add(TargetComp);

				float PlayRate = FMath::IsNearlyZero(DissolveDuration) ? 1.0f : (1.0f / DissolveDuration); // Zero Divide
				DissolveTimeline->SetPlayRate(PlayRate);

				DissolveTimeline->PlayFromStart();
			}
		}
	}
}

void UJAEquipmentComponent::OnSetMaterialFxValue(const FName& InName, float Value)
{
	for (auto CurMesh : ActiveMeshMap)
	{
		CurMesh->SetScalarParameterValueOnMaterials(InName, Value);
	}
}

void UJAEquipmentComponent::OnDissolveUpdate(float Value)
{
	// 타임라인이 재생되는 동안만 호출됨
	if (CurrentDissolvingMesh)
	{
		CurrentDissolvingMesh->SetScalarParameterValueOnMaterials("DissolveAmount", Value);
	}
}
