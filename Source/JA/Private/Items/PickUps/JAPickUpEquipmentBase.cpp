// JhLim All Rights Reserved


#include "Items/PickUps/JAPickUpEquipmentBase.h"
#include "Characters/JAHeroCharacter.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/HeroGameplayAbility_Equipments.h"
#include "JAGameplayTags.h"

void AJAPickUpEquipmentBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AJAHeroCharacter* OverlappedHeroCharacter = Cast<AJAHeroCharacter>(OtherActor))
	{
		switch (EquipmentType)
		{
		case EJAEquipmentType::Armour_Chest:
			OverlappedHeroCharacter->GetJAAbilitySystemComponent()->TryActivateAbilityByTag(JAGameplayTags::Player_Ability_Equipment_Chest);
			break;
		case EJAEquipmentType::Armour_Pants:
			OverlappedHeroCharacter->GetJAAbilitySystemComponent()->TryActivateAbilityByTag(JAGameplayTags::Player_Ability_Equipment_Pants);
			break;
		case EJAEquipmentType::Armour_Boots:
			OverlappedHeroCharacter->GetJAAbilitySystemComponent()->TryActivateAbilityByTag(JAGameplayTags::Player_Ability_Equipment_Boots);
			break;
		case EJAEquipmentType::Armour_Gloves:
			OverlappedHeroCharacter->GetJAAbilitySystemComponent()->TryActivateAbilityByTag(JAGameplayTags::Player_Ability_Equipment_Gloves);
			break;
		default:
			break;
		}
	}
}
