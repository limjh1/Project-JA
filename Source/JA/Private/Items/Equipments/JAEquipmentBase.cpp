// JhLim All Rights Reserved


#include "Items/Equipments/JAEquipmentBase.h"

AJAEquipmentBase::AJAEquipmentBase()
{
	PrimaryActorTick.bCanEverTick = false;

	EquipmentMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
	SetRootComponent(EquipmentMesh);
}
