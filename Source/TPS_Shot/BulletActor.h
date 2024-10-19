// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPS_ShotCharacter.h"
#include "BulletActor.generated.h"


UCLASS()
class TPS_SHOT_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletActor();

protected:
	virtual void BeginPlay();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* _mesh;

private:

	class UProjectileMovementComponent* _projectileMovement;

	const float SELFDESTROY_DURATION = 3.0f;

	FTimerHandle _destroyDelayTimerHandle;

protected:
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
