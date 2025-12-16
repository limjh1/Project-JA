// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/JAHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UJAWidgetBase;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class JA_API UHeroGameplayAbility_TargetLock : public UJAHeroGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);

private:
	void TryLockOnTarget();
	void GetAvailableActorsToLock();
	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*> InAvailableActors);

	void DrawTargetLockWidget();
	void SetTargetLockWidgetPosition();
	
	void InitTargetLockMovement();
	void InitTargetLockMappingContext();

	void CancelTargetLockAbility();
	void CleanUp();
	
	void ResetTargetLockMovement();
	void ResetTargetLockMappingContext();

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float BoxTraceDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	bool bShowPersistentDebugShape = false;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TSubclassOf<UJAWidgetBase> TargetLockWidgetClass; // hard ref, 작은 위젯

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float TargetLockRotationInterpSpeed = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float TargetLockMaxWalkSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	UInputMappingContext* TargetLockMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float TargetLockCameraOffetDistance = 20.f;

	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;

	UPROPERTY()
	AActor* CurrentLockedActor;

	UPROPERTY()
	UJAWidgetBase* DrawnTargetLockWidget;

	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	UPROPERTY()
	float CachedDefaultMaxWalkSpeed = 0.f;
};
