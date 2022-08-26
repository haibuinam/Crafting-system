// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "ButtonDescription_WC.h"
#include "OptionOfFindButton_WC.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedButtonOfFindButton, const int, IdButtonOfFindButton);

UCLASS()
class MYPROJECT_API UOptionOfFindButton_WC : public UUserWidget
{
	GENERATED_BODY()

public:
    /*----- DATA -----*/

    UPROPERTY()
    class AMyProjectCharacter* MyProjectCharacter = nullptr;

    UPROPERTY()
    class UCraftingComponent* CraftingComponent;

    UPROPERTY(BlueprintAssignable)
    FOnClickedButtonOfFindButton OnClickedButtonOfFindButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TSubclassOf<UButtonDescription_WC> DescriptionSlotClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Number", DisplayName = "Number Of Button")
    int32 NumberOfButton = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option", DisplayName = "Option Slot Size")
    FVector2D OptionSlotSize {-1,- 1};

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
    UFUNCTION(BlueprintCallable, Category = "OptionsOfFindButton|DescriptionUI")
    void InitOptionOfFindButtonUI();

    UFUNCTION(BlueprintCallable)
    void CalculateWidgetDimension();

    UFUNCTION(BlueprintCallable)
    void OnClickedButton(const int IdButton);

};
