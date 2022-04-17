// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WAItem.h"
#include "WAInventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWAInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	void UpdateSlot();

	UFUNCTION()
	FText AmountTextBinding();

	UFUNCTION()
	ESlateVisibility AmountTextVisibilityBinding();

	AWAInventory* GetInventory();
	int32 GetSlotIndex();
	void SetSlotIndex(int32 Index);
	void SetOwningInventory(AWAInventory* Inventory);


private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* SlotButton;

	UPROPERTY(Meta = (BindWidget))
	class UImage* ItemIcon;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* AmountText;

	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	int32 SlotIndex;

	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	class AWAInventory* OwningInventory;

	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FItemInfo ItemInfo;

	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	int32 Amount;
};
