// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "CraftingWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UCraftingWidget : public UUserWidget
{
    GENERATED_BODY()


public:
    UPROPERTY(meta=(BindWidget))
    UButton* AcceptButton;

    // should this image be an array of images so that we can change the amount of materials to put in just by changing the size?
    // a circle will have a graph equation: (x-xo)^2 + (y-yo)^2 = R^2, with (xo,yo) being the center coords
    // from this equation we can actually calculate any point coordinates around the circle 
    // UPROPERTY(EditAnywhere, BlueprintCallable, Category="", meta=(AllowPrivateAccess=true))
    // UImage* Image;
};
