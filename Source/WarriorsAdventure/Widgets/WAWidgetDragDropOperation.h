// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "WAWidgetDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWAWidgetDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	void SetWidgetToDrag(class UUserWidget* _WidgetToDrag);
	void SetMouseOffset(FVector2D _MouseOffset);
	class UUserWidget* GetWidgetToDrag();
	FVector2D GetMouseOffset();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	class UUserWidget* WidgetToDrag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FVector2D MouseOffset;
};
