// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventorySystem/Common/Constants.h"
#include "CraftingButton_WC.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedButton, const int, IdCraftingButton);

UCLASS()
class MYPROJECT_API UCraftingButton_WC : public UUserWidget
{
	GENERATED_BODY()

public:
    /*----- DATA -----*/
    UPROPERTY()
    class AMyProjectCharacter* MyProjectCharacter = nullptr;

    UPROPERTY()
    int32 Id = INDEX_NONE;

    UPROPERTY(BlueprintAssignable)
    FOnClickedButton OnClickedButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crafting|Metadata")
    FName ButtonName = BUTTON_NAME_DEFAULT;

	/*----------- UI ELEMENTS -----------*/
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text;

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	UPROPERTY(meta = (BindWidget))
	UImage* ButtonIcon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Dimension")
    FVector2D Dimension = { -1, -1 }; // for other to reference size from

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;
public:

    UFUNCTION(BlueprintCallable, Category="CraftingButton")
    void SendIdCraftingButton();

};
