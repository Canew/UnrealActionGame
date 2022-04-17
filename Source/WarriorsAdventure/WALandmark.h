// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WALandmark.generated.h"

UCLASS()
class WARRIORSADVENTURE_API AWALandmark : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWALandmark();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UTexture2D* GetLandmarkTexture() { return IconOnRader; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	UBillboardComponent* Billboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	UTexture2D* IconOnRader;
};
