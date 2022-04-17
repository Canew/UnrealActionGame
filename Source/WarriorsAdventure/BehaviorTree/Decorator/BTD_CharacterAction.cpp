// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CharacterAction.h"
#include "WAAIController.h"
#include "WACharacter.h"

UBTD_CharacterAction::UBTD_CharacterAction()
{
	NodeName = TEXT("Character Action");
}

bool UBTD_CharacterAction::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess)
	{
		return false;
	}

	AWACharacter* pCharacter = Cast<AWACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (pCharacter == nullptr)
	{
		return false;
	}

	if (pCharacter->GetCharacterAction() == CharacterAction)
	{
		if (BasicOperation == EBasicKeyOperation::Set)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (BasicOperation == EBasicKeyOperation::Set)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}