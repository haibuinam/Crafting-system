// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventorySystem/Common/Constants.h"
#include "ButtonDescription_WC.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedButtonDescription, const int, IdButtonDescription);

UCLASS()
class MYPROJECT_API UButtonDescription_WC : public UUserWidget
{
	GENERATED_BODY()
public:
    /*----- DATA -----*/
    UPROPERTY()
    class AMyProjectCharacter* MyProjectCharacter = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crafting|Metadata")
    FName ButtonName = BUTTON_NAME_DEFAULT;

    UPROPERTY(BlueprintAssignable)
    FOnClickedButtonDescription OnClickedButtonDescription;

    UPROPERTY()
    int Id = INDEX_NONE;;

    /*----------- UI ELEMENTS -----------*/
    UPROPERTY(meta = (BindWidget))
    USizeBox* SizeBox = nullptr;

    UPROPERTY(meta = (BindWidget))
    UButton* Button = nullptr;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TextBlock = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Dimension")
    FVector2D Dimension = { -1, -1 }; // for other to reference size from

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;
public:

    UFUNCTION(BlueprintCallable, Category="CraftingButton")
    void IdButtonOnClicked();

};
