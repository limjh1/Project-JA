// JhLim All Rights Reserved


#include "Items/PickUps/JAStoneBase.h"
#include "Characters/JAHeroCharacter.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "JAGameplayTags.h"

void AJAStoneBase::Consume(UJAAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	check(StoneGameplayEffectClass);

	UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		EffectCDO,
		ApplyLevel,
		AbilitySystemComponent->MakeEffectContext()
	);

	BP_OnStoneConsumed();
}

void AJAStoneBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AJAHeroCharacter* OverlappedHeroCharacter = Cast<AJAHeroCharacter>(OtherActor))
	{
		OverlappedHeroCharacter->GetJAAbilitySystemComponent()->TryActivateAbilityByTag(JAGameplayTags::Player_Ability_PickUp_Stones);
	}
}
