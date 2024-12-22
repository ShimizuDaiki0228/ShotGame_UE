// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h" 
#include "../Utility/Public/TimeManagerUtility.h"


// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> staticMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	if (staticMeshAsset.Succeeded())
	{
		_mesh->SetStaticMesh(staticMeshAsset.Object);

		_mesh->SetSimulatePhysics(true);
		_mesh->SetEnableGravity(false);

		_mesh->SetNotifyRigidBodyCollision(true);
	}




	_projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	//�y�̃R���|�[�l���g����ɂ��ē����𐧌䂷�邩��ݒ�
	_projectileMovement->SetUpdatedComponent(_mesh);
	_projectileMovement->InitialSpeed = 2000.0f;
	_projectileMovement->MaxSpeed = 2000.0f;
	_projectileMovement->bRotationFollowsVelocity = true;
	_projectileMovement->bShouldBounce = true;

	_projectileMovement->ProjectileGravityScale = 0.0f;
}

void ABulletActor::BeginPlay()
{
	Super::BeginPlay();

	_mesh->OnComponentHit.AddDynamic(this, &ABulletActor::OnHit);

	TimeManagerUtility::GetInstance().Delay(GetWorld(), [this]()
		{
			Destroy();
		}, SELFDESTROY_DURATION, _destroyDelayTimerHandle);
}

void ABulletActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UKismetSystemLibrary::PrintString(this, TEXT("Hit!!!!!!!!!!"), true, true, FColor::Green, 2.f);

	Destroy();
}