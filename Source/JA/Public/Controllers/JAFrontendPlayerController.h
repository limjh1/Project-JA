// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JAFrontendPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JA_API AJAFrontendPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	//~ Begin APlayerController Interface
	virtual void OnPossess(APawn* aPawn) override;
	//~ End APlayerController Interface

};
