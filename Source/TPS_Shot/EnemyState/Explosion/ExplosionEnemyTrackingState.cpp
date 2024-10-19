// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionEnemyTrackingState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../../Utility/TimeManagerUtility.h"

void ExplosionEnemyTrackingState::EnterState(AExplosionEnemyActor* enemy)
{

	_targetDistance = CalculateTargetDistance(enemy);

	FVector enemyLocation = enemy->GetActorLocation();
	FVector targetLocation = enemy->GetTarget()->GetActorLocation();

	// �^�[�Q�b�g�̕����ւ̃x�N�g�����v�Z���A���K��
	FVector DirectionToTarget = (targetLocation - enemyLocation).GetSafeNormal();

	// ���݂̐i�s�������擾
	FVector CurrentForward = enemy->GetActorForwardVector();

	// �^�[�Q�b�g�����Ɍ����Ẵg���N�i��]�́j���v�Z
	FVector TorqueDirection = FVector::CrossProduct(CurrentForward, DirectionToTarget) * 500.0f;  // 500.0f�͉�]�͂̃X�P�[��
	enemy->GetMesh()->AddTorqueInRadians(TorqueDirection);

	//// �i�s�����ɗ͂�������
	FVector Force = DirectionToTarget * 300000.0f;
	enemy->GetMesh()->AddImpulse(Force);

	_updateEnable = true;

	//MaterialSetting(enemy);

	TimeManagerUtility::GetInstance().Delay(enemy->GetWorld(), [this, enemy]() {
		ChangeUpdateBrakeEnable(enemy);
		}, UPDATE_ENABLE_INTERVAL, _brakeTimerHandle);
	_timerHandles.Add(_brakeTimerHandle);

	TimeManagerUtility::GetInstance().Delay(enemy->GetWorld(), [this, enemy]()
		{
			enemy->Explosion();
		}, EXPLODE_TIME, _explodeTimerHandle);
	_timerHandles.Add(_explodeTimerHandle);
}

void ExplosionEnemyTrackingState::UpdateState(AExplosionEnemyActor* enemy)
{
	if (_updateEnable)
	{
		_updateEnable = false;

		float distance = CalculateTargetDistance(enemy);

		// �����Ɋ�Â��đ��x������������
		FVector currentVelocity = enemy->GetMesh()->GetPhysicsLinearVelocity();

		// �����ɉ��������������v�Z
		float speedFactor = FMath::Clamp(distance / _targetDistance, 0.0f, 0.95f);

		// ���x������������
		// ���x���������Ă��܂��\�������邽�ߍő�ł����݂̑��x���班�����Ƃ������x��
		FVector newVelocity = currentVelocity * speedFactor;
		enemy->GetMesh()->SetPhysicsLinearVelocity(newVelocity);
	}
}

void ExplosionEnemyTrackingState::ExitState(AExplosionEnemyActor* enemy)
{
	if (_timerHandles.Num() > 0)
	{
		for (auto timerHandle : _timerHandles)
		{
			TimeManagerUtility::GetInstance().Cancel(enemy->GetWorld(), timerHandle);
		}
	}
}

float ExplosionEnemyTrackingState::CalculateTargetDistance(AExplosionEnemyActor* enemy)
{
    FVector objectLocation = enemy->GetActorLocation();
    FVector playerLocation = enemy->GetTarget()->GetActorLocation();
    return FVector::Dist(objectLocation, playerLocation);
}

void ExplosionEnemyTrackingState::ChangeUpdateBrakeEnable(AExplosionEnemyActor* enemy)
{
	// �t���O�� true �ɐݒ�
	_updateEnable = true;

	// �Ă�n�b��Ƀt���O��true�ɂ��邽�߂ɍēxDelay���Ăяo��
	TimeManagerUtility::GetInstance().Delay(enemy->GetWorld(), [this, enemy]()
		{
			// �m��ł͂Ȃ����AState���ς�����Ƃ��ɌĂяo����ăN���b�V�����Ă���\��������
			if (this)
			{
				ChangeUpdateBrakeEnable(enemy);
			}
		}, UPDATE_ENABLE_INTERVAL, _brakeTimerHandle);
}



// �F��ύX�����悤�Ƃ������N���b�V�����ď�肭�����Ȃ��������߂���������u
//void ExplosionEnemyTrackingState::MaterialSetting(AExplosionEnemyActor* enemy)
//{
//	UStaticMeshComponent* mesh = enemy->GetMesh();
//
//	UMaterialInterface* originMaterial = mesh->GetMaterial(0);
//	UMaterialInterface* ExplodeMaterial = mesh->GetMaterial(1);
//
//	_materialInstance = UMaterialInstanceDynamic::Create(originMaterial, enemy);
//
//	mesh->SetMaterial(0, _materialInstance);
//
//	FLinearColor startColor = GetMaterialColor(originMaterial, TEXT("M_Floor_01"));
//	FLinearColor endColor = GetMaterialColor(ExplodeMaterial, TEXT("M_Floor_01"));
//
//	StartColorChange(_materialInstance, enemy, startColor, endColor);
//}
//
//FLinearColor ExplosionEnemyTrackingState::GetMaterialColor(UMaterialInterface* material, FName parameterName)
//{
//	FLinearColor color;
//	UMaterialInstanceDynamic* dynamicMaterialInstance = Cast<UMaterialInstanceDynamic>(material);
//
//	if (dynamicMaterialInstance)
//	{
//		dynamicMaterialInstance->GetVectorParameterValue(parameterName, color);
//	}
//	else
//	{
//		UMaterial* baseMaterial = material->GetBaseMaterial();
//		if (baseMaterial)
//		{
//			// �K���ȐF���Z�b�g
//			color = FLinearColor::White;
//		}
//	}
//
//	return color;
//}
//
//void ExplosionEnemyTrackingState::StartColorChange(
//	UMaterialInstanceDynamic* materialInstance,
//	AExplosionEnemyActor* enemy,
//	FLinearColor startColor,
//	FLinearColor endColor)
//{
//	_timeElapsed = 0.0f;
//	
//	TimeManagerUtility::GetInstance().Delay(enemy->GetWorld(), [this, materialInstance, enemy, startColor, endColor]()
//		{
//			UpdateColor(materialInstance, enemy, startColor, endColor);
//		}, EXPLODE_INTERVAL_TIME);
//}
//
//void ExplosionEnemyTrackingState::UpdateColor(
//	UMaterialInstanceDynamic* materialInstance,
//	AExplosionEnemyActor* enemy,
//	FLinearColor startColor,
//	FLinearColor endColor)
//{
//	_timeElapsed += 0.1f;
//	float alpha = FMath::Clamp(_timeElapsed / COLOR_CHANGE_DURATION, 0.0f, 1.0f);
//	FLinearColor currentColor = FMath::Lerp(startColor, endColor, alpha);
//
//	materialInstance->SetVectorParameterValue(FName("M_Floor_01"), currentColor);
//
//	TimeManagerUtility::GetInstance().Delay(enemy->GetWorld(), [this, materialInstance, enemy, startColor, endColor]()
//		{
//			UpdateColor(materialInstance, enemy, startColor, endColor);
//		}, EXPLODE_INTERVAL_TIME);
//}
//
//
