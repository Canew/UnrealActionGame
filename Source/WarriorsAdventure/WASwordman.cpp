// Fill out your copyright notice in the Description page of Project Settings.


#include "WASwordman.h"
#include "WASwordmanAnimInstance.h"
#include "Component_FootIK.h"
#include "Components/CharacterStatComponent.h"
#include "Components/CharacterSkillComponent.h"
#include "Engine/DataTable.h"
#include "Engine/TriggerBox.h"

// Sets default values
AWASwordman::AWASwordman()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_SWORDMAN(TEXT("SkeletalMesh'/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/Kwang_GDC.Kwang_GDC'"));
	if (SK_SWORDMAN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_SWORDMAN.Object);
	}

	// Set AnimBlueprint.
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_SWORDMAN(TEXT("AnimBlueprint'/Game/ParagonKwang/Characters/Heroes/Kwang/Animations/WASwordmanAnimBlueprint.WASwordmanAnimBlueprint_C'"));
	if (ANIM_SWORDMAN.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM_SWORDMAN.Class);
	}

	// Set Hit Particle.
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HIT(TEXT("ParticleSystem'/Game/ParagonKwang/FX/Particles/Abilities/Primary/FX/P_Kwang_Primary_Impact.P_Kwang_Primary_Impact'"));
	if (P_HIT.Succeeded())
	{
		HitImpactParticle = P_HIT.Object;
	}

	// Create IK Foot
	IK_Foot = CreateDefaultSubobject<UComponent_FootIK>(TEXT("IKFOOT"));
	IK_Foot->SetIKSocketName(TEXT("Foot_L"), TEXT("Foot_R"));

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

// Called when the game starts or when spawned
void AWASwordman::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWASwordman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bGuardBlocking)
	{
		GuardBlockingPushVelocity = FMath::FInterpTo(GuardBlockingPushVelocity, 0.0f, DeltaTime, 5.0f);
		SetActorRelativeLocation(GetActorLocation() + GuardBlockingPushDirection * GuardBlockingPushVelocity);
	}
}

// Called to bind functionality to input
void AWASwordman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("RightMouseButton"), EInputEvent::IE_Pressed, this, &AWASwordman::GuardOn);
	PlayerInputComponent->BindAction(TEXT("RightMouseButton"), EInputEvent::IE_Released, this, &AWASwordman::GuardOff);
	PlayerInputComponent->BindAction(TEXT("Up"), EInputEvent::IE_DoubleClick, this, &AWASwordman::EvadeFront);
	PlayerInputComponent->BindAction(TEXT("Down"), EInputEvent::IE_DoubleClick, this, &AWASwordman::EvadeBack);
	PlayerInputComponent->BindAction(TEXT("Left"), EInputEvent::IE_DoubleClick, this, &AWASwordman::EvadeLeft);
	PlayerInputComponent->BindAction(TEXT("Right"), EInputEvent::IE_DoubleClick, this, &AWASwordman::EvadeRight);
}

void AWASwordman::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AnimInstance = Cast<UWASwordmanAnimInstance>(GetMesh()->GetAnimInstance());
	
	AnimInstance->OnAttackHitCheck.AddUObject(this, &AWASwordman::AttackHitCheck);
	AnimInstance->OnSpecialAttackHitCheck.AddUObject(this, &AWASwordman::SpecialAttackCheck);
}

float AWASwordman::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

				Cast<UWASwordmanAnimInstance>(GetMesh()->GetAnimInstance())->PlayGuardParryMontage();
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

				Cast<UWASwordmanAnimInstance>(GetMesh()->GetAnimInstance())->PlayGuardBlockMontage();
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
		}
	}

	CharacterStat->SetCurrentHP(CharacterStat->GetCurrentHP() - FinalDamage);

	return FinalDamage;
}

void AWASwordman::GuardOn()
{
	if (GetCharacterAction() != ECharacterAction::Idle)
	{
		return;
	}

	SetCharacterAction(ECharacterAction::Guard);
	Cast<UWASwordmanAnimInstance>(GetMesh()->GetAnimInstance())->PlayGuardOnMontage();

	bCanParry = true;
	FTimerHandle ParryTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ParryTimerHandle, FTimerDelegate::CreateLambda([&]() {
		bCanParry = false;
		}), ParryableTime, false);
}

void AWASwordman::GuardOff()
{
	if (GetCharacterAction() == ECharacterAction::Guard)
	{
		SetCharacterAction(ECharacterAction::Idle);
		Cast<UWASwordmanAnimInstance>(GetMesh()->GetAnimInstance())->PlayGuardOffMontage();
		bCanParry = false;
	}
}

void AWASwordman::EvadeFront()
{
	if (GetCharacterAction() == ECharacterAction::Idle || GetCharacterAction() == ECharacterAction::Guard)
	{
		SetCharacterAction(ECharacterAction::Evade);
		Cast<UWASwordmanAnimInstance>(GetMesh()->GetAnimInstance())->PlayEvadeFrontMontage();
	}
}

void AWASwordman::EvadeBack()
{
	if (GetCharacterAction() == ECharacterAction::Idle || GetCharacterAction() == ECharacterAction::Guard)
	{
		SetCharacterAction(ECharacterAction::Evade);
		Cast<UWASwordmanAnimInstance>(GetMesh()->GetAnimInstance())->PlayEvadeBackMontage();
	}
}

void AWASwordman::EvadeLeft()
{
	if (GetCharacterAction() == ECharacterAction::Idle || GetCharacterAction() == ECharacterAction::Guard)
	{
		SetCharacterAction(ECharacterAction::Evade);
		Cast<UWASwordmanAnimInstance>(GetMesh()->GetAnimInstance())->PlayEvadeLeftMontage();
	}
}

void AWASwordman::EvadeRight()
{
	if (GetCharacterAction() == ECharacterAction::Idle || GetCharacterAction() == ECharacterAction::Guard)
	{
		SetCharacterAction(ECharacterAction::Evade);
		Cast<UWASwordmanAnimInstance>(GetMesh()->GetAnimInstance())->PlayEvadeRightMontage();
	}
}

void AWASwordman::SpecialAttackCheck()
{
	//const FVector SpawnLocation = GetActorLocation();
	//ATriggerBox* CollisionBox = Cast<ATriggerBox>(GetWorld()->SpawnActor(ATriggerBox::StaticClass(), &SpawnLocation));
	//CollisionBox->OnActorBeginOverlap.AddDynamic(this, &AWASwordman::SpecialAttackBeginOverlap);

	//FTimerHandle CollisionTimerHandle;
	//float CollisionTime = 0.1f;
	//GetWorld()->GetTimerManager().SetTimer(CollisionTimerHandle, FTimerDelegate::CreateLambda([&]() {
	//	CollisionBox->SetActorRelativeLocation(GetActorLocation() + GetActorForwardVector());
	//	}), CollisionTime, true);
}

void AWASwordman::SpecialAttackBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::White, FString(TEXT("SpecialAttackBeginOverlap")), false);
}