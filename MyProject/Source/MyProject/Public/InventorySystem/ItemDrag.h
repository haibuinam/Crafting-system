// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Item/ItemBase.h"
#include "ItemDrag.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UItemDrag : public UDragDropOperation
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    int32 SlotIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    UTexture2D* ItemImage;

    bool bIsFromInventoryMenu = true; // can this be replaced when we have a separate class for Item and Action already?

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    AItemBase* DraggedItem;
};
