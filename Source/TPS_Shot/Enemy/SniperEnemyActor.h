// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include "CoreMinimal.h"
#include "EnemyActor.h"
#include "EnemyShotActor.h"
#include "../EnemyState/Sniper/SniperEnemyIdleState.h"
#include "../Niagara/EnemyBeamEffect.h"
#include "../RayObject.h"
#include "SniperEnemyActor.generated.h"

//class SniperEnemyIdleState;
/**
 * 
 */
UCLASS()
class TPS_SHOT_API ASniperEnemyActor : public AEnemyActor
{
	GENERATED_BODY()
	
public:
	ASniperEnemyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual void SelfDestroy() override;

private:
	// ��X�v���C���[�Ɠ��l�̒e���g�������̂ŏ�������
	
	FTimerHandle _createBulletTimerHandle;
	FTimerHandle _shotTimeHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> _bulletActor;

	std::unique_ptr<ISniperEnemyState> _currentState;

	void SetPatrolAreaOrder();

	TArray<int8> _alreadyAppearanceNumber;

	AActor* _nextPosition;

	/*UPROPERTY(EditDefaultsOnly, Category = "Beam")
	TSubclassOf<AEnemyBeamEffect> _beamEffect;*/

	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	TSubclassOf<AEnemyShotActor> _enemyShotActorClass;

	USpawnManager* _shotSpawnManager;

	UNiagaraComponent* _beamEffectSystemInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundBase* _beamShotSound;

	// ���b���Ƃ�ray���΂���
	// ���܂�ɔ�΂�������ƕ��ׂ��傫������
	const float RAY_SPAN = 0.5f;
	float _elapsedRayTime;

	const float SHOT_DURATION = 3.0f;
	float _shotLimit;
	bool _isTargetLockOn;
	const FRotator SHOT_ROTATION_OFFSET = FRotator(90, 0, 0);
	const FVector TARGET_OFFSET = FVector(0, 0, -100);

	URayObject* _rayObject;

	UWorld* _cacheWorld;

	FTimerHandle _nextMoveTimerHandle;

	bool btest;

public:
	void ChangeState(ISniperEnemyState* newState);
	virtual void Initialized(ATPS_ShotCharacter* character) override;

	void BeamShot(int shotNum);

	void SetupCurrentPatrolArea();
	TArray<int8> PatrolAreaOrder;

	void SelectPosition();

	bool bIsBeforeApprearance;

	AActor* GetNextPosition() const { return _nextPosition; }

	FVector StartLocation;
	FVector EndLocation;
};
