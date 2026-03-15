// JhLim All Rights Reserved


#include "Items/PickUps/JAPickUpEquipmentBase.h"
#include "Characters/JAHeroCharacter.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/HeroGameplayAbility_Equipments.h"
#include "JAGameplayTags.h"
#include "DataAssets/Equipment/DataAsset_Equipment.h"

void AJAPickUpEquipmentBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!EquipmentData || !OtherActor)
    {
        return;
    }

    if (AJAHeroCharacter* OverlappedHeroCharacter = Cast<AJAHeroCharacter>(OtherActor))
    {
        UAbilitySystemComponent* ASC = OverlappedHeroCharacter->GetAbilitySystemComponent();
        if (ASC)
        {
            FGameplayEventData Payload;
            Payload.OptionalObject = EquipmentData;
            ASC->HandleGameplayEvent(JAGameplayTags::Player_Ability_PickUp_Equipment, &Payload);
        }
    }
}

void AJAPickUpEquipmentBase::OnPickUpCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor)
    {
        return;
    }

    if (AJAHeroCharacter* OverlappedHeroCharacter = Cast<AJAHeroCharacter>(OtherActor))
    {
        UJAAbilitySystemComponent* ASC = OverlappedHeroCharacter->GetJAAbilitySystemComponent();
        if (ASC)
        {
            ASC->TryCancelAbilityByTag(JAGameplayTags::Player_Ability_PickUp_Equipment);
        }
    }
}

void AJAPickUpEquipmentBase::Interact(UJAAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
    if (EquipmentData->EquipEffect)
    {
        UGameplayEffect* EffectCDO = EquipmentData->EquipEffect->GetDefaultObject<UGameplayEffect>();

        AbilitySystemComponent->ApplyGameplayEffectToSelf(
            EffectCDO,
            ApplyLevel,
            AbilitySystemComponent->MakeEffectContext()
        );
    }

    BP_OnInteractEquipment(); // Sound, Destroy, FX
}
