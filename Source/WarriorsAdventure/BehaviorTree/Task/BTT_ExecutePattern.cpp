// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ExecutePattern.h"
#include "WACharacter.h"
#include "WAAIController.h"

UBTT_ExecutePattern::UBTT_ExecutePattern()
{
	NodeName = TEXT("Execute Pattern");
}

EBTNodeResult::Type UBTT_ExecutePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AWACharacter* pCharacter = Cast<AWACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (pCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (PatternType == EPatternType::Melee)
	{
		pCharacter->ExecuteMeleePattern();

		return EBTNodeResult::Succeeded;
	}
	if (PatternType == EPatternType::Range)
	{
		pCharacter->ExecuteRangePattern();

		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}