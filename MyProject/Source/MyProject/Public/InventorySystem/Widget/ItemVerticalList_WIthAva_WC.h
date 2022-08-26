// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/GridPanel.h"
#include "Components/ScrollBox.h"
#include "ItemDetail_WithIcon_WC.h"
#include "LikeButton_WC.h"
#include "InventorySystem/Widget/ItemDetail_WithIcon_WC.h"
#include "ItemVerticalList_WIthAva_WC.generated.h"

/**
 * 
 */
class AItemBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedNameOfItemButton, AItemBase*, Item);

UCLASS()
class MYPROJECT_API UItemVerticalList_WIthAva_WC : public UUserWidget
{
	GENERATED_BODY()

public:
    /*----- DATA -----*/
    UPROPERTY()
    class AMyProjectCharacter* MyProjectCharacter;

    UPROPERTY()
    class UCraftingComponent* CraftingComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemDetail_WithIcon_WBP")
    TSubclassOf<UItemDetail_WithIcon_WC> ITemDetailWithIconClass;

    UPROPERTY(BlueprintAssignable)
    FOnClickedNameOfItemButton OnClickedNameOfItemButton;

	/*----------- UI ELEMENTS -----------*/
	UPROPERTY(meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBoxOf_ItemVerticalList;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox;

    UPROPERTY()
    TMap<AItemBase*, int> CountNumberOfClickedItem;

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;

public:
    UFUNCTION()
    void Update();

    UFUNCTION()
    void ShowItemCanCraft();

    UFUNCTION()
    void ShowLikedItem();

    UFUNCTION()
    void ShowFoundItems();

    UFUNCTION()
    void AddItemToList(AItemBase* ClickedItem);

    UFUNCTION()
    void OnClickedNameOfItem(AItemBase* ItemData);

    UFUNCTION()
    void ShowItemAToZ();

    UFUNCTION()
    void ShowItemPriceHighToLow();

    UFUNCTION()
    void ShowItemPriceLowToHigh();

    UFUNCTION()
    void ShowItemLatest();

    UFUNCTION()
    void ShowItemRarityLowToHigh();

    UFUNCTION()
    void ShowItemRarityHighToLow();
};
