// JhLim All Rights Reserved


#include "AbilitySystem/Abilities/HeroGameplayAbility_Equipments.h"
#include "DataAssets/Equipment/DataAsset_Equipment.h"
#include "Components/Equipment/JAEquipmentComponent.h"
#include "Characters/JAHeroCharacter.h"

void UHeroGameplayAbility_Equipments::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
    if (!TriggerEventData)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    const UDataAsset_Equipment* EquipmentData = Cast<UDataAsset_Equipment>(TriggerEventData->OptionalObject);
    if (!EquipmentData)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true); 
        return;
    }

    // 캐릭터의 장비 컴포넌트 호출
    if (AJAHeroCharacter* Character = Cast<AJAHeroCharacter>(GetAvatarActorFromActorInfo()))
    {
        if (UJAEquipmentComponent* EquipComp = Character->GetJAEquipmentComponent())
        {
            EquipComp->EquipItem(EquipmentData);
        }
    }

    // 정상 종료
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UHeroGameplayAbility_Equipments::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
