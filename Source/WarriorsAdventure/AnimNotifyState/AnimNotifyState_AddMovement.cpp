// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AddMovement.h"
#include "WACharacter.h"

void UAnimNotifyState_AddMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AWACharacter* pCharacter = Cast<AWACharacter>(MeshComp->GetOwner());
	if (IsValid(pCharacter))
	{
		switch (MoveDirection)
		{
		case EMoveDirection::Forward:
			Direction = pCharacter->GetActorForwardVector();
			break;

		case EMoveDirection::Backward:
			Direction = -pCharacter->GetActorForwardVector();
			break;

		case EMoveDirection::Left:
			Direction = -pCharacter->GetActorRightVector();
			break;

		case EMoveDirection::Right:
			Direction = pCharacter->GetActorRightVector();
			break;
		}
	}

}

void UAnimNotifyState_AddMovement::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	AWACharacter* pCharacter = Cast<AWACharacter>(MeshComp->GetOwner());
	if (IsValid(pCharacter))
	{
		UWorld* pWorld = MeshComp->GetWorld();
		if (pWorld)
		{
			LineTraceEnd = pCharacter->GetActorLocation() + Direction * (pCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius());
			bool bResult = pWorld->LineTraceSingleByChannel(HitResult, pCharacter->GetActorLocation(), LineTraceEnd, ECollisionChannel::ECC_WorldStatic);

			pCharacter->SetActorRelativeLocation(pCharacter->GetActorLocation() + MoveSpeed * Direction, bResult);
		}
	}
}

void UAnimNotifyState_AddMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}