// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DraggedItem_W.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "InventorySystem/Common/Structs.h"
#include "InventorySystem/Inventory/InventoryComponent.h"
#include "InventorySlot_W.generated.h"

class UTextBlock;
class UImage;
class UButton;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventorySwap, const int32&, IndexFrom, const int32&, IndexTo);

/**
 * 
 */
UCLASS()
class MYPROJECT_API UInventorySlot_W : public UUserWidget
{
    GENERATED_BODY()

public:
    /*----- DATA -----*/
    // PERSISTENT
    UPROPERTY()
    class AMyProjectCharacter* MyProjectCharacter = nullptr;

    // UNIQUE 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item", meta=(AllowPrivateAccess=true))
    class AItemBase* Item = nullptr;

    UPROPERTY()
    // this is the index in `both` Inventory (Data array) and InventorySlot_Widgets (Widget array)
    int32 IndexInInventory_1D = INDEX_NONE;

    UPROPERTY(BlueprintAssignable)
    FOnInventorySwap OnInventorySwap;

    /*----- UI ELEMENTS -----*/
    UPROPERTY(meta=(BindWidget))
    UButton* Button = nullptr;

    UPROPERTY(meta=(BindWidget))
    UImage* ItemIcon = nullptr;

    UPROPERTY(meta=(BindWidget))
    USizeBox* SizeBox = nullptr;

    UPROPERTY(meta=(BindWidget))
    UTextBlock* ItemAmount = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Dimension")
    FVector2D Dimension = {-1, -1}; // for other to reference size from

    /*-- STYLE --*/
    // override/update the button style of Blueprint for each state of the icon (in slot, being dragged, default)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Style")
    FButtonStyle EnabledButtonStyle; // custom style for active action slots/slots that's occupied? 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Style")
    FButtonStyle DragOverStyle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Style")
    FButtonStyle DefaultButtonStyle; // a custom for empty slots (default style for a slot)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UDraggedItem_W> DraggedItemWidgetClass;

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;

    /*----- DRAG & DROP -----*/
    virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                   UDragDropOperation* InOperation) override;

    virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                      UDragDropOperation*& OutOperation) override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                              UDragDropOperation* InOperation) override;

    /*----- MOUSE -----*/
    virtual FReply
    NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
    UFUNCTION(BlueprintCallable, Category="Inventory|C++ Endpoint")
    void UseItem();
};
