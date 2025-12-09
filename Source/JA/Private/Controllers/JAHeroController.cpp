// JhLim All Rights Reserved


#include "Controllers/JAHeroController.h"

AJAHeroController::AJAHeroController()
{
    HeroTeamID = FGenericTeamId(0);
}

FGenericTeamId AJAHeroController::GetGenericTeamId() const
{
    return HeroTeamID;
}
