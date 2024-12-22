// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorScreenSizeCalculator.h"

#include "GameFramework/PlayerController.h"

// 画面に描画されているアクターのサイズを取得
FVector2D UActorScreenSizeCalculator::CalculateScreenSize(const AActor* actor, const APlayerController* playerController)
{
	FVector2D min, max;
	if (GetScreenBounds(actor, playerController, min, max))
	{
		return FVector2D( FMath::Abs(max.X - min.X),  FMath::Abs(max.Y - min.Y));
	}
	
	return FVector2D::ZeroVector;
}

bool UActorScreenSizeCalculator::GetScreenBounds(const AActor* actor, const APlayerController* playerController,
	FVector2D& outMin, FVector2D& outMax)
{
	if (playerController && actor)
	{
		FVector origin, boxExtent;
		actor->GetActorBounds(false, origin, boxExtent);

		// カメラの距離でアクターの描画サイズが変わるのでそれに合わせる
		FVector cameraLocation = playerController->PlayerCameraManager->GetCameraLocation();
		float distanceToCamera = FVector::Dist(cameraLocation, origin);
		
		float scaledSize = BASE_SIZE * (SCALE_FACTOR / distanceToCamera);
		
		outMin = FVector2D(-scaledSize / 2, -scaledSize / 2);
		outMax = FVector2D(scaledSize / 2, scaledSize / 2); 
		
		return true;
	}

	return false;
}
