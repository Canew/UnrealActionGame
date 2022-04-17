// Fill out your copyright notice in the Description page of Project Settings.


#include "WASevarogAnimInstance.h"
#include "WASevarog.h"

UWASevarogAnimInstance::UWASevarogAnimInstance()
{
	// Attack montages
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_BASICATTACK_A(TEXT("AnimMontage'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Animations/Montage_BasicAttack_A.Montage_BasicAttack_A'"));
	if (MONTAGE_BASICATTACK_A.Succeeded())
	{
		Montage_BasicAttack_A = MONTAGE_BASICATTACK_A.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_BASICATTACK_B(TEXT("AnimMontage'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Animations/Montage_BasicAttack_B.Montage_BasicAttack_B'"));
	if (MONTAGE_BASICATTACK_B.Succeeded())
	{
		Montage_BasicAttack_B = MONTAGE_BASICATTACK_B.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_BASICATTACK_C(TEXT("AnimMontage'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Animations/Montage_BasicAttack_C.Montage_BasicAttack_C'"));
	if (MONTAGE_BASICATTACK_C.Succeeded())
	{
		Montage_BasicAttack_C = MONTAGE_BASICATTACK_C.Object;
	}

	// Move Montage.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_BACKSTEP(TEXT("AnimMontage'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Animations/Montage_Backstep.Montage_Backstep'"));
	if (MONTAGE_BACKSTEP.Succeeded())
	{
		Montage_Backstep = MONTAGE_BACKSTEP.Object;
	}

	// Skill Montage.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_DISEASE_BALL(TEXT("AnimMontage'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Animations/Montage_Disease_Ball.Montage_Disease_Ball'"));
	if (MONTAGE_DISEASE_BALL.Succeeded())
	{
		Montage_Disease_Ball = MONTAGE_DISEASE_BALL.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_DISEASE_RAIN(TEXT("AnimMontage'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Animations/Montage_Disease_Rain.Montage_Disease_Rain'"));
	if (MONTAGE_DISEASE_RAIN.Succeeded())
	{
		Montage_Disease_Rain = MONTAGE_DISEASE_RAIN.Object;
	}
}

void UWASevarogAnimInstance::PlayBasicAttackMontage(int32 CurrentAttackCount)
{
	switch (CurrentAttackCount)
	{
	case 1:
		if (Montage_BasicAttack_A)
		{
			Montage_Play(Montage_BasicAttack_A);
		}
		break;

	case 2:
		if (Montage_BasicAttack_B)
		{
			Montage_Play(Montage_BasicAttack_B);
		}
		break;

	case 3:
		if (Montage_BasicAttack_C)
		{
			Montage_Play(Montage_BasicAttack_C);
		}
		break;
	}
}

void UWASevarogAnimInstance::PlayBackstepMontage()
{
	if (Montage_Backstep)
	{
		Montage_Play(Montage_Backstep);
	}
}

void UWASevarogAnimInstance::PlayDiseaseBallMontage()
{
	if (Montage_Disease_Ball)
	{
		Montage_Play(Montage_Disease_Ball);
	}
}

void UWASevarogAnimInstance::PlayDiseaseRainMontage()
{
	if (Montage_Disease_Rain)
	{
		Montage_Play(Montage_Disease_Rain);
	}
}

void UWASevarogAnimInstance::AnimNotify_FireDiseaseBall()
{
	AWASevarog* Sevarog = Cast<AWASevarog>(GetOwningActor());
	if (Sevarog)
	{
		Sevarog->FireDiseaseBall();
	}
}

void UWASevarogAnimInstance::AnimNotify_SpawnDiseaseRain()
{
	AWASevarog* Sevarog = Cast<AWASevarog>(GetOwningActor());
	if (Sevarog)
	{
		Sevarog->SpawnDiseaseRain();
	}
}

void UWASevarogAnimInstance::AnimNotify_FireDiseaseRain()
{
	AWASevarog* Sevarog = Cast<AWASevarog>(GetOwningActor());
	if (Sevarog)
	{
		Sevarog->FireDiseaseRain();
	}
}