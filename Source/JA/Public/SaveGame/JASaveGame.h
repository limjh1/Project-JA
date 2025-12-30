// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "JATypes/JAEnumTypes.h"
#include "JASaveGame.generated.h"

/**
 * 
 */
UCLASS()
class JA_API UJASaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	EJAGameDifficulty SavedCurrentGameDifficulty;

};
