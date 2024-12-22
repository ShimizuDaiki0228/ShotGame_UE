// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletControllerComponent.h"

#include "ShotCharacterPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "../Utility/Public/SoundManagerUtility.h"
#include "TPS_ShotCharacter.h"
#include "TPS_ShotGameMode.h"
#include "../Enemy/Public/EnemyActor.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values for this component's properties
UBulletControllerComponent::UBulletControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBulletControllerComponent::Initialized(ATPS_ShotCharacter* character)
{
	_cacheCharacter = character;
}

bool UBulletControllerComponent::Create()
{
	if (!_fireSound)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("fireSound isn't exist"), true, true, FColor::Red);
		return false;
	}

	SoundManagerUtility::GetInstance().Play(_fireSound, this);

	const USkeletalMeshSocket* barrelSocket = _cacheCharacter->GetMesh()->GetSocketByName("BarrelSocket");
	if (barrelSocket)
	{
		const FTransform socketTransform = barrelSocket->GetSocketTransform(_cacheCharacter->GetMesh());

		if (!_muzzleFlash)
		{
			UKismetSystemLibrary::PrintString(this, TEXT("muzzleFlash isn't exist"), true, true, FColor::Red);
			return false;
		}
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _muzzleFlash, socketTransform);


		FVector beamEnd;
		bool bBeamEnd = GetBeamEndLocation(socketTransform.GetLocation(), beamEnd);
		if (bBeamEnd)
		{
			if (!_impatctParticle)
			{
				UKismetSystemLibrary::PrintString(this, TEXT("impactParticle isn't exist"), true, true, FColor::Red);
				return false;
			}
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _impatctParticle, beamEnd);

			if (_beamParticle)
			{
				UParticleSystemComponent* beam = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					_beamParticle,
					socketTransform);
				if (beam)
				{
					beam->SetVectorParameter(FName("Target"), beamEnd);
				}
			}
			return true;
		}
	}

	UKismetSystemLibrary::PrintString(this, TEXT("barrelSocket isn't exist"), true, true, FColor::Red);
	return false;
}

bool UBulletControllerComponent::GetBeamEndLocation(const FVector& muzzleSocketLocation, FVector& outBeamLocation)
{
	FVector2D viewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(viewPortSize);
	}

	FVector2D crosshairLocation = FVector2D(viewPortSize.X / 2, viewPortSize.Y / 2);
	FVector crosshairWorldPosition;
	FVector crosshairWorldDirection;

	// �X�N���[�����W�����[���h���W�ɕϊ�
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		crosshairLocation,
		crosshairWorldPosition,
		crosshairWorldDirection);

	if (bScreenToWorld)
	{
		FHitResult screenTraceHit;
		const FVector start = crosshairWorldPosition;
		const FVector end = crosshairWorldPosition + crosshairWorldDirection * 50'000.f;

		outBeamLocation = end;

		GetWorld()->LineTraceSingleByChannel(screenTraceHit, start, end, ECollisionChannel::ECC_Visibility);

		if (screenTraceHit.bBlockingHit)
		{
			outBeamLocation = screenTraceHit.Location;

			AActor* hitActor = screenTraceHit.GetActor();
			if (hitActor)
			{
				AEnemyActor* enemy = Cast<AEnemyActor>(hitActor);
				if (enemy)
				{
					// �����ƃ_���[�W��ݒ肷��A���͎���
					if (enemy->DecreaseHP(100))
					{
						// 絶対にダメな気がするので直す
						auto playerState = _cacheCharacter->GetGameMode()->GetPlayerState();
						playerState->ChangeScore(playerState->GetScore() + 1);
					}
				}
			}
		}

		// �N���X�w�A����̃g���[�X�ƕ��킩��̃g���[�X�ō��ق����܂�邽�߂�萳�m�ɂ��邽�߂�
		FHitResult weaponTraceHit;
		const FVector weaponTraceStart = muzzleSocketLocation;
		const FVector weaponTraceEnd = outBeamLocation;
		GetWorld()->LineTraceSingleByChannel(
			weaponTraceHit,
			weaponTraceStart,
			weaponTraceEnd,
			ECollisionChannel::ECC_Visibility
		);
		if (weaponTraceHit.bBlockingHit)
		{
			outBeamLocation = weaponTraceHit.Location;
		}

		return true;
	}

	return false;
}
