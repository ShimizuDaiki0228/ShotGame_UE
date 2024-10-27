// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../TPS_ShotCharacter.h"
#include "../EnemyState/IEnemyState.h"
#include "../LevelManager.h"
#include "../SpawnManager.h"
#include "../Niagara/ExplosionEffect.h"
#include "EnemyActor.generated.h"


UCLASS()
class TPS_SHOT_API AEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SelfDestroy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* _mesh;

	ATPS_ShotCharacter* _character;

public:
	virtual void Initialized(ATPS_ShotCharacter* character, ALevelManager* levelManager);

	ATPS_ShotCharacter* GetTarget() const { return _character; }

	UStaticMeshComponent* GetMesh() const { return _mesh; }

	ALevelManager* GetLevelManager() const { return _levelManager; }

	// hpがある一定の値以下になった場合に破棄してスコアを上げるためにbool型
	bool DecreaseHP();

	AExplosionEffect* Explosion();
private:
	ALevelManager* _levelManager;

	const int MAX_HP = 3;
	int _hp;

	USpawnManager* _explosionEffectSpawnManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AExplosionEffect> _explosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundBase* _soundToPlay;

	FTimerHandle _destroyTimerHandle;
};
