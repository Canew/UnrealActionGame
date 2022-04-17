// Fill out your copyright notice in the Description page of Project Settings.


#include "WAGothicKnight.h"
#include "WAGothicKnightAnimInstance.h"
#include "WAPlayerController.h"
#include "Component_FootIK.h"
#include "Components/CharacterStatComponent.h"
#include "Components/CharacterSkillComponent.h"
#include "Engine/DataTable.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"

AWAGothicKnight::AWAGothicKnight()
{
	PrimaryActorTick.bCanEverTick = true;

	// Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GOTHICKNIGHT(TEXT("SkeletalMesh'/Game/GKnight/Meshes/SK_GothicKnight_VA.SK_GothicKnight_VA'"));
	if (SK_GOTHICKNIGHT.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_GOTHICKNIGHT.Object);
	}

	// Weapon Static Mesh
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GOTHIC_SWORD(TEXT("StaticMesh'/Game/GKnight/Meshes/Weapon/SM_WP_GothicKnight_Sword.SM_WP_GothicKnight_Sword'"));
	if (SM_GOTHIC_SWORD.Succeeded())
	{
		Weapon->SetStaticMesh(SM_GOTHIC_SWORD.Object);
	}

	FName WeaponSocket(TEXT("hand_r_socket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket) && Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	// Set AnimBlueprint.
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_GOTHICKNIGHT(TEXT("AnimBlueprint'/Game/GKnight/Animation/WAGothicKnightAnimBlueprint.WAGothicKnightAnimBlueprint_C'"));
	if (ANIM_GOTHICKNIGHT.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM_GOTHICKNIGHT.Class);
	}

	// Set Hit Particle.
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> P_HIT_IMPACT(TEXT("NiagaraSystem'/Game/GKnight/FX/NS_GothicKnight_Impact.NS_GothicKnight_Impact'"));
	if (P_HIT_IMPACT.Succeeded())
	{
		HitImpactNiagara = P_HIT_IMPACT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> P_SPECIAL_ATTACK_IMPACT(TEXT("NiagaraSystem'/Game/Hack_And_Slash_FX/VFX_Niagara/Impacts/NS_Holy_Slash_Impact.NS_Holy_Slash_Impact'"));
	if (P_SPECIAL_ATTACK_IMPACT.Succeeded())
	{
		SpecialAttackImpactNiagara = P_SPECIAL_ATTACK_IMPACT.Object;
	}

	// Create IK Foot
	IK_Foot = CreateDefaultSubobject<UComponent_FootIK>(TEXT("IKFOOT"));
	IK_Foot->SetIKSocketName(TEXT("ik_foot_l_socket"), TEXT("ik_foot_r_socket"));

	// Initialize Attack properties
	MaxBasicAttackCombo = 4;

	// Initialize Character Stat
	CharacterStat->SetMaxHP(100.0f);
	CharacterStat->SetCurrentHP(CharacterStat->GetMaxHP());
	CharacterStat->SetDamage(10.0f);
	CharacterStat->SetAttackRange(250.0f);
	CharacterStat->SetAttackHorizontalAngle(45.0f);
	CharacterStat->SetAttackHalfHeight(GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	CharacterStat->SetArmor(1.0f);
}

void AWAGothicKnight::BeginPlay()
{
	Super::BeginPlay();
}

void AWAGothicKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bGuardBlocking)
	{
		GuardBlockingPushVelocity = FMath::FInterpTo(GuardBlockingPushVelocity, 0.0f, DeltaTime, 5.0f);
		SetActorRelativeLocation(GetActorLocation() + GuardBlockingPushDirection * GuardBlockingPushVelocity);
	}
}

void AWAGothicKnight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("RightMouseButton"), EInputEvent::IE_Pressed, this, &AWAGothicKnight::GuardOn);
	PlayerInputComponent->BindAction(TEXT("RightMouseButton"), EInputEvent::IE_Released, this, &AWAGothicKnight::OnRightMouseButtonUp);
	PlayerInputComponent->BindAction(TEXT("Up"), EInputEvent::IE_DoubleClick, this, &AWAGothicKnight::EvadeFront);
	PlayerInputComponent->BindAction(TEXT("Down"), EInputEvent::IE_DoubleClick, this, &AWAGothicKnight::EvadeBack);
	PlayerInputComponent->BindAction(TEXT("Left"), EInputEvent::IE_DoubleClick, this, &AWAGothicKnight::EvadeLeft);
	PlayerInputComponent->BindAction(TEXT("Right"), EInputEvent::IE_DoubleClick, this, &AWAGothicKnight::EvadeRight);
}

void AWAGothicKnight::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UWAGothicKnightAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->OnAttackHitCheck.AddUObject(this, &AWAGothicKnight::AttackHitCheck);
	AnimInstance->OnSpecialAttackHitCheck.AddUObject(this, &AWAGothicKnight::SpecialAttackHitCheck);
}

float AWAGothicKnight::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	AWACharacter* pInstigatorCharacter = Cast<AWACharacter>(DamageCauser);
	if (pInstigatorCharacter)
	{

		// Guard
		if (GetCharacterAction() == ECharacterAction::Guard && IsInAngle(this, pInstigatorCharacter, 45.0f / 2.0f))
		{
			if (bCanParry)
			{
				FinalDamage = 0.0f;

				bGuardBlocking = true;

				GuardBlockingPushDirection = pInstigatorCharacter->GetActorForwardVector();
				GuardBlockingPushDirection.Z = 0.0f;
				GuardBlockingPushVelocity = 2.5f;
				FTimerHandle GuardBlockingTimerHandle;
				float GuardBlockingTime = 0.25f;
				GetWorld()->GetTimerManager().SetTimer(GuardBlockingTimerHandle, FTimerDelegate::CreateLambda([&]() {
					bGuardBlocking = false;
					}), GuardBlockingTime, false);

				Cast<UWAGothicKnightAnimInstance>(GetMesh()->GetAnimInstance())->PlayGuardParryMontage();
			}
			else
			{
				FinalDamage /= 50.0f;

				bGuardBlocking = true;

				GuardBlockingPushDirection = pInstigatorCharacter->GetActorForwardVector();
				GuardBlockingPushDirection.Z = 0.0f;
				GuardBlockingPushVelocity = 5.0f;
				FTimerHandle GuardBlockingTimerHandle;
				float GuardBlockingTime = 0.5f;
				GetWorld()->GetTimerManager().SetTimer(GuardBlockingTimerHandle, FTimerDelegate::CreateLambda([&]() {
					bGuardBlocking = false;
					}), GuardBlockingTime, false);

				Cast<UWAGothicKnightAnimInstance>(GetMesh()->GetAnimInstance())->PlayGuardBlockMontage();
			}
		}

		// Evade
		else if (GetCharacterAction() == ECharacterAction::Evade)
		{
			FinalDamage = 0.0f;

			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25f);
			FTimerHandle EvadeTimerHandle;
			float EvadeTime = 0.05f;
			GetWorld()->GetTimerManager().SetTimer(EvadeTimerHandle, FTimerDelegate::CreateLambda([&]() {
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
				}), EvadeTime, false);
		}

		else
		{
			// Cause Hit React
			Hitstun();

			//Play Sound
			CharacterAudio->SetSound(BeHitSoundCue);
			CharacterAudio->Play();
		}
	}

	CharacterStat->SetCurrentHP(CharacterStat->GetCurrentHP() - FinalDamage);

	return FinalDamage;
}

void AWAGothicKnight::Ultimate()
{
	SetCharacterAction(ECharacterAction::Attack);
	AnimInstance->PlayUltimateMontage();
}

void AWAGothicKnight::GuardOn()
{
	if (GetCharacterAction() != ECharacterAction::Idle)
	{
		return;
	}

	SetCharacterAction(ECharacterAction::Guard);
	Cast<UWAGothicKnightAnimInstance>(GetMesh()->GetAnimInstance())->PlayGuardOnMontage();

	bCanParry = true;
	FTimerHandle ParryTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ParryTimerHandle, FTimerDelegate::CreateLambda([&]() {
		bCanParry = false;
		}), ParryableTime, false);
}

void AWAGothicKnight::GuardOff()
{
	if (GetCharacterAction() == ECharacterAction::Guard)
	{
		SetCharacterAction(ECharacterAction::Idle);
		Cast<UWAGothicKnightAnimInstance>(GetMesh()->GetAnimInstance())->PlayGuardOffMontage();
		bCanParry = false;
	}
}

void AWAGothicKnight::EvadeFront()
{
	if (GetCharacterAction() == ECharacterAction::Idle || GetCharacterAction() == ECharacterAction::Guard)
	{
		FRotator ControlRotation = GetController()->GetControlRotation();
		ControlRotation.Roll = 0.0f;
		ControlRotation.Pitch = 0.0f;
		SetActorRelativeRotation(ControlRotation.Quaternion());
		SetCharacterAction(ECharacterAction::Evade);
		Cast<UWAGothicKnightAnimInstance>(GetMesh()->GetAnimInstance())->PlayEvadeFrontMontage();
	}
}

void AWAGothicKnight::EvadeBack()
{
	if (GetCharacterAction() == ECharacterAction::Idle || GetCharacterAction() == ECharacterAction::Guard)
	{
		FRotator ControlRotation = GetController()->GetControlRotation();
		ControlRotation.Roll = 0.0f;
		ControlRotation.Pitch = 0.0f;
		SetActorRelativeRotation(ControlRotation.Quaternion());
		SetCharacterAction(ECharacterAction::Evade);
		Cast<UWAGothicKnightAnimInstance>(GetMesh()->GetAnimInstance())->PlayEvadeBackMontage();
	}
}

void AWAGothicKnight::EvadeLeft()
{
	if (GetCharacterAction() == ECharacterAction::Idle || GetCharacterAction() == ECharacterAction::Guard)
	{
		FRotator ControlRotation = GetController()->GetControlRotation();
		ControlRotation.Roll = 0.0f;
		ControlRotation.Pitch = 0.0f;
		SetActorRelativeRotation(ControlRotation.Quaternion());
		SetCharacterAction(ECharacterAction::Evade);
		Cast<UWAGothicKnightAnimInstance>(GetMesh()->GetAnimInstance())->PlayEvadeLeftMontage();
	}
}

void AWAGothicKnight::EvadeRight()
{
	if (GetCharacterAction() == ECharacterAction::Idle || GetCharacterAction() == ECharacterAction::Guard)
	{
		FRotator ControlRotation = GetController()->GetControlRotation();
		ControlRotation.Roll = 0.0f;
		ControlRotation.Pitch = 0.0f;
		SetActorRelativeRotation(ControlRotation.Quaternion());
		SetCharacterAction(ECharacterAction::Evade);
		Cast<UWAGothicKnightAnimInstance>(GetMesh()->GetAnimInstance())->PlayEvadeRightMontage();
	}
}

void AWAGothicKnight::OnRightMouseButtonUp()
{
	if (GetCharacterAction() == ECharacterAction::Guard)
	{
		GuardOff();
	}

	
}

void AWAGothicKnight::SpecialAttackHitCheck()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepMultiByChannel(HitResults,
		GetActorLocation() + 100.0f * GetActorForwardVector(),
		GetActorLocation() + 500.0f * GetActorForwardVector(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeCapsule(75.0f, 200.0f),
		Params);

	if (bResult)
	{
		for (FHitResult& HitResult : HitResults)
		{
			if (HitResult.Actor.IsValid())
			{
				// If is team return.
				if (IsTeamTowards(*(HitResult.Actor)))
				{
					continue;
				}

				// Cause Damage.
				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(CharacterStat->GetDamage(), DamageEvent, GetController(), this);

				// Spawn Particle
				if (SpecialAttackImpactNiagara)
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpecialAttackImpactNiagara, HitResult.GetActor()->GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));
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
