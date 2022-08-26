// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionBar_WC.h"
#include "Inventory_W.h"
#include "Blueprint/UserWidget.h"
#include "Components/BackgroundBlur.h"
//#include "Option_OfVerticalList_WC.h"
#include "InventorySystem/Widget/HUDCrafting_WC.h"
#include "HUD_WC.generated.h"

// TODO: add the Hide classes to hide unnecessary classes from the blueprint
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UHUD_WC : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta=(BindWidget))
    UBackgroundBlur* BackgroundBlur;

    UPROPERTY()
    AMyProjectCharacter* MyProjectCharacter;

protected:
    virtual void NativeOnInitialized() override;

    virtual void SynchronizeProperties() override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                              UDragDropOperation* InOperation) override;

public:
    UFUNCTION(BlueprintImplementableEvent, Category="HUD")
    UInventory_W* GetInventoryWidget();

    UFUNCTION(BlueprintImplementableEvent, Category="HUD")
    UActionBar_WC* GetActionBarWidget();

    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    UHUDCrafting_WC* GetHUDCraftingWidget();

    UPROPERTY()
    UInventory_W* InventoryWidget;

    UPROPERTY()
    UActionBar_WC* ActionBarWidget;

    UPROPERTY()
    UHUDCrafting_WC* HUDCraftingWidget;

    UFUNCTION(BlueprintCallable)
    void ShowOptionOfFindButton();

    UFUNCTION(BlueprintCallable)
    void InitHUD();




   /* UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UHUDCrafting_WC> HUDCraftingClass = nullptr;

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* CircleCanvasPanel;*/
};
