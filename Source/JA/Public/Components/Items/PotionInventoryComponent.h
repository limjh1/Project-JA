// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PotionInventoryComponent.generated.h"

class UGameplayEffect;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JA_API UPotionInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPotionInventoryComponent();

	void SetPotionQuantity(int32 InQuantity) { PotionQuantity = InQuantity; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetPotionQuantity() const { return PotionQuantity; }

	UFUNCTION(BlueprintCallable)
	void DrinkPotion();

	UFUNCTION(BlueprintCallable)
	void SpawnPotion();

	UFUNCTION(BlueprintCallable)
	void DespawnPotion();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PotionQuantity = 10.f;

	UPROPERTY(EditAnywhere)
	FName PotionSocketName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PotionClass;

	UPROPERTY()
	TObjectPtr<AActor> PotionActor;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> PotionEffect;
};
