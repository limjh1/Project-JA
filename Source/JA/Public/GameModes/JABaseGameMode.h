// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JATypes/JAEnumTypes.h"
#include "JABaseGameMode.generated.h"


/**
 * 
 */
UCLASS()
class JA_API AJABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AJABaseGameMode();

public:
	FORCEINLINE EJAGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EJAGameDifficulty CurrentGameDifficulty;
};
