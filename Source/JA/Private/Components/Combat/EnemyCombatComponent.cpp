// JhLim All Rights Reserved


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "JAGameplayTags.h"
#include "JAFunctionLibrary.h"
#include "Characters/JAEnemyCharacter.h"
#include "Components/BoxComponent.h"

#include "JADebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	//#TODO Implement block check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UJAFunctionLibrary::NativeDoesActorHaveTag(HitActor, JAGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = UJAFunctionLibrary::NativeDoesActorHaveTag(GetOwningPawn(), JAGameplayTags::Enemy_Status_Unblockable);

	if (bIsPlayerBlocking && false == bIsMyAttackUnblockable)
	{
		bIsValidBlock = UJAFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	}

	if (bIsValidBlock)
	{
		UJAFunctionLibrary::SendGameplayEventToActor(JAGameplayTags::Player_Event_SuccessfulBlock, HitActor, GetOwningPawn(), HitActor);
	}
	else
	{
		UJAFunctionLibrary::SendGameplayEventToActor(JAGameplayTags::Shared_Event_MeleeHit, GetOwningPawn(), GetOwningPawn(), HitActor);
	}
}

void UEnemyCombatComponent::ToggleBodyCollisionBoxCollsion(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	Super::ToggleBodyCollisionBoxCollsion(bShouldEnable, ToggleDamageType);

	AJAEnemyCharacter* OwningEnemyCharacter = GetOwningPawn<AJAEnemyCharacter>();
	check(OwningEnemyCharacter);

	UBoxComponent* LeftHandCollisionBox = OwningEnemyCharacter->GetLeftHandCollisionBox();
	UBoxComponent* RightHandCollisionBox = OwningEnemyCharacter->GetRightHandCollisionBox();

	check(LeftHandCollisionBox && RightHandCollisionBox);

	switch (ToggleDamageType)
	{
	case EToggleDamageType::LeftHand:
		LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	case EToggleDamageType::RightHand:
		RightHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	default:
		break;
	}

	if (!bShouldEnable)
	{
		OverlappedActors.Empty();
	}
}
