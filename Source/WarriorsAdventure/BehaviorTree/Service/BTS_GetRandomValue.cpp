// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_GetRandomValue.h"
#include "WAAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_GetRandomValue::UBTS_GetRandomValue()
{
	NodeName = TEXT("UBTS_GetRandomValue");
}

void UBTS_GetRandomValue::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto AIController = Cast<AWAAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
		return;

	int32 RandomValue = FMath::RandRange(Min, Max);

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(AIController->RandomPatternValue, RandomValue);
}