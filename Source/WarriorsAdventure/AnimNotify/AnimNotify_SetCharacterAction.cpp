// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SetCharacterAction.h"
#include "WACharacter.h"

void UAnimNotify_SetCharacterAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AWACharacter* pCharacter = Cast<AWACharacter>(MeshComp->GetOwner());
	if (pCharacter)
	{
		pCharacter->SetCharacterAction(CharacterAction);
	}
}