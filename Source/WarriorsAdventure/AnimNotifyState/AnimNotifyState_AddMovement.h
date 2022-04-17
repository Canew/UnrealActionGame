// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AddMovement.generated.h"

UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right
};

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UAnimNotifyState_AddMovement : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

private:
	UPROPERTY(EditAnywhere, Category = "Movement", Meta = (AllowPrivateAccess = true))
	EMoveDirection MoveDirection;

	UPROPERTY(EditAnywhere, Category = "Movement", Meta = (AllowPrivateAccess = true))
	float MoveSpeed = 1.0f;

	FHitResult HitResult;
	FVector LineTraceEnd;
	FVector Direction;
};
