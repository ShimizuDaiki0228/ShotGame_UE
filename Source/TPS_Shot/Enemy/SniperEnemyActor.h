// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyActor.h"
#include "../EnemyState/Sniper/SniperEnemyIdleState.h"
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
	// 後々プレイヤーと同様の弾を使いたいので消したい
	
	FTimerHandle _createBulletTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> _bulletActor;

	//

	std::unique_ptr<ISniperEnemyState> _currentState;

	void SetPatrolAreaOrder();

	TArray<int8> _alreadyAppearanceNumber;

	AActor* _nextPosition;

	

public:
	void ChangeState(ISniperEnemyState* newState);
	virtual void Initialized(ATPS_ShotCharacter* character, ALevelManager* levelManager) override;

	void CreateBulletActor();

	void SetupCurrentPatrolArea();
	TArray<int8> PatrolAreaOrder;

	void SelectPosition();

	bool bIsBeforeApprearance;

	AActor* GetNextPosition() const { return _nextPosition; }

	FVector StartLocation;
	FVector EndLocation;
};
