// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Backstep.h"
#include "WACharacter.h"
#include "WAAIController.h"

EBTNodeResult::Type UBTT_Backstep::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AWACharacter* pCharacter = Cast<AWACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (pCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	pCharacter->Backstep();

	return EBTNodeResult::Succeeded;
}