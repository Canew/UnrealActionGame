// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "../Widgets/WARadarDirectionWidget.h"
#include "../Widgets/WARadarMarkerWidget.h"
#include "../Widgets/WARadarEnemyMarkerWidget.h"
#include "WARadarComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIORSADVENTURE_API UWARadarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWARadarComponent();

	virtual void BeginPlay() override;

	void InitializeRadar();
	void ToggleVisibility(bool Animate);
	void SetVisibility(bool Visible, bool Animate);
	float RotationToCircleDegrees(float Degree);
	float GetDeltaClockwise(float A, float B, bool Clockwise);
	FVector2D RotationsToTranslation(FRotator RotatorA, FRotator RotatorB, bool& Clockwise_out, bool& InRaderSight_out);
	int32 GetDistanceToPlayer(AActor* Actor);

	// Direction

	void UpdateDirectionWidgets();

	// Quest Marker

	void AddMarkerToRader(FMarkerInfo Info);
	void UpdateMarkersDistance();
	void UpdateMarkersPosition();
	bool RemoveQuestMarker(FMarkerInfo Info);
	bool RemoveMarkerAtIndex(int32 MarkerIndex);

	// Enemy Marker

	void UpdateEnemyDistance(class AWAEnemyCharacter* Enemy);
	void UpdateEnemyPosition(class AWAEnemyCharacter* Enemy);
	void UpdateEnemyMarkersDistance();
	void UpdateEnemyMarkersPosition();
	bool AddEnemyToRadar(class AWAEnemyCharacter* Enemy);
	bool RemoveEnemyToRadar(class AWAEnemyCharacter* Enemy);
	void StartDetectingEnemy();
	void EndDetectingEnemy();

	// Landmark

	void AddLandmarkToRader(class AWALandmark* Landmark);
	void UpdateLandmarksPosition();

	// Event

	void OnPlayerTurned();
	void OnPlayerMoved();
	void OnMonsterMoved(class AWAEnemyCharacter* Enemy);
	void OnEnterSecretRange(class AWASecret* Secret);
	void OnLeaveSecretRange(class AWASecret* Secret);
	void OnBecomeVisible();

private:
	TSubclassOf<class UWAMainWidget> MainWidgetClass;
	class UWAMainWidget* MainWidget;

	TSubclassOf<class UWARadarWidget> RadarWidgetClass;
	class UWARadarWidget* RadarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", Meta = (AllowPrivateAccess = true))
	TArray<struct FDirectionInfo> Directions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker", Meta = (AllowPrivateAccess = true))
	TArray<struct FMarkerInfo> QuestMarkers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker", Meta = (AllowPrivateAccess = true))
	TArray<struct FMarkerInfo> DefaultQuestMarkers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker", Meta = (AllowPrivateAccess = true))
	TArray<class AWALandmark*> Landmarks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker", Meta = (AllowPrivateAccess = true))
	TArray<class AWASecret*> SecretsInRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker", Meta = (AllowPrivateAccess = true))
	TArray<class AWAEnemyCharacter*> EnemiesOnRadar;

	/** If enemy in range, add to EnemiesOnRadar array. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker", Meta = (AllowPrivateAccess = true))
	float EnemyMarkerRadius;

	FTimerHandle EnemyDetectTimerHandle;
	float EnemyDetectInterval = 0.05f;

	float MaxWidgetTranslation = 320.0f;
	float UnitsPerMeter = 150.0f;

	bool bVisible = true;
	bool bCanChangeVisibility = true;

};
