// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	FRotator _shotDirection;

	const float BASE_SPEED = 2000.0f;

	float _duration;
	float _elapsedTime = 0.0f;

	bool _canShot;
};
