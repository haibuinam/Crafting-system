// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GAS_AttributeSet.generated.h"

//macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class MYPROJECT_API UGAS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSetBase_Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Health)

//	UPROPERTY(BlueprintReadOnly, Category = "AttributeSetBase_Health")
//	FGameplayAttributeData MaxHealth;
//	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, MaxHealth)

//	UPROPERTY(BlueprintReadOnly, Category = "AttributeSetBase_Health")
//	FGameplayAttributeData HealthRegenRate;
//	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, HealthRegenRate)

	UPROPERTY(BlueprintReadOnly, Category = "AttributeSetBase_AttackPower")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "AttributeSetBase_Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Stamina)

	//UPROPERTY(BlueprintReadOnly, Category = "AttributeSetBase_Armor")
	//FGameplayAttributeData Armor;
	//ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Armor)

	//UPROPERTY(BlueprintReadOnly, Category = "AttributeSetBase_Level")
	//FGameplayAttributeData Level;
	//ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Level)

	//UPROPERTY(BlueprintReadOnly, Category = "AttributeSetBase_XP")
	//FGameplayAttributeData XP;
	//ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, XP)

public:
	UGAS_AttributeSet();

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	//UFUNCTION()
	//virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	//UFUNCTION()
	//virtual void OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate);

	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	//UFUNCTION()
	//virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	//UFUNCTION()
	//virtual void OnRep_Level(const FGameplayAttributeData& OldLevel);

	//UFUNCTION()
	//virtual void OnRep_XP(const FGameplayAttributeData& OldXP);

public:
	//to clamp new value (stl::clamp c++ 20): Clamp(Health, 0, MaxHealth)
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
