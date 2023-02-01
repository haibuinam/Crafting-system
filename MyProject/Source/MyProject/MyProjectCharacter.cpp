// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"

#include "LogUtilities.h"
#include "MyPlayerController.h"
#include "MyPlayerState.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Crafting System/CraftingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InventorySystem/Inventory/InventoryComponent.h"
#include "InventorySystem/Item/ItemBase.h"
#include <AbilitySystemComponent.h>

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

    // Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
    // instead of recompiling to adjust them
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
    Inventory->Capacity = 20.f;

    Crafting = CreateDefaultSubobject<UCraftingComponent>("Crafting");
    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
    // are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


    // Initialize GAS related components
    AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));

    //Enable replication
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
    Attributes = CreateDefaultSubobject<UGAS_AttributeSet>(TEXT("Attributes"));

}

void AMyProjectCharacter::UseItem(AItemBase* Item)
{
    if (Item)
    {
        Item->OnUse();
    }
    else
    {
        V_LOG("Item is Null");
    }
}

UAbilitySystemComponent* AMyProjectCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Set up game-play key bindings
    check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Toggle Inventory", IE_Pressed, this, &AMyProjectCharacter::ToggleInventory);

    PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMyProjectCharacter::MoveForward);
    PlayerInputComponent->BindAxis("Move Right / Left", this, &AMyProjectCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turn-rate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AMyProjectCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AMyProjectCharacter::LookUpAtRate);

    // handle touch devices
    PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyProjectCharacter::TouchStarted);
    PlayerInputComponent->BindTouch(IE_Released, this, &AMyProjectCharacter::TouchStopped);
}

void AMyProjectCharacter::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = GetWorld()->GetFirstPlayerController();

    // to be able to access the inventory anytime from the player state
    MyPlayerState = Cast<AMyPlayerState>(this->GetPlayerState());

    // init HUD master Widget
    HUD_Widget = CreateWidget<UHUD_WC>(PlayerController, HUD_WidgetClass, TEXT("HUD"));
    HUD_Widget->AddToViewport();

    //HUD_Crafting_Widget = HUD_Widget->HUDCraftingWidget;
    // CloseInventory();
}

void AMyProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
    Jump();
}

void AMyProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
    StopJumping();
}

void AMyProjectCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AMyProjectCharacter::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void AMyProjectCharacter::ToggleInventory()
{
    if (!PlayerController)
    {
        V_LOG("PlayerController is null!");
        return;
    }

    if (!HUD_Widget->InventoryWidget)
    {
        V_LOG("HUD_Widget->InventoryWidget is null!");
        return;
    }

    if (!bIsInventoryOpen)
    {
        OpenInventory();
    }
    else
    {
        CloseInventory();
    }
}

void AMyProjectCharacter::OpenInventory() const
{
    // set the Visibility of the Inventory widget through the up-most parent of the UI hierarchy
    HUD_Widget->InventoryWidget->Border->SetVisibility(ESlateVisibility::Visible);

    //temporarily put it here
    HUD_Widget->HUDCraftingWidget->Border->SetVisibility(ESlateVisibility::Visible);
    if(!Inventory->IsEmpty())
    {
        Crafting->UpdateCraftLists();
        HUD_Widget->HUDCraftingWidget->ItemVerticalListWIthAva->Update();
    }

    // set the visibility of background blur when HUD is activated
    HUD_Widget->BackgroundBlur->SetVisibility(ESlateVisibility::Visible);

    // set Input mode to only UI keyboard mapping
    PlayerController->SetInputMode(FInputModeUIOnly());
    HUD_Widget->SetFocus();


    // set the cursor visible
    PlayerController->SetShowMouseCursor(true);
    bIsInventoryOpen = true;
}

void AMyProjectCharacter::CloseInventory()
{
    // set the Visibility of the Inventory widget through the up-most parent of the UI hierarchy
    HUD_Widget->InventoryWidget->Border->SetVisibility(ESlateVisibility::Hidden);

    //temporarily put it here
    HUD_Widget->HUDCraftingWidget->Border->SetVisibility(ESlateVisibility::Hidden);
    //HUD_Widget->CraftingCircle_With8Item_CraftingWidget->Border->SetVisibility(ESlateVisibility::Hidden);
    //HUD_Widget->OptionOfVerticalListWidget->Border->SetVisibility(ESlateVisibility::Hidden);

    // set the visibility of background blur when HUD is activated
    HUD_Widget->BackgroundBlur->SetVisibility(ESlateVisibility::Hidden);

    // hide the mouse cursor when toggle inventory off
    PlayerController->bShowMouseCursor = false;

    // close UI thus have to set input back to game mode
    PlayerController->SetInputMode(FInputModeGameOnly());
    bIsInventoryOpen = false;
}
