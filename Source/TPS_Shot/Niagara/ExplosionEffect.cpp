// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionEffect.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"

AExplosionEffect::AExplosionEffect()
{
	if (GetRootComponent() == nullptr)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}

	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	if (collisionSphere == nullptr)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Nullptr")), true, true, FColor::Red, 2.f);

	}
	else
	{
		collisionSphere->InitSphereRadius(BLAST_RANGE);
		collisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

		collisionSphere->SetupAttachment(GetRootComponent());

	}
}


void AExplosionEffect::Initialized(ATPS_ShotCharacter* player)
{
	LauncherPlayer(player);
}


void AExplosionEffect::LauncherPlayer(ATPS_ShotCharacter* player) const
{
	float proximityValue = CalculateBlastPowerRatio(player);

	FVector launchVelocity = CalculateLaunchVelocity(player, proximityValue);

	if (player->GetHP() > 0)
	{
		player->LaunchCharacter(launchVelocity, true, true);
	}
}

float AExplosionEffect::CalculateBlastPowerRatio(ATPS_ShotCharacter* player) const
{
	FVector effectCenterPosition = GetActorLocation();
	FVector playerCenterPosition = player->GetActorLocation();
	float distance = FVector::Dist(effectCenterPosition, playerCenterPosition);

	float maxRadius = collisionSphere->GetScaledSphereRadius();

	int damage = FMath::Clamp(1.0f - (distance / maxRadius), 0, 1.0f) * 300;
	player->ChangeHP(player->GetHP() - damage);
	// player->ChangeHP(0);

	float blastPowerRatio = FMath::Clamp(1.0f - (distance / (maxRadius * 1.2)), 0, 1.0f);
	return blastPowerRatio;
}

FVector AExplosionEffect::CalculateLaunchVelocity(ATPS_ShotCharacter* player, float proximityValue) const
{
	FVector LaunchDirection = player->GetActorLocation() - GetActorLocation();
	LaunchDirection.Normalize();

	float launchStrength = 1500.0f * proximityValue;
	float launchHeight = 1200.0f * proximityValue;

	FVector launchVelocity = LaunchDirection * launchStrength;
	launchVelocity.Z += launchHeight;

	return launchVelocity;
}
