// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Backstep.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UBTT_Backstep : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
