// Fill out your copyright notice in the Description page of Project Settings.


#include "WAGothicKnightAnimInstance.h"
#include "WAGothicKnight.h"


UWAGothicKnightAnimInstance::UWAGothicKnightAnimInstance()
{
	// Attack montages
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_BASICATTACK_A(TEXT("AnimMontage'/Game/GKnight/Animation/Montage_BasicAttack_A.Montage_BasicAttack_A'"));
	if (MONTAGE_BASICATTACK_A.Succeeded())
	{
		Montage_BasicAttack_A = MONTAGE_BASICATTACK_A.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_BASICATTACK_B(TEXT("AnimMontage'/Game/GKnight/Animation/Montage_BasicAttack_B.Montage_BasicAttack_B'"));
	if (MONTAGE_BASICATTACK_B.Succeeded())
	{
		Montage_BasicAttack_B = MONTAGE_BASICATTACK_B.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_BASICATTACK_C(TEXT("AnimMontage'/Game/GKnight/Animation/Montage_BasicAttack_C.Montage_BasicAttack_C'"));
	if (MONTAGE_BASICATTACK_C.Succeeded())
	{
		Montage_BasicAttack_C = MONTAGE_BASICATTACK_C.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_BASICATTACK_D(TEXT("AnimMontage'/Game/GKnight/Animation/Montage_BasicAttack_D.Montage_BasicAttack_D'"));
	if (MONTAGE_BASICATTACK_D.Succeeded())
	{
		Montage_BasicAttack_D = MONTAGE_BASICATTACK_D.Object;
	}	

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_CHARGEDBASICATTACK(TEXT("AnimMontage'/Game/GKnight/Animation/Montage_Thrust.Montage_Thrust'"));
	if (MONTAGE_CHARGEDBASICATTACK.Succeeded())
	{
		Montage_ChargedBasicAttack = MONTAGE_CHARGEDBASICATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_DASHATTACK(TEXT("AnimMontage'/Game/GKnight/Animation/Montage_Dash_Attack.Montage_Dash_Attack'"));
	if (MONTAGE_DASHATTACK.Succeeded())
	{
		Montage_DashAttack = MONTAGE_DASHATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_JUMPATTACK(TEXT("AnimMontage'/Game/GKnight/Animation/Montage_Jump_Attack.Montage_Jump_Attack'"));
	if (MONTAGE_JUMPATTACK.Succeeded())
	{
		Montage_JumpAttack = MONTAGE_JUMPATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_SPECIALATTACK(TEXT("AnimMontage'/Game/GKnight/Animation/Montage_SpecialAttack.Montage_SpecialAttack'"));
	if (MONTAGE_SPECIALATTACK.Succeeded())
	{
		Montage_SpecialAttack = MONTAGE_SPECIALATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_ULTIMATE(TEXT("AnimMontage'/Game/GKnight/Animation/Montage_Ultimate.Montage_Ultimate'"));
	if (MONTAGE_ULTIMATE.Succeeded())
	{
		Montage_Ultimate = MONTAGE_ULTIMATE.Object;
	}

	// Guard montages
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_GUARD(TEXT("AnimMontage'/Game/GKnight/Animation/Montage_Guard.Montage_Guard'"));
	if (MONTAGE_GUARD.Succeeded())
	{
		Montage_Guard = MONTAGE_GUARD.Object;
	}

	// Evade montages.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_EVADE(TEXT("AnimMontage'/Game/GKnight/Animation/Montage_Evade.Montage_Evade'"));
	if (MONTAGE_EVADE.Succeeded())
	{
		Montage_Evade = MONTAGE_EVADE.Object;
	}

	// Hit react montages.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_HITSTUN(TEXT("AnimMontage'/Game/GKnight/Animation/Montage_Hitstun.Montage_Hitstun'"));
	if (MONTAGE_HITSTUN.Succeeded())
	{
		Montage_Hitstun = MONTAGE_HITSTUN.Object;
	}
}

void UWAGothicKnightAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Get IKFoot Component from owner
	AWAGothicKnight* pCharacter = Cast<AWAGothicKnight>(TryGetPawnOwner());
	if (pCharacter != nullptr)
	{
		UActorComponent* pActorComp = pCharacter->GetComponentByClass(UComponent_FootIK::StaticClass());
		if (pActorComp != nullptr)
		{
			IK_Foot = Cast<UComponent_FootIK>(pActorComp);
		}
	}
}

void UWAGothicKnightAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AWAGothicKnight* pCharacter = Cast<AWAGothicKnight>(TryGetPawnOwner());
	if (IsValid(pCharacter))
	{
		// Foot IK
		IKAnimValue = IK_Foot->GetIKAnimValue();

		bEvading = Montage_IsPlaying(Montage_Evade);
	}
}

void UWAGothicKnightAnimInstance::PlayBasicAttackMontage(int32 CurrentAttackCount)
{
	if (bDead)
	{
		return;
	}

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

	case 4:
		if (Montage_BasicAttack_D)
		{
			Montage_Play(Montage_BasicAttack_D);
		}
		break;
	}
}

void UWAGothicKnightAnimInstance::PlayChargedBasicAttackMontage()
{
	if (Montage_ChargedBasicAttack)
	{
		Montage_Play(Montage_ChargedBasicAttack);
	}
}

void UWAGothicKnightAnimInstance::PlayDashAttackMontage()
{
	if (Montage_DashAttack)
	{
		Montage_Play(Montage_DashAttack);
	}
}

void UWAGothicKnightAnimInstance::PlayJumpAttackMontage()
{
	if (Montage_JumpAttack)
	{
		Montage_Play(Montage_JumpAttack);
	}
}

void UWAGothicKnightAnimInstance::PlaySpecialAttackMontage()
{
	if (Montage_SpecialAttack)
	{
		Montage_Play(Montage_SpecialAttack);
	}
}

void UWAGothicKnightAnimInstance::PlayUltimateMontage()
{
	if (Montage_Ultimate)
	{
		Montage_Play(Montage_Ultimate);
	}
}

void UWAGothicKnightAnimInstance::PlayGuardOnMontage()
{
	if (Montage_Guard)
	{
		Montage_Play(Montage_Guard);
	}
}

void UWAGothicKnightAnimInstance::PlayGuardOffMontage()
{
	if (Montage_Guard)
	{
		Montage_Stop(0.5f, Montage_Guard);
	}
}

void UWAGothicKnightAnimInstance::PlayGuardBlockMontage()
{
	if (Montage_Guard)
	{
		Montage_JumpToSection(FName(TEXT("GuardBlock")), Montage_Guard);
	}
}

void UWAGothicKnightAnimInstance::PlayGuardParryMontage()
{
	if (Montage_Guard)
	{
		Montage_JumpToSection(FName(TEXT("GuardParry")), Montage_Guard);
	}
}

void UWAGothicKnightAnimInstance::PlayEvadeFrontMontage()
{
	if (Montage_Evade)
	{
		Montage_Play(Montage_Evade);
		Montage_JumpToSection(FName(TEXT("Evade_Front")), Montage_Evade);
	}
}

void UWAGothicKnightAnimInstance::PlayEvadeBackMontage()
{
	if (Montage_Evade)
	{
		Montage_Play(Montage_Evade);
		Montage_JumpToSection(FName(TEXT("Evade_Back")), Montage_Evade);
	}
}

void UWAGothicKnightAnimInstance::PlayEvadeLeftMontage()
{
	if (Montage_Evade)
	{
		Montage_Play(Montage_Evade);
		Montage_JumpToSection(FName(TEXT("Evade_Left")), Montage_Evade);
	}
}

void UWAGothicKnightAnimInstance::PlayEvadeRightMontage()
{
	if (Montage_Evade)
	{
		Montage_Play(Montage_Evade);
		Montage_JumpToSection(FName(TEXT("Evade_Right")), Montage_Evade);
	}
}

void UWAGothicKnightAnimInstance::PlayHitstunMontage()
{
	if (Montage_Hitstun)
	{
		Montage_Play(Montage_Hitstun);
	}
}