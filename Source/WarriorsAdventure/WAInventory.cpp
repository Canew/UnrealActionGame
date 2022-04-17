// Fill out your copyright notice in the Description page of Project Settings.


#include "WAInventory.h"
#include "WACharacter.h"
#include "WAPlayerController.h"
#include "Widgets/WAInventoryMainWidget.h"
#include "Widgets/WAInventoryWidget.h"
#include "Widgets/WAInventorySlotWidget.h"

// Sets default values
AWAInventory::AWAInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UWAInventoryMainWidget> UI_INVENTORY_MAIN(TEXT("/Game/InventorySystem/Widgets/UI_InventoryMain.UI_InventoryMain_C"));
	if (UI_INVENTORY_MAIN.Succeeded())
	{
		InventoryMainWidgetClass = UI_INVENTORY_MAIN.Class;
	}
}

// Called when the game starts or when spawned
void AWAInventory::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWAInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWAInventory::CreateInventory()
{
	Slots.SetNum(AmountOfSlots);

	AWAPlayerController* PlayerController = Cast<AWAPlayerController>(CharacterRef->GetController());
	if (PlayerController)
	{
		InventoryMainWidget = CreateWidget<UWAInventoryMainWidget>(PlayerController, InventoryMainWidgetClass);
		InventoryMainWidget->SetInventory(this);
		InventoryMainWidget->SetSlotsPerRow(SlotsPerRow);
		InventoryMainWidget->AddToViewport();

		InventoryWidget = InventoryMainWidget->GetInventoryWidget();
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		bVisible = false;
	}
}

bool AWAInventory::IsSlotEmpty(int32 Index)
{
	if (Slots.IsValidIndex(Index))
	{
		if (Slots[Index].ItemClass != nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	return false;
}

FItemInfo AWAInventory::GetItemInfoAtIndex(int32 Index)
{
	if (Slots.IsValidIndex(Index))
	{
		if (Slots[Index].ItemClass)
		{
			FItemInfo DefaultItemInfo = Cast<AWAItem>(Slots[Index].ItemClass)->GetItemInfo();
			return DefaultItemInfo;
		}
	}

	return FItemInfo();
}

int32 AWAInventory::GetItemAmountAtIndex(int32 Index)
{
	if (Slots.IsValidIndex(Index))
	{
		if (Slots[Index].ItemClass)
		{
			int32 ItemAmount = Slots[Index].Amount;
			return ItemAmount;
		}
	}

	return 0;
}

int32 AWAInventory::FindEmptySlot()
{
	if (Slots.Num() == 0)
	{
		return -1;
	}

	int32 Index = 0;

	for (FInventorySlot Slot : Slots)
	{
		if (!Slot.ItemClass)
		{
			return Index;
		}
		Index += 1;
	}

	return -1;
}

int32 AWAInventory::FindCanStackSlot(AWAItem* Item)
{
	if (!Item->GetItemInfo().bCanBeStacked)
	{
		return -1;
	}

	int32 Index = 0;
	if (Slots.Num() == 0)
	{
		return -1;
	}
	else
	{
		for (FInventorySlot Slot : Slots)
		{
			if (Slot.ItemClass && Item->StaticClass() == Slot.ItemClass->StaticClass() && Slot.Amount < MaxStackSize)
			{
				return Index;
			}
			Index += 1;
		}
	}

	return -1;
}

bool AWAInventory::AddItem(AWAItem* Item, int32 Amount, int32& Rest_Out)
{
	/** If item can be stacked. */
	if (Item->GetItemInfo().bCanBeStacked)
	{
		int32 StackSlotIndex = FindCanStackSlot(Item);
		if (StackSlotIndex >= 0)
		{
			if ((Amount + Slots[StackSlotIndex].Amount) > MaxStackSize)
			{
				Slots[StackSlotIndex].ItemClass = Item;
				Slots[StackSlotIndex].Amount = MaxStackSize;
				UpdateSlotAtIndex(StackSlotIndex);
				AddItem(Item, Amount + Slots[StackSlotIndex].Amount - MaxStackSize, Rest_Out);

				return true;
			}
			else
			{
				Slots[StackSlotIndex].ItemClass = Item;
				Slots[StackSlotIndex].Amount = Amount + Slots[StackSlotIndex].Amount;
				UpdateSlotAtIndex(StackSlotIndex);

				Rest_Out = 0;
				return true;
			}
		}
		else
		{
			int32 EmptySlotIndex = FindEmptySlot();
			if (EmptySlotIndex >= 0)
			{
				if (Amount > MaxStackSize)
				{
					Slots[EmptySlotIndex].ItemClass = Item;
					Slots[EmptySlotIndex].Amount = MaxStackSize;
					UpdateSlotAtIndex(EmptySlotIndex);
					AddItem(Item, Amount - MaxStackSize, Rest_Out);

					return true;
				}
				else
				{
					Slots[EmptySlotIndex].ItemClass = Item;
					Slots[EmptySlotIndex].Amount = Amount;
					UpdateSlotAtIndex(EmptySlotIndex);

					Rest_Out = 0;
					return true;
				}
			}
			else
			{
				Rest_Out = Amount;
				return false;
			}
		}
	}

	/** If item cannot be stacked. */
	else
	{
		int32 EmptySlotIndex = FindEmptySlot();
		if (EmptySlotIndex >= 0)
		{
			Slots[EmptySlotIndex].ItemClass = Item;
			Slots[EmptySlotIndex].Amount = 1;
			UpdateSlotAtIndex(EmptySlotIndex);

			if (Amount > 1)
			{
				AddItem(Item, Amount - 1, Rest_Out);

				return true;
			}

			Rest_Out = 0;
			return true;
		}
		else
		{
			Rest_Out = Amount;
			return false;
		}
	}

	return false;
}

void AWAInventory::UpdateSlotAtIndex(int32 Index)
{
	if (InventoryWidget)
	{
		TArray<UWAInventorySlotWidget*>& InventorySlotWidgets = InventoryWidget->GetInventorySlotWidgets();
		if (InventorySlotWidgets.IsValidIndex(Index))
		{
			InventorySlotWidgets[Index]->UpdateSlot();
		}
	}
}

bool AWAInventory::RemoveItemAtIndex(int32 Index, int32 Amount)
{
	if (!Slots.IsValidIndex(Index))
	{
		return false;
	}

	if (!IsSlotEmpty(Index) && Amount > 0)
	{
		if (Amount >= GetItemAmountAtIndex(Index))
		{
			Slots[Index].ItemClass = nullptr;
			Slots[Index].Amount = 0;
		}
		else
		{
			Slots[Index].Amount -= Amount;
		}
		UpdateSlotAtIndex(Index);

		return true;
	}

	return false;
}

bool AWAInventory::SwapSlots(int32 Index1, int32 Index2)
{
	if (!Slots.IsValidIndex(Index1) || !Slots.IsValidIndex(Index2))
	{
		return false;
	}

	FInventorySlot TemporarySlot = Slots[Index2];
	Slots[Index2] = Slots[Index1];
	Slots[Index1] = TemporarySlot;
	UpdateSlotAtIndex(Index1);
	UpdateSlotAtIndex(Index2);

	return false;
}

bool AWAInventory::SplitStack(int32 Index, int32 Amount)
{
	if (!Slots.IsValidIndex(Index) || IsSlotEmpty(Index))
	{
		return false;
	}

	if (GetItemInfoAtIndex(Index).bCanBeStacked && GetItemAmountAtIndex(Index) > Amount)
	{
		int32 SplitIndex = FindEmptySlot();
		Slots[Index].Amount -= Amount;
		Slots[SplitIndex].ItemClass = Slots[Index].ItemClass;
		Slots[SplitIndex].Amount = Amount;
		UpdateSlotAtIndex(Index);
		UpdateSlotAtIndex(SplitIndex);

		return true;
	}
	else
	{
		return false;
	}
}

bool AWAInventory::UseItemAtIndex(int32 Index)
{
	if (!Slots.IsValidIndex(Index))
	{
		return false;
	}

	if (Slots[Index].ItemClass)
	{
		Slots[Index].ItemClass->EventOnUsed();
	}

	return false;
}

void AWAInventory::ShowInventory()
{
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	InventoryWidget->SetIsEnabled(true);
	bVisible = true;
}

void AWAInventory::HideInventory()
{
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->SetIsEnabled(false);
	bVisible = false;
}

UWAInventoryWidget* AWAInventory::GetInventoryWidget()
{
	return InventoryWidget;
}

TArray<FInventorySlot> AWAInventory::GetInventorySlots()
{
	return Slots;
}

bool AWAInventory::IsVisible()
{
	return bVisible;
}

void AWAInventory::SetCharacterRef(AWACharacter* _Character)
{
	CharacterRef = _Character;
}

void AWAInventory::SetAmountOfSlots(int32 _AmountOfSlots)
{
	AmountOfSlots = _AmountOfSlots;
}

void AWAInventory::SetSlotsPerRow(int32 _SlotsPerRow)
{
	SlotsPerRow = _SlotsPerRow;
}
