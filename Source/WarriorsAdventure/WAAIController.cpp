// Fill out your copyright notice in the Description page of Project Settings.


#include "WAAIController.h"
#include "WAPlayerCharacter.h"
#include "WAPlayerController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"


AWAAIController::AWAAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_MELEEBASICMINION(TEXT("BehaviorTree'/Game/AI/BT_MeleeBasicMinion.BT_MeleeBasicMinion'"));
	if (BT_MELEEBASICMINION.Succeeded())
	{
		BT_MeleeBasicMinion = BT_MELEEBASICMINION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_MELEEBASICMINION(TEXT("BlackboardData'/Game/AI/BB_MeleeBasicMinion.BB_MeleeBasicMinion'"));
	if (BB_MELEEBASICMINION.Succeeded())
	{
		BB_MeleeBasicMinion = BB_MELEEBASICMINION.Object;
	}

	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	
	// Set team.
	SetGenericTeamId(FGenericTeamId(2));
}

void AWAAIController::BeginPlay()
{
	Super::BeginPlay();

	//GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AWAAIController::TargetPerceptionUpdated);
}

void AWAAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BB_MeleeBasicMinion, Blackboard))
	{
		RunBehaviorTree(BT_MeleeBasicMinion);
	}

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AWAAIController::TargetPerceptionUpdated);
}

void AWAAIController::PerceptionUpdated(const TArray<AActor*>& Actors)
{

}

void AWAAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor->ActorHasTag(TEXT("Player")) && Stimulus.WasSuccessfullySensed())
	{
		GetWorld()->GetTimerManager().ClearTimer(LineOfSightTimerHandle);
		GetBlackboardComponent()->SetValueAsBool(FName(TEXT("HasLineOfSight")), true);
		if (Actor)
		{
			GetBlackboardComponent()->SetValueAsObject(FName(TEXT("EnemyCharacter")), Actor);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(LineOfSightTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				GetBlackboardComponent()->SetValueAsBool(FName(TEXT("HasLineOfSight")), false);
				if (Actor)
				{
					GetBlackboardComponent()->SetValueAsObject(FName(TEXT("EnemyCharacter")), Actor);
				}
			}), LineOfSightTimer, false);
	}
}

ETeamAttitude::Type AWAAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			FGenericTeamId OtherTeamId = TeamAgent->GetGenericTeamId();
			if (GetGenericTeamId() == OtherTeamId)
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;
			}
		}
	}

	return ETeamAttitude::Neutral;
}

void AWAAIController::SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority)
{
	Super::SetFocus(NewFocus, InPriority);

	AWAPlayerCharacter* PlayerCharacter = Cast<AWAPlayerCharacter>(NewFocus);
	if (PlayerCharacter)
	{
		AWAPlayerController* PlayerController = Cast<AWAPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			AWACharacter* BossCharacter = Cast<AWACharacter>(GetPawn());
			PlayerController->AddBossWidget(BossCharacter);
		}
	}
}
