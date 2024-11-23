// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Utility/SoundManagerUtility.h"
#include "../Utility/TimeManagerUtility.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = _mesh;
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		return;
	}

	_healthBarWidget = CreateWidget<UEnemyHpBarUserWidget>(GetWorld(), _healthBarComponent);
	if (_healthBarWidget.IsValid())
	{
		_healthBarWidget->AddToViewport();
	}

	FVector2D screenPosition;
	bool bProjected = UGameplayStatics::ProjectWorldToScreen(PlayerController, GetActorLocation(), screenPosition);

	if (bProjected)
	{
		// �E�B�W�F�b�g�̃T�C�Y���擾
		FVector2D widgetSize = _healthBarWidget->GetDesiredSize();
		FVector2D centeredPosition = screenPosition - (widgetSize);

		// �X�N���[�����W���E�B�W�F�b�g�̈ʒu�ɓK�p
		_healthBarWidget->SetVisibility(ESlateVisibility::Visible);
		_healthBarWidget->SetPositionInViewport(centeredPosition, true);
	}
	else
	{
		// ��ʊO�̏ꍇ�A�E�B�W�F�b�g���\���ɂ���i�I�v�V�����j
		_healthBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	CurrentHpProp->OnValueChanged.AddLambda([this](const int& newValue)
		{
			_healthBarWidget->UpdateHpBar(static_cast<float>(newValue) / static_cast<float>(MAX_HP));
		});
}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		return;
	}
	if (_healthBarWidget != nullptr)
	{
		_healthBarWidget->AddToViewport();
	}

	FVector2D screenPosition;
	bool bProjected = UGameplayStatics::ProjectWorldToScreen(PlayerController, GetActorLocation() + FVector(0, 0, 100), screenPosition);

	if (bProjected)
	{
		// �E�B�W�F�b�g�̃T�C�Y���擾
		FVector2D widgetSize = _healthBarWidget->GetDesiredSize();
		FVector2D centeredPosition = screenPosition - (widgetSize * 0.5);

		// �X�N���[�����W���E�B�W�F�b�g�̈ʒu�ɓK�p
		_healthBarWidget->SetVisibility(ESlateVisibility::Visible);
		_healthBarWidget->SetPositionInViewport(centeredPosition, true);
	}
	else
	{
		// ��ʊO�̏ꍇ�A�E�B�W�F�b�g���\���ɂ���i�I�v�V�����j
		_healthBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AEnemyActor::SelfDestroy()
{
	TimeManagerUtility::GetInstance().Cancel(GetWorld(), _destroyTimerHandle);

	// ウィジェットを削除
	if (_healthBarWidget.IsValid())
	{
		_healthBarWidget->RemoveFromParent();
		_healthBarWidget = nullptr; // ポインタを無効化
	}
	
	Destroy();
}

AExplosionEffect* AEnemyActor::Explosion()
{
	if (_explosionEffect)
	{
		if (_explosionSound)
		{
			SoundManagerUtility::GetInstance().Play(_explosionSound, this);
		}
		
		_explosionEffectSpawnManager = NewObject<USpawnManager>();
		FActorSpawnParameters explodeEffectSpawnParameters;
		explodeEffectSpawnParameters.Owner = this;
		explodeEffectSpawnParameters.Instigator = GetInstigator();
		FVector location = GetActorLocation();
		_explosionEffectSpawnManager->SetUp(explodeEffectSpawnParameters, location);

		AExplosionEffect* spawnedExplosionEffect = _explosionEffectSpawnManager->SpawnActor(_explosionEffect);
		
		if (spawnedExplosionEffect)
		{
			spawnedExplosionEffect->Initialized(GetTarget());
			TimeManagerUtility::GetInstance().Delay(GetWorld(), [this, spawnedExplosionEffect]()
				{
					spawnedExplosionEffect->Destroy();
				}, 3.0f, _destroyTimerHandle);
		}
		
		SelfDestroy();

		return spawnedExplosionEffect;
	}

	return nullptr;
}

void AEnemyActor::Initialized(ATPS_ShotCharacter* character, ALevelManager* levelManager)
{
	_character = character;
	_levelManager = levelManager;

	_currentHpProp->SetValue(MAX_HP);
}

bool AEnemyActor::DecreaseHP(int damage)
{
	if (_currentHpProp.IsValid())
	{
		_currentHpProp->SetValue(_currentHpProp->GetValue() - damage);
		
		if (_currentHpProp->GetValue() <= 0)
		{
			SelfDestroy();
			return true;
		}
	}
	return false;
}
