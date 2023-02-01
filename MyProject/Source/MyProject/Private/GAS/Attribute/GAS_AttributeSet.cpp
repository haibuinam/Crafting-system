// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/GAS_AttributeSet.h"
#include <Net/UnrealNetwork.h>

UGAS_AttributeSet::UGAS_AttributeSet()
{

}

void UGAS_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Health, OldHealth);
}

//void UGAS_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
//{
//	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxHealth, OldMaxHealth);
//}

//void UGAS_AttributeSet::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate)
//{
//	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, HealthRegenRate, OldHealthRegenRate);
//}

void UGAS_AttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, AttackPower, OldAttackPower )
}

void UGAS_AttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Stamina, OldStamina );
}

//void UGAS_AttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
//{
//	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Armor, OldArmor);
//}

//void UGAS_AttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel)
//{
//	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Level, OldLevel);
//}

//void UGAS_AttributeSet::OnRep_XP(const FGameplayAttributeData& OldXP)
//{
//	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, XP, OldXP);
//}

void UGAS_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
}

void UGAS_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, AttackPower, COND_None, REPNOTIFY_Always);

}
