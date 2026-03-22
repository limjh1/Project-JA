// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JAGameplayTags.h"
#include "AfterImageComponent.generated.h"

class UPoseableMeshComponent;
class UMaterialInterface;

// 잔상 데이터와 해당 잔상을 제어하는 타이머 핸들을 묶는 구조체
USTRUCT()
struct FGhostData
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UPoseableMeshComponent> MeshComponent = nullptr;

	FTimerHandle DeactivateTimerHandle;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JA_API UAfterImageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAfterImageComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void OnPerfectDodgeTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void StartAfterImage(float Interval);
	void StopAfterImage();
	
	void SpawnAfterImage();

private:
	int32 CurrentGhostIndex = 0;
	bool bIsSpawning = false;
	float LastSpawnRealTime = 0.f;

	UPROPERTY()
	TArray<FGhostData> GhostPool;

	UPROPERTY(EditAnywhere, Category = "AfterImage|Settings")
	int32 MaxPoolSize = 10;

	UPROPERTY(EditAnywhere, Category = "AfterImage|Settings")
	float GhostLifeTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = "AfterImage|Settings")
	float SpawnInterval = 0.1f;

	UPROPERTY(EditAnywhere, Category = "AfterImage|Settings")
	TObjectPtr<UMaterialInterface> AfterImageMaterial;

	FTimerHandle SpawnTimerHandle;

	FDelegateHandle TagDelegateHandle;
};
