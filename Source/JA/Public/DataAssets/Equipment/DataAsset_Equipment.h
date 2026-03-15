// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "JAGameplayTags.h"
#include "DataAsset_Equipment.generated.h"

class USkeletalMesh;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class JA_API UDataAsset_Equipment : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    // 장착 부위 식별 태그
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
    FGameplayTag SlotTag;

    // 비동기 로딩을 위한 Soft Pointer 사용 
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
    TSoftObjectPtr<USkeletalMesh> ArmorMesh;

    // 장착 시 부여할 능력치
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
    TSubclassOf<UGameplayEffect> EquipEffect;
};
