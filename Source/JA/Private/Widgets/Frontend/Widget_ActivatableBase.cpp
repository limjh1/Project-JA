// JhLim All Rights Reserved


#include "Widgets/Frontend/Widget_ActivatableBase.h"
#include "Controllers/JAFrontendPlayerController.h"

AJAFrontendPlayerController* UWidget_ActivatableBase::GetOwningJAFrontendPlayerController()
{
	if (!CachedOwningJAFrontendPC.IsValid())
	{
		CachedOwningJAFrontendPC = GetOwningPlayer<AJAFrontendPlayerController>();
	}

    return CachedOwningJAFrontendPC.IsValid() ? CachedOwningJAFrontendPC.Get() : nullptr;
}
