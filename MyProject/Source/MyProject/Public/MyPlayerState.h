// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InventorySystem/Inventory/InventoryComponent.h"
// #include "MyProject/MyProjectCharacter.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Info")
    // AMyProjectCharacter* MyCharacter;

    // we leave the InventoryComponent (inventory storage information) here instead of:
    // 1. Character: because if we attach the component to the character -> when the char dies, the inventory is GONE (not good)
    // 2. PlayerController: because it's not replicated over the net
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    UInventoryComponent* Inventory;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
    float Health = 100.f;

protected:
    AMyPlayerState();

    virtual void BeginPlay() override;
};
