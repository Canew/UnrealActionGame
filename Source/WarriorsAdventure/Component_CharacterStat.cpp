// Fill out your copyright notice in the Description page of Project Settings.


#include "Component_CharacterStat.h"

// Sets default values for this component's properties
UComponent_CharacterStat::UComponent_CharacterStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UComponent_CharacterStat::BeginPlay()
{
	Super::BeginPlay();


	
}


// Called every frame
void UComponent_CharacterStat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

