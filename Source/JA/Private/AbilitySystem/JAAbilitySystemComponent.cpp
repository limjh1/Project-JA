// JhLim All Rights Reserved


#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/JAHeroGameplayAbility.h"
#include "JAGameplayTags.h"

#include "JADebugHelper.h"

void UJAAbilitySystemComponent::OnInternalAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
	if (AbilityEndedData.AbilityThatEnded)
	{
		FGameplayTag AbilityTag = AbilityEndedData.AbilityThatEnded->AbilityTags.First();
		OnJAAbilityEnded.Broadcast(AbilityTag);
	}
}

void UJAAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	OnAbilityEnded.AddUObject(this, &UJAAbilitySystemComponent::OnInternalAbilityEnded);
}

void UJAAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
		{
			continue;
		}

		FGameplayEventData Payload;
		Payload.EventTag = JAGameplayTags::Player_Event_InputPressed;
		Payload.Instigator = GetAvatarActor();
		HandleGameplayEvent(Payload.EventTag, &Payload);

		if (InInputTag.MatchesTag(JAGameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
		else
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UJAAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
		{
			continue;
		}

		if (InInputTag.MatchesTag(JAGameplayTags::InputTag_MustBeHeld) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	}
}

void UJAAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FJAHeroAbilitySet>& InDefaultWeaponAbilities, const TArray<FJAHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty())
	{
		return;
	}

	for (const FJAHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid())
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}

	for (const FJAHeroSpecialAbilitySet& AbilitySet : InSpecialWeaponAbilities)
	{
		if (!AbilitySet.IsValid())
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UJAAbilitySystemComponent::RemoveGrantedHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if(InSpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	InSpecHandlesToRemove.Empty(); // Remove elements
}

bool UJAAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());
	
	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	if (!FoundAbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(SpecToActivate);

		if (!SpecToActivate->IsActive())
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}
	}

	return false;
}

bool UJAAbilitySystemComponent::TryCancelAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	if (!AbilityTagToActivate.IsValid()) return false;

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AbilityTagToActivate);

	// 인자: (취소할 태그들, 제외할 태그들, 무시할 인스턴스)
	// AbilitySpec들을 순회하며 Active 상태인 능력을 종료시킵니다.
	CancelAbilities(&TagContainer);
	return true; 
}