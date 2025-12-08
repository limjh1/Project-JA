// JhLim All Rights Reserved


#include "Widgets/JAWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"

void UJAWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UHeroUIComponent* HeroUIComponent = PawnUIInterface->GetHeroUIComponent())
		{
			BP_OnOwningHeroUIComponentInitialized(HeroUIComponent);
		}
	}
}

void UJAWidgetBase::InitEnemyCreatedWidget(AActor* OwningEnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningEnemyActor))
	{
		UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();
		checkf(EnemyUIComponent, TEXT("Failed to extract an EnemyUIComponent from %s"), *OwningEnemyActor->GetActorNameOrLabel());
		
		BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
	}
}
