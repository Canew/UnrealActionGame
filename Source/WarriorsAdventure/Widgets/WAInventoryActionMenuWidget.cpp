// Fill out your copyright notice in the Description page of Project Settings.


#include "WAInventoryActionMenuWidget.h"
#include "WAInventory.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UWAInventoryActionMenuWidget::NativeConstruct()
{
	UseButton->OnClicked.AddDynamic(this, &UWAInventoryActionMenuWidget::OnClickedUseButton);
	ThrowButton->OnClicked.AddDynamic(this, &UWAInventoryActionMenuWidget::OnClickedThrowButton);
	SplitButton->OnClicked.AddDynamic(this, &UWAInventoryActionMenuWidget::OnClickedSplitButton);
	CancelButton->OnClicked.AddDynamic(this, &UWAInventoryActionMenuWidget::OnClickedCancelButton);
}

void UWAInventoryActionMenuWidget::Update(int32 NewIndex)
{
	CurrentIndex = NewIndex;
	if (Inventory && Inventory->GetInventorySlots().IsValidIndex(CurrentIndex) && Inventory->GetInventorySlots()[CurrentIndex].ItemClass)
	{
		ItemInfo = Inventory->GetInventorySlots()[CurrentIndex].ItemClass->GetItemInfo();
		Amount = Inventory->GetInventorySlots()[CurrentIndex].Amount;

		if (ItemInfo.bCanBeUsed)
		{
			UseText->SetText(ItemInfo.UseText);
			UseButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UseButton->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (ItemInfo.Category == EItemCategory::Quest)
		{
			ThrowButton->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			ThrowButton->SetVisibility(ESlateVisibility::Visible);
		}

		if (ItemInfo.bCanBeStacked && Amount > 1)
		{
			ThrowButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ThrowButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UWAInventoryActionMenuWidget::SetInventory(AWAInventory* NewInventory)
{
	Inventory = NewInventory;
}

void UWAInventoryActionMenuWidget::OnClickedUseButton()
{
	if (Inventory && Inventory->GetInventorySlots().IsValidIndex(CurrentIndex))
	{
		Inventory->UseItemAtIndex(CurrentIndex);
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWAInventoryActionMenuWidget::OnClickedThrowButton()
{
	if (Inventory && Inventory->GetInventorySlots().IsValidIndex(CurrentIndex))
	{
		Inventory->RemoveItemAtIndex(CurrentIndex, 1);
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWAInventoryActionMenuWidget::OnClickedSplitButton()
{
	if (Inventory && Inventory->GetInventorySlots().IsValidIndex(CurrentIndex))
	{
		Inventory->SplitStack(CurrentIndex, Amount / 2);
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWAInventoryActionMenuWidget::OnClickedCancelButton()
{
	if (Inventory)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}
