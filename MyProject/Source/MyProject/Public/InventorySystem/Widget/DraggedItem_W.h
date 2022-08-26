// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "DraggedItem_W.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UDraggedItem_W : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    UImage* DraggedItem = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
    UTexture2D* DraggedImage = nullptr;

    // UPROPERTY(meta=(BindWidget))
    // UImage* InventoryItemIcon;

protected:
    virtual void NativeOnInitialized() override;

    virtual void NativeConstruct() override;

public:
    UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
    UImage* GetImage_DraggedItem() const;
};
