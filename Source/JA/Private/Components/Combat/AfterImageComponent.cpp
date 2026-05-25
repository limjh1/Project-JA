// JhLim All Rights Reserved


#include "Components/Combat/AfterImageComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "JAFunctionLibrary.h"
#include "TimerManager.h"
#include "Characters/JABaseCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "JADebugHelper.h"

UAfterImageComponent::UAfterImageComponent()
	: LastSpawnRealTime(0.f)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UAfterImageComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar || !OwnerChar->GetMesh())
	{
		return;
	}

	GhostPool.SetNum(MaxPoolSize);
	for (int32 i = 0; i < MaxPoolSize; ++i)
	{
		UPoseableMeshComponent* Ghost = NewObject<UPoseableMeshComponent>(OwnerChar);
		if (Ghost)
		{
			Ghost->RegisterComponent();
			Ghost->SetHiddenInGame(true);
			Ghost->SetComponentTickEnabled(false);
			Ghost->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Ghost->SetSkinnedAssetAndUpdate(OwnerChar->GetMesh()->GetSkinnedAsset());

			int32 NumMaterials = Ghost->GetNumMaterials();
			for (int32 MatIdx = 0; MatIdx < NumMaterials; ++MatIdx)
			{
				Ghost->SetMaterial(MatIdx, AfterImageMaterial);
			}

			GhostPool[i].MeshComponent = Ghost;
		}
	}

	if (UAbilitySystemComponent* ASC = UJAFunctionLibrary::NativeGetJAASCFromAcotr(OwnerChar))
	{
		TagDelegateHandle = ASC->RegisterGameplayTagEvent(JAGameplayTags::Player_Event_AfterImage, EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &UAfterImageComponent::OnPerfectDodgeTagChanged);
	}
}

void UAfterImageComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopAfterImage();

	if (UWorld* World = GetWorld())
	{
		for (FGhostData& GhostData : GhostPool)
		{
			World->GetTimerManager().ClearTimer(GhostData.DeactivateTimerHandle);
		}
	}

	if (UAbilitySystemComponent* ASC = UJAFunctionLibrary::NativeGetJAASCFromAcotr(GetOwner()))
	{
		ASC->RegisterGameplayTagEvent(JAGameplayTags::Player_Event_AfterImage, EGameplayTagEventType::NewOrRemoved)
			.Remove(TagDelegateHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void UAfterImageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSpawning)
	{
		// 엔진 시간축 왜곡을 완전히 무시하는 절대 시간
		float CurrentRealTime = GetWorld()->GetRealTimeSeconds();
		if (CurrentRealTime - LastSpawnRealTime >= SpawnInterval)
		{
			SpawnAfterImage();
			LastSpawnRealTime = CurrentRealTime;
		}
	}
}

void UAfterImageComponent::OnPerfectDodgeTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		StartAfterImage(SpawnInterval);
	}
	else
	{
		StopAfterImage();
	}
}

void UAfterImageComponent::StartAfterImage(float Interval)
{
	SetComponentTickEnabled(true);

	bIsSpawning = true;

	// 시작 시점의 절대 시간을 기록하여 즉시 첫 잔상이 나오도록 유도
	LastSpawnRealTime = GetWorld()->GetRealTimeSeconds() - Interval;
}

void UAfterImageComponent::StopAfterImage()
{
	SetComponentTickEnabled(false);

	bIsSpawning = false;
}

void UAfterImageComponent::SpawnAfterImage()
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar || !OwnerChar->GetMesh() || GhostPool.IsEmpty())
	{
		return;
	}

	FGhostData& GhostData = GhostPool[CurrentGhostIndex];
	CurrentGhostIndex = (CurrentGhostIndex + 1) % MaxPoolSize;

	UPoseableMeshComponent* Ghost = GhostData.MeshComponent;
	if (Ghost)
	{
		Ghost->SetHiddenInGame(false);
		Ghost->SetWorldLocationAndRotation(OwnerChar->GetMesh()->GetComponentLocation(), OwnerChar->GetMesh()->GetComponentRotation());
		Ghost->CopyPoseFromSkeletalComponent(OwnerChar->GetMesh());
		Ghost->RefreshBoneTransforms();

		// 만약 해당 잔상의 비활성화 타이머가 아직 돌고 있다면 강제로 끔 (안전장치)
		GetWorld()->GetTimerManager().ClearTimer(GhostData.DeactivateTimerHandle);

		TWeakObjectPtr<UPoseableMeshComponent> WeakGhost = Ghost;

		// 구조체가 소유한 전용 타이머 핸들에 새 타이머 등록
		GetWorld()->GetTimerManager().SetTimer(GhostData.DeactivateTimerHandle, [WeakGhost]()
			{
				if (WeakGhost.IsValid())
				{
					WeakGhost->SetHiddenInGame(true);
				}
			}, GhostLifeTime, false);
	}
}