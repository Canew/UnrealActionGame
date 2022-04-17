// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WARadarComponent.h"
#include "WACharacter.h"
#include "WAPlayerCharacter.h"
#include "WAPlayerController.h"
#include "Widgets/WAMainWidget.h"
#include "Widgets/WARadarWidget.h"
#include "WALandmark.h"
#include "WASecret.h"
#include "WAEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UWARadarComponent::UWARadarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_MAIN(TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/UI_Main.UI_Main_C'"));
	if (UI_MAIN.Succeeded())
	{
		MainWidgetClass = UI_MAIN.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_RADAR(TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/UI_Radar.UI_Radar_C'"));
	if (UI_RADAR.Succeeded())
	{
		RadarWidgetClass = UI_RADAR.Class;
	}

	// Add Directions
	FDirectionInfo North, East, South, West;
	North.Name = FText::FromString(TEXT("N"));
	North.WorldRotation = 0;
	East.Name = FText::FromString(TEXT("E"));
	East.WorldRotation = 90;
	South.Name = FText::FromString(TEXT("S"));
	South.WorldRotation = 180;
	West.Name = FText::FromString(TEXT("W"));
	West.WorldRotation = -90;
	Directions.Add(North);
	Directions.Add(East);
	Directions.Add(South);
	Directions.Add(West);

	// Enemy Marker
	EnemyMarkerRadius = 500.0f;
}


void UWARadarComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWARadarComponent::InitializeRadar()
{
	ACharacter* WACharacter = Cast<ACharacter>(GetOwner());
	if (WACharacter)
	{
		if (WACharacter->IsPlayerControlled())
		{
			AWAPlayerController* WAPlayerController = Cast<AWAPlayerController>(WACharacter->GetController());
			
			MainWidget = CreateWidget<UWAMainWidget>(WAPlayerController, MainWidgetClass);
			if (MainWidget)
			{
				RadarWidget = MainWidget->GetRadarWidget();

				for (FDirectionInfo Direction : Directions)
				{
					RadarWidget->AddDirection(Direction);
				}

				for (FMarkerInfo MarkerInfo : DefaultQuestMarkers)
				{
					AddMarkerToRader(MarkerInfo);
				}
				
				TArray<AActor*> LandmarkResults;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWALandmark::StaticClass(), LandmarkResults);
				for (AActor* LandmarkResult : LandmarkResults)
				{
					AddLandmarkToRader(Cast<AWALandmark>(LandmarkResult));
				}

				OnBecomeVisible();

				MainWidget->AddToViewport();
				//StartDetectingEnemy();
			}
		}
	}
}

void UWARadarComponent::ToggleVisibility(bool Animate)
{
	SetVisibility(!bVisible, Animate);
}

void UWARadarComponent::SetVisibility(bool Visible, bool Animate)
{
	if (bVisible == Visible || (RadarWidget == nullptr))
	{
		return;
	}

	if (bCanChangeVisibility)
	{
		bCanChangeVisibility = false;
		bVisible = Visible;

		if (Animate)
		{
			if (Visible)
			{
				OnBecomeVisible();
				RadarWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				RadarWidget->PlayAnimation(RadarWidget->GetBlendOutAnim(), 0.0f, 1, EUMGSequencePlayMode::Reverse);

				FTimerHandle WidgetAnimTimerHandle;
				float WidgetAnimTime = RadarWidget->GetBlendOutAnim()->GetEndTime();;
				GetWorld()->GetTimerManager().SetTimer(WidgetAnimTimerHandle, FTimerDelegate::CreateLambda([&]()
					{
						bCanChangeVisibility = true;
					}), WidgetAnimTime, false);
			}
			else
			{
				RadarWidget->PlayAnimation(RadarWidget->GetBlendOutAnim(), 0.0f, 1, EUMGSequencePlayMode::Forward);

				FTimerHandle WidgetAnimTimerHandle;
				float WidgetAnimTime = RadarWidget->GetBlendOutAnim()->GetEndTime();;
				GetWorld()->GetTimerManager().SetTimer(WidgetAnimTimerHandle, FTimerDelegate::CreateLambda([&]()
					{
						RadarWidget->SetVisibility(ESlateVisibility::Hidden);
						bCanChangeVisibility = true;
					}), WidgetAnimTime, false);
			}
		}

		else
		{
			if (Visible)
			{
				OnBecomeVisible();
				RadarWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				RadarWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			bCanChangeVisibility = true;
		}
	}
}

float UWARadarComponent::RotationToCircleDegrees(float Degree)
{
	if (Degree < 0.0f)
	{
		Degree += 360.0f;
		Degree = std::fmod(Degree, 360.0f);
		return Degree;
	}
	else
	{
		return Degree;
	}
}

float UWARadarComponent::GetDeltaClockwise(float A, float B, bool Clockwise)
{
	float Delta = 0.0f;
	A = std::fmod(A, 360.0f);
	B = std::fmod(B, 360.0f);

	if (Clockwise)
	{
		if (A == B)
		{
			return Delta;
		}
		else if (A > B)
		{
			Delta = 360.0f - (A - B);
			return Delta;
		}
		else
		{
			Delta = B - A;
			return Delta;
		}
	}
	else
	{
		if (A == B)
		{
			return Delta;
		}
		else if (A > B)
		{
			Delta = A - B;
			return Delta;
		}
		else
		{
			Delta = 360.0f - (B - A);
			return Delta;
		}
	}
}

FVector2D UWARadarComponent::RotationsToTranslation(FRotator RotatorA, FRotator RotatorB, bool& Clockwise_out, bool& InRaderSight_out)
{
	float FieldOfView = 90.0f;
	AWAPlayerCharacter* WAPlayerCharacter = Cast<AWAPlayerCharacter>(GetOwner());
	if (WAPlayerCharacter)
	{
		if (WAPlayerCharacter->IsPlayerControlled())
		{
			FieldOfView = WAPlayerCharacter->Camera->FieldOfView;
		}
		else
		{
			FieldOfView = 90.0f;
		}
	}

	float ClockwiseDelta = GetDeltaClockwise(RotationToCircleDegrees(RotatorA.Yaw), RotationToCircleDegrees(RotatorB.Yaw), true);
	float CounterClockwiseDelta = GetDeltaClockwise(RotationToCircleDegrees(RotatorA.Yaw), RotationToCircleDegrees(RotatorB.Yaw), false);

	if (ClockwiseDelta > CounterClockwiseDelta)
	{
		if ((CounterClockwiseDelta / FieldOfView) <= 1.0f)
		{
			Clockwise_out = false;
			InRaderSight_out = true;
			return FVector2D(-1.0f * FMath::Lerp(0.0f, MaxWidgetTranslation, CounterClockwiseDelta / FieldOfView), 0.0f);
		}
		else
		{
			Clockwise_out = false;
			InRaderSight_out = false;
			return FVector2D(FMath::Lerp(0.0f, MaxWidgetTranslation, CounterClockwiseDelta / FieldOfView), 0.0f);
		}
	}	
	else
	{
		if ((ClockwiseDelta / FieldOfView) <= 1.0f)
		{
			Clockwise_out = true;
			InRaderSight_out = true;
			return FVector2D(FMath::Lerp(0.0f, MaxWidgetTranslation, ClockwiseDelta / FieldOfView), 0.0f);
		}
		else
		{
			Clockwise_out = true;
			InRaderSight_out = false;
			return FVector2D(0.0f, 0.0f);
		}
	}
}

int32 UWARadarComponent::GetDistanceToPlayer(AActor* Actor)
{
	AActor* PlayerActor = GetOwner();
	if (!Actor || !PlayerActor)
	{
		return 0.0f;
	}

	FVector TargetLocation = FVector(Actor->GetActorLocation().X, Actor->GetActorLocation().Y, 0.0f);
	FVector PlayerLocation = FVector(PlayerActor->GetActorLocation().X, PlayerActor->GetActorLocation().Y, 0.0f);

	float Distance = (TargetLocation - PlayerLocation).Size();
	Distance /= UnitsPerMeter;
	return FMath::RoundToInt(Distance);
}

void UWARadarComponent::UpdateDirectionWidgets()
{
	AWAPlayerCharacter* WAPlayerCharacter = Cast<AWAPlayerCharacter>(GetOwner());
	if (WAPlayerCharacter && WAPlayerCharacter->IsPlayerControlled())
	{
		TArray<UWARadarDirectionWidget*> DirectionWidgets = RadarWidget->GetDirectionWidgets();
		for (UWARadarDirectionWidget* DirectionWidget : DirectionWidgets)
		{
			FRotator RotationA, RotationB;
			bool Clockwise, InRaderSight;

			// Get Rotation A from Character Camera.
			UCameraComponent* Camera = WAPlayerCharacter->Camera;
			if (Camera)
			{
				RotationA = Camera->GetComponentRotation();
			}

			// Get Rotation B from World Rotation in Direction Info.
			RotationB = FRotator(0.0f, DirectionWidget->GetDirectionInfo().WorldRotation, 0.0f);

			FVector2D Translation2D = RotationsToTranslation(RotationA, RotationB, Clockwise, InRaderSight);

			if (InRaderSight)
			{
				DirectionWidget->SetRenderTranslation(Translation2D);
				if (!DirectionWidget->IsVisible())
				{
					DirectionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
			else
			{
				if (DirectionWidget->IsVisible())
				{
					DirectionWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
}

void UWARadarComponent::AddMarkerToRader(FMarkerInfo Info)
{
	FMarkerInfo LocalInfo = Info;
	QuestMarkers.Add(LocalInfo);

	if (RadarWidget)
	{
		RadarWidget->AddMarker(LocalInfo.Type);
	}
}

void UWARadarComponent::UpdateMarkersDistance()
{
	int32 MarkerIndex = 0;
	for (TWeakObjectPtr<UWARadarMarkerWidget> MarkerWidget : RadarWidget->GetMarkerWidgets())
	{
		if (MarkerWidget.IsValid() && QuestMarkers.IsValidIndex(MarkerIndex))
		{
			float DistanceToMarker = (QuestMarkers[MarkerIndex].Location - GetOwner()->GetActorLocation()).Size();
			MarkerWidget->UpdateDistance(FMath::RoundToInt(DistanceToMarker /= UnitsPerMeter));

			MarkerIndex++;
		}
	}
}

void UWARadarComponent::UpdateMarkersPosition()
{
	AWAPlayerCharacter* WAPlayerCharacter = Cast<AWAPlayerCharacter>(GetOwner());
	if (!WAPlayerCharacter || !WAPlayerCharacter->IsPlayerControlled())
	{
		return;
	}
	UCameraComponent* PlayerCamera = WAPlayerCharacter->Camera;

	int32 MarkerIndex = 0;
	for (TWeakObjectPtr<UWARadarMarkerWidget> MarkerWidget : RadarWidget->GetMarkerWidgets())
	{
		if (MarkerWidget.IsValid() && QuestMarkers.IsValidIndex(MarkerIndex))
		{
			FRotator CameraRotation = PlayerCamera->GetComponentRotation();
			FRotator PlayerToMarkerRotation = UKismetMathLibrary::FindLookAtRotation(WAPlayerCharacter->GetActorLocation(), QuestMarkers[MarkerIndex].Location);
			bool Clockwise, InRaderSight;
			FVector2D Translation = RotationsToTranslation(CameraRotation, PlayerToMarkerRotation, Clockwise, InRaderSight);

			if (InRaderSight)
			{
				MarkerWidget->SetRenderTranslation(Translation);
				MarkerWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				MarkerWidget->SetVisibility(ESlateVisibility::Hidden);
			}

			MarkerIndex++;
		}
	}
}

bool UWARadarComponent::RemoveQuestMarker(FMarkerInfo Info)
{
	int32 FindMarkerIndex = -1;
	int32 MarkerIndex = 0;
	for (FMarkerInfo QuestMarker : QuestMarkers)
	{
		if (Info.Type == QuestMarker.Type && Info.Location == QuestMarker.Location)
		{
			FindMarkerIndex = MarkerIndex;
			break;
		}
		++MarkerIndex;
	}
	if (FindMarkerIndex >= 0)
	{
		QuestMarkers.RemoveAt(FindMarkerIndex);
		RadarWidget->GetMarkerWidgets()[FindMarkerIndex]->RemoveFromParent();
		RadarWidget->GetMarkerWidgets().RemoveAt(FindMarkerIndex);

		return true;
	}
	else
	{
		return false;
	}
}

bool UWARadarComponent::RemoveMarkerAtIndex(int32 MarkerIndex)
{
	if (QuestMarkers.Num() > 0 && MarkerIndex >= 0 && MarkerIndex < QuestMarkers.Num())
	{
		RemoveQuestMarker(QuestMarkers[MarkerIndex]);
		return true;
	}
	else
	{
		return false;
	}
}

void UWARadarComponent::UpdateEnemyDistance(AWAEnemyCharacter* Enemy)
{
	int32 EnemyIndex = EnemiesOnRadar.Find(Enemy);
	if (EnemyIndex != INDEX_NONE && RadarWidget->GetEnemyMarkerWidgets().IsValidIndex(EnemyIndex))
	{
		UWARadarEnemyMarkerWidget* EnemyMarker = RadarWidget->GetEnemyMarkerWidgets()[EnemyIndex];
		if (EnemyMarker)
		{
			EnemyMarker->UpdateDistance(GetDistanceToPlayer(Enemy));
		}
	}
}

void UWARadarComponent::UpdateEnemyPosition(AWAEnemyCharacter* Enemy)
{
	int32 EnemyIndex = EnemiesOnRadar.Find(Enemy);
	if (EnemyIndex != INDEX_NONE && RadarWidget->GetEnemyMarkerWidgets().IsValidIndex(EnemyIndex))
	{
		UWARadarEnemyMarkerWidget* EnemyMarkerWidget = RadarWidget->GetEnemyMarkerWidgets()[EnemyIndex];
		if (EnemyMarkerWidget)
		{
			AWAPlayerCharacter* WAPlayerCharacter = Cast<AWAPlayerCharacter>(GetOwner());
			if (!WAPlayerCharacter || !WAPlayerCharacter->IsPlayerControlled())
			{
				return;
			}
			UCameraComponent* PlayerCamera = WAPlayerCharacter->Camera;

			FRotator CameraRotation = PlayerCamera->GetComponentRotation();
			FRotator PlayerToMarkerRotation = UKismetMathLibrary::FindLookAtRotation(WAPlayerCharacter->GetActorLocation(), Enemy->GetActorLocation());
			bool Clockwise, InRaderSight;
			FVector2D Translation = RotationsToTranslation(CameraRotation, PlayerToMarkerRotation, Clockwise, InRaderSight);

			if (EnemyMarkerWidget)
			{
				if (InRaderSight)
				{
					EnemyMarkerWidget->SetRenderTranslation(Translation);
					EnemyMarkerWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
				else
				{
					EnemyMarkerWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
}

void UWARadarComponent::UpdateEnemyMarkersDistance()
{
	for (AWAEnemyCharacter* Enemy : EnemiesOnRadar)
	{
		UpdateEnemyDistance(Enemy);
	}
}

void UWARadarComponent::UpdateEnemyMarkersPosition()
{
	for (AWAEnemyCharacter* Enemy : EnemiesOnRadar)
	{
		UpdateEnemyPosition(Enemy);
	}
}

bool UWARadarComponent::AddEnemyToRadar(AWAEnemyCharacter* Enemy)
{
	if (!RadarWidget)
	{
		return false;
	}

	if (EnemiesOnRadar.Contains(Enemy))
	{
		return false;
	}
	else
	{
		EnemiesOnRadar.Add(Enemy);
		RadarWidget->AddEnemyMarker();
		UpdateEnemyDistance(Enemy);
		UpdateEnemyPosition(Enemy);

		return true;
	}
}

bool UWARadarComponent::RemoveEnemyToRadar(AWAEnemyCharacter* Enemy)
{
	int32 EnemyIndex = EnemiesOnRadar.Find(Enemy);
	if (EnemyIndex != INDEX_NONE && RadarWidget->GetEnemyMarkerWidgets().IsValidIndex(EnemyIndex))
	{
		EnemiesOnRadar.RemoveAt(EnemyIndex);
		UWARadarEnemyMarkerWidget* EnemyMarkerWidget = RadarWidget->GetEnemyMarkerWidgets()[EnemyIndex];
		if (EnemyMarkerWidget)
		{
			EnemyMarkerWidget->RemoveFromParent();
			RadarWidget->GetEnemyMarkerWidgets().RemoveAt(EnemyIndex);

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void UWARadarComponent::StartDetectingEnemy()
{
	GetWorld()->GetTimerManager().SetTimer(EnemyDetectTimerHandle, FTimerDelegate::CreateLambda([&]() {

		// Add enemies in range to array.

		TArray<FOverlapResult> OverlapResults;
		FCollisionShape CollisionSphere;
		CollisionSphere.SetSphere(EnemyMarkerRadius);
		FCollisionObjectQueryParams CollisionObjectQueryParams;
		CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel6);
		if (GetWorld()->OverlapMultiByObjectType(OverlapResults, GetOwner()->GetActorLocation(), FQuat::Identity, CollisionObjectQueryParams, CollisionSphere))
		{
			for (FOverlapResult& OverlapResult : OverlapResults)
			{
				AddEnemyToRadar(Cast<AWAEnemyCharacter>(OverlapResult.Actor)); 
			}
		}
		
		// Remove enemies out of range.
		
		for (TWeakObjectPtr<AWAEnemyCharacter> EnemyCharacter : EnemiesOnRadar)
		{
			if (EnemyCharacter.IsValid())
			{
				float DistanceToEnemy = (EnemyCharacter->GetActorLocation() - GetOwner()->GetActorLocation()).Size();
				if (DistanceToEnemy > EnemyMarkerRadius)
				{
					RemoveEnemyToRadar(EnemyCharacter.Get());
				}
			}
		}

		// Update enemy markers.

		UpdateEnemyMarkersDistance();
		UpdateEnemyMarkersPosition();

		}), EnemyDetectInterval, true);
}

void UWARadarComponent::EndDetectingEnemy()
{
	GetWorld()->GetTimerManager().ClearTimer(EnemyDetectTimerHandle);
}

void UWARadarComponent::AddLandmarkToRader(AWALandmark* Landmark)
{
	if (Landmarks.Contains(Landmark))
	{
		return;
	}
	else
	{
		Landmarks.Add(Landmark);
		if (RadarWidget)
		{
			RadarWidget->AddLandmark(Landmark->GetLandmarkTexture());
		}
	}
}

void UWARadarComponent::UpdateLandmarksPosition()
{
	AWAPlayerCharacter* WAPlayerCharacter = Cast<AWAPlayerCharacter>(GetOwner());
	if (!WAPlayerCharacter || !WAPlayerCharacter->IsPlayerControlled())
	{
		return;
	}
	UCameraComponent* PlayerCamera = WAPlayerCharacter->Camera;

	int32 LandmarkIndex = 0;
	for (UWARadarLandmarkWidget* LandmarkWidget : RadarWidget->GetLandmarkWidgets())
	{
		FRotator CameraRotation = PlayerCamera->GetComponentRotation();
		FRotator PlayerToMarkerRotation = UKismetMathLibrary::FindLookAtRotation(WAPlayerCharacter->GetActorLocation(), Landmarks[LandmarkIndex]->GetActorLocation());
		bool Clockwise, InRaderSight;
		FVector2D Translation = RotationsToTranslation(CameraRotation, PlayerToMarkerRotation, Clockwise, InRaderSight);

		if (LandmarkWidget)
		{
			if (InRaderSight)
			{
				LandmarkWidget->SetRenderTranslation(Translation);
				LandmarkWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				LandmarkWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		LandmarkIndex++;
	}
}

void UWARadarComponent::OnPlayerTurned()
{
	if (bVisible)
	{
		UpdateDirectionWidgets();
		UpdateMarkersPosition();
		UpdateEnemyMarkersPosition();
		UpdateLandmarksPosition();
	}
}

void UWARadarComponent::OnPlayerMoved()
{
	if (bVisible)
	{
		UpdateMarkersDistance();
		UpdateMarkersPosition();
		UpdateEnemyMarkersDistance();
		UpdateEnemyMarkersPosition();
		UpdateLandmarksPosition();
	}
}

void UWARadarComponent::OnMonsterMoved(AWAEnemyCharacter* Enemy)
{
	UpdateEnemyDistance(Enemy);
	UpdateEnemyPosition(Enemy);
}

void UWARadarComponent::OnEnterSecretRange(AWASecret* Secret)
{
	if (!SecretsInRange.Contains(Secret))
	{
		SecretsInRange.Add(Secret);
		if (SecretsInRange.Num() == 1)
		{
			RadarWidget->HighlightRadar(true);
		}
	}
}

void UWARadarComponent::OnLeaveSecretRange(AWASecret* Secret)
{
	if (SecretsInRange.Contains(Secret))
	{
		SecretsInRange.Remove(Secret);
		if (SecretsInRange.Num() == 0)
		{
			RadarWidget->HighlightRadar(false);
		}
	}
}

void UWARadarComponent::OnBecomeVisible()
{
	UpdateMarkersPosition();
	UpdateMarkersDistance();
	UpdateEnemyMarkersDistance();
	UpdateEnemyMarkersPosition();
	UpdateDirectionWidgets();
	UpdateLandmarksPosition();
}