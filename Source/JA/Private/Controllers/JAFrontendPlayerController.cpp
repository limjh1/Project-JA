// JhLim All Rights Reserved


#include "Controllers/JAFrontendPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "JASettings/JAGameUserSettings.h"

void AJAFrontendPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("Default"), FoundCameras);

	if (!FoundCameras.IsEmpty())
	{
		SetViewTarget(FoundCameras[0]);
	}

	UJAGameUserSettings* GameUserSettings = UJAGameUserSettings::Get();

	if (-1.f == GameUserSettings->GetLastCPUBenchmarkResult() || -1.f == GameUserSettings->GetLastGPUBenchmarkResult())
	{
		GameUserSettings->RunHardwareBenchmark();
		GameUserSettings->ApplyHardwareBenchmarkResults();
	}
}
