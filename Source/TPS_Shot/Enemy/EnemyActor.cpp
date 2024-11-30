// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Utility/SoundManagerUtility.h"
#include "../Utility/TimeManagerUtility.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "../ActorScreenSizeCalculator.h"

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
	SetWidgetSetting(_cachedPlayerController);

	CurrentHpProp->OnValueChanged.AddLambda([this](const int& newValue)
		{
			_healthBarWidget->UpdateHpBar(static_cast<float>(newValue) / static_cast<float>(MAX_HP));
		});
}

void AEnemyActor::SetWidgetSetting(TWeakObjectPtr<APlayerController> playerController)
{
	if (!playerController.IsValid())
	{
		return;
	}

	FVector2D screenPosition;
	bool bProjected = UGameplayStatics::ProjectWorldToScreen(playerController.Get(), GetActorLocation() + FVector(0, 0, 100), screenPosition);

	if (bProjected)
	{
		if (!_healthBarWidget->IsInViewport())
		{
			_healthBarWidget->AddToViewport();
		}
		
		float thisScreenSize = UActorScreenSizeCalculator::CalculateScreenSize(this,
			playerController.Get(),
			_healthBarWidget->HPBAR_CLAMP_SIZE_MIN,
			_healthBarWidget->HPBAR_CLAMP_SIZE_MAX
			);

		if (thisScreenSize != 0)
		{
			UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(_healthBarWidget->GetHpBar()->Slot);
			UCanvasPanelSlot* canvasSlot2 = Cast<UCanvasPanelSlot>(_healthBarWidget->GetDamageBar()->Slot);
			_healthBarWidget->TestSetSize(canvasSlot, thisScreenSize, canvasSlot->GetSize().Y);
			_healthBarWidget->TestSetSize(canvasSlot2, thisScreenSize, canvasSlot2->GetSize().Y);
			// _healthBarWidget->SetSize(thisScreenSize, _healthBarWidget->GetHpBar()->GetDesiredSize().Y);
		}

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

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetWidgetSetting(_cachedPlayerController);
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

UPooledObjectActorComponent* AEnemyActor::Explosion()
{
	if (_explosionEffect)
	{
		if (_explosionSound)
		{
			SoundManagerUtility::GetInstance().Play(_explosionSound, this);
		}

		UPooledObjectActorComponent* object = _levelManager->GetEnemyExplosionPool()->GetPooledObject(this);
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
		// _currentHpProp->SetValue(0);
		
		if (_currentHpProp->GetValue() <= 0)
		{
			UPooledObjectActorComponent* explosionPoolActor = Explosion();
			return true;
		}
	}
	return false;
}
