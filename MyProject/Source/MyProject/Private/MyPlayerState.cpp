// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

AMyPlayerState::AMyPlayerState()
{
    Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
    Inventory->Capacity = 20.f;
}

void AMyPlayerState::BeginPlay()
{
    Super::BeginPlay();
}
