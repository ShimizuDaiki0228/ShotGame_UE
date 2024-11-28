// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyShotActor.h"
#include "Components/StaticMeshComponent.h"
#include "../Utility/EasingAnimationUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "../DesignPattern/ObjectPattern/PooledParticleSystemComponent.h"
#include "../SpawnManager.h"

// Sets default values
AEnemyShotActor::AEnemyShotActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_beamMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeamMeshComponent"));
	RootComponent = _beamMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> beamMeshAsset(TEXT("/Engine/BasicShapes/Cylinder"));
	if (beamMeshAsset.Succeeded())
	{
		_beamMeshComponent->SetStaticMesh(beamMeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> beamMaterialAsset(TEXT("/Game/Assets/FX/FX/Materials/Heros/Belica/Skins/HeavyArmor/M_Belica_Heavy_BoltMat"));
	if (beamMaterialAsset.Succeeded())
	{
		_beamMeshComponent->SetMaterial(0, beamMaterialAsset.Object);
	}
}

// Called when the game starts or when spawned
void AEnemyShotActor::BeginPlay()
{
	Super::BeginPlay();
	
	_beamMeshComponent->OnComponentHit.AddDynamic(this, &AEnemyShotActor::OnHit);
}

void AEnemyShotActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && (OtherActor != this))
	{
		if (_levelManager)
		{
			TWeakObjectPtr<UPooledParticleSystemComponent> hitParticleSystemComponent = _levelManager->GetEnemyEffectPool()->GetPooledObject(this);
		}

		Destroy();
	}
}

// Called every frame
void AEnemyShotActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_canShot)
	{
		_elapsedTime += DeltaTime;
		FVector currentLocation;

		float durationPercentageTime = FMath::Clamp(_elapsedTime / _duration, 0.0f, 1.0f);
		float easeTime = EasingAnimationUtility::EaseInQuart(durationPercentageTime);
		currentLocation = EasingAnimationUtility::CalculateBezierPoint(easeTime,
			_startPosition,
			_alphaPoint - directionUnitVector * _alphaPointFromStartDistance / 2,
			_alphaPoint + directionUnitVector * _alphaPointFromStartDistance / 2,
			_betaPoint,
			_endPosition);
		
		SetActorLocation(currentLocation);
		FVector progressDirection = (currentLocation - _cachedPosition).GetSafeNormal();
		FRotator newRotation = progressDirection.Rotation();
		newRotation.Pitch += 90.0f;
		SetActorRotation(newRotation);

		_cachedPosition = currentLocation;

		if (_elapsedTime >= _duration)
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("End")), true, true, FColor::Black, 2.f);
			_beamMeshComponent->SetEnableGravity(true);
			_canShot = false;
		}
	}
}

void AEnemyShotActor::Initialized(const FVector& startPosition, const FVector& endPosition, const FRotator& shotDirection, const ALevelManager* levelManager)
{
	_levelManager = levelManager;

	_startPosition = startPosition;
	_endPosition = endPosition;
	_shotDirection = shotDirection;

	SetShotRoot(startPosition, endPosition);
	_canShot = true;
}

void AEnemyShotActor::SetShotRoot(const FVector& startPosition, const FVector& endPosition)
{
	float distance = FVector::Dist(startPosition, endPosition);
	_duration = distance / BASE_SPEED;

	// �ڕW�ւ̕����ƁA���̐�������
	directionUnitVector = _shotDirection.Vector().GetSafeNormal();
	FVector directionPerpendicular1 = FVector(-directionUnitVector.Y, directionUnitVector.X, 0).GetSafeNormal();
	FVector directionPerpendicular2 = FVector(directionUnitVector.Z, 0, -directionUnitVector.X).GetSafeNormal();

	// ���n�_�ƃ��n�_�͏ی���(0,0)�őΏۂ̈ʒu�ɂ���悤��
	float alphaOffset1 = FMath::RandRange(-RADIUS, RADIUS) + RANDOM_OFFSET;
	float alphaOffset2 = FMath::RandRange(-RADIUS, RADIUS) + RANDOM_OFFSET;
	float betaOffset1 = alphaOffset1 < RANDOM_OFFSET ? FMath::RandRange(0.0f, RADIUS) + RANDOM_OFFSET : FMath::RandRange(-RADIUS, 0.0f) + RANDOM_OFFSET;
	float betaOffset2 = alphaOffset2 < RANDOM_OFFSET ? FMath::RandRange(0.0f, RADIUS) + RANDOM_OFFSET : FMath::RandRange(-RADIUS, 0.0f) + RANDOM_OFFSET;

	// �X�^�[�g�n�_�����alpha�̈ʒu�����߂�̂Ɏg�p
	_alphaPointFromStartDistance = FMath::RandRange(0.1f, 0.4f) * distance;
	// �G���h�n�_�����beta�̈ʒu�����߂�̂Ɏg�p
	_betaPointFromEndDistance = FMath::RandRange(0.1f, 0.4f) * distance;

	_alphaPoint = _startPosition + directionUnitVector * _alphaPointFromStartDistance;
	_alphaPoint += directionPerpendicular1 * alphaOffset1 + directionPerpendicular2 * alphaOffset2;
	_betaPoint = _startPosition + directionUnitVector * (distance - _alphaPointFromStartDistance);
	_betaPoint += directionPerpendicular1 * betaOffset1 + directionPerpendicular2 * betaOffset2;

	_cachedPosition = _startPosition;

}
