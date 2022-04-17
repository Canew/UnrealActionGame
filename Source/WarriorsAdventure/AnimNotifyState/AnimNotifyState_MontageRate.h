// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_MontageRate.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UAnimNotifyState_MontageRate : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

private:
	UPROPERTY(EditAnywhere, Category = Rate)
	float Rate = 1.0f;

	float OriginRate;
};
