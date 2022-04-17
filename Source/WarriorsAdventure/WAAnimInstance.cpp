// Fill out your copyright notice in the Description page of Project Settings.


#include "WAAnimInstance.h"
#include "WACharacter.h"
#include "WAPlayerCharacter.h"

void UWAAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UWAAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AWACharacter* Character = Cast<AWACharacter>(TryGetPawnOwner());
	if (IsValid(Character))
	{
		CurrentCharacterAction = Character->GetCharacterAction();

		CurrentPawnSpeed = Character->GetVelocity().Size();
		if (CurrentPawnSpeed < KINDA_SMALL_NUMBER)
		{
			bDashing = false;
			Character->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		}

		// Get Character Move Direction.
		FVector CharacterVelocity = Character->GetCharacterMovement()->Velocity;
		CharacterVelocity.Normalize();
		FVector CharacterForward = Character->GetActorForwardVector();
		float AngleBetweenVelocityAndDirection = FVector::DotProduct(CharacterForward, CharacterVelocity);
		bool bRight = FVector::CrossProduct(CharacterForward, CharacterVelocity).Z > 0.0f ? true : false;

		if (bRight)
		{
			CurrentPawnDirection = FMath::RadiansToDegrees(FMath::Acos(AngleBetweenVelocityAndDirection));
		}
		else
		{
			CurrentPawnDirection = -FMath::RadiansToDegrees(FMath::Acos(AngleBetweenVelocityAndDirection));
		}

		bInAir = Character->GetCharacterMovement()->IsFalling();
		if (bInAir && Character->CanWallJump() == false)
		{
			bWallJumped = true;
		}
		else
		{
			bWallJumped = false;
		}

		if (Character->IsPlayerControlled())
		{
			bFixedRotationToController = Cast<AWAPlayerCharacter>(Character)->IsTargetLocking();

			AWACharacter* LockedTarget = Cast<AWAPlayerCharacter>(Character)->GetLockedTarget();
			if (LockedTarget)
			{
				GetAimOffsetValue(Character, LockedTarget);
			}
			else
			{
				AimOffsetYaw = 0.0f;
				AimOffsetPitch = 0.0f;
			}
		}
	}
}

void UWAAnimInstance::SetInputMoveDirectionX(float NewAxisValue)
{
	InputMoveDirectionX = NewAxisValue;
}

void UWAAnimInstance::SetInputMoveDirectionY(float NewAxisValue)
{
	InputMoveDirectionY = NewAxisValue;
}

void UWAAnimInstance::GetAimOffsetValue(AActor* Center, AActor* Target)
{
	FVector CenterForward = Center->GetActorForwardVector();

	// Get AimOffset Yaw
	FVector CenterToTarget = Target->GetActorLocation() - Center->GetActorLocation();
	CenterToTarget.Z = 0.0f;
	CenterToTarget.Normalize();

	float AngleBetweenCenterForwardAndToTarget = FVector::DotProduct(CenterForward, CenterToTarget);
	bool bRight = FVector::CrossProduct(CenterForward, CenterToTarget).Z > 0.0f ? true : false;

	AimOffsetYaw = FMath::RadiansToDegrees(FMath::Acos(AngleBetweenCenterForwardAndToTarget));
	if (bRight == false)
	{
		AimOffsetYaw *= -1;
	}

	// Get AimOffset Pitch
	CenterToTarget = Target->GetActorLocation() - Center->GetActorLocation();
	CenterToTarget.Normalize();
	FVector CenterToTargetZeroZ = CenterToTarget;
	CenterToTargetZeroZ.Z = 0.0f;
	CenterToTargetZeroZ.Normalize();

	float AngleBetweenToTargetAndToTargetZeroZ = FVector::DotProduct(CenterToTarget, CenterToTargetZeroZ);
	AimOffsetPitch = FMath::RadiansToDegrees(FMath::Acos(AngleBetweenToTargetAndToTargetZeroZ));
	if (Center->GetActorLocation().Z > Target->GetActorLocation().Z)
	{
		AimOffsetPitch *= -1;
	}
}

void UWAAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UWAAnimInstance::AnimNotify_SpecialAttackHitCheck()
{
	OnSpecialAttackHitCheck.Broadcast();
}
