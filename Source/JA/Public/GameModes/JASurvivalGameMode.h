// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameModes/JABaseGameMode.h"
#include "JASurvivalGameMode.generated.h"

class AJAEnemyCharacter;

UENUM(BlueprintType)
enum class EJASurvivalGameModeState : uint8
{
	WaitSpawnNewWave,
	SpawningNewWave,
	InProgress,
	WaveCompleted,
	AllWavesDone,
	PlayerDied
};

USTRUCT(BlueprintType)
struct FJAEnemyWaveSpawnerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AJAEnemyCharacter> SoftEnemyClassToSpawn;

	UPROPERTY(EditAnywhere)
	int32 MinPerSpawnCount = 1;

	UPROPERTY(EditAnywhere)
	int32 MaxPerSpawnCount = 3;
};

USTRUCT(BlueprintType)
struct FJAEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FJAEnemyWaveSpawnerInfo> EnemyWaveSpawnerDefinitions;

	UPROPERTY(EditAnywhere)
	int32 TotalEnemyToSpawnThisWave = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvivalGameModeStateChangedDelegate, EJASurvivalGameModeState, CurrentState);

/**
 * 
 */
UCLASS()
class JA_API AJASurvivalGameMode : public AJABaseGameMode
{
	GENERATED_BODY()

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void RegisterSpawnedEnemies(const TArray<AJAEnemyCharacter*>& InEnemiesToRegister);

private:
	void SetCurrentSurvivalGameModeState(EJASurvivalGameModeState InState);
	bool HasFinishedAllWaves() const;
	void PreLoadNextWaveEnemies();
	FJAEnemyWaveSpawnerTableRow* GetCurrentWaveSpawnerTableRow() const;
	int32 TrySpawnWaveEnemies();
	bool ShouldKeepSpawnEnemies() const;

private:
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);

private:
	UPROPERTY()
	EJASurvivalGameModeState CurrentSurvivalGameModeState;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSurvivalGameModeStateChangedDelegate OnSurvivalGameModeStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyWaveSpawnerDataTable;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 TotalWavesToSpawn;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveCount = 1;

	UPROPERTY()
	int32 CurrentSpawnedEnemiesCounter = 0;

	UPROPERTY()
	int32 TotalSpawnedEnemiesThisWaveCounter = 0;

	UPROPERTY()
	TArray<AActor*> TargetPointsArray;

	UPROPERTY()
	float TimePassedSinceStart = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnNewWaveWaitTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnEnemisDelayTime = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float WaveCompletedWaitTime = 5.f;

private:
	UPROPERTY()
	TMap<TSoftClassPtr<AJAEnemyCharacter>, UClass*> PreLoadedEnemyClassMap;

};
