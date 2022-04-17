// Fill out your copyright notice in the Description page of Project Settings.


#include "WAMeleeBasicMinionAnimInstance.h"

UWAMeleeBasicMinionAnimInstance::UWAMeleeBasicMinionAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_BASICATTACK_A(TEXT("AnimMontage'/Game/ParagonMinions/Characters/Buff/Buff_White/Animations/Montage_BasicAttack.Montage_BasicAttack'"));
	if (MONTAGE_BASICATTACK_A.Succeeded())
	{
		Montage_BasicAttack_A = MONTAGE_BASICATTACK_A.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_HITSTUN(TEXT("AnimMontage'/Game/ParagonMinions/Characters/Buff/Buff_White/Animations/Montage_Hitstun.Montage_Hitstun'"));
	if (MONTAGE_HITSTUN.Succeeded())
	{
		Montage_Hitstun = MONTAGE_HITSTUN.Object;
	}
}

void UWAMeleeBasicMinionAnimInstance::PlayBasicAttackMontage(int32 CurrentAttackCount)
{
	switch (CurrentAttackCount)
	{
	case 1:
		if (Montage_BasicAttack_A)
		{
			Montage_Play(Montage_BasicAttack_A);
		}
		break;
	}
}

void UWAMeleeBasicMinionAnimInstance::PlayHitstunMontage()
{
	if (Montage_Hitstun)
	{
		Montage_Play(Montage_Hitstun);
	}
}