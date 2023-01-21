#pragma once
#include "CoreMinimal.h"
#include "GAS_Structs.generated.h"

UENUM(BlueprintType)
enum class EGASAbilityInputID : uint8
{
  None,
  Confirm,
  Cancel,
  Attack
};