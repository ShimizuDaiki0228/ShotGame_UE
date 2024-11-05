// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyShotActor.h"
#include "Components/StaticMeshComponent.h"
#include "../Utility/EasingAnimationUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
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
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("HIT!!!!!!!!!!!!! : %s"), *OtherActor->GetName()), true, true, FColor::Green, 2.f);

	if (OtherActor && (OtherActor != this))
	{
		USpawnManager* spawnManager = NewObject<USpawnManager>();
		FActorSpawnParameters spawnParameters;
		spawnParameters.Owner = this;
		spawnParameters.Instigator = GetInstigator();
		spawnManager->SetUp(spawnParameters, GetActorLocation());
		spawnManager->SpawnActor(_explosionEffect);

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
		float easeTime = EasingAnimationUtility::EaseInSine(durationPercentageTime);
		currentLocation = EasingAnimationUtility::CalculateBezierPoint(easeTime,
			_startPosition,
			_alphaPoint - directionUnitVector * TEST,
			_alphaPoint + directionUnitVector * TEST,
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

			_canShot = false;
		}
	}
}

void AEnemyShotActor::Initialized(const FVector& startPosition, const FVector& endPosition, const FRotator& shotDirection)
{
	_startPosition = startPosition;
	_endPosition = endPosition;
	_shotDirection = shotDirection;

	float distance = FVector::Dist(startPosition, endPosition);
	_duration = distance / BASE_SPEED;

	// 目標への方向と、その垂直方向
	directionUnitVector = _shotDirection.Vector().GetSafeNormal();
	FVector directionPerpendicular1 = FVector(-directionUnitVector.Y, directionUnitVector.X, 0).GetSafeNormal();
	FVector directionPerpendicular2 = FVector(directionUnitVector.Z, 0, -directionUnitVector.X).GetSafeNormal();

	
	/*float alphaOffset1 = FMath::RandRange(-RADIUS, RADIUS);
	float alphaOffset2 = FMath::RandRange(-RADIUS, RADIUS);
	float betaOffset1 = FMath::RandRange(-RADIUS, RADIUS);
	float betaOffset2 = FMath::RandRange(-RADIUS, RADIUS);*/
	float alphaOffset1 = 200;
	float alphaOffset2 = 200;
	float betaOffset1 = -200;
	float betaOffset2 = -200;
	
	_alphaPoint = _startPosition + directionPerpendicular1 * alphaOffset1 + directionPerpendicular2 * alphaOffset2;
	_betaPoint = _startPosition + directionPerpendicular1 * betaOffset1 + directionPerpendicular2 * betaOffset2;
	

	// α地点とβ地点に対する単位ベクトルを取得
	_alphaPointUnitVector = _alphaPoint.GetSafeNormal();
	_betaPointUnitVector = _betaPoint.GetSafeNormal();
	_alphaToBetaUnitVector = (_betaPoint - _alphaPoint).GetSafeNormal();

	_alphaPoint += directionUnitVector * 0.3f * distance;
	_betaPoint += directionUnitVector * 0.8f * distance;

	float distanceStartToAlpha = FVector::Dist(startPosition, _alphaPoint);
	float distanceAlphaToBeta = FVector::Dist(_alphaPoint, _betaPoint);
	float distanceBetaToEnd = FVector::Dist(_betaPoint, _endPosition);

	_cachedPosition = _startPosition;
	
	_canShot = true;
}