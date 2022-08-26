// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanel.h"

#include "ItemDescription_WC.generated.h"

/**
 * 
 */

class AItemBase;

UCLASS()
class MYPROJECT_API UItemDescription_WC : public UUserWidget
{
	GENERATED_BODY()
public:
	/*----------- UI ELEMENTS -----------*/
	UPROPERTY(meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameOfItem;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*	Description;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalBox_Damage;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonItemUses;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DetailItem;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_MoreBuff;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemRarity;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemStyle;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemUses;

    UFUNCTION()
    void Update(AItemBase* Item);
};
