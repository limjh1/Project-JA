// JhLim All Rights Reserved


#include "Subsystems/JAScalabilitySubsystem.h"
#include "HAL/IConsoleManager.h"

#include "JADebugHelper.h"

void UJAScalabilitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

#if WITH_EDITOR
    Debug::Print(FString(TEXT("[UJAScalabilitySubsystem] UJAScalabilitySubsystem::Initialize")));
#endif
}

void UJAScalabilitySubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UJAScalabilitySubsystem::Tick(float DeltaTime)
{
    // 게임월드가 유효할 때만 작동
    if (!GetWorld() || GetWorld()->IsPaused())
    {
        return;
    }

    Accumulator += DeltaTime;
    FrameCount++;
    Timer += DeltaTime;

    // 평균 1초
    if (Timer >= CheckInterval)
    {
        float AverageMS = (Accumulator / FrameCount) * 1000.0f;
        AnalyzePerformance(AverageMS);

        Timer = 0.0f;
        Accumulator = 0.0f;
        FrameCount = 0;
    }
}

void UJAScalabilitySubsystem::AnalyzePerformance(float AverageMS)
{
    // 성능 하락 시
    if (AverageMS > 20.0f)
    {
        if (CurrentTier == EOptimizationTier::Epic)
        {
            ApplyScalability(EOptimizationTier::High);
        }
        else if (CurrentTier == EOptimizationTier::High)
        {
            ApplyScalability(EOptimizationTier::Medium);
        }
        else if (CurrentTier == EOptimizationTier::Medium)
        {
            ApplyScalability(EOptimizationTier::Low);
        }
    }
    // 성능 여유 시
    else if (AverageMS < 13.0f)
    {
        if (CurrentTier == EOptimizationTier::Low) 
        { 
            ApplyScalability(EOptimizationTier::Medium); 
        }
        else if (CurrentTier == EOptimizationTier::Medium)
        {
            ApplyScalability(EOptimizationTier::High);
        }
        else if (CurrentTier == EOptimizationTier::High)
        {
            ApplyScalability(EOptimizationTier::Epic);
        }
    }

#if WITH_EDITOR
    Debug::Print(TEXT("[UJAScalabilitySubsystem] AverageMS"), AverageMS);

    const UEnum* EnumPtr = StaticEnum<EOptimizationTier>();
    FString TierName = EnumPtr ? EnumPtr->GetDisplayNameTextByValue((int64)CurrentTier).ToString() : TEXT("Unknown");
    Debug::Print(TEXT("[UJAScalabilitySubsystem] CurrentTier ") + TierName);
#endif
}

void UJAScalabilitySubsystem::ApplyScalability(EOptimizationTier NewTier)
{
    if (CurrentTier == NewTier) return;

    CurrentTier = NewTier;

    UpdateShadowSettings(CurrentTier);
    UpdateLumenSettings(CurrentTier);
    UpdateGlobalPresets(CurrentTier);

#if WITH_EDITOR
    const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/ProjectJA.EOptimizationTier"), true);
    FString TierName = EnumPtr ? EnumPtr->GetNameStringByValue((int64)CurrentTier) : TEXT("Unknown");
    Debug::Print(TEXT("[UJAScalabilitySubsystem] Scalability Switched to ") + TierName);
#endif
}

void UJAScalabilitySubsystem::UpdateShadowSettings(EOptimizationTier Tier)
{
}

void UJAScalabilitySubsystem::UpdateLumenSettings(EOptimizationTier Tier)
{
}

void UJAScalabilitySubsystem::UpdateGlobalPresets(EOptimizationTier Tier)
{
}

void UJAScalabilitySubsystem::SetConsoleVar(FString Name, float Val)
{
    if (auto* ConsoleVar = IConsoleManager::Get().FindConsoleVariable(*Name))
    {
        ConsoleVar->Set(Val);
    }
}