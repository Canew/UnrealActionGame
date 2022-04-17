// Fill out your copyright notice in the Description page of Project Settings.


#include "WAMeleeBasicMinion.h"
#include "WAMeleeBasicMinionAnimInstance.h"
#include "Components/CharacterStatComponent.h"

AWAMeleeBasicMinion::AWAMeleeBasicMinion()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set Mesh.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MELEEBASICMINION(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/White_Camp_Minion/Meshes/Minion_melee.Minion_melee'"));
	if (SK_MELEEBASICMINION.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MELEEBASICMINION.Object);
	}

	// Set Anim Instance.
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_SWORDMAN(TEXT("AnimBlueprint'/Game/ParagonMinions/Characters/Minions/White_Camp_Minion/Meshes/MeleeBasicMinionAnimBlueprint.MeleeBasicMinionAnimBlueprint_C'"));
	if (ANIM_SWORDMAN.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM_SWORDMAN.Class);
	}
	
	// Set Hit Particle.
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HIT(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Shared/P_Minion_Melee_Impact.P_Minion_Melee_Impact'"));
	if (P_HIT.Succeeded())
	{
		HitImpactParticle = P_HIT.Object;
	}

	// Initialize Character Stat
	CharacterStat->SetMaxHP(50.0f);
	CharacterStat->SetCurrentHP(CharacterStat->GetMaxHP());
	CharacterStat->SetDamage(5.0f);
	CharacterStat->SetAttackRange(150.0f);
	CharacterStat->SetAttackHorizontalAngle(45.0f);
	CharacterStat->SetAttackHalfHeight(GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	CharacterStat->SetArmor(0.0f);
	
	// Initialize Character Movement properties.
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void AWAMeleeBasicMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AnimInstance = Cast<UWAMeleeBasicMinionAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->OnAttackHitCheck.AddUObject(this, &AWAMeleeBasicMinion::AttackHitCheck);
}

float AWAMeleeBasicMinion::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Hitstun();

	//Play Sound
	CharacterAudio->SetSound(BeHitSoundCue);
	CharacterAudio->Play();

	CharacterStat->SetCurrentHP(CharacterStat->GetCurrentHP() - FinalDamage);

	return FinalDamage;
}