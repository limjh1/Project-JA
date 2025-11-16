// JhLim All Rights Reserved


#include "AnimInstances/Hero/JAHeroLinkedAnimLayer.h"
#include "AnimInstances/Hero/JAHeroAnimInstance.h"

UJAHeroAnimInstance* UJAHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
    return Cast<UJAHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
