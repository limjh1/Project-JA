// JhLim All Rights Reserved

#include "GameModes/JASurvivalGameMode.h"
#include "Engine/AssetManager.h"
#include "Characters/JAEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"

#include "JADebugHelper.h"

void AJASurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	checkf(EnemyWaveSpawnerDataTable, TEXT("Forgot to Assign a valid data table in survival game mode bp"));

	SetCurrentSurvivalGameModeState(EJASurvivalGameModeState::WaitSpawnNewWave);

	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

	PreLoadNextWaveEnemies(); // First Wave
}

void AJASurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EJASurvivalGameModeState::WaitSpawnNewWave == CurrentSurvivalGameModeState)
	{
		TimePassedSinceStart += DeltaTime;

		if (SpawnNewWaveWaitTime <= TimePassedSinceStart)
		{
			TimePassedSinceStart = 0.f;

			SetCurrentSurvivalGameModeState(EJASurvivalGameModeState::SpawningNewWave);
		}
	}

	if (EJASurvivalGameModeState::SpawningNewWave == CurrentSurvivalGameModeState)
	{
		TimePassedSinceStart += DeltaTime;

		if (SpawnEnemisDelayTime <= TimePassedSinceStart)
		{
			CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();

			TimePassedSinceStart = 0.f;

			SetCurrentSurvivalGameModeState(EJASurvivalGameModeState::InProgress);
		}
	}

	if (EJASurvivalGameModeState::WaveCompleted == CurrentSurvivalGameModeState)
	{
		TimePassedSinceStart += DeltaTime;

		if (WaveCompletedWaitTime <= TimePassedSinceStart)
		{
			TimePassedSinceStart = 0.f;
			
			++CurrentWaveCount;

			if (HasFinishedAllWaves())
			{
				SetCurrentSurvivalGameModeState(EJASurvivalGameModeState::AllWavesDone);
			}
			else
			{
				SetCurrentSurvivalGameModeState(EJASurvivalGameModeState::WaitSpawnNewWave);
				PreLoadNextWaveEnemies();
			}
		}
	}
}

void AJASurvivalGameMode::RegisterSpawnedEnemies(const TArray<AJAEnemyCharacter*>& InEnemiesToRegister)
{
	for (AJAEnemyCharacter* SpawnedEnemy : InEnemiesToRegister)
	{
		if (SpawnedEnemy)
		{
			++CurrentSpawnedEnemiesCounter;
			SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
		}
	}
}

void AJASurvivalGameMode::SetCurrentSurvivalGameModeState(EJASurvivalGameModeState InState)
{
	CurrentSurvivalGameModeState = InState;

	OnSurvivalGameModeStateChanged.Broadcast(CurrentSurvivalGameModeState);
}

bool AJASurvivalGameMode::HasFinishedAllWaves() const
{
	return (TotalWavesToSpawn < CurrentWaveCount);
}

void AJASurvivalGameMode::PreLoadNextWaveEnemies()
{
	if (HasFinishedAllWaves())
	{
		return;
	}

	PreLoadedEnemyClassMap.Empty();

	for (const FJAEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
		{
			continue;
		}

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[SpawnerInfo, this]()
				{
					if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
					{
						PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
					}
				}
			)
		);
	}
}

FJAEnemyWaveSpawnerTableRow* AJASurvivalGameMode::GetCurrentWaveSpawnerTableRow() const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));

	FJAEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FJAEnemyWaveSpawnerTableRow>(RowName, FString());

	checkf(FoundRow, TEXT("Could not find a valid row under the name %s in the data table"), *RowName.ToString());

	return FoundRow;
}

int32 AJASurvivalGameMode::TrySpawnWaveEnemies()
{
	if (TargetPointsArray.IsEmpty())
	{
		// 레벨 모든 액터를 검토하기 때문에 최초
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
	}

	checkf(!TargetPointsArray.IsEmpty(), TEXT("No Valid Target Point Found In Level: %s For Spawning enemies"), *GetWorld()->GetName());
	
	uint32 EnemiesSpawnedThisTime = 0;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const FJAEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
		{
			continue;
		}

		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);

		UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

		for (int32 i = 0; i < NumToSpawn; ++i)
		{
			const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
			const FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
			const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 400.f);

			RandomLocation += FVector(0.f, 0.f, 150.f);

			AJAEnemyCharacter* SpawnedEnemey = GetWorld()->SpawnActor<AJAEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParam);
			if (SpawnedEnemey)
			{
				SpawnedEnemey->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);

				++EnemiesSpawnedThisTime;
				++TotalSpawnedEnemiesThisWaveCounter;
			}

			if (!ShouldKeepSpawnEnemies())
			{
				return EnemiesSpawnedThisTime;
			}
		}
	}

	return EnemiesSpawnedThisTime;
}

bool AJASurvivalGameMode::ShouldKeepSpawnEnemies() const
{
	return (TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave);
}

void AJASurvivalGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
	--CurrentSpawnedEnemiesCounter;

	if (ShouldKeepSpawnEnemies())
	{
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	}
	else if (0 == CurrentSpawnedEnemiesCounter)
	{
		TotalSpawnedEnemiesThisWaveCounter = 0;
		CurrentSpawnedEnemiesCounter = 0;

		SetCurrentSurvivalGameModeState(EJASurvivalGameModeState::WaveCompleted);
	}
}
