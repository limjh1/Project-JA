// JhLim All Rights Reserved


#include "JASettings/JALoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UJALoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
    checkf(!SoftLoadingScreenWidgetClass.IsNull(), TEXT("Forgot to assign a valid widget blueprint in the project settings as loading screen"));

    TSubclassOf<UUserWidget> LoadedLoadingScreenWidget = SoftLoadingScreenWidgetClass.LoadSynchronous();

    return LoadedLoadingScreenWidget;
}
