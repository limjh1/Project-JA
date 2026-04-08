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
    const int32 PERFORM_THRESHOLD_UP = 13.f; // 약 77fps
    const int32 PERFORM_THRESHOLD_DOWN = 20.f; // 약 50fps

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
    void AnalyzePerformance(float AverageMS);
    void ApplyScalability(EOptimizationTier NewTier);

    void UpdateShadowSettings(EOptimizationTier Tier);
    void UpdateLumenSettings(EOptimizationTier Tier);
    void UpdateGlobalPresets(EOptimizationTier Tier);

    void SetConsoleVar(FString Name, float Val);

private:
    bool bInitialResourcesRegistered = false;

    float Accumulator = 0.0f;
    int32 FrameCount = 0;
    float CheckInterval = 1.0f;
    float Timer = 0.0f;

    EOptimizationTier CurrentTier = EOptimizationTier::Epic;

private:
    const float TierChangeCooldown = 3.0f;
    float LastTierChangeTime = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Optimization", meta = (AllowPrivateAccess = "true"))
    bool bEnableDebugVisual = false;

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
};
