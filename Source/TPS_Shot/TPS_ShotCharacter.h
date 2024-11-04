// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Sound/SoundCue.h"
#include "ReactiveProperty/ReactiveProperty.h"
#include "ReactiveProperty/ReadonlyReactiveProperty.h"
#include "TPS_ShotCharacter.generated.h"

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

	void CreateBullet();

	bool bCanShot;

	bool ChangeNumberOfBullet(int currentNumberOfBullet);
	bool bIsReloading;
	int _currentLoadNumber;

	void ChangeNotUseShot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class USoundCue* _fireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* _muzzleFlash;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* _hipFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* _reloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* _impatctParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* _beamParticle;

	bool GetBeamEndLocation(const FVector& muzzleSocketLocation, FVector& outBeamLocation);

	TSharedPtr<ReactiveProperty<int>> _currentHPProp = MakeShared<ReactiveProperty<int>>();

	TSharedPtr<ReactiveProperty<int>> _scoreProp = MakeShared<ReactiveProperty<int>>();

	TSharedPtr<ReactiveProperty<int>> _numberOfBulletProp = MakeShared<ReactiveProperty<int>>();

	void Reload();

	bool bAiming;

private:
	FTimerHandle _reloadTimerHandle;

public:
	void ChangeHP(int newHP);

	TSharedPtr<ReadOnlyReactiveProperty<int>> CurrentHPProp
		= MakeShared<ReadOnlyReactiveProperty<int>>(_currentHPProp);

	TSharedPtr<ReadOnlyReactiveProperty<int>> ScoreProp
		= MakeShared<ReadOnlyReactiveProperty<int>>(_scoreProp);

	TSharedPtr<ReadOnlyReactiveProperty<int>> NumberOfBulletProp 
		= MakeShared<ReadOnlyReactiveProperty<int>>(_numberOfBulletProp);

	FORCEINLINE bool GetAiming() const { return bAiming; }

public:
	void Initialized();
	void Bind();
	void Reset();

public:
	FORCEINLINE int GetHP() const { return _currentHPProp->GetValue(); }
};

