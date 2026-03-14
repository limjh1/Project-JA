// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JAEquipmentBase.generated.h"

UCLASS()
class JA_API AJAEquipmentBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AJAEquipmentBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	USkeletalMeshComponent* EquipmentMesh;
};
