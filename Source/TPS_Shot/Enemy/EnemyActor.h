// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../TPS_ShotCharacter.h"
#include "../EnemyState/IEnemyState.h"
#include "../LevelManager.h"
#include "../SpawnManager.h"
#include "../Niagara/ExplosionEffect.h"
#include "../Utility/SoundManagerUtility.h"
#include "Components/WidgetComponent.h"
#include "../Widget/EnemyHpBarUserWidget.h"
#include "TPS_Shot/DesignPattern/ObjectPattern/PooledObjectActorComponent.h"
#include "EnemyActor.generated.h"


UCLASS()
class TPS_SHOT_API AEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetWidgetSetting(TWeakObjectPtr<APlayerController> playerController);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SelfDestroy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* _mesh;

	

protected:
	ATPS_ShotCharacter* _character;

	ALevelManager* _levelManager;

public:
	virtual void Initialized(ATPS_ShotCharacter* character, ALevelManager* levelManager);

	UPooledObjectActorComponent* Explosion();
public:
	// hp��������̒l�ȉ��ɂȂ����ꍇ�ɔj�����ăX�R�A���グ�邽�߂�bool�^
	bool DecreaseHP(int damage);

public:
	ATPS_ShotCharacter* GetTarget() const { return _character; }
	UStaticMeshComponent* GetMesh() const { return _mesh; }


private:
	const int MAX_HP = 500;
	//int _hp;

	USpawnManager* _explosionEffectSpawnManager;

	FTimerHandle _destroyTimerHandle;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AExplosionEffect> _explosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundBase* _explosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UEnemyHpBarUserWidget> _healthBarComponent;
	TWeakObjectPtr<UEnemyHpBarUserWidget> _healthBarWidget;

	TWeakObjectPtr<APlayerController> _cachedPlayerController;
	
private:
	TSharedPtr<ReactiveProperty<int>> _currentHpProp = MakeShared<ReactiveProperty<int>>();

public:
	TSharedPtr<ReadOnlyReactiveProperty<int>> CurrentHpProp
		= MakeShared<ReadOnlyReactiveProperty<int>>(_currentHpProp);
};
