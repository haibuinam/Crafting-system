// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "InventorySystem/Common/Constants.h"
#include "LikeButton_WC.generated.h"

/**
 * 
 */
class AItemBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedLikeButton, AItemBase*, Item);

UCLASS()
class MYPROJECT_API ULikeButton_WC : public UUserWidget
{
	GENERATED_BODY()

public:
	/*----------- UI ELEMENTS -----------*/
    //UPROPERTY(meta = (BindWidget))
    //UBorder* Border;

    UPROPERTY(meta = (BindWidget))
    USizeBox* SizeBox;

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	UPROPERTY(meta = (BindWidget))
	UImage* Image;

    /*----- DATA -----*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item", meta=(AllowPrivateAccess=true))
    class AItemBase* Item = nullptr;

    UPROPERTY(BlueprintAssignable)
    FOnClickedLikeButton OnClickedLikeButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LikeButton|Metadata")
    FName NameButton = BUTTON_NAME_DEFAULT;

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;

public:
    UFUNCTION(BlueprintCallable, Category = "CraftingButton")
    void ClickedItem();

    UFUNCTION(BlueprintCallable, Category = "CraftingButton")
    void GetIcon();
};
