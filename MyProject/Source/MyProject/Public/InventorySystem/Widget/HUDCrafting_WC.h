// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Circle_With8Item_Crafting_WC.h"
#include "BarComponentCrafting_WC.h"
#include "ItemDescription_WC.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Option_OfVerticalList_WC.h"
#include "ItemVerticalList_WIthAva_WC.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "OptionOfFindButton_WC.h"
#include "QuantityAndTotalAmount_WC.h"
#include "OptionOfSortedModeButton_WC.h"

//#include "Crafting System/CraftingComponent.h"
#include "HUDCrafting_WC.generated.h"

/**
 * 
 */

class AItemBase;

UCLASS()
class MYPROJECT_API UHUDCrafting_WC : public UUserWidget
{
	GENERATED_BODY()

public:
    /*----------- DATA-----------*/
    UPROPERTY()
    class AMyProjectCharacter* MyProjectCharacter;

    UPROPERTY()
    class AMyPlayerController* MyPlayerController;

    UPROPERTY()
    TArray<UUserWidget*>ArrayUserWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    TSubclassOf<UItemVerticalList_WIthAva_WC> ItemDetailClass;

    UPROPERTY()
    class UCraftingComponent* CraftingComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsShowOptionModeButton = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsShowOptionOfSortedModeButton = false;

	/*----------- UI ELEMENTS -----------*/
	UPROPERTY(meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CraftingCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas_ListItemCraft;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalBox_NameOfScreenCrafting;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_CraftingItemDisplay;
	
	UPROPERTY(meta = (BindWidget))
	UImage* ImageCraftingItemDisplay;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_CraftingItemDisplay;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalBox_InforCharacter;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_PowerCharacter;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_Level;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_Exp;
	
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Image_ButtonExit;

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation) override;

public:
    UFUNCTION(BlueprintImplementableEvent, Category="HUDCrafting")
    UCircle_With8Item_Crafting_WC* GetCraftingCircleWidget();

    UFUNCTION(BlueprintImplementableEvent, Category="HUDCrafting")
    UOption_OfVerticalList_WC* GetOptionOfVerticalListWidget();

    UFUNCTION(BlueprintImplementableEvent, Category="HUDCrafting")
    UBarComponentCrafting_WC* GetBarComponentCraftingWidget();

    UFUNCTION(BlueprintImplementableEvent, Category="HUDCrafting")
    UItemVerticalList_WIthAva_WC* GetUItemVerticalListWIthAvaWidget();

    UFUNCTION(BlueprintImplementableEvent, Category="HUDCrafting")
    UItemDescription_WC* GetUItemDescriptionWidget();

    UFUNCTION(BlueprintImplementableEvent, Category="HUDCrafting")
    UOptionOfFindButton_WC* GetOptionOfFindButtonWidget();

    UFUNCTION(BlueprintImplementableEvent, Category="HUDCrafting")
    UOptionOfSortedModeButton_WC* GetOptionOfSortedModeButtonWidget();

    UFUNCTION(BlueprintImplementableEvent, Category="HUDCrafting")
    UQuantityAndTotalAmount_WC* GetQuantityAndTotalAmountWidget();

    UPROPERTY()
    UCircle_With8Item_Crafting_WC* CraftingCircle_With8Item_CraftingWidget;

    UPROPERTY()
    UOption_OfVerticalList_WC* OptionOfVerticalListWidget;

    UPROPERTY()
    UBarComponentCrafting_WC* BarComponentCrafting;

    UPROPERTY()
    UItemVerticalList_WIthAva_WC* ItemVerticalListWIthAva;

    UPROPERTY()
    UItemDescription_WC* ItemDescriptionCrafting;

    UPROPERTY()
    UOptionOfFindButton_WC* OptionOfFindButton;

    UPROPERTY()
    UOptionOfSortedModeButton_WC* OptionOfSortedModeButton;;

    UPROPERTY()
    UQuantityAndTotalAmount_WC* QuantityAndTotalAmountWidget;;

public:
    UFUNCTION(BlueprintCallable)
    void InitHUDCraftingWidget();

    UFUNCTION(BlueprintCallable)
    void CloseHUDCrafting();

    UFUNCTION(BlueprintCallable)
    void OnClickedButtonsOfOptionOfVerticalListWidget(const int IdButtonOfOptionVerticalList);

    UFUNCTION(BlueprintCallable)
    void OnClickedButtonOfFindButtonWC(const int IdButtonOfFindButtonWC);

    //when player clicked name of Item (ItemVerticalListWithAva)
    UFUNCTION(BlueprintCallable)
    void OnClickedNameOfItemVerticalListWithAva(AItemBase* ItemClicked);

    UFUNCTION(BlueprintCallable)
    void OnClickedButtonOfOptionOfSortedModeButton(const int IdButtonOfOptionSortedMode);

    UFUNCTION(BlueprintCallable)
    void OnClickedForgingButton(AItemBase* SelectedItem, const int NumberItemPlayerWantToMake);

    UFUNCTION(BlueprintCallable)
    void OnClickedBarComponentButton(const int IdBarButtonWidget);
};
