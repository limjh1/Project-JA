// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "JAScalabilitySubsystem.generated.h"

UENUM(BlueprintType)
enum class EOptimizationTier : uint8
{
    Epic = 0    UMETA(DisplayName = "에픽"),
    High = 1    UMETA(DisplayName = "높음"),
    Medium = 2  UMETA(DisplayName = "중간"),
    Low = 3     UMETA(DisplayName = "낮음")
};

/**
 * 
 */
UCLASS()
class JA_API UJAScalabilitySubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

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

    void UpdateShadowSettings(EOptimizationTier Tier); // 그림자 제어
    void UpdateLumenSettings(EOptimizationTier Tier);  // 루멘 제어
    void UpdateGlobalPresets(EOptimizationTier Tier);  // 전체 프리셋 제어

    void SetConsoleVar(FString Name, float Val);

private:
    float Accumulator = 0.0f;
    int32 FrameCount = 0;
    float CheckInterval = 1.0f;
    float Timer = 0.0f;

    EOptimizationTier CurrentTier = EOptimizationTier::Epic;
};
