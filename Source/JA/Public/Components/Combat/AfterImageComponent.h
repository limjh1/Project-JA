// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JAGameplayTags.h"
#include "AfterImageComponent.generated.h"

class UPoseableMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JA_API UAfterImageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAfterImageComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** 태그 변화를 감지하여 잔상 시작/중지 제어 */
	void OnPerfectDodgeTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void StartAfterImage(float Interval);
	void StopAfterImage();

	/** 타이머 루프에서 호출될 실제 스폰 함수 */
	void SpawnAfterImage();

	/** 풀에서 Hidden 상태인 컴포넌트를 반환 */
	UPoseableMeshComponent* GetAvailableGhost() const;

private:
	/** 성능 최적화를 위한 오브젝트 풀링 컨테이너 */
	UPROPERTY()
	TArray<TObjectPtr<UPoseableMeshComponent>> GhostPool;

	UPROPERTY(EditAnywhere, Category = "AfterImage|Settings")
	int32 MaxPoolSize = 10;

	UPROPERTY(EditAnywhere, Category = "AfterImage|Settings")
	float GhostLifeTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = "AfterImage|Settings")
	float SpawnInterval = 0.1f;

	FTimerHandle GhostTimerHandle;

	/** 태그 감시용 델리게이트 핸들 보관 (메모리 누수 방지) */
	FDelegateHandle TagDelegateHandle;
};
