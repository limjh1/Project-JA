// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/JAPickUpBase.h"
#include "JATypes/JAEnumTypes.h"
#include "JAPickUpEquipmentBase.generated.h"

/**
 * 
 */
UCLASS()
class JA_API AJAPickUpEquipmentBase : public AJAPickUpBase
{
	GENERATED_BODY()

protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	EJAEquipmentType EquipmentType = EJAEquipmentType::Armour_Chest;

};
