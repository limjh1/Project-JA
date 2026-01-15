// JhLim All Rights Reserved


#include "Components/Movement/JACustomMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/JAHeroCharacter.h"
#include "Components/CapsuleComponent.h"

#include "JADebugHelper.h"

void UJACustomMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType,  ThisTickFunction);
    
    //TraceClimbableSurfaces();
    //TraceFromEyeHeight(ClimbLineTraceDist);
}

void UJACustomMovementComponent::BeginPlay()
{
    Super::BeginPlay();

}

void UJACustomMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
    AJAHeroCharacter* HeroCharacter = Cast<AJAHeroCharacter>(CharacterOwner);
    if (!HeroCharacter)
    {
        return;
    }

    float CharacterCapsuleHeight = HeroCharacter->GetCapsuleHeight();

    if (IsClimbing())
    {
        bOrientRotationToMovement = false;
        HeroCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(CharacterCapsuleHeight * 0.5f);
    }

    // 바로 이전 클라이밍 중이었다면
    if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == ECustomMovementMode::MOVE_Climb)
    {
        bOrientRotationToMovement = true;
        HeroCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(CharacterCapsuleHeight);

        const FRotator DirtyRotation = UpdatedComponent->GetComponentRotation();
        const FRotator CleanStandRotation = FRotator(0.f, DirtyRotation.Yaw, 0.f);
        UpdatedComponent->SetRelativeRotation(CleanStandRotation);

        StopMovementImmediately();
    }
 
    Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

void UJACustomMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
    if (IsClimbing())
    {
        PhysClimb(deltaTime, Iterations);
    }

    Super::PhysCustom(deltaTime, Iterations);
}

float UJACustomMovementComponent::GetMaxSpeed() const
{
    if (IsClimbing())
    {
        return MaxClimbSpeed;
    }
    
    return Super::GetMaxSpeed();
}

float UJACustomMovementComponent::GetMaxAcceleration() const
{
    if (IsClimbing())
    {
        return MaxClimbAcceleration;
    }

    return Super::GetMaxAcceleration();
}

TArray<FHitResult> UJACustomMovementComponent::DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End, bool bShowDebugShape, bool bDrawPersistantShapes)
{
    TArray<FHitResult> OutCapsuleTraceHitResults;

    EDrawDebugTrace::Type DebugTraceType = EDrawDebugTrace::None;

    if (bShowDebugShape)
    {
        DebugTraceType = bDrawPersistantShapes ? EDrawDebugTrace::Persistent : EDrawDebugTrace::ForOneFrame;
    }

    UKismetSystemLibrary::CapsuleTraceMultiForObjects(
        this,
        Start,
        End,
        ClimbCapsuleTraceRadius,
        ClimbCapsuleTraceHalfHeight,
        ClimbableSurfaceTraceTypes,
        false,
        TArray<AActor*>(),
        DebugTraceType,
        OutCapsuleTraceHitResults,
        false
    );

    return OutCapsuleTraceHitResults;
}

FHitResult UJACustomMovementComponent::DoLineTraceSingleByObject(const FVector& Start, const FVector& End, bool bShowDebugShape, bool bDrawPersistantShapes)
{
    FHitResult OutHit;

    EDrawDebugTrace::Type DebugTraceType = EDrawDebugTrace::None;

    if (bShowDebugShape)
    {
        DebugTraceType = bDrawPersistantShapes ? EDrawDebugTrace::Persistent : EDrawDebugTrace::ForOneFrame;
    }

    UKismetSystemLibrary::LineTraceSingleForObjects(
        this,
        Start,
        End,
        ClimbableSurfaceTraceTypes,
        false,
        TArray<AActor*>(),
        DebugTraceType,
        OutHit,
        false
    );

    return OutHit;
}

// Trace for Climbable surfaces, return true if there are indeed valid surfaces, false otherwise
bool UJACustomMovementComponent::TraceClimbableSurfaces()
{
    const FVector StartOffset = UpdatedComponent->GetForwardVector() * 30.f;
    const FVector Start = UpdatedComponent->GetComponentLocation() + StartOffset;
    const FVector End = Start + UpdatedComponent->GetForwardVector(); // 길이는 1일 것.

    ClimbableSurfacesTracedResults = DoCapsuleTraceMultiByObject(Start, End);

    return !(ClimbableSurfacesTracedResults.IsEmpty());
}

FHitResult UJACustomMovementComponent::TraceFromEyeHeight(float TraceDist, float TraceStartOffset)
{
    const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
    const FVector EyeHeightOffset = UpdatedComponent->GetUpVector() * (CharacterOwner->BaseEyeHeight + TraceStartOffset);
    
    const FVector Start = ComponentLocation + EyeHeightOffset;
    const FVector End = Start + UpdatedComponent->GetForwardVector() * TraceDist;

    return DoLineTraceSingleByObject(Start, End);
}

bool UJACustomMovementComponent::CanStartClimbing()
{
    if (IsFalling())
    {
        return false;
    }
    
    if (!TraceClimbableSurfaces())
    {
        return false;
    }

    if (!TraceFromEyeHeight(ClimbLineTraceDist).bBlockingHit)
    {
        return false;
    }

    return true;
}

void UJACustomMovementComponent::StartClimbing()
{
    SetMovementMode(MOVE_Custom, ECustomMovementMode::MOVE_Climb);
}

void UJACustomMovementComponent::StopClimbing()
{
    if (IsClimbing())
    {
        SetMovementMode(MOVE_Falling);
    }    
}

void UJACustomMovementComponent::PhysClimb(float deltaTime, int32 Iterations)
{
    if (deltaTime < MIN_TICK_TIME)
    {
        return;
    }

    // Process all the climable surface info
    TraceClimbableSurfaces();
    ProcessClimbableSurfaceInfo();

    // Check if we should stop climbing
    if (CheckShouldStopClimbing())
    {
        StopClimbing();
    }

    RestorePreAdditiveRootMotionVelocity();

    if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
    {        
        //Define Max Climb speed and acceleration
        CalcVelocity(deltaTime, 0.f, true, MaxBreakClimbDeceleration);
    }

    ApplyRootMotionToVelocity(deltaTime);

    FVector OldLocation = UpdatedComponent->GetComponentLocation();
    const FVector Adjusted = Velocity * deltaTime;
    FHitResult Hit(1.f);

    // Handle Climb Rotation
    SafeMoveUpdatedComponent(Adjusted, GetClimbRotation(deltaTime), true, Hit);

    if (Hit.Time < 1.f)
    {      
        //adjust and try again
        HandleImpact(Hit, deltaTime, Adjusted);
        SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
    }

    if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
    {
        Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
    }

    // Snap Movement To Climb Surfaces
    SnapMovementToClimableSurfaces(deltaTime);
}

void UJACustomMovementComponent::ProcessClimbableSurfaceInfo()
{
    CurrentClimableSurfaceLocation = FVector::ZeroVector;
    CurrentClimableSurfaceNormal = FVector::ZeroVector;

    if (ClimbableSurfacesTracedResults.IsEmpty())
    {
        return;
    }

    for (const FHitResult& TracedHitResult : ClimbableSurfacesTracedResults)
    {
        CurrentClimableSurfaceLocation += TracedHitResult.ImpactPoint;
        CurrentClimableSurfaceNormal += TracedHitResult.ImpactNormal;
    }

    CurrentClimableSurfaceLocation /= ClimbableSurfacesTracedResults.Num(); // 평균 위치
    CurrentClimableSurfaceNormal = CurrentClimableSurfaceNormal.GetSafeNormal(); // 정규화
}

bool UJACustomMovementComponent::CheckShouldStopClimbing()
{
    if (ClimbableSurfacesTracedResults.IsEmpty())
    {
        return true;
    }

    const float DotResult = FVector::DotProduct(CurrentClimableSurfaceNormal, FVector::UpVector);
    const float DegreeDiff = FMath::RadiansToDegrees(FMath::Acos(DotResult));

    if (DegreeDiff <= 60.f)
    {
        return true;
    }

    return false;
}

FQuat UJACustomMovementComponent::GetClimbRotation(float DeltaTime)
{
    const FQuat CurrentQuat = UpdatedComponent->GetComponentQuat();
    if (HasAnimRootMotion() || CurrentRootMotion.HasOverrideVelocity())
    {
        return CurrentQuat;
    }

    const FQuat TargetQuat = FRotationMatrix::MakeFromX((-1.f * CurrentClimableSurfaceNormal)).ToQuat();

    return FMath::QInterpTo(CurrentQuat, TargetQuat, DeltaTime, 5.f);
}

void UJACustomMovementComponent::SnapMovementToClimableSurfaces(float DeltaTime)
{
    const FVector ComponentForward = UpdatedComponent->GetForwardVector();
    const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();

    const FVector ProjectedCharacterToSurface = 
        (CurrentClimableSurfaceLocation - ComponentLocation).ProjectOnTo(ComponentForward); // 표면 바라보는 벡터 구하고 투영

    const FVector SnapVector = (-1.f * CurrentClimableSurfaceNormal) * ProjectedCharacterToSurface.Length(); // 캐릭터가 표면에 스냅

    UpdatedComponent->MoveComponent(
        SnapVector * DeltaTime * GetMaxSpeed(),
        UpdatedComponent->GetComponentQuat(),
        true
    );
}

bool UJACustomMovementComponent::IsClimbing() const
{
    return (MovementMode == MOVE_Custom && CustomMovementMode == ECustomMovementMode::MOVE_Climb);
}
