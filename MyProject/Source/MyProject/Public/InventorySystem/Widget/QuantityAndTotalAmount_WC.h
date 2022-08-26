// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "InventorySystem/Widget/CraftingButton_WC.h"
#include "InventorySystem/Widget/ButtonDescription_WC.h"
#include "QuantityAndTotalAmount_WC.generated.h"

/**
 * 
 */
class AItemBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClickedForgingButton, AItemBase*, Item, 
                                                                                                const int32, NumberOfItemPlayerWantToMake );

UCLASS()
class MYPROJECT_API UQuantityAndTotalAmount_WC : public UUserWidget
{
	GENERATED_BODY()
	
public:
    /*----- DATA -----*/
    UPROPERTY()
    class AMyProjectCharacter* MyProjectCharacter;

    UPROPERTY()
    class UCraftingComponent* CraftingComponent;

    UPROPERTY(BlueprintAssignable)
    FOnClickedForgingButton OnClickedForgingButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuantityAndTotalAmount")
    TSubclassOf<UCraftingButton_WC> CraftingButtonSlotClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item", meta=(AllowPrivateAccess=true))
    class AItemBase* SelectedItem = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Number", DisplayName = "Number Of Total Amount Button")
    int32 TotalAmountButtonNumber = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Number", DisplayName = "Number Of Quantity Button")
    int32 QuantityButtonNumber = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Dimension")
    FVector2D Dimension = {-1, -1}; // for other to reference size from

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Total Amount Dimension")
    FVector2D TotalAmountDimension = {-1, -1}; // for other to reference size from

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Quantity Dimension")
    FVector2D QuantityDimension = {-1, -1}; // for other to reference size from

    /*----- UI ELEMENTS -----*/
    UPROPERTY(meta=(BindWidget))
    UBorder* Border = nullptr;

    UPROPERTY(meta=(BindWidget))
    UBorder* BorderOfTotalAmount = nullptr;

    UPROPERTY(meta=(BindWidget))
    UBorder* BorderOfEditableTextBox = nullptr;

    UPROPERTY(meta=(BindWidget))
    USizeBox* SizeBox = nullptr;

    UPROPERTY(meta=(BindWidget))
    UCanvasPanel* CanvasPanel = nullptr;

    UPROPERTY(meta=(BindWidget))
    UHorizontalBox* TotalAmountHorizontalBox = nullptr;

    UPROPERTY(meta=(BindWidget))
    UHorizontalBox* EditQuantityHorizontalBox = nullptr;

    UPROPERTY(meta=(BindWidget))
    USizeBox* SizeOfTotalAmount = nullptr;

    UPROPERTY(meta=(BindWidget))
    USizeBox* SizeEditableOfTextBox = nullptr;

    UPROPERTY(meta=(BindWidget))
    USizeBox* SizeEditQuantityHorizontalBox = nullptr;

    UPROPERTY(meta=(BindWidget))
    UEditableText* EditNumberOfItem;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* GoldText = nullptr;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* RubyText = nullptr;

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;

public:
    UFUNCTION()
    void InitQuantityAndTotalAmount();

    UFUNCTION()
    void InitTotalAmount();

    UFUNCTION()
    void InitQuantity();

    UFUNCTION(BlueprintCallable)
    void CalculateWidgetDimension();

    UFUNCTION(BlueprintCallable)
    void GetSelectedItem(AItemBase* Item);

    UFUNCTION(BlueprintCallable)
    void ClickEditQuantityHorizontalBox(const int IdButtonEditQuantity);

    UFUNCTION(BlueprintCallable)
    void ClickTotalAmountHorizontalBox(const int IdButtonTotalAmount);

    UFUNCTION(BlueprintCallable)
    int GetNumberFromEditableText(FText Input);
};
