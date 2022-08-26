// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "ButtonDescription_WC.h"
#include "OptionOfSortedModeButton_WC.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedButtonOfSortedMode, const int, IdButtonOfSortedMode);

UCLASS()
class MYPROJECT_API UOptionOfSortedModeButton_WC : public UUserWidget
{
	GENERATED_BODY()

public:
    /*----- DATA -----*/

    UPROPERTY()
    class AMyProjectCharacter* MyProjectCharacter = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TSubclassOf<UButtonDescription_WC> DescriptionSlotClass;

    UPROPERTY(BlueprintAssignable)
    FOnClickedButtonOfSortedMode OnClickedButtonOfSortedMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Number", DisplayName = "Number Of Button")
    int32 NumberOfButton = 6;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option", DisplayName = "Option Slot Size")
    FVector2D OptionSlotSize {-1, -1};

    /*----------- UI ELEMENTS -----------*/
    UPROPERTY(meta = (BindWidget))
    UBorder* Border = nullptr;

    UPROPERTY(meta = (BindWidget))
    USizeBox* SizeBox = nullptr;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* ListOptions = nullptr;

    /*-----------PRE-CALCULATE HINTS-----------*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Dimension")
    FVector2D Dimension = { -1, -1 }; // for other to reference size from

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Options Sorted Mode Button|DescriptionUI")
    void InitOptionOfFindButtonUI();

    UFUNCTION(BlueprintCallable)
    void CalculateWidgetDimension();

    UFUNCTION(BlueprintCallable)
    void OnClickedButton(const int IdButton);
};
