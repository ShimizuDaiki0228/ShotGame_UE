// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Public/TPS_ShotCharacter.h"
#include "../Public/SpawnManager.h"
#include "../Niagara/Public/ExplosionEffect.h"
#include "../Widget/Public/EnemyHpBarUserWidget.h"
#include "TPS_Shot/DesignPattern/ObjectPattern/Public/PooledObjectActor.h"
#include "../Public/CharacterWidgetController.h"
#include "EnemyActor.generated.h"

class AMyPlayerControllerBase;
class ARPGCharacterBase;

UCLASS()
class TPS_SHOT_API AEnemyActor : public ARPGCharacterBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SelfDestroy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* _mesh;

	

protected:
	UPROPERTY()
	ATPS_ShotCharacter* _character;

public:
	virtual void Initialized(ATPS_ShotCharacter* character);

	UPooledObjectActorComponent* Explosion();
public:
	// hp��������̒l�ȉ��ɂȂ����ꍇ�ɔj�����ăX�R�A���グ�邽�߂�bool�^
	void DecreaseHP(int damage);

public:
	ATPS_ShotCharacter* GetTarget() const { return _character; }
	UStaticMeshComponent* GetMesh() const { return _mesh; }


private:
	const int MAX_HP = 500;

	UPROPERTY()
	USpawnManager* _explosionEffectSpawnManager;

	FTimerHandle _destroyTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AExplosionEffect> _explosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundBase* _explosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UEnemyHpBarUserWidget> _healthBarComponent;

	UPROPERTY()
	UEnemyHpBarUserWidget* _healthBarWidget;

	UPROPERTY()
	AMyPlayerControllerBase* _cachedPlayerController;

	UPROPERTY()
	ATPS_ShotGameMode* _gameMode;

	UPROPERTY()
	UCharacterWidgetController* _characterWidgetController;
	
private:
	TSharedPtr<ReactiveProperty<int>> _currentHpProp = MakeShared<ReactiveProperty<int>>();

public:
	TSharedPtr<ReadOnlyReactiveProperty<int>> CurrentHpProp
		= MakeShared<ReadOnlyReactiveProperty<int>>(_currentHpProp);
};
