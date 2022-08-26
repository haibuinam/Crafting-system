// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlot_W.h"
#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "Components/UniformGridPanel.h"
//#include "InventorySystem/Common/Structs.h"
#include "Inventory_W.generated.h"

class UHUD_WC;

/**
 * 
 */
UCLASS()
class MYPROJECT_API UInventory_W : public UUserWidget
{
    GENERATED_BODY()

public:
    /*----- DATA -----*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
    class UInventoryComponent* Inventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", DisplayName="Inventory Slot Size")
    FVector2D InventorySlotSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|InventoryBox",
        DisplayName="Number Of Inventory Slot Rows")
    int32 InventoryBoxMaxRow = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|InventoryBox",
        DisplayName="Number Of Inventory Slot Columns")
    int32 InventoryBoxMaxCol = 8;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory|Dimension",
        DisplayName="Number Of Inventory Slots")
    int64 NumOfSlot = 0;

    UPROPERTY(VisibleDefaultsOnly)
    FVector2D Offset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    TSubclassOf<UInventorySlot_W> InventorySlotClass;

    UPROPERTY()
    TArray<UUniformGridSlot*> InventorySlot_Widgets;

    UPROPERTY()
    AMyProjectCharacter* MyProjectCharacter;

    UPROPERTY()
    AMyPlayerController* MyPlayerController;

    UPROPERTY()
    UHUD_WC* HUDReference;

    /*----- UI ELEMENTS -----*/
    UPROPERTY(meta=(BindWidget))
    USizeBox* SizeBox = nullptr;

    UPROPERTY(meta=(BindWidget))
    UBorder* Border = nullptr;

    UPROPERTY(meta=(BindWidget))
    UButton* CloseButton = nullptr;

    UPROPERTY(meta=(BindWidget)) //pointer of UniformGridPanel
    UUniformGridPanel* InventoryBox = nullptr;

    /*-----------PRE-CALCULATED HINTS-----------*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Dimension")
    FVector2D Dimension = {-1, -1}; // for other to reference size from

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SizeBox|Styling", DisplayName="Extra Space")
    FOverrideExtraSpace SizeBoxOverrideExtraSpace = {-1, -1};

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;

    /*------------------MOUSE------------------*/
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    /*------------------DRAG & DROP------------------*/
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                      UDragDropOperation*& OutOperation) override;

public:
    UFUNCTION(BlueprintCallable, Category="Inventory|UI")
    void InitInventoryUI();

    UFUNCTION(BlueprintCallable, Category="Inventory|UI")
    void UpdateInventoryUI();

    UFUNCTION(BlueprintCallable, Category="Inventory|UI")
    void UpdateSlotUI(AItemBase* ItemToUpdate, int32 ItemIndex);

    UFUNCTION(BlueprintCallable, Category="Inventory|UI")
    void SwapSlotWidgets(const int32& IndexFrom, const int32& IndexTo);

    UFUNCTION(BlueprintCallable)
    void CalculateWidgetDimension();

    /*UFUNCTION(BlueprintCallable)
    UInventorySlot_W GetInventorySlot_W(const int32 &Index);*/
};
