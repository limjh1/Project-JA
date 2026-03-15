// JhLim All Rights Reserved


#include "AbilitySystem/Abilities/HeroGameplayAbility_PickUpEquip.h"
#include "Components/UI/HeroUIComponent.h"
#include "Characters/JAHeroCharacter.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "JAGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/PickUps/JAPickUpEquipmentBase.h"

void UHeroGameplayAbility_PickUpEquip::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!TriggerEventData->OptionalObject || !ActorInfo->AvatarActor.Get())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    GetHeroUIComponentFromActorInfo()->OnInteracted.Broadcast(true);

    CachedActor = ActorInfo->AvatarActor.Get();
    CachedObject = const_cast<UObject*>(TriggerEventData->OptionalObject.Get());
}

void UHeroGameplayAbility_PickUpEquip::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
    GetHeroUIComponentFromActorInfo()->OnInteracted.Broadcast(false);
}

void UHeroGameplayAbility_PickUpEquip::InteractEquipment()
{
    check(CachedActor);
    check(CachedObject);

    if (AJAHeroCharacter* OverlappedHeroCharacter = Cast<AJAHeroCharacter>(CachedActor))
    {
        UAbilitySystemComponent* ASC = OverlappedHeroCharacter->GetAbilitySystemComponent();
        if (ASC)
        {
            FGameplayEventData Payload;
            Payload.OptionalObject = CachedObject;
            ASC->HandleGameplayEvent(JAGameplayTags::Player_Event_Equipment_Armour, &Payload);

			for (AJAPickUpEquipmentBase* Collected : CollectedEquipments)
			{
				if (Collected)
				{
					Collected->Interact(GetJAAbilitySystemComponentFromActorInfo(), GetAbilityLevel());
				}
			}
        }
    }

    // End Ability On BP
}

void UHeroGameplayAbility_PickUpEquip::CollectEquipments()
{
	CollectedEquipments.Empty();

	TArray<FHitResult> TraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),
		GetHeroCharacterFromActorInfo()->GetActorLocation(),
		GetHeroCharacterFromActorInfo()->GetActorLocation() + ((-1.f * GetHeroCharacterFromActorInfo()->GetActorUpVector()) * BoxTraceDistance),
		TraceBoxSize / 2.f, // Half Size,
		(-1.f * GetHeroCharacterFromActorInfo()->GetActorUpVector()).ToOrientationRotator(),
		EquipmentTraceChannel,
		false,
		TArray<AActor*>(),
		bDrawDebugSphere ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		TraceHits,
		true
	);

	for (const FHitResult& TraceHit : TraceHits)
	{
		if (AJAPickUpEquipmentBase* Founded = Cast<AJAPickUpEquipmentBase>(TraceHit.GetActor()))
		{
			CollectedEquipments.AddUnique(Founded);
		}
	}

	if (CollectedEquipments.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}
