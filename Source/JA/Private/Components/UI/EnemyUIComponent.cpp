// JhLim All Rights Reserved


#include "Components/UI/EnemyUIComponent.h"
#include "Widgets/JAWidgetBase.h"

void UEnemyUIComponent::RegisterEnemyDrawnWidget(UJAWidgetBase* InWidgetToRegister)
{
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgetsIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty())
	{
		return;
	}

	for (UJAWidgetBase* DrawnWidget : EnemyDrawnWidgets) 
	{
		if (DrawnWidget)
		{
			DrawnWidget->RemoveFromParent();
		}
	}

	EnemyDrawnWidgets.Empty();
}
