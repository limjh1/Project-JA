// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "JAScalabilitySubsystem.generated.h"

UENUM(BlueprintType)
enum class EOptimizationTier : uint8
{
    Low = 0         UMETA(DisplayName = "낮음"),
    Medium = 1      UMETA(DisplayName = "중간"),
    High = 2        UMETA(DisplayName = "높음"),
    Epic = 3        UMETA(DisplayName = "에픽")
    //Cinematic 제외
};

UENUM(BlueprintType)
enum class EJAScalabilitySubsystemState : uint8
{
    Warmup,
    Measuring,
    Applying,
    Idle,
};


class ULightComponent;
class AJAHISMManager;

/**
 * 
 */
UCLASS()
class JA_API UJAScalabilitySubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

private:
    const int32 PERFORM_THRESHOLD_UP = 25.f;
    const int32 PERFORM_THRESHOLD_MID = 50.f;
    const int32 PERFORM_THRESHOLD_DOWN = 70.f;

public:
    // ~Begin UGameInstanceSubsystem Interface
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    // ~End UGameInstanceSubsystem Interface
    
    // ~Begin FTickableGameObject Interface
    virtual void Tick(float DeltaTime) override;
    virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
    virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UAJAScalabilitySubsystem, STATGROUP_Tickables); }
    // ~End FTickableGameObject Interface

private:
    void ApplyScalability(EOptimizationTier NewTier);

    void UpdateShadowSettings(EOptimizationTier Tier);
    void UpdateLumenSettings(EOptimizationTier Tier);
    void UpdateGlobalPresets(EOptimizationTier Tier);

    void SetConsoleVar(FString Name, float Val);

private:
    // ~Begin Benchmark
    void StartPerformanceBenchmark(); // 호출 시, 현재 시점을 기점으로 벤치마크
    void UpdateBenchmarkState(float DeltaTime);

    EJAScalabilitySubsystemState CurrentState = EJAScalabilitySubsystemState::Measuring;
    const float BenchmarkDuration = 5.f;
    const float WarmupDuration = 2.f;
    float BenchmarkTimer = 0.f;
    // ~End Benchmark

private:
    // ~Begin Light Opt
    void RegisterAllLights();
    void UpdateLightVisibilityDistance(EOptimizationTier Tier);

    UPROPERTY()
    TArray<TWeakObjectPtr<ULightComponent>> ManagedLights;
    // ~End Light Opt

private:
    // ~Begin HISM
    void BatchStaticMeshActorsToHISM();
    AJAHISMManager* GetOrCreateHISMManager();
    // ~End HISM

private:
    bool bInitialResourcesRegistered = false;

    float Accumulator = 0.0f;
    int32 FrameCount = 0;
    float CheckInterval = 1.0f;
    float Timer = 0.0f;

    EOptimizationTier CurrentTier = EOptimizationTier::Epic;

private:
    UPROPERTY(EditAnywhere, Category = "Optimization", meta = (AllowPrivateAccess = "true"))
    bool bEnableDebugVisual = false;
};
