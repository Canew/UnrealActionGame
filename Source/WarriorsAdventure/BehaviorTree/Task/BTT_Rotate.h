// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Rotate.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UBTT_Rotate : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_Rotate();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector TargetKey;
};
