// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/EnemyActor.h"
#include "../Utility/Public/SoundManagerUtility.h"
#include "../Utility/Public/TimeManagerUtility.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "../DesignPattern/ObjectPattern/UObject/Widget/Public/PooledUText.h"

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

	_cachedPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	_healthBarWidget = CreateWidget<UEnemyHpBarUserWidget>(GetWorld(), _healthBarComponent);
	_healthBarWidget->ManualBeginPlay();

	_characterWidgetController = NewObject<UCharacterWidgetController>();
	_characterWidgetController->Initialized(this, _cachedPlayerController);
	_characterWidgetController->SetWidgetSetting(
		_healthBarWidget,
		_healthBarWidget->HPBAR_CLAMP_SIZE_MIN,
		_healthBarWidget->HPBAR_CLAMP_SIZE_MAX,
		_healthBarWidget->GetHpBar()->GetDesiredSize().Y);

	CurrentHpProp->OnValueChanged.AddLambda([this](const int& newValue)
		{
			_healthBarWidget->UpdateHpBar(static_cast<float>(newValue) / static_cast<float>(MAX_HP));
		});
}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_characterWidgetController->SetWidgetSetting(
		_healthBarWidget,
		_healthBarWidget->HPBAR_CLAMP_SIZE_MIN,
		_healthBarWidget->HPBAR_CLAMP_SIZE_MAX,
		_healthBarWidget->GetHpBar()->GetDesiredSize().Y);
}

void AEnemyActor::SelfDestroy()
{
	TimeManagerUtility::GetInstance().Cancel(GetWorld(), _destroyTimerHandle);

	// ウィジェットを削除
	if (_healthBarWidget)
	{
		_healthBarWidget->RemoveFromParent();
		_healthBarWidget = nullptr; // ポインタを無効化
	}
	
	Destroy();
}

UPooledObjectActorComponent* AEnemyActor::Explosion()
{
	if (_explosionEffect)
	{
		if (_explosionSound)
		{
			SoundManagerUtility::GetInstance().Play(_explosionSound, this);
		}

		UPooledObjectActorComponent* object = ALevelManager::GetInstance()->GetEnemyExplosionPool()->GetPooledObject(this);
		AExplosionEffect* spawnedExplosionEffect = static_cast<AExplosionEffect*>(object->GetOwner());
		if (spawnedExplosionEffect)
		{
			spawnedExplosionEffect->Initialized(GetTarget());

			// TODO
			// 実装する
			// 	TimeManagerUtility::GetInstance().Delay(GetWorld(), [this, spawnedExplosionEffect]()
			// 		{
			// 			spawnedExplosionEffect->Destroy();
			// 		}, 3.0f, _destroyTimerHandle);
		}
		
		SelfDestroy();

		return object;
	}

	return nullptr;
}

void AEnemyActor::Initialized(ATPS_ShotCharacter* character)
{
	_character = character;

	_currentHpProp->SetValue(MAX_HP);
}

bool AEnemyActor::DecreaseHP(int damage)
{
	if (_currentHpProp.IsValid())
	{
		_currentHpProp->SetValue(_currentHpProp->GetValue() - damage);
		// _currentHpProp->SetValue(0);

		ALevelManager::GetInstance()->GetUserWidgetPool()->GetPooledObject(FString::FromInt(damage), _characterWidgetController);
		
		
		if (_currentHpProp->GetValue() <= 0)
		{
			UPooledObjectActorComponent* explosionPoolActor = Explosion();
			return true;
		}
	}
	return false;
}
