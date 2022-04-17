// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WAItem.h"
#include "WAInventory.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
	UPROPERTY()
	class AWAItem* ItemClass;

	UPROPERTY()
	int32 Amount;
};

UCLASS()
class WARRIORSADVENTURE_API AWAInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWAInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void CreateInventory();
	virtual void ShowInventory();
	virtual void HideInventory();
	virtual bool IsSlotEmpty(int32 Index);
	virtual FItemInfo GetItemInfoAtIndex(int32 Index);
	virtual int32 GetItemAmountAtIndex(int32 Index);
	virtual int32 FindEmptySlot();
	virtual int32 FindCanStackSlot(AWAItem* Item);
	virtual bool AddItem(AWAItem* Item, int32 Amount, int32& Rest_Out);
	virtual void UpdateSlotAtIndex(int32 Index);
	virtual bool RemoveItemAtIndex(int32 Index, int32 Amount);
	virtual bool SwapSlots(int32 Index1, int32 Index2);
	virtual bool SplitStack(int32 Index, int32 Amount);
	virtual bool UseItemAtIndex(int32 Index);

	class UWAInventoryWidget* GetInventoryWidget();
	TArray<FInventorySlot> GetInventorySlots();
	bool IsVisible();
	void SetCharacterRef(class AWACharacter* _Character);
	void SetAmountOfSlots(int32 _AmountOfSlots);
	void SetSlotsPerRow(int32 _SlotsPerRow);

private:
	int32 AmountOfSlots = 20;
	int32 MaxStackSize = 99;

	class AWACharacter* CharacterRef;

	TArray<FInventorySlot> Slots;
	int32 SlotsPerRow = 4;

	TSubclassOf<class UWAInventoryMainWidget> InventoryMainWidgetClass;
	class UWAInventoryMainWidget* InventoryMainWidget;
	class UWAInventoryWidget* InventoryWidget;

	bool bVisible = false;
};
