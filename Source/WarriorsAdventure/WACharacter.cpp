// Fill out your copyright notice in the Description page of Project Settings.


#include "WACharacter.h"
#include "WAPlayerController.h"
#include "WAAIController.h"
#include "WAAnimInstance.h"
#include "WAStatHudUserWidget.h"
#include "Widgets/WACharacterWidget.h"
#include "Components/CharacterStatComponent.h"
#include "Kismet/KismetMathLibrary.h"	
#include "Components/WidgetComponent.h"
#include "CharacterSkillDataTable.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AWACharacter::AWACharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), FRotator(0.0f, -90.0f, 0.0f));

	// Set controller properties.
	bUseControllerRotationYaw = false;

	// Set Character Movement Component.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxAcceleration = 1200.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;

	// Create Character Audio Component
	CharacterAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("CHARACTERAUDIO"));
	CharacterAudio->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USoundBase> SC_BEHIT(TEXT("SoundCue'/Game/Sounds/Hit/Hit_Sound_Cue.Hit_Sound_Cue'"));
	if (SC_BEHIT.Succeeded())
	{
		BeHitSoundCue = SC_BEHIT.Object;
	}

	// Create Character Stat Component
	CharacterStat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("CHARACTERSTAT"));

	// Set collision channel.
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("WACharacter"));
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	// Initialize Camera Shake blueprint.
	static ConstructorHelpers::FClassFinder<UCameraShake> CS_ATTACK(TEXT("Blueprint'/Game/Blueprints/BP_AttackCameraShake.BP_AttackCameraShake_C'"));
	if (CS_ATTACK.Succeeded())
	{
		AttackCameraShake = CS_ATTACK.Class;
	}

	// Create Widget
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh());

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f * GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HPBAR(TEXT("WidgetBlueprint'/Game/UI/UI_CharacterHPBar.UI_CharacterHPBar_C'"));
	if (UI_HPBAR.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HPBAR.Class);
	}
	

	// Set AI Class
	static ConstructorHelpers::FClassFinder<AAIController> BP_WAAICONTROLLER(TEXT("Blueprint'/Game/Blueprints/BP_WAAIController.BP_WAAIController_C'"));
	if (CS_ATTACK.Succeeded())
	{
		AIControllerClass = BP_WAAICONTROLLER.Class;
	}
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	
}

// Called when the game starts or when spawned
void AWACharacter::BeginPlay()
{
	Super::BeginPlay();

	// Widget Binding
	UWACharacterWidget* CharacterWidget = Cast<UWACharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (CharacterWidget != nullptr)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
	
	CharacterStat->OnHPIsZero.AddDynamic(this, &AWACharacter::Dead);
	
	if (IsPlayerControlled())
	{
		Tags.Add(FName(TEXT("Player")));

		AWAPlayerController* PlayerController = Cast<AWAPlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->GetStatHudWidget()->BindCharacterStat(CharacterStat);
		}
	}
	else
	{
		bUseControllerRotationYaw = true;
	}

}

// Called every frame
void AWACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move toward MoveDirection.
	if (GetCharacterAction() == ECharacterAction::Idle)
	{
		Move();
	}

	// Reset wall jump chance.
	if (bCanWallJump == false && GetCharacterMovement()->IsFalling() == false)
	{
		bCanWallJump = true;
	}

	// hit stun push
	if (GetCharacterAction() == ECharacterAction::Hitstun)
	{
		HitstunPushVelocity = FMath::VInterpTo(HitstunPushVelocity, FVector::ZeroVector, DeltaTime, 5.0f);
		
		FVector HitstunPushDirection = HitstunPushVelocity;
		HitstunPushDirection.Normalize();
		FHitResult HitResult;
		FVector LineTraceEnd = GetActorLocation() + HitstunPushDirection * (GetCapsuleComponent()->GetScaledCapsuleRadius());
		SetActorRelativeLocation(GetActorLocation() + HitstunPushVelocity, GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), LineTraceEnd, ECollisionChannel::ECC_WorldStatic));
	}
}

// Called to bind functionality to input
void AWACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void AWACharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UWAAnimInstance>(GetMesh()->GetAnimInstance());
}

void AWACharacter::SetCharacterAction(ECharacterAction NewCharacterAction)
{
	if (GetCharacterAction() == NewCharacterAction)
	{
		return;
	}

	switch (NewCharacterAction)
	{
	case ECharacterAction::Idle:
		CurrentBasicAttackCombo = 0;
		break;

	case ECharacterAction::Attack:
		break;

	case ECharacterAction::Guard:
		CurrentBasicAttackCombo = 0;
		break;

	case ECharacterAction::Evade:
		CurrentBasicAttackCombo = 0;
		break;
	}

	CurrentCharacterAction = NewCharacterAction;
}

void AWACharacter::Attack()
{
	// Dash Attack when dashing in ground.
	if (GetCharacterAction() == ECharacterAction::Idle && bDashing && !GetCharacterMovement()->IsFalling())
	{
		DashEnd();
		SetCharacterAction(ECharacterAction::DashAttack);
		AnimInstance->PlayDashAttackMontage();
	}
	// Jump Attack
	else if (GetCharacterAction() == ECharacterAction::Idle && GetCharacterMovement()->IsFalling())
	{
		SetCharacterAction(ECharacterAction::Attack);
		AnimInstance->PlayJumpAttackMontage();
	}
	// Basic Attack.
	else
	{
		// Progress Combo	
		if (GetCurrentBasicAttackCombo() > 0 && GetCurrentBasicAttackCombo() < GetMaxBasicAttackCombo())
		{
			bNextBasicAttackInputOn = true;
		}
		// Special Attack
		else if (GetCharacterAction() == ECharacterAction::Guard && bCanParry)
		{
			if (GetCharacterMovement()->IsFalling())
			{
				BasicAttack(CurrentBasicAttackCombo);
			}
			else
			{
				SpecialAttack();
			}
		}
		// Basic Attack & Charged Attack
		else if (GetCharacterAction() == ECharacterAction::Idle || GetCharacterAction() == ECharacterAction::Guard)
		{
			if (GetCharacterMovement()->IsFalling())
			{
				BasicAttack(CurrentBasicAttackCombo);
			}
			else
			{
				ChargedBasicAttack();

				// if left mouse button is released, play basic attack.
				// else keep playing charged basic attack.
				GetWorld()->GetTimerManager().SetTimer(ChargedBasicAttackTimerHandle, ChargedBasicAttackTime, false);
			}
		}
	}
}

void AWACharacter::AttackReleased()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ChargedBasicAttackTimerHandle) && GetCurrentBasicAttackCombo() == 0)
	{
		BasicAttack(CurrentBasicAttackCombo);
		GetWorld()->GetTimerManager().ClearTimer(ChargedBasicAttackTimerHandle);
	}
}

void AWACharacter::BasicAttack(int32 NewBasicAttackCombo)
{
	SetCharacterAction(ECharacterAction::Attack);
	CurrentBasicAttackCombo = NewBasicAttackCombo + 1;
	AnimInstance->PlayBasicAttackMontage(CurrentBasicAttackCombo);
}

void AWACharacter::BasicAttackEnd()
{
	CurrentBasicAttackCombo = 0;
	bNextBasicAttackInputOn = false;

	if (GetCharacterAction() == ECharacterAction::Attack)
	{
		SetCharacterAction(ECharacterAction::Idle);
	}
}

void AWACharacter::BasicAttackCombo()
{
	bNextBasicAttackInputOn = false;
	CurrentBasicAttackCombo += 1;
	FMath::Clamp(CurrentBasicAttackCombo, 0, MaxBasicAttackCombo);
	if (CurrentBasicAttackCombo <= MaxBasicAttackCombo)
	{
		AnimInstance->PlayBasicAttackMontage(CurrentBasicAttackCombo);
	}
}

void AWACharacter::ChargedBasicAttack()
{
	SetCharacterAction(ECharacterAction::Attack);
	AnimInstance->PlayChargedBasicAttackMontage();
}

void AWACharacter::SpecialAttack()
{
	SetCharacterAction(ECharacterAction::Attack);
	AnimInstance->PlaySpecialAttackMontage();
}

float AWACharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	FVector PushDirection = (GetActorLocation() - DamageCauser->GetActorLocation());
	PushDirection.Normalize();
	HitstunPushVelocity = 5.0f * PushDirection;
	HitstunPushVelocity.Z = 0.0f;

	return FinalDamage;
}

void AWACharacter::Move()
{
	if (MoveDirection.IsZero())
	{
		bDashing = false;
		AnimInstance->SetDashing(false);
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		GetCharacterMovement()->MaxAcceleration = 1200.0f;

		return;
	}

	AddMovementInput(MoveDirection);
	MoveDirection.Set(0.0f, 0.0f, 0.0f);
}

void AWACharacter::Rotate()
{
	FRotator RotateDirection = MoveDirection.Rotation();
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotateDirection, GetWorld()->GetDeltaSeconds(), 2.5f));
}

void AWACharacter::UpDown(float NewAxisValue)
{
	AnimInstance->SetInputMoveDirectionX(NewAxisValue);

	if (GetCharacterAction() == ECharacterAction::Evade || GetCharacterAction() == ECharacterAction::Hitstun)
	{
		return;
	}

	FVector Direction = NewAxisValue * FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);
	Direction.Z = 0.0f;
	Direction.Normalize();
	MoveDirection += Direction;
	MoveDirection.Normalize();

	if (NewAxisValue != 0.0f)
	{
		if (GetCharacterAction() == ECharacterAction::Idle)
		{
			AnimInstance->Montage_Stop(0.5f);
		}
		else
		{
			Rotate();
		}
	}
}

void AWACharacter::LeftRight(float NewAxisValue)
{
	AnimInstance->SetInputMoveDirectionY(NewAxisValue);

	if (GetCharacterAction() == ECharacterAction::Evade || GetCharacterAction() == ECharacterAction::Hitstun)
	{
		return;
	}

	FVector Direction = NewAxisValue * FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y);
	Direction.Z = 0.0f;
	Direction.Normalize();
	MoveDirection += Direction;
	MoveDirection.Normalize();

	if (NewAxisValue != 0.0f)
	{
		if (GetCharacterAction() == ECharacterAction::Idle)
		{
			AnimInstance->Montage_Stop(0.5f);
		}
		else
		{
			Rotate();
		}
	}
}

void AWACharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void AWACharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AWACharacter::Dash()
{
	if (GetCharacterAction() != ECharacterAction::Idle && GetCharacterMovement()->IsFalling())
	{
		return;
	}

	bDashing = true;
	AnimInstance->SetDashing(true);
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
	GetCharacterMovement()->MaxAcceleration = 2000.0f;
}

void AWACharacter::DashEnd()
{
	bDashing = false;
	AnimInstance->SetDashing(false);
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MaxAcceleration = 1200.0f;
}

void AWACharacter::Jump()
{
	if (GetCharacterAction() != ECharacterAction::Idle)
	{
		return;
	}
	
	Super::Jump();

	// Wall Jump if character can.
	if (GetCharacterMovement()->IsFalling() && bCanWallJump)
	{
		FVector LineTraceStart = GetActorLocation();
		LineTraceStart.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.0f;
		FVector LineTraceEnd = GetActorLocation() + 75.0f * GetActorForwardVector();
		LineTraceEnd.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.0f;
		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Visibility))
		{
			bCanWallJump = false;

			// Add horizontal acceleration according to input direction.
			bool bFrontInput = Cast<APlayerController>(GetController())->IsInputKeyDown(EKeys::W);
			bool bBackInput = Cast<APlayerController>(GetController())->IsInputKeyDown(EKeys::S);
			bool bLeftInput = Cast<APlayerController>(GetController())->IsInputKeyDown(EKeys::A);
			bool bRightInput = Cast<APlayerController>(GetController())->IsInputKeyDown(EKeys::D);
			int AxisX = bFrontInput ? 1 : 0;
			AxisX += bBackInput ? -1 : 0;
			int AxisY = bRightInput ? 1 : 0;
			AxisY += bLeftInput ? -1 : 0;
			
			FVector VelocityX = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);
			FVector VelocityY = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y);
			VelocityX *= (AxisX * 200.0f);
			VelocityY *= (AxisY * 200.0f);

			FVector LaunchVector = VelocityX + VelocityY;
			LaunchVector.Z = 500.0f;

			LaunchCharacter(LaunchVector, false, true);
		}
	}
}

void AWACharacter::Backstep()
{
	AnimInstance->PlayBackstepMontage();
}

void AWACharacter::AttackHitCheck()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(CharacterStat->GetAttackRange()),
		Params);

	if (bResult)
	{
		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			if (OverlapResult.Actor.IsValid() && IsInAngle(this, OverlapResult.Actor, CharacterStat->GetAttackHorizontalAngle() / 2.0f)
				&& abs(GetActorLocation().Z - OverlapResult.Actor->GetActorLocation().Z) < CharacterStat->GetAttackHalfHeight())
			{
				// If is team return.
				if (IsTeamTowards(*(OverlapResult.Actor)))
				{
					continue;
				}

				// Cause Damage.
				FDamageEvent DamageEvent;
				OverlapResult.Actor->TakeDamage(CharacterStat->GetDamage(), DamageEvent, GetController(), this);

				// Spawn Particle
				if (HitImpactParticle)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitImpactParticle, OverlapResult.GetActor()->GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));
				}
				if (HitImpactNiagara)
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitImpactNiagara, OverlapResult.GetActor()->GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));
				}

				// Hit Stop	
				FTimerHandle HitStopHandle;
				float WaitTime = 0.01f;
				AnimInstance->Montage_Pause(AnimInstance->GetCurrentActiveMontage());
				GetWorld()->GetTimerManager().SetTimer(HitStopHandle, FTimerDelegate::CreateLambda([&]()
					{
						AnimInstance->Montage_Resume(AnimInstance->GetCurrentActiveMontage());
					}), WaitTime, false);

				// Shake Camera
				if (IsPlayerControlled())
				{
					Cast<AWAPlayerController>(GetController())->ClientPlayCameraShake(AttackCameraShake, 1.0f, ECameraAnimPlaySpace::CameraLocal);
				}
			}
		}
	}
}

bool AWACharacter::IsInAngle(TWeakObjectPtr<AWACharacter> Center, TWeakObjectPtr<AActor> Target, float AttackAngle)
{
	float DotValue = FMath::Cos(FMath::DegreesToRadians(AttackAngle / 2.0f));
	FVector TargetVector = Target->GetActorLocation() - Center->GetActorLocation();
	if (FVector::DotProduct(Center->GetActorForwardVector(), TargetVector) > DotValue)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AWACharacter::Ultimate()
{
	
}

void AWACharacter::Hitstun()
{
	SetCharacterAction(ECharacterAction::Hitstun);
	AnimInstance->PlayHitstunMontage();
}

void AWACharacter::Dead()
{
	SetActorEnableCollision(false);
	SetCanBeDamaged(false);
	HPBarWidget->SetHiddenInGame(true);
	AnimInstance->SetDead(true);
	AnimInstance->StopAllMontages(0.0f);

	FTimerHandle DestroyTimerHandle;
	float DestroyTime = 0.6f;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), DestroyTime, false);
}

bool AWACharacter::IsTeamTowards(const AActor& Other)
{
	ETeamAttitude::Type TeamAttitudeType = ETeamAttitude::Neutral;

	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (IsPlayerControlled())
	{
		AWAPlayerController* PlayerController = Cast<AWAPlayerController>(GetController());
		if (PlayerController)
		{
			TeamAttitudeType = PlayerController->GetTeamAttitudeTowards(Other);
		}
	}
	else
	{
		AWAAIController* AIController = Cast<AWAAIController>(GetController());
		if (AIController)
		{
			TeamAttitudeType = AIController->GetTeamAttitudeTowards(Other);
		}
	}

	
	if (TeamAttitudeType == ETeamAttitude::Friendly)
	{
		return true;
	}
	else
	{
		return false;
	}
}