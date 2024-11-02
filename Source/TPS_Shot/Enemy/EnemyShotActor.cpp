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

		float t = FMath::Clamp(_elapsedTime / _duration, 0.0f, 1.0f);
		float ease = EasingAnimationUtility::EaseInCirc(t);

		FVector currentLocation = FMath::Lerp(_startPosition, _endPosition, ease);
		SetActorLocation(currentLocation);

		if (t >= 1.0f)
		{
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

	_canShot = true;


}