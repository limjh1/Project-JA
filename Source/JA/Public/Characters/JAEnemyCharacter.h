// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/JABaseCharacter.h"
#include "JAEnemyCharacter.generated.h"

class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class JA_API AJAEnemyCharacter : public AJABaseCharacter
{
	GENERATED_BODY()
	
public:
	AJAEnemyCharacter();

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

private:
	void InitEnemyStartUpData();

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
};
