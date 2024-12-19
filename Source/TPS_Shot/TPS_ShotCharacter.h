// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Dispose/Observer.h"
#include "Dispose/Subject.h"
#include "Sound/SoundCue.h"
#include "ReactiveProperty/ReactiveProperty.h"
#include "ReactiveProperty/ReadonlyReactiveProperty.h"
#include "TPS_ShotCharacter.generated.h"

class AShotCharacterPlayerState;
class ATPS_ShotGameMode;
class UPlayerBehaviourController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeHPBar, float, newHP);

UCLASS(config=Game)
class ATPS_ShotCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShotAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

public:
	ATPS_ShotCharacter();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual void BeginDestroy() override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	const int MAX_BULLET_NUMBER = 10;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> _bulletActor;

	bool bCanShot;
	bool bIsReloading;
	bool bAiming;
	bool bMovingClip;

	FTimerHandle _reloadTimerHandle;

	int _currentLoadNumber;

	UPROPERTY()
	UAnimInstance* _animInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class USoundCue* _fireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* _muzzleFlash;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* _hipFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* _reloadMontage;

	UPROPERTY()
	AShotCharacterPlayerState* _shotCharacterPlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* _impatctParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* _beamParticle;

	UPROPERTY(VisibleDefaultsOnly, Category = "ActorComponent")
	UPlayerBehaviourController* _behaviourController;

	UPROPERTY()
	TWeakObjectPtr<ATPS_ShotGameMode> _shotGameMode;

private:
	TSharedPtr<ReactiveProperty<int>> _currentHPProp = MakeShared<ReactiveProperty<int>>();
	
	TSharedPtr<ReactiveProperty<int>> _numberOfBulletProp = MakeShared<ReactiveProperty<int>>();
	
private:
	void ReleaseClip();
	void GrabClip();
	void Reload();
	void ChangeNotUseShot();
	void CreateBullet();

	bool GetBeamEndLocation(const FVector& muzzleSocketLocation, FVector& outBeamLocation);

public:
	TSharedPtr<ReadOnlyReactiveProperty<int>> CurrentHPProp
		= MakeShared<ReadOnlyReactiveProperty<int>>(_currentHPProp);

	TSharedPtr<ReadOnlyReactiveProperty<int>> NumberOfBulletProp 
		= MakeShared<ReadOnlyReactiveProperty<int>>(_numberOfBulletProp);

	FORCEINLINE bool GetAiming() const { return bAiming; }

public:
	void Initialized();
	void Bind();
	void SetEvent();
	void Reset();
	void GameOver();

public:
	void ChangeHP(int newHP);
	
public:
	FORCEINLINE int GetHP() const { return _currentHPProp->GetValue(); }
};

