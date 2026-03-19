// JhLim All Rights Reserved


#include "Components/Combat/AfterImageComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "JAFunctionLibrary.h"
#include "TimerManager.h"
#include "Characters/JABaseCharacter.h"

UAfterImageComponent::UAfterImageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAfterImageComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* Owner = GetOwner();
	if (!Owner) 
	{ 
		return; 
	}

	// 1. 오브젝트 풀 초기화 (BeginPlay에서 수행해야 안전함)
	for (int32 i = 0; i < MaxPoolSize; ++i)
	{
		// 풀링된 컴포넌트가 액터 소멸 시 함께 정리되도록 등록
		UPoseableMeshComponent* Ghost = NewObject<UPoseableMeshComponent>(Owner);
		if (Ghost)
		{
			Ghost->RegisterComponent(); // 렌더링 스레드 등록
			Ghost->SetHiddenInGame(true);
			Ghost->SetComponentTickEnabled(false);
			Ghost->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 물리 부하 방지
			GhostPool.Add(Ghost);
		}
	}

	// 2. GameplayTag 변화 감지 (Delegate 바인딩)
	if (UAbilitySystemComponent* ASC = UJAFunctionLibrary::NativeGetJAASCFromAcotr(Owner))
	{
		TagDelegateHandle = ASC->RegisterGameplayTagEvent(JAGameplayTags::Player_Status_PerfectDodge, EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &UAfterImageComponent::OnPerfectDodgeTagChanged);
	}
}

void UAfterImageComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 3. 자원 정리 (메모리 누수 및 댕글링 타이머 방지)
	StopAfterImage();

	if (UAbilitySystemComponent* ASC = UJAFunctionLibrary::NativeGetJAASCFromAcotr(GetOwner()))
	{
		ASC->RegisterGameplayTagEvent(JAGameplayTags::Player_Status_PerfectDodge, EGameplayTagEventType::NewOrRemoved)
			.Remove(TagDelegateHandle);
	}

	Super::EndPlay(EndPlayReason);
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
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(GhostTimerHandle, this, &UAfterImageComponent::SpawnAfterImage, Interval, true);
	}
}

void UAfterImageComponent::StopAfterImage()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(GhostTimerHandle);
	}
}

void UAfterImageComponent::SpawnAfterImage()
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar || !OwnerChar->GetMesh())
	{
		return;
	}

	UPoseableMeshComponent* Ghost = GetAvailableGhost();
	if (Ghost)
	{
		Ghost->SetHiddenInGame(false);

		// 스켈레탈 메시의 현재 월드 트랜스폼 복사
		Ghost->SetWorldLocationAndRotation(OwnerChar->GetMesh()->GetComponentLocation(), OwnerChar->GetMesh()->GetComponentRotation());

		// [기술적 핵심] 현재 포즈 스냅샷 복사
		Ghost->CopyPoseFromSkeletalComponent(OwnerChar->GetMesh());

		// 람다 캡처 시 TWeakObjectPtr를 사용하여 GC에 의한 유효성 문제 방어
		TWeakObjectPtr<UPoseableMeshComponent> WeakGhost = Ghost;
		FTimerHandle DeactivateHandle;

		GetWorld()->GetTimerManager().SetTimer(DeactivateHandle, [WeakGhost]()
			{
				if (WeakGhost.IsValid())
				{
					WeakGhost->SetHiddenInGame(true);
				}
			}, GhostLifeTime, false);
	}
}

UPoseableMeshComponent* UAfterImageComponent::GetAvailableGhost() const
{
	for (UPoseableMeshComponent* Ghost : GhostPool)
	{
		if (Ghost && Ghost->bHiddenInGame)
		{
			return Ghost;
		}
	}
	return nullptr;
}
