// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DraggedItem_W.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "ActionSlot_WC.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UActionSlot_WC : public UUserWidget
{
    GENERATED_BODY()

public:
    /*-- DATA --*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
    int32 ActionSlotIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UDraggedItem_W> DraggedItemWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Dimension")
    FVector2D Dimension; // for other to reference size from

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
    UTexture2D* ActionImage = nullptr;

    /*-- STYLE --*/
    // override/update the button style of Blueprint for each state of the icon (in slot, being dragged, default)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Style")
    FButtonStyle EnabledButtonStyle; // custom style for active action slots/slots that's occupied? 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Style")
    FButtonStyle DragOverStyle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Style")
    FButtonStyle DefaultButtonStyle; // a custom for empty slots (default style for a slot)

    /*-- UI ELEMENTS --*/
    UPROPERTY(meta=(BindWidget))
    USizeBox* SizeBox;

    UPROPERTY(meta=(BindWidget))
    UImage* ActionImg = nullptr;

    UPROPERTY(meta=(BindWidget))
    UButton* Button = nullptr;

protected:
    virtual void NativeOnInitialized() override;

    virtual void NativeConstruct() override;

    virtual void SynchronizeProperties() override;

    virtual FSlateBrush GetBrush();

    /*----- DRAG & DROP -----*/
    virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                   UDragDropOperation* InOperation) override;

    virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                      UDragDropOperation*& OutOperation) override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                              UDragDropOperation* InOperation) override;

    /*----- MOUSE -----*/
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
    UFUNCTION(BlueprintCallable)
    void SetDefaultButton();
};
