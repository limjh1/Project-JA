// JhLim All Rights Reserved


#include "AnimInstances/JABaseAnimInstance.h"
#include "JAFunctionLibrary.h"

bool UJABaseAnimInstance::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
    if (APawn* OwningPawn = TryGetPawnOwner())
    {
        return UJAFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
    }

    return false;
}
