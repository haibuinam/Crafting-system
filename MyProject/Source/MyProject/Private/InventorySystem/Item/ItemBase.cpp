// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Item/ItemBase.h"

#include "MyProject/MyProjectCharacter.h"
#include "MyProject/LogUtilities.h"

AItemBase::AItemBase()
{
    // setup default root component
    SceneRootComponent = CreateDefaultSubobject<USceneComponent>("Default Scene Root");
    RootComponent = SceneRootComponent;

    // setting graphic data
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
    Mesh->SetupAttachment(RootComponent);
}

void AItemBase::BeginPlay()
{
    Super::BeginPlay();

    // getting the common data
    World = this->GetWorld();
    if (World)
    {
        MyPlayerController = Cast<AMyPlayerController>(World->GetFirstPlayerController());
    }
    else
    {
        V_LOG("World is NULL!");
    }

    if (MyPlayerController)
    {
        MyProjectCharacter = Cast<AMyProjectCharacter>(MyPlayerController->GetCharacter());
        MyPlayerState = MyPlayerController->GetPlayerState<AMyPlayerState>();
    }
    else
    {
        V_LOG("PlayerController is NULL!");
    }
}

void AItemBase::OnInteract()
{
    // this just to warn that the default parent behavior is called, child is not implemented!
    V_LOG("Parent Interact is  being called!");
}

void AItemBase::OnUse()
{
    V_LOG("Parent Interact is called!");
}
