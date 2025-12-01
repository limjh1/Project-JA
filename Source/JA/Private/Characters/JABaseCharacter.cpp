// JhLim All Rights Reserved


#include "Characters/JABaseCharacter.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "AbilitySystem/JAAttributeSet.h"

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
}

UAbilitySystemComponent* AJABaseCharacter::GetAbilitySystemComponent() const
{
	return GetJAAbilitySystemComponent();
}

UPawnCombatComponent* AJABaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
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
