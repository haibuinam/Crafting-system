// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSlot_WC.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/CheckBox.h"
#include "Components/SizeBox.h"
#include "Components/UniformGridPanel.h"
#include "InventorySystem/Common/Structs.h"
#include "ActionBar_WC.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UActionBar_WC : public UUserWidget
{
    GENERATED_BODY()

public:
    /*----------- DATA-----------*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsBarLocked = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UActionSlot_WC> ActionSlotClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UniformGridPanel", DisplayName="Number Of Action Rows")
    int32 UniformGridPanelMaxRow = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UniformGridPanel", DisplayName="Number Of Action Columns")
    int32 UniformGridPanelMaxCol = 8;

    UPROPERTY(VisibleDefaultsOnly)
    FVector2D Offset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SizeBox", DisplayName="Extra Space")
    FOverrideExtraSpace SizeBoxOverrideExtraSpace;

    /*----------- UI ELEMENTS -----------*/
    UPROPERTY(meta=(BindWidget))
    UCheckBox* CheckBox;

    UPROPERTY(meta=(BindWidget))
    UUniformGridPanel* UniformGridPanel;

    UPROPERTY(meta=(BindWidget))
    USizeBox* SizeBox;

    /*-----------PRE-CALCULATE HINTS-----------*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Dimension")
    FVector2D Dimension; // for other to reference size from

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SizeBox|Styling", DisplayName="Action Slot Size")
    FVector2D ActionSlotSize;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dimension")
    int64 NumOfActions = 0;

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;

    /*------------------MOUSE------------------*/
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    /*------------------DRAG & DROP------------------*/
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                      UDragDropOperation*& OutOperation) override;

public:
    UFUNCTION(BlueprintCallable)
    void LockBar();

    UFUNCTION(BlueprintCallable)
    bool RefreshActionBar();

    UFUNCTION(BlueprintCallable)
    void CalculateWidgetDimension();
};
