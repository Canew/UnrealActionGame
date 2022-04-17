// Fill out your copyright notice in the Description page of Project Settings.


#include "WAItem.h"
#include "WAInventory.h"

// Sets default values
AWAItem::AWAItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWAItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWAItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWAItem::EventOnUsed()
{
	FString DebugText = FString(TEXT("Using ")) + ItemInfo.Name.ToString();
	UE_LOG(LogTemp, Log, TEXT("%s"), *DebugText);
}

FItemInfo AWAItem::GetItemInfo()
{
	return ItemInfo;
}

