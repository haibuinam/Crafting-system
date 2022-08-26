// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerState.h"
#include "GameFramework/Character.h"
#include "InventorySystem/Common/Constants.h"
#include "InventorySystem/Widget/HUD_WC.h"
#include "InventorySystem/Widget/Inventory_W.h"

#include "InventorySystem/Widget/HUDCrafting_WC.h"

#include "InventorySystem/Widget/Circle_With8Item_Crafting_WC.h"
#include "MyProjectCharacter.generated.h"


UCLASS(config=Game)
class AMyProjectCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    class USpringArmComponent* CameraBoom = nullptr;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    class UCameraComponent* FollowCamera = nullptr;
#pragma endregion

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
    float TurnRateGamepad = 50.f;

    // This one might have to change the position
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Inventory")
    class UInventoryComponent* Inventory = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    TSubclassOf<UInventory_W> InventoryWidget_BPClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AMyPlayerState* MyPlayerState = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    mutable bool bIsInventoryOpen = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float Health = CHARACTER_HEALTH_DEFAULT;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float Damage = CHARACTER_DAMAGE_DEFAULT;

    // This one might have to change the position
    UPROPERTY()
    UInventory_W* InventoryWidget = nullptr;

    UPROPERTY()
    APlayerController* PlayerController = nullptr;

    // only accept this class or its child classes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
    TSubclassOf<UHUD_WC> HUD_WidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
    UHUD_WC* HUD_Widget = nullptr;

    //test
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
    //UHUDCrafting_WC* HUD_Crafting_Widget = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Crafting")
    class UCraftingComponent* Crafting = nullptr;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crafting")
    //UCircle_With8Item_Crafting_WC* Crafting_Widget = nullptr;


protected:
    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    /** 
     * Called via input to turn at a given rate. 
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void TurnAtRate(float Rate);

    /**
     * Called via input to turn look up/down at a given rate. 
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void LookUpAtRate(float Rate);

    /** Handler for when a touch input begins. */
    void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

    /** Handler for when a touch input stops. */
    void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // End of APawn interface

    virtual void BeginPlay() override;

public:
    AMyProjectCharacter();

    /** Handle Inventory */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    void ToggleInventory();

    UFUNCTION(BlueprintCallable, Category="Inventory")
    void OpenInventory() const;

    UFUNCTION(BlueprintCallable, Category="Inventory")
    void CloseInventory();

    /** Handle Item */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    void UseItem(class AItemBase* Item);
};
