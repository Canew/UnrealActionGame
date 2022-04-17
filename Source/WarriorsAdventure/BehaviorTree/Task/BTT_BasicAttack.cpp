// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BasicAttack.h"
#include "WACharacter.h"
#include "WAAIController.h"

EBTNodeResult::Type UBTT_BasicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Enable TickTask()
	bNotifyTick = true;

	AWACharacter* pCharacter = Cast<AWACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (pCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}


	int32 BasicAttackCombo = FMath::RandRange(0, pCharacter->GetMaxBasicAttackCombo() - 1);
	pCharacter->BasicAttack(BasicAttackCombo);

	return EBTNodeResult::InProgress;
}

void UBTT_BasicAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AWACharacter* pCharacter = Cast<AWACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (pCharacter)
	{
		if (pCharacter->GetCharacterAction() == ECharacterAction::Idle)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}