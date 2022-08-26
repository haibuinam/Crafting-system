// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "LikeButton_WC.h"
//#include "CraftingButton_WC.h"
#include "ItemDetail_WithIcon_WC.generated.h"

/**
 * 
 */
class AItemBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSendClickedItem, AItemBase*, Item);

UCLASS()
class MYPROJECT_API UItemDetail_WithIcon_WC : public UUserWidget
{
	GENERATED_BODY()
	
public:
    /*----- DATA -----*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item", meta=(AllowPrivateAccess=true))
    class AItemBase* Item = nullptr;

    UPROPERTY(BlueprintAssignable)
    FSendClickedItem SendClickedItem;

	/*----------- UI ELEMENTS -----------*/
	UPROPERTY(meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(meta = (BindWidget))
	USizeBox* ItemDetailWithIconSizeBox;

	UPROPERTY(meta = (BindWidget))
	USizeBox* ItemNameSizeBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemTextName;

    UPROPERTY(meta = (BindWidget))
    USizeBox* NotificationSizeText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* NotificationText;

    UFUNCTION(BlueprintImplementableEvent, Category="LikeButton")
    ULikeButton_WC* GetLikeButton_WC();

    UPROPERTY()
    ULikeButton_WC* LikeButton;

    UPROPERTY(meta = (BindWidget))
    UButton* NameOfItemButton;

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;

public:
    UFUNCTION(BlueprintCallable)
    void SendItem();
};
