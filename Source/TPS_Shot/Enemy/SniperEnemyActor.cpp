// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperEnemyActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../EnemyBulletActor.h"
#include "../Utility/TimeManagerUtility.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ASniperEnemyActor::ASniperEnemyActor()
{
	PrimaryActorTick.bCanEverTick = true;	


}

void ASniperEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	_mesh->SetSimulatePhysics(false);
}

void ASniperEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_beamEffectSystemInstance && GetTarget())
	{
		_beamEffectSystemInstance->SetWorldLocation(GetActorLocation());
		FVector targetLocation = GetTarget()->GetActorLocation();
		_beamEffectSystemInstance->SetVectorParameter(TEXT("Beam End"), targetLocation);
	}


	if (_currentState)
	{
		_currentState->UpdateState(this);
	}
}

void ASniperEnemyActor::ChangeState(ISniperEnemyState* newState)
{
	if (_currentState.get() == newState)
	{
		return;
	}

	if (_currentState)
	{
		_currentState->ExitState(this);
	}

	_currentState.reset(newState);

	if (_currentState)
	{
		_currentState->EnterState(this);
	}
}

void ASniperEnemyActor::Initialized(ATPS_ShotCharacter* character, ALevelManager* levelManager)
{
	Super::Initialized(character, levelManager);

	SetPatrolAreaOrder();

	if (_nextPosition != nullptr)
	{
		//èâä˙èÛë‘ÇÕIdleÇ…ê›íË
		_currentState = std::make_unique<SniperEnemyIdleState>();
		_currentState->EnterState(this);

		CreateBulletActor();
	}

	if (_beamEffect)
	{
		FActorSpawnParameters beamEffectSpawnParams;
		beamEffectSpawnParams.Owner = this;

		AEnemyBeamEffect* beamEffect = GetWorld()->SpawnActor<AEnemyBeamEffect>(_beamEffect, GetActorLocation(), FRotator::ZeroRotator, beamEffectSpawnParams);

		if (beamEffect)
		{
			_beamEffectSystemInstance = beamEffect->GetNiagaraComponent();

			/*UNiagaraSystem* beamEffectSystem = _beamEffect.GetDefaultObject();
			_beamEffectSystemInstance = UNiagaraFunctionLibrary::SpawnSystemAttached(
				beamEffectSystem,
				RootComponent,
				NAME_None,
				GetActorLocation(),
				FRotator::ZeroRotator,
				EAttachLocation::KeepWorldPosition,
				true
			);*/

			if (_beamEffectSystemInstance && GetTarget())
			{
				FVector targetLocation = GetTarget()->GetActorLocation();
				_beamEffectSystemInstance->SetVectorParameter(TEXT("BeamEnd"), targetLocation);
			}
		}

		
	}
}

void ASniperEnemyActor::SetPatrolAreaOrder()
{
	TMap<AActor*, bool> map = GetLevelManager()->GetPatrolAreaMap();

	int8 mapSize = map.Num();

	for (int i = 0; i < mapSize; i++)
	{
		int random = FMath::RandRange(0, mapSize - 1);

		if (_alreadyAppearanceNumber.Contains(random))
		{
			while (!_alreadyAppearanceNumber.Contains(random))
			{
				random = (random + 1) % mapSize;
			}
		}

		PatrolAreaOrder.Add(random);
		_alreadyAppearanceNumber.Add(random);
	}
	
	SelectPosition();
}

void ASniperEnemyActor::SelectPosition()
{
	const TMap<AActor*, bool>& patrolAreaMap = GetLevelManager()->GetPatrolAreaMap();

	if (!patrolAreaMap.IsEmpty())
	{
		TArray<AActor*> keys;
		patrolAreaMap.GetKeys(keys);

		for (int8 i = 0; i < keys.Num(); i++)
		{
			int8 patrolAreaNumber = PatrolAreaOrder[i];

			AActor* patrolArea = keys[patrolAreaNumber];
			bool isSelected = patrolAreaMap[patrolArea];

			if (!isSelected)
			{
				GetLevelManager()->SetPatrolAreaMap(_nextPosition, patrolArea);
				_nextPosition = patrolArea;
				return;
			}
		}

		// Ç‡Çµà⁄ìÆÇ∑ÇÈèÍèäÇ™Ç»Ç¢èÍçáÇÕìGÇÃêîÇ™å¿äEÇ‹Ç≈Ç¢ÇÈÇΩÇﬂîjä¸Ç∑ÇÈ
		UKismetSystemLibrary::PrintString(this, TEXT("Position is Null, this enemy destroy"), true, true, FColor::Green, 2.f);
		SelfDestroy();
	}
}

void ASniperEnemyActor::CreateBulletActor()
{
	/*if (!bCanShot) return;

	if (_fireSound)
	{
		UGameplayStatics::PlaySound2D(this, _fireSound);
	}

	const USkeletalMeshSocket* barrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	if (barrelSocket)
	{
		const FTransform socketTransform = barrelSocket->GetSocketTransform(GetMesh());

		if (_muzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _muzzleFlash, socketTransform);
		}

		_numberOfBulletProp->SetValue(_numberOfBulletProp->GetValue() - 1);
		bIsReloading = ChangeNumberOfBullet(_numberOfBulletProp->GetValue());

		FVector beamEnd;
		bool bBeamEnd = GetBeamEndLocation(socketTransform.GetLocation(), beamEnd);
		if (bBeamEnd)
		{
			if (_impatctParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _impatctParticle, beamEnd);
			}

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
		}

		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance && _hipFireMontage)
		{
			animInstance->Montage_Play(_hipFireMontage);
			animInstance->Montage_JumpToSection(FName("StartFire"));
		}*/
	


	/*if (_bulletActor)
	{
		FVector location = GetActorLocation() + GetActorForwardVector() * 500.0f;
		FRotator rotation = GetActorRotation();

		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();

		AEnemyBulletActor* bullet = GetWorld()->SpawnActor<AEnemyBulletActor>(_bulletActor, location, rotation, spawnParams);

		if (bullet)
		{
			bullet->Initialized(this);
		}

		TimeManagerUtility::GetInstance().Delay(GetWorld(), this, &ASniperEnemyActor::CreateBulletActor, 1.0f, _createBulletTimerHandle);

	}*/
}

void ASniperEnemyActor::SetupCurrentPatrolArea()
{
}

void ASniperEnemyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetLevelManager()->SetPatrolAreaMap(nullptr, _nextPosition);

	TimeManagerUtility::GetInstance().Cancel(GetWorld(), _createBulletTimerHandle);
}

void ASniperEnemyActor::SelfDestroy()
{

	Super::SelfDestroy();
}
