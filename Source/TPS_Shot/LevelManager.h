// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DesignPattern/ObjectPattern/ObjectPoolUObject.h"
#include "LevelManager.generated.h"

class AParticleSystemPoolActor;

UCLASS()
class TPS_SHOT_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reference", meta = (AllowPrivateAccess = "true"))
		class ASpawnVolumeActor* _volumeActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PatrolArea", meta = (AllowPrivateAccess = "true"))
		TMap<AActor*, bool> _patrolAreaMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class AParticleSystemPoolActor* _testPool;

	//UPROPERTY(EditDefaultsOnly, Category = "Manager")
	//APoolManager* _enemyShotPoolManager;

public:
	ASpawnVolumeActor* GetVolumeActor() const { return _volumeActor; }

	const TMap<AActor*, bool> GetPatrolAreaMap() const { return _patrolAreaMap; }
	void SetPatrolAreaMap(AActor* currentPatrolArea, AActor* newPatrolArea);

	AParticleSystemPoolActor* GetPool() const { return _testPool; }

	//APoolManager* GetEnemyShotPoolManager() const { return _enemyShotPoolManager; }
};
