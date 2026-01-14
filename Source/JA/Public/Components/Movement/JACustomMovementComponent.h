// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JACustomMovementComponent.generated.h"

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
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

private:
	TArray<FHitResult> DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End, bool bShowDebugShape = false, bool bDrawPersistantShapes = false);
	FHitResult DoLineTraceSingleByObject(const FVector& Start, const FVector& End, bool bShowDebugShape = false, bool bDrawPersistantShapes = false);

private:
	bool TraceClimbableSurfaces();
	FHitResult TraceFromEyeHeight(float TraceDist, float TraceStartOffset = 0.f);

public:
	bool IsClimbing() const;
	bool CanStartClimbing();
	
	void StartClimbing();
	void StopClimbing();

private:
	void PhysClimb(float deltaTime, int32 Iterations);

public:
	TArray<FHitResult> ClimbableSurfacesTracedResults;

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
};
