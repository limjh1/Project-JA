// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/JAPickUpBase.h"
#include "JAStoneBase.generated.h"

class UJAAbilitySystemComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class JA_API AJAStoneBase : public AJAPickUpBase
{
	GENERATED_BODY()
	
public:
	void Consume(UJAAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);

protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
	void BP_OnStoneConsumed();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;
};
