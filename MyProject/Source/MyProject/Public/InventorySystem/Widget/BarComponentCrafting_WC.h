// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "CraftingButton_WC.h"
#include "Components/HorizontalBox.h"
//#include "ItemCraftingSlot_WC.h"
#include "BarComponentCrafting_WC.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedBarComponentButton, const int, IdBarComponentButton);

UCLASS()
class MYPROJECT_API UBarComponentCrafting_WC : public UUserWidget
{
	GENERATED_BODY()
public: 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UCraftingButton_WC> BarSlotClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Number", DisplayName = "Number Of Button")
    int32 NumberOfButton = 6;

    UPROPERTY(BlueprintAssignable)
    FOnClickedBarComponentButton OnClickedBarComponentButton;

	/*----------- UI ELEMENTS -----------*/
	UPROPERTY(meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalBox;

    UPROPERTY(meta = (BindWidget))
    USizeBox* SizeBox;

  /*-----------PRE-CALCULATE HINTS-----------*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonSize", DisplayName = "Button Size")
    FVector2D ButtonSize {-1, -1};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Dimension")
    FVector2D Dimension {-1, -1}; // for other to reference size from

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;

public:
    UFUNCTION(BlueprintCallable)
    void InitBarUI();

    UFUNCTION(BlueprintCallable)
    void CalculateWidgetDimension();

    UFUNCTION(BlueprintCallable)
    void OnClickedButton(const int IdButton);
};
