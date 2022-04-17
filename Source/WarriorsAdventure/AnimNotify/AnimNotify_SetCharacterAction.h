// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SetCharacterAction.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UAnimNotify_SetCharacterAction : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

private:
	UPROPERTY(EditAnywhere, Category = "Action")
	ECharacterAction CharacterAction;
};
