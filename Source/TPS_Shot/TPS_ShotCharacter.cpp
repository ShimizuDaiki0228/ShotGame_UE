// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPS_ShotCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/TimeManagerUtility.h"
#include "Utility/ConstUtility.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "ShotCharacterPlayerState.h"
#include "Particles/ParticleSystemComponent.h"
#include "Utility/SoundManagerUtility.h"
#include "TPS_ShotGameMode.h"
#include "Enemy/EnemyActor.h"
#include "GameFramework/GameMode.h"
#include "PlayerBehaviourController.h"

//////////////////////////////////////////////////////////////////////////
// ATPS_ShotCharacter

ATPS_ShotCharacter::ATPS_ShotCharacter()
{
	_behaviourController = CreateDefaultSubobject<UPlayerBehaviourController>(TEXT("BehaviourController"));
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ATPS_ShotCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld() != nullptr)
	{
		TimeManagerUtility::GetInstance().Cancel(GetWorld(), _reloadTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void ATPS_ShotCharacter::BeginDestroy()
{
	Super::BeginDestroy();
}

void ATPS_ShotCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (auto gameModeBase = GetWorld()->GetAuthGameMode())
	{
		if (auto shotGameMode = Cast<ATPS_ShotGameMode>(gameModeBase))
		{
			_shotGameMode = TWeakObjectPtr<ATPS_ShotGameMode>(shotGameMode);
			if (!_shotGameMode.IsValid())
			{
				UKismetSystemLibrary::PrintString(this, TEXT("don't Get GameMode"), true, true, FColor::Red);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATPS_ShotCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPS_ShotCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPS_ShotCharacter::Look);

		EnhancedInputComponent->BindAction(ShotAction, ETriggerEvent::Triggered, this, &ATPS_ShotCharacter::CreateBullet);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ATPS_ShotCharacter::Reload);
	}

}

void ATPS_ShotCharacter::CreateBullet()
{
	if (!bCanShot) return;

	if (bIsReloading) return;


	ChangeNotUseShot();

	if (_fireSound)
	{
		SoundManagerUtility::GetInstance().Play(_fireSound, this);
	}

	const USkeletalMeshSocket* barrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	if (barrelSocket)
	{
		const FTransform socketTransform = barrelSocket->GetSocketTransform(GetMesh());

		if (_muzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _muzzleFlash, socketTransform);
		}

		_numberOfBulletProp->SetValue(_numberOfBulletProp->GetValue() - 1);
		bIsReloading = ChangeNumberOfBullet(_numberOfBulletProp->GetValue());

		FVector beamEnd;
		bool bBeamEnd = GetBeamEndLocation(socketTransform.GetLocation(), beamEnd);
		if (bBeamEnd)
		{
			if (_impatctParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _impatctParticle, beamEnd);
			}

			if (_beamParticle)
			{
				UParticleSystemComponent* beam = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					_beamParticle,
					socketTransform);
				if (beam)
				{
					beam->SetVectorParameter(FName("Target"), beamEnd);
				}
			}
		}
		
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance && _hipFireMontage)
		{
			animInstance->Montage_Play(_hipFireMontage);
			animInstance->Montage_JumpToSection(FName("StartFire"));
		}
	}
}

void ATPS_ShotCharacter::ChangeNotUseShot()
{
	bCanShot = false;

	TimeManagerUtility::GetInstance().Delay(GetWorld(), [this]()
		{
			bCanShot = true;
		}, 0.2f, _reloadTimerHandle);
}

bool ATPS_ShotCharacter::ChangeNumberOfBullet(int currentNumberOfBullet)
{
	if (currentNumberOfBullet <= 0)
	{
		// �񓯊��Ń����[�h�A�j���[�V�����ƃ����[�h�e�L�X�g�A�j���[�V������s����bIsReloading��false�ɂ�����
		return true;
	}
	return false;
}



void ATPS_ShotCharacter::Move(const FInputActionValue& Value)
{
	_behaviourController->Move(Value);
}

void ATPS_ShotCharacter::Look(const FInputActionValue& Value)
{
	_behaviourController->Look(Value);
}

void ATPS_ShotCharacter::Reload()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && _reloadMontage)
	{
		UKismetSystemLibrary::PrintString(this, "Reload Animation", true, true, FColor::Cyan, 2.f, TEXT("None"));

		// TODO�@����𕡐��g�p����ꍇ�A����̃^�C�v�ɂ���ĕύX�ł���悤�ɂ���K�v������
		FName montageSection(TEXT("Reload SMG"));

		animInstance->Montage_Play(_reloadMontage);
		animInstance->Montage_JumpToSection(montageSection);
	}
}

void ATPS_ShotCharacter::GrabClip()
{
	
}

void ATPS_ShotCharacter::ReleaseClip()
{
}

void ATPS_ShotCharacter::ChangeHP(int newHP)
{
	_currentHPProp->SetValue(newHP);
}

void ATPS_ShotCharacter::Initialized()
{
	_behaviourController->Initialized(this);
	Bind();
}

void ATPS_ShotCharacter::Bind()
{
	
}

void ATPS_ShotCharacter::Reset()
{
	bCanShot = true;

	_currentLoadNumber = MAX_BULLET_NUMBER;
	_numberOfBulletProp->SetValue(MAX_BULLET_NUMBER);
	_currentHPProp->SetValue(ConstUtility::PLAYER_MAX_HP);
}

void ATPS_ShotCharacter::GameOver()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMovementComponent()->MovementState.bCanJump = false;
}

bool ATPS_ShotCharacter::GetBeamEndLocation(const FVector& muzzleSocketLocation, FVector& outBeamLocation)
{
	FVector2D viewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(viewPortSize);
	}

	FVector2D crosshairLocation = FVector2D(viewPortSize.X / 2, viewPortSize.Y / 2);
	FVector crosshairWorldPosition;
	FVector crosshairWorldDirection;

	// �X�N���[�����W�����[���h���W�ɕϊ�
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		crosshairLocation,
		crosshairWorldPosition,
		crosshairWorldDirection);

	if (bScreenToWorld)
	{
		FHitResult screenTraceHit;
		const FVector start = crosshairWorldPosition;
		const FVector end = crosshairWorldPosition + crosshairWorldDirection * 50'000.f;

		outBeamLocation = end;

		GetWorld()->LineTraceSingleByChannel(screenTraceHit, start, end, ECollisionChannel::ECC_Visibility);

		if (screenTraceHit.bBlockingHit)
		{
			outBeamLocation = screenTraceHit.Location;

			AActor* hitActor = screenTraceHit.GetActor();
			if (hitActor)
			{
				AEnemyActor* enemy = Cast<AEnemyActor>(hitActor);
				if (enemy)
				{
					// �����ƃ_���[�W��ݒ肷��A���͎���
					if (enemy->DecreaseHP(100))
					{
						auto playerState = _shotGameMode->GetPlayerState();
						playerState->ChangeScore(playerState->GetScore() + 1);
					}
				}
			}
		}

		// �N���X�w�A����̃g���[�X�ƕ��킩��̃g���[�X�ō��ق����܂�邽�߂�萳�m�ɂ��邽�߂�
		FHitResult weaponTraceHit;
		const FVector weaponTraceStart = muzzleSocketLocation;
		const FVector weaponTraceEnd = outBeamLocation;
		GetWorld()->LineTraceSingleByChannel(
			weaponTraceHit,
			weaponTraceStart,
			weaponTraceEnd,
			ECollisionChannel::ECC_Visibility
		);
		if (weaponTraceHit.bBlockingHit)
		{
			outBeamLocation = weaponTraceHit.Location;
		}

		return true;
	}

	return false;
}

