// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FindRandomPatrol.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UBTT_FindRandomPatrol : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	UPROPERTY(EditAnywhere, Category = Patrol)
	float PatrolRadius;
};
