// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JAGameplayTags.h"
#include "JAEquipmentComponent.generated.h"

class USkeletalMeshComponent;
class UDataAsset_Equipment;
class UTimelineComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JA_API UJAEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UJAEquipmentComponent();

	virtual void BeginPlay() override;

public:
	void InitializeEquipmentMeshes(
		USkeletalMeshComponent* InChestMesh,
		USkeletalMeshComponent* InPantsMesh,
		USkeletalMeshComponent* InGlovesMesh,
		USkeletalMeshComponent* InBootsMesh);

	void EquipItem(const UDataAsset_Equipment* InData);

	UFUNCTION(BlueprintCallable)
	void OnSetMaterialFxValue(const FName& InName, float Value);

protected:
	UFUNCTION()
	void OnDissolveUpdate(float Value);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	TObjectPtr<UCurveFloat> DissolveCurve;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> DissolveTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	float DissolveDuration = 1.f;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> CurrentDissolvingMesh;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<USkeletalMeshComponent>> ArmorMeshMap;

	UPROPERTY()
	TArray<TObjectPtr<USkeletalMeshComponent>> ActiveMeshMap;
		
};
