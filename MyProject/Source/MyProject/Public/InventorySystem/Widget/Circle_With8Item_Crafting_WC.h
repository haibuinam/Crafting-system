// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "MyPlayerController.h"
#include "Components/CanvasPanel.h"
#include "CraftingButton_WC.h"
#include "ItemCraftingSlot_WC.h"
#include "InventorySystem/Inventory/InventoryComponent.h"
#include "InventorySystem/Widget/ItemDescription_WC.h"
#include "Circle_With8Item_Crafting_WC.generated.h"

/**
 * 
 */
class UHUD_WC;
class AItemBase;

UCLASS()
class MYPROJECT_API UCircle_With8Item_Crafting_WC : public UUserWidget
{
    GENERATED_BODY()

public:
    /*----------- DATA-----------*/
    UPROPERTY()
    AMyProjectCharacter* MyProjectCharacter;

    UPROPERTY()
    AMyPlayerController* MyPlayerController;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UItemCraftingSlot_WC> CraftingSlotClass = nullptr;

    UPROPERTY()
    class UCraftingComponent* CraftingComponent;

    UPROPERTY()
    class UInventoryComponent* Inventory;

    UPROPERTY()
    UItemDescription_WC* ItemDescriptionWidget = nullptr;

    UFUNCTION(BlueprintImplementableEvent, Category = "Circle")
    UItemDescription_WC* GetItemDescriptionWidget();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsShowItemDescription = false;

    UPROPERTY()
    UTexture2D* DefaultIcon2D;

    UPROPERTY(VisibleDefaultsOnly)
    FVector2D Offset;

    UPROPERTY()
    UCanvasPanelSlot* FinishedItem;

    UPROPERTY()
    TArray<UCanvasPanelSlot*> CraftingSlot_Widgets;

    UPROPERTY()
    UHUD_WC* HUDReference;

    /*----------- UI ELEMENTS -----------*/
    UPROPERTY(meta = (BindWidget))
    UBorder* Border = nullptr;

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* CircleCanvasPanel;

    UPROPERTY(meta = (BindWidget))
    USizeBox* SizeBox;

    UPROPERTY(meta = (BindWidget))
    UButton* CenterCraftingSlot;

    UPROPERTY(meta = (BindWidget))
    UImage* ImageCenterCraftingSlot;

    /*-----------PRE-CALCULATE HINTS-----------*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Dimension")
    FVector2D Dimension; // for other to reference size from

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SizeBox|Styling", DisplayName = "Crafting Slot Size")
    FVector2D CraftingSlotSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location", DisplayName = "Location Of Crafting Slot")
    FVector2D Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R", DisplayName = "Radius")
    int32 R = 200;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NumberOfItems", DisplayName = "Number of items")
    int32 NumberOfItems = 8;

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;

    /*------------------MOUSE------------------*/
    //virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    /*------------------DRAG & DROP------------------*/
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
        UDragDropOperation*& OutOperation) override;

public:
    UFUNCTION(BlueprintCallable)
    bool InitCraftingWidget();

    UFUNCTION()
    void GetDefaultIcon();

    UFUNCTION(BlueprintCallable)
    void CalculateWidgetDimension();

    UFUNCTION(BlueprintCallable, Category="Crafting|UI")
    void AddSlotWidgetsFromInventory(UItemDrag* WidgetFromInventory, const int32& IndexTo);

    UFUNCTION(BlueprintCallable, Category="Crafting|UI")
    void Update( AItemBase* ItemUpdate);

    UFUNCTION(BlueprintCallable, Category="Crafting|UI")
    void UpdateComponent(const TSet<AItemBase*> &ItemElement);

    UFUNCTION()
    int GetAmountOfItemInInventory(const TArray<AItemBase*>& Items, AItemBase* Item);

    UFUNCTION()
    void UpdateSlot(UItemCraftingSlot_WC* &Item_W,  AItemBase* &ItemPop);

    UFUNCTION()
    void ShowItemDescription(AItemBase* ClickedItem, const FVector2D LocationWidget);
};
