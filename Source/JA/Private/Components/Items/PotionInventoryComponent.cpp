// JhLim All Rights Reserved


#include "Components/Items/PotionInventoryComponent.h"
#include "Characters/JAHeroCharacter.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "Components/UI/HeroUIComponent.h"

UPotionInventoryComponent::UPotionInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPotionInventoryComponent::DrinkPotion()
{
	if (0 >= PotionQuantity)
	{
		return;
	}

	if (PotionEffect)
	{
		if (AJAHeroCharacter* HeroCharacter = Cast<AJAHeroCharacter>(GetOwner()))
		{
			UJAAbilitySystemComponent* AbilitySystemComponent = HeroCharacter->GetJAAbilitySystemComponent();
			if (UGameplayEffect* EffectCDO = PotionEffect->GetDefaultObject<UGameplayEffect>())
			{
				AbilitySystemComponent->ApplyGameplayEffectToSelf(
					EffectCDO,
					1,
					AbilitySystemComponent->MakeEffectContext()
				);

				--PotionQuantity;

				HeroCharacter->GetHeroUIComponent()->OnChangedPotionQuantity.Broadcast(PotionQuantity);
			}
		}
	}
}

void UPotionInventoryComponent::SpawnPotion()
{
	if (PotionActor = GetWorld()->SpawnActor<AActor>(PotionClass, GetOwner()->GetActorTransform()))
	{
		if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			PotionActor->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), PotionSocketName);
		}
	}
}

void UPotionInventoryComponent::DespawnPotion()
{
	if (IsValid(PotionActor))
	{
		PotionActor->Destroy();
	}
}
