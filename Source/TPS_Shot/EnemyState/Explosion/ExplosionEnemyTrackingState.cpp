// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionEnemyTrackingState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../../Utility/TimeManagerUtility.h"

void ExplosionEnemyTrackingState::EnterState(AExplosionEnemyActor* enemy)
{

	_targetDistance = CalculateTargetDistance(enemy);

	FVector enemyLocation = enemy->GetActorLocation();
	FVector targetLocation = enemy->GetTarget()->GetActorLocation();

	// ターゲットの方向へのベクトルを計算し、正規化
	FVector DirectionToTarget = (targetLocation - enemyLocation).GetSafeNormal();

	// 現在の進行方向を取得
	FVector CurrentForward = enemy->GetActorForwardVector();

	//// 進行方向に力を加える
	FVector Force = DirectionToTarget * 200000.0f;
	enemy->GetMesh()->AddImpulse(Force);

	//MaterialSetting(enemy);

	TimeManagerUtility::GetInstance().Delay(enemy->GetWorld(), [this, enemy]()
		{
			enemy->Explosion();
		}, EXPLODE_TIME, _explodeTimerHandle);
	_timerHandles.Add(_explodeTimerHandle);
}

void ExplosionEnemyTrackingState::UpdateState(AExplosionEnemyActor* enemy)
{
	
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



// 色を変更させようとしたがクラッシュして上手くいかなかったためいったん放置
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
//			// 適当な色をセット
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
