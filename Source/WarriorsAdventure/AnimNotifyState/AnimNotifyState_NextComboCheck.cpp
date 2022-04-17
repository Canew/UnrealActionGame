// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_NextComboCheck.h"
#include "WACharacter.h"

void UAnimNotifyState_NextComboCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AWACharacter* pCharacter = Cast<AWACharacter>(MeshComp->GetOwner());
	if (IsValid(pCharacter))
	{
		pCharacter->SetNextBasicAttackInputOn(false);
		bNextAttack = false;
	}
}

void UAnimNotifyState_NextComboCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	AWACharacter* pCharacter = Cast<AWACharacter>(MeshComp->GetOwner());
	if (IsValid(pCharacter))
	{
		if (pCharacter->IsNextBasicAttackInputOn())
		{
			pCharacter->BasicAttackCombo();
			bNextAttack = true;
		}
	}
}

void UAnimNotifyState_NextComboCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AWACharacter* pCharacter = Cast<AWACharacter>(MeshComp->GetOwner());
	if (IsValid(pCharacter))
	{
		if (!bNextAttack || (pCharacter->GetCurrentBasicAttackCombo() > pCharacter->GetMaxBasicAttackCombo()))
		{
			pCharacter->BasicAttackEnd();
		}
	}
}