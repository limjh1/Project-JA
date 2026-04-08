// JhLim All Rights Reserved


#include "Subsystems/JAScalabilitySubsystem.h"
#include "HAL/IConsoleManager.h"
#include "Components/LightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Optimization/JAHISMManager.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"

#include "JADebugHelper.h"

void UJAScalabilitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

#if WITH_EDITOR
    //Debug::Print(FString(TEXT("[UJAScalabilitySubsystem] UJAScalabilitySubsystem::Initialize")));
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

    if (!bInitialResourcesRegistered)
    {
        RegisterAllLights();
        BatchStaticMeshActorsToHISM();

        bInitialResourcesRegistered = true;
    }    

    Accumulator += DeltaTime;
    FrameCount++;
    Timer += DeltaTime;

    // 평균 1초
    if (Timer >= CheckInterval)
    {
        float AverageMS = (Accumulator / FrameCount) * 1000.f;
        AnalyzePerformance(AverageMS);

        Timer = 0.f;
        Accumulator = 0.f;
        FrameCount = 0;
    }
}

void UJAScalabilitySubsystem::AnalyzePerformance(float AverageMS)
{
    uint8 TierValue = static_cast<uint8>(CurrentTier);
    bool bChanged = false;

    if (PERFORM_THRESHOLD_DOWN < AverageMS && (uint8)EOptimizationTier::Low < TierValue)
    {
        --TierValue;
        bChanged = true;
    }
    else if (PERFORM_THRESHOLD_UP > AverageMS && (uint8)EOptimizationTier::Epic > TierValue)
    {
        ++TierValue;
        bChanged = true;
    }

    if (bChanged)
    {
        ApplyScalability(static_cast<EOptimizationTier>(TierValue));
    }

#if WITH_EDITOR
    //Debug::Print(TEXT("[UJAScalabilitySubsystem] AverageMS"), AverageMS);
    //
    //const UEnum* EnumPtr = StaticEnum<EOptimizationTier>();
    //FString TierName = EnumPtr ? EnumPtr->GetDisplayNameTextByValue((int64)CurrentTier).ToString() : TEXT("Unknown");
    //Debug::Print(TEXT("[UJAScalabilitySubsystem] CurrentTier ") + TierName);
#endif
}

void UJAScalabilitySubsystem::ApplyScalability(EOptimizationTier NewTier)
{
    if (CurrentTier == NewTier)
    {
        return;
    }

    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (TierChangeCooldown > CurrentTime - LastTierChangeTime)
    {
        return;
    }

    LastTierChangeTime = CurrentTime;
    CurrentTier = NewTier;

    UpdateShadowSettings(CurrentTier);
    UpdateLumenSettings(CurrentTier);
    UpdateGlobalPresets(CurrentTier);
    UpdateLightVisibilityDistance(CurrentTier);

#if WITH_EDITOR
    //const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/ProjectJA.EOptimizationTier"), true);
    //FString TierName = EnumPtr ? EnumPtr->GetNameStringByValue((int64)CurrentTier) : TEXT("Unknown");
    //Debug::Print(TEXT("[UJAScalabilitySubsystem] Scalability Switched to ") + TierName);
#endif
}

void UJAScalabilitySubsystem::UpdateShadowSettings(EOptimizationTier Tier)
{
    float Resolution = 1024.f;
    float Distance = 1.f;

    switch (Tier)
    {
    case EOptimizationTier::Epic:   Resolution = 1024.f;    Distance = 1.f;     break;
    case EOptimizationTier::High:   Resolution = 512.f;     Distance = 0.8f;    break;
    case EOptimizationTier::Medium: Resolution = 256.f;     Distance = 0.6f;    break;
    case EOptimizationTier::Low:    Resolution = 128.f;     Distance = 0.4f;    break;
    }

    SetConsoleVar(TEXT("r.Shadow.MaxResolution"), Resolution);  // 해상도
    SetConsoleVar(TEXT("r.Shadow.DistanceScale"), Distance);    // 가시거리
    SetConsoleVar(TEXT("r.Shadow.CSM.MaxCascades"), (Tier == EOptimizationTier::Epic) ? 4.0f : 2.0f); // 캐스케이드 개수, 그림자 정밀도
}

void UJAScalabilitySubsystem::UpdateLumenSettings(EOptimizationTier Tier)
{
    float Quality = 1.0f;
    float ReflectionQual = 1.0f;

    switch (Tier)
    {
    case EOptimizationTier::Epic:   Quality = 1.0f;     ReflectionQual = 1.0f;      break;
    case EOptimizationTier::High:   Quality = 0.5f;     ReflectionQual = 0.5f;      break;
    case EOptimizationTier::Medium: Quality = 0.25f;    ReflectionQual = 0.25f;     break;
    case EOptimizationTier::Low:    Quality = 0.1f;     ReflectionQual = 0.0f;      break;
    }

    SetConsoleVar(TEXT("r.Lumen.SceneQuality"), Quality);               // 루멘 씬 갱신 정밀도
    SetConsoleVar(TEXT("r.Lumen.FinalGather.Quality"), Quality);        // 최종 빛 수집 밀도, 노이즈 관련
    SetConsoleVar(TEXT("r.Lumen.Reflections.Quality"), ReflectionQual); // 반사 품질
}

void UJAScalabilitySubsystem::UpdateGlobalPresets(EOptimizationTier Tier)
{
    float QualityVal = static_cast<float>(Tier);

    SetConsoleVar(TEXT("sg.ViewDistanceQuality"), QualityVal);
    SetConsoleVar(TEXT("sg.PostProcessQuality"), QualityVal);
    SetConsoleVar(TEXT("sg.TextureQuality"), QualityVal);
    SetConsoleVar(TEXT("sg.EffectsQuality"), QualityVal);
    SetConsoleVar(TEXT("sg.ShadingQuality"), QualityVal);
    SetConsoleVar(TEXT("sg.FoliageQuality"), QualityVal);

    // 셰이더 재컴파일 필요한 옵션은 최소화 sg.ShadingQuality, sg.GlobalIlluminationQuality
}

void UJAScalabilitySubsystem::SetConsoleVar(FString Name, float Val)
{
    if (auto* ConsoleVar = IConsoleManager::Get().FindConsoleVariable(*Name))
    {
        ConsoleVar->Set(Val);
    }
}

void UJAScalabilitySubsystem::RegisterAllLights()
{
    ManagedLights.Empty();

    // 월드 내의 모든 라이트 컴포넌트 검색 (BP 내부 포함 검색 필요)
    for (TObjectIterator<ULightComponent> It; It; ++It)
    {
        if (It->GetWorld() == GetWorld())
        {
            // 디렉셔널 라이트는 컬링 대상에서 제외
            if (It->IsA<UDirectionalLightComponent>())
            {
                continue;
            }

            ManagedLights.Add(*It);
        }
    }
}

void UJAScalabilitySubsystem::UpdateLightVisibilityDistance(EOptimizationTier Tier)
{
    float NewMaxDrawDistance = 1000.f;
    float FadeRangeRatio = 0.2f;

    switch (Tier)
    {
    case EOptimizationTier::Epic:   NewMaxDrawDistance = 2000.f; break;
    case EOptimizationTier::High:   NewMaxDrawDistance = 1200.f; break;
    case EOptimizationTier::Medium: NewMaxDrawDistance = 1000.f; break;
    case EOptimizationTier::Low:    NewMaxDrawDistance = 600.f;  break;
    }

    for (auto& LightPtr : ManagedLights)
    {
        if (ULightComponent* Light = LightPtr.Get())
        {
            if (Light->MaxDrawDistance != NewMaxDrawDistance)
            {
                Light->MaxDrawDistance = NewMaxDrawDistance;
                Light->MaxDistanceFadeRange = NewMaxDrawDistance * FadeRangeRatio;
                Light->MarkRenderStateDirty();
            }
        }
    }
}

void UJAScalabilitySubsystem::BatchStaticMeshActorsToHISM()
{
    UWorld* World = GetWorld();
    if (!World) 
    { 
        return; 
    }

    AJAHISMManager* HISMManager = GetOrCreateHISMManager();
    if (!HISMManager)
    {
        return;
    }
    
    // 월드 내 모든 액터 검색
    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* TargetActor = *It;

        // 특정 태그 경우만 처리
        if (TargetActor && TargetActor->ActorHasTag(TEXT("Optimize_HISM")))
        {
            TArray<UStaticMeshComponent*> MeshComps;
            TargetActor->GetComponents<UStaticMeshComponent>(MeshComps);

            for (UStaticMeshComponent* MeshComp : MeshComps)
            {
                if (MeshComp && MeshComp->GetStaticMesh())
                {
                    HISMManager->AddInstance(MeshComp->GetStaticMesh(), MeshComp->GetComponentTransform());

                    // 액터는 살려두고 메시 컴포넌트만 파괴하여 드로우콜 제거
                    MeshComp->DestroyComponent();
                }
            }
        }
    }
}

AJAHISMManager* UJAScalabilitySubsystem::GetOrCreateHISMManager()
{
    UWorld* World = GetWorld();

    for (TActorIterator<AJAHISMManager> It(World); It; ++It)
    {
        return *It;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return World->SpawnActor<AJAHISMManager>(AJAHISMManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
}
