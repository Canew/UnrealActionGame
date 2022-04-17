// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WAItem.h"
#include "WAInventoryActionMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWAInventoryActionMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void Update(int32 NewIndex);

	void SetInventory(class AWAInventory* NewInventory);

private:
	UFUNCTION()
	void OnClickedUseButton();
	UFUNCTION()
	void OnClickedThrowButton();
	UFUNCTION()
	void OnClickedSplitButton();
	UFUNCTION()
	void OnClickedCancelButton();

private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* UseButton;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* UseText;

	UPROPERTY(Meta = (BindWidget))
	class UButton* ThrowButton;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* ThrowText;

	UPROPERTY(Meta = (BindWidget))
	class UButton* SplitButton;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* SplitText;

	UPROPERTY(Meta = (BindWidget))
	class UButton* CancelButton;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* CancelText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class AWAInventory* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 CurrentIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FItemInfo ItemInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 Amount;
};
