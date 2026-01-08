// JhLim All Rights Reserved


#include "Characters/JABaseCharacter.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "AbilitySystem/JAAttributeSet.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "JADebugHelper.h"

// Sets default values
AJABaseCharacter::AJABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	// decal 영향
	GetMesh()->bReceivesDecals = false;
	
	JAAbilitySystemComponent = CreateDefaultSubobject<UJAAbilitySystemComponent>(TEXT("JAAbilitySystemComponent"));

	JAAttributeSet = CreateDefaultSubobject<UJAAttributeSet>(TEXT("JAAttributeSet"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAbilitySystemComponent* AJABaseCharacter::GetAbilitySystemComponent() const
{
	return GetJAAbilitySystemComponent();
}

UPawnCombatComponent* AJABaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AJABaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

void AJABaseCharacter::ActivateRagdoll()
{
    //Debug::Print(TEXT("ActivateRagdoll"));

    // 이동 입력 및 AI 로직 차단
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->StopMovementImmediately();
        GetCharacterMovement()->DisableMovement();
    }

    // 캡슐 컴포넌트 충돌 비활성화 
    // (이걸 안 하면 캡슐이 메쉬의 물리 시뮬레이션을 방해하여 공중에 끼일 수 있음)
    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    if (GetMesh())
    {
        // 기존 애니메이션 몽타주 즉시 정지 (충돌 방지)
        if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
        {
            AnimInstance->StopAllMontages(0.0f);
        }

        // 물리 에셋에 설정된 'Ragdoll' 콜리전 프로필 적용 (프로젝트 세팅 확인 필요)
        GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

        GetMesh()->SetAllBodiesSimulatePhysics(true);
        GetMesh()->SetSimulatePhysics(true);

        GetMesh()->WakeAllRigidBodies();

        // 임펄스를 주어 튕겨 나가는 효과 추가
        GetMesh()->AddImpulse(GetActorForwardVector() * -5000.0f, NAME_None, true);

        //Debug::Print(TEXT("ActivateRagdoll Finished"));
    }

    // BT 작동 중지 등 추가적인 사망 후처리 로직
    // Todo
}

void AJABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (JAAbilitySystemComponent)
	{
		JAAbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		const TArray<UAttributeSet*>& Sets = JAAbilitySystemComponent->GetSpawnedAttributes();
		UE_LOG(LogTemp, Warning, TEXT("Spawned AttributeSets: %d"), Sets.Num());
		for (UAttributeSet* S : Sets)
		{
			UE_LOG(LogTemp, Warning, TEXT("  %s"), *GetNameSafe(S));
		}

		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgot to Assign start up data to %s"), *GetName());
	}
}
