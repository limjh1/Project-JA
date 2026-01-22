// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JACustomMovementComponent.generated.h"

class AJABaseCharacter;

UENUM(BlueprintType)
namespace ECustomMovementMode 
{
	enum Type
	{
		MOVE_Climb UMETA(DisplayName = "Climb Mode")
	};
}

/**
 * 
 */
UCLASS()
class JA_API UJACustomMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual FVector ConstrainAnimRootMotionVelocity(const FVector& RootMotionVelocity, const FVector& CurrentVelocity) const override;

	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;

private:
	TArray<FHitResult> DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End, bool bShowDebugShape = false, bool bDrawPersistantShapes = false);
	FHitResult DoLineTraceSingleByObject(const FVector& Start, const FVector& End, bool bShowDebugShape = false, bool bDrawPersistantShapes = false);

private:
	bool TraceClimbableSurfaces();
	FHitResult TraceFromEyeHeight(float TraceDist, float TraceStartOffset = 0.f);

public:
	FORCEINLINE FVector GetClimbableSurfaceNormal() const { return CurrentClimableSurfaceNormal; }
	FVector GetUnrotatedClimbVelocity() const;

	bool IsClimbing() const;
	bool CanStartClimbing();
	bool CanClimbDownLedge();
	bool CanStartVaulting(FVector& OutVaultStartPosition, FVector& OutVaultLandPosition);
	
	UFUNCTION(BlueprintCallable)
	void StartClimbing();

	UFUNCTION(BlueprintCallable)
	void StopClimbing();

	UFUNCTION(BlueprintCallable)
	bool TryStartVaulting();

private:
	void PhysClimb(float deltaTime, int32 Iterations);
	void ProcessClimbableSurfaceInfo();

	bool CheckShouldStopClimbing();
	bool CheckHasReachedFloor();
	bool CheckHasReachedLedge();
	 
	FQuat GetClimbRotation(float DeltaTime);

	void SnapMovementToClimableSurfaces(float DeltaTime);

	void SetMotionWarpTarget(const FName& InWarpTargetName, const FVector& InTargetPosition);

public:
	TArray<FHitResult> ClimbableSurfacesTracedResults;
	
	FVector CurrentClimableSurfaceLocation;
	FVector CurrentClimableSurfaceNormal;

private:
	UPROPERTY()
	AJABaseCharacter* OwningCharacter;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ClimbableSurfaceTraceTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbCapsuleTraceRadius = 50.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbCapsuleTraceHalfHeight = 72.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbLineTraceDist = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float MaxBreakClimbDeceleration = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float MaxClimbSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float MaxClimbAcceleration = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbDownWalkableSurfaceTraceOffset = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbDownLedgeTraceOffset = 50.f;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Vaulting", meta = (AllowPrivateAccess = "true"))
	float MaxVaultScanIterations = 5.f; // 볼팅 지형을 탐색할 최대 횟수

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Vaulting", meta = (AllowPrivateAccess = "true"))
	float VaultLandIndexByLast = 2.f; // 마지막 지점을 기준으로 랜딩 인덱스

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Vaulting", meta = (AllowPrivateAccess = "true"))
	float VaultScanStepDistance = 100.f; // 각 트레이스 간의 간격 (간격이 넓을수록 넓은 지형 체크 가능)

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Vaulting", meta = (AllowPrivateAccess = "true"))
	float VaultTraceVerticalExtent = 100.f; // 트레이스의 수직 탐색 범위
};
