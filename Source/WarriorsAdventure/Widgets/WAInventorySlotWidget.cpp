// Fill out your copyright notice in the Description page of Project Settings.


#include "WAInventorySlotWidget.h"
#include "WAInventory.h"
#include "WAInventoryWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UWAInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AmountText->SynchronizeProperties();
	AmountText->TextDelegate.BindUFunction(this, "AmountTextBinding");
	AmountText->VisibilityDelegate.BindUFunction(this, "AmountTextVisibilityBinding");
}

void UWAInventorySlotWidget::UpdateSlot()
{
	if (OwningInventory->IsSlotEmpty(SlotIndex))
	{
		SlotButton->SetIsEnabled(false);
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SlotButton->SetIsEnabled(true);
		ItemInfo = OwningInventory->GetItemInfoAtIndex(SlotIndex);
		Amount = OwningInventory->GetItemAmountAtIndex(SlotIndex);
		ItemIcon->SetBrushFromTexture(ItemInfo.Icon);
		ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

FText UWAInventorySlotWidget::AmountTextBinding()
{
	FText DisplayText = FText::FromString(FString(TEXT("x") + FString::FromInt(Amount)));
	return DisplayText;
}

ESlateVisibility UWAInventorySlotWidget::AmountTextVisibilityBinding()
{
	if (OwningInventory->IsSlotEmpty(SlotIndex))
	{
		return ESlateVisibility::Hidden;
	}
	else
	{
		if (ItemInfo.bCanBeStacked)
		{
			return ESlateVisibility::HitTestInvisible;
		}
		else
		{
			return ESlateVisibility::Hidden;
		}
	}
}

AWAInventory* UWAInventorySlotWidget::GetInventory()
{
	return OwningInventory;
}

int32 UWAInventorySlotWidget::GetSlotIndex()
{
	return SlotIndex;
}

void UWAInventorySlotWidget::SetSlotIndex(int32 Index)
{
	SlotIndex = Index;
}

void UWAInventorySlotWidget::SetOwningInventory(AWAInventory* Inventory)
{
	OwningInventory = Inventory;
}

FReply UWAInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (SlotButton->GetIsEnabled())
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			OwningInventory->GetInventoryWidget()->OnSlotClicked(this);
		}
	}
	else
	{
		return FReply::Handled();
	}

	return FReply::Unhandled();
}
