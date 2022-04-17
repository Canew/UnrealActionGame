// Fill out your copyright notice in the Description page of Project Settings.


#include "WASevarog.h"
#include "WASevarogAnimInstance.h"
#include "Components/CharacterStatComponent.h"
#include "WAStatHudUserWidget.h"

// Sets default values
AWASevarog::AWASevarog()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(150.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f * GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	// Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_SEVAROG(TEXT("SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'"));
	if (SK_SEVAROG.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_SEVAROG.Object);
	}

	// Set AnimBlueprint.
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_SEVAROG(TEXT("AnimBlueprint'/Game/ParagonSevarog/Characters/Heroes/Sevarog/WASevarogAnimBlueprint.WASevarogAnimBlueprint_C'"));
	if (ANIM_SEVAROG.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM_SEVAROG.Class);
	}

	// Initialize Character Stat
	CharacterStat->SetMaxHP(100.0f);
	CharacterStat->SetCurrentHP(CharacterStat->GetMaxHP());
	CharacterStat->SetDamage(10.0f);
	CharacterStat->SetAttackRange(275.0f);
	CharacterStat->SetAttackHorizontalAngle(45.0f);
	CharacterStat->SetAttackHalfHeight(GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	CharacterStat->SetArmor(1.0f);

	// Initialize Attack properties
	MaxBasicAttackCombo = 3;

	// Set AI Class
	static ConstructorHelpers::FClassFinder<AAIController> BP_WAAICONTROLLER(TEXT("Blueprint'/Game/Blueprints/BP_WASevarogAIController.BP_WASevarogAIController_C'"));
	if (BP_WAAICONTROLLER.Succeeded())
	{
		AIControllerClass = BP_WAAICONTROLLER.Class;
	}

	// Set Disease Ball Class
	static ConstructorHelpers::FClassFinder<AActor> BP_DISEASE_BALL(TEXT("Blueprint'/Game/ParagonSevarog/BP_Sevarog_Disease_Ball.BP_Sevarog_Disease_Ball_C'"));
	if (BP_DISEASE_BALL.Succeeded())
	{
		DiseaseBallClass = BP_DISEASE_BALL.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> BP_DISEASE_RAIN(TEXT("Blueprint'/Game/ParagonSevarog/BP_Sevarog_Disease_Rain.BP_Sevarog_Disease_Rain_C'"));
	if (BP_DISEASE_RAIN.Succeeded())
	{
		DiseaseRainClass = BP_DISEASE_RAIN.Class;
	}

	
}

// Called when the game starts or when spawned
void AWASevarog::BeginPlay()
{
	Super::BeginPlay();
	
}

float AWASevarog::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	
	CharacterStat->SetCurrentHP(CharacterStat->GetCurrentHP() - FinalDamage);

	//Play Sound
	CharacterAudio->SetSound(BeHitSoundCue);
	CharacterAudio->Play();

	return FinalDamage;
}

// Called every frame
void AWASevarog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

}

// Called to bind functionality to input
void AWASevarog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWASevarog::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SevarogAnimInstance = Cast<UWASevarogAnimInstance>(GetMesh()->GetAnimInstance());

	SevarogAnimInstance->OnAttackHitCheck.AddUObject(this, &AWASevarog::AttackHitCheck);
}

void AWASevarog::PlayDiseaseBall()
{
	SetCharacterAction(ECharacterAction::Attack);
	SevarogAnimInstance->PlayDiseaseBallMontage();
}

void AWASevarog::FireDiseaseBall()
{
	const FVector SpawnLocation = GetActorLocation() + GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * GetActorForwardVector();
	const FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Instigator = this;
	GetWorld()->SpawnActor(DiseaseBallClass,
		&SpawnLocation,
		&SpawnRotation,
		ActorSpawnParameters);
}

void AWASevarog::PlayDiseaseRain()
{
	SetCharacterAction(ECharacterAction::Attack);
	SevarogAnimInstance->PlayDiseaseRainMontage();
}

void AWASevarog::SpawnDiseaseRain()
{
	const FVector SpawnLocation = GetActorLocation() + 2.0f * GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * GetActorUpVector();
	const FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Instigator = this;
	GetWorld()->SpawnActor(DiseaseBallClass,
		&SpawnLocation,
		&SpawnRotation,
		ActorSpawnParameters);
}

void AWASevarog::FireDiseaseRain()
{
	OnFireDiseaseRain.Broadcast();
}

void AWASevarog::ExecuteMeleePattern()
{
	MaxMeleePatternKind = 2;
	int32 PatternNumber = FMath::RandRange(0, MaxMeleePatternKind - 1);

	switch (PatternNumber)
	{
	case 0:
	{
		int32 BasicAttackCombo = FMath::RandRange(0, GetMaxBasicAttackCombo() - 1);
		BasicAttack(BasicAttackCombo);
		break;
	}

	case 1:
	{
		Backstep();
		break;
	}
	}
}

void AWASevarog::ExecuteRangePattern()
{
	MaxRangePatternKind = 2;
	int32 PatternNumber = FMath::RandRange(0, MaxRangePatternKind - 1);

	switch (PatternNumber)
	{
	case 0:
	{
		PlayDiseaseBall();
		break;
	}
	case 1:
		PlayDiseaseRain();
		break;
	}
}