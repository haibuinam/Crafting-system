// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "CraftingButton_WC.h"
#include "Option_OfVerticalList_WC.generated.h"

/**
 * 
 */
class UHUDCrafting_WC;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedButtonOfVerticalList, const int, IdButtonOfOptionOfVerticlList);

UCLASS()
class MYPROJECT_API UOption_OfVerticalList_WC : public UUserWidget
{
	GENERATED_BODY()
public:
    /*----- DATA -----*/
    UPROPERTY()
    class  AMyProjectCharacter* MyProjectCharacter;

    UPROPERTY()
    class  AMyPlayerController* MyPlayerController;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crafting")
    TSubclassOf<UCraftingButton_WC> OptionOfVerticalListSlotClass;

    UPROPERTY()
    TArray<UHorizontalBoxSlot*> OptionVerticalListSlot_Widgets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Number", DisplayName = "Number Of Button")
    int32 NumberOfButton = 3;

    UPROPERTY(BlueprintAssignable)
    FOnClickedButtonOfVerticalList OnClickedButtonOfVerticalList;

    UPROPERTY()
    class UCraftingComponent* CraftingComponent;

	/*----------- UI ELEMENTS -----------*/
	UPROPERTY(meta = (BindWidget))
	UBorder* Border;

    UPROPERTY(meta = (BindWidget))
    USizeBox* SizeBox;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* HorizontalBoxButtons;

	UPROPERTY(meta = (BindWidget))
	UEditableText* EditableTextFindItemByName;

    UPROPERTY(meta = (BindWidget))
    USizeBox* EditableTextSize;

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
    void InitOption_OfVerticalList_Widget();

    UFUNCTION(BlueprintCallable)
    void CalculateWidgetDimension();

    UFUNCTION(BlueprintCallable)
    void OnClickedButton(const int IdOptionVerticalListButton);

    UFUNCTION(BlueprintCallable)
    bool ReportAfterSearching(FString NameItem, FString TextInput, int NumberOfCharactersAllowedToBeWrong);

    UFUNCTION(BlueprintCallable)
    void OnTextChangedWhenPlayerClicked(const FText& InText);

    UFUNCTION(BlueprintCallable)
    void OnTextCommitWhenPlayerClicked(const FText& InText, ETextCommit::Type InCommitType);

};
