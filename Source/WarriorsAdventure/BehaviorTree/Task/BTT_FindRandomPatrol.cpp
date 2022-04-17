// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindRandomPatrol.h"
#include "WACharacter.h"
#include "WAAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTT_FindRandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result =	 Super::ExecuteTask(OwnerComp, NodeMemory);

	AWACharacter* pCharacter = Cast<AWACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (pCharacter)
	{
		FVector PatrolLocation = UNavigationSystemV1::GetRandomPointInNavigableRadius(GetWorld(), pCharacter->GetActorLocation(), PatrolRadius);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PatrolLocation")), PatrolLocation);

		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}