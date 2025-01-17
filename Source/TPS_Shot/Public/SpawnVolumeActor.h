// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Enemy/Public/EnemyActor.h"
#include "../Enemy/Public/SniperEnemyActor.h"
#include "../Enemy/Public/ExplosionEnemyActor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "SpawnVolumeActor.generated.h"

UCLASS()
class TPS_SHOT_API ASpawnVolumeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolumeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* _whereToSpawn;

	void SpawnEnemy(USpawnManager* enemySpawnManager);

	float _spawnDelay;

	TSubclassOf<AExplosionEnemyActor> _explosionEnemyActor;

	TSubclassOf<ASniperEnemyActor> _sniperEnemyActor;

	ATPS_ShotCharacter* _character;

	TArray<TWeakObjectPtr<AEnemyActor>> _spawnEnemies;

	FTimerHandle _spawnEnemyTimerHandle;

	void DestroySpawnEnemies();
private:
	FVector GetRandomPointInVolume();

	TWeakObjectPtr<AEnemyActor> DecideGenerateEnemy(const UWorld* world, USpawnManager* spawnManager);

protected:
	

public:
	void Initialized(
		TSubclassOf<AExplosionEnemyActor> explosionEnemy,
		TSubclassOf<ASniperEnemyActor> sniperEnemy,
		ATPS_ShotCharacter* character);

public:
	void GameOver();
};
