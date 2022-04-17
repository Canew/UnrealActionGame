// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Dilation.h"
#include "WACharacter.h"

void UAnimNotifyState_Dilation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AWACharacter* pCharacter = Cast<AWACharacter>(MeshComp->GetOwner());
	if (IsValid(pCharacter))
	{
		pCharacter->CustomTimeDilation *= Dilation;
	}
}

void UAnimNotifyState_Dilation::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UAnimNotifyState_Dilation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AWACharacter* pCharacter = Cast<AWACharacter>(MeshComp->GetOwner());
	if (IsValid(pCharacter))
	{
		pCharacter->CustomTimeDilation /= Dilation;
	}
}
