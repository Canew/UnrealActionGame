// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "WAInteractInterface.h"
#include "GameFramework/Actor.h"
#include "WABonfire.generated.h"

UCLASS()
class WARRIORSADVENTURE_API AWABonfire : public AActor, public IWAInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWABonfire();	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeInteracted(AActor* InteractCauser) override;
	
private:
	void Activate();

public:
	UPROPERTY(VisibleAnywhere, Category = "Bonfire")
	class UStaticMeshComponent* Bonfire;
	UPROPERTY(VisibleAnywhere, Category = "Bonfire")
	class UParticleSystemComponent* FireParticle;

private:
	bool bActivated = false;
};
