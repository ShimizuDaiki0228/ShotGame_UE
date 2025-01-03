// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyActor.h"
#include "GameFramework/Actor.h"
#include "EnemyShotActor.generated.h"

UCLASS()
class TPS_SHOT_API AEnemyShotActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyShotActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Initialized(const FVector& startPosition, const FVector& endPosition, const FRotator& shotDirection);

private:
	void SetShotRoot(const FVector& startPosition, const FVector& endPosition);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Beam", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _beamMeshComponent;

	FVector _startPosition;
	FVector _endPosition;
	FVector _alphaPoint;
	FVector _betaPoint;
	FVector directionUnitVector;
	FRotator _shotDirection;
	float _alphaPointFromStartDistance;
	float _betaPointFromEndDistance;

	const float BASE_SPEED = 2000.0f;
	const float RADIUS = 200.0f;
	const float TEST = 100.0f;
	const float RANDOM_OFFSET = 100.0f;

	float _duration;
	float _elapsedTime = 0.0f;

	bool _canShot;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
	UParticleSystem* _shotHitParticle;

	UPROPERTY(Category = "Test", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UParticleSystemComponent> reusableHitEffect;

	// �O��̃t���[���Ɣ�r���Č��݂̐i�s�������擾���邽�߂Ɏg�p����
	FVector _cachedPosition;

	UPROPERTY()
	const AEnemyActor* _owner;

	FTimerHandle _destroyTimerHandle;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> _damageAttributeEffect;
};
