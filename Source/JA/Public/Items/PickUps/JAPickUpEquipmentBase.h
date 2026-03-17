// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/JAPickUpBase.h"
#include "JATypes/JAEnumTypes.h"
#include "JAPickUpEquipmentBase.generated.h"

class UDataAsset_Equipment;
class UJAAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class JA_API AJAPickUpEquipmentBase : public AJAPickUpBase
{
	GENERATED_BODY()

protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnPickUpCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	void Interact(UJAAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Interact Equipment"))
	void BP_OnInteractEquipment();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataAsset_Equipment> EquipmentData;
};
