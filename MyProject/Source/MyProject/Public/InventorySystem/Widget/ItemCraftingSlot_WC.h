// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"

#include "DraggedItem_W.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "ItemCraftingSlot_WC.generated.h"

class UItemDrag;
class AItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCraftingAddWidget, UItemDrag*, WidgetFromInventory, const int32&, IndexTo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClickedItemCraftingButton, AItemBase*, Item, const FVector2D, LocationWidget);
/**
 * 
 */
UCLASS()
class MYPROJECT_API UItemCraftingSlot_WC : public UUserWidget
{
	GENERATED_BODY()

public: 
    /*----- DATA -----*/
    UPROPERTY()
    class AMyProjectCharacter* MyProjectCharacter = nullptr;

    // UNIQUE 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crafting|Item", meta=(AllowPrivateAccess=true))
    class AItemBase* Item = nullptr;

    UPROPERTY()
    int32 IndexInCrafting = INDEX_NONE;

    UPROPERTY(BlueprintAssignable)
    FOnCraftingAddWidget OnCraftingAddWidget;

    UPROPERTY(BlueprintAssignable)
    FOnClickedItemCraftingButton OnClickedItemCraftingButton;

    /*----- UI ELEMENTS -----*/
    UPROPERTY(meta=(BindWidget))
    UButton* Button = nullptr;

    UPROPERTY(meta=(BindWidget))
    UImage* ItemIcon = nullptr;

    UPROPERTY(meta=(BindWidget))
    USizeBox* SizeBox = nullptr ;

    UPROPERTY(meta=(BindWidget))
    UTextBlock* ItemAmount = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Location")
    FVector2D LocationVector2DWidget = {-1, -1}; // for other to reference size from

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Dimension")
    FVector2D Dimension = {-1, -1}; // for other to reference size from

 /*-- STYLE --*/
    // override/update the button style of Blueprint for each state of the icon (in slot, being dragged, default)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
    FButtonStyle EnabledButtonStyle; // custom style for active action slots/slots that's occupied? 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
    FButtonStyle DragOverStyle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
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
    UFUNCTION()
    void SendNameItemWhenPlayerClicked();
    
    UFUNCTION()
    void GetDefaultIcon();
};
