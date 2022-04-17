// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetFocus.h"
#include "WAAIController.h"
#include "WACharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_SetFocus::UBTT_SetFocus()
{
	NodeName = TEXT("Set Focus");
}

EBTNodeResult::Type UBTT_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AWAAIController* WAAIController = Cast<AWAAIController>(OwnerComp.GetOwner());
	if (WAAIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (bSet)
	{
		AWACharacter* Target = Cast<AWACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
		if (Target == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		WAAIController->SetFocus(Target);

		return EBTNodeResult::Succeeded;
	}
	else
	{
		WAAIController->SetFocus(nullptr);

		return EBTNodeResult::Succeeded;
	}
}