// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "ClickableSlot_WC.generated.h"

// TODO: after creating `Draggable` interface, make this include that
/**
 * 
 */
UCLASS()
class MYPROJECT_API UClickableSlot_WC : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta=(BindWidget))
    UButton* Button;

    UPROPERTY(meta=(BindWidget))
    UImage* Image;
};
