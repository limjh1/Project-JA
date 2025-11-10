// JhLim All Rights Reserved


#include "Characters/JABaseCharacter.h"

// Sets default values
AJABaseCharacter::AJABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	// decal 영향
	GetMesh()->bReceivesDecals = false;
	

}
