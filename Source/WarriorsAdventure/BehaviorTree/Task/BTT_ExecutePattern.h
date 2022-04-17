// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ExecutePattern.generated.h"

UENUM(BlueprintType)
enum class EPatternType : uint8
{
	Melee,
	Range
};

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UBTT_ExecutePattern : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_ExecutePattern();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere)
	EPatternType PatternType;
};
