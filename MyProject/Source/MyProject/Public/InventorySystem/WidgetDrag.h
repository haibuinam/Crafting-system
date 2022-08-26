// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Inventory/InventoryComponent.h"
#include "WidgetDrag.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWidgetDrag : public UDragDropOperation
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    UUserWidget* WidgetToDrag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    FVector2D DropWindowOffset; // prevent dragged item to pop to different location
};
