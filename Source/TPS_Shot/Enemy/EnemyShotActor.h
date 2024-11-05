// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Niagara/ExplosionEffect.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Beam", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _beamMeshComponent;

	FVector _startPosition;
	FVector _endPosition;
	FVector _alphaPoint;
	FVector _betaPoint;
	FVector directionUnitVector;
	FVector _alphaPointUnitVector;
	FVector _betaPointUnitVector;
	FVector _alphaToBetaUnitVector;
	FRotator _shotDirection;

	const float BASE_SPEED = 2000.0f;
	const float RADIUS = 200.0f;
	const float TEST = 100.0f;

	float _duration;
	float _elapsedTime = 0.0f;

	bool _canShot;

	UPROPERTY(EditDefaultsOnly, Category = "Explode")
	TSubclassOf<AExplosionEffect> _explosionEffect;

	// 前回のフレームと比較して現在の進行方向を取得するために使用する
	FVector _cachedPosition;
};
