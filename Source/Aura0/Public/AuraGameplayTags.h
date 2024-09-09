// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"




/**
* AuraGameplayTags
* 
 * 
 */
struct FAuraGameplayTags
{
public:

	static const FAuraGameplayTags& Get(){ return GameplayTags; }
	static void InitializeNativeGameplayTags();



	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;


	FGameplayTag Attribute_Secondary_Aromr;
	FGameplayTag Attribute_Secondary_AromrPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;


	//º¸≈Ã”≥…‰µƒ±Í«©
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	
	//…À∫¶±Í«©
	FGameplayTag Damage;



protected:

private:


	static FAuraGameplayTags GameplayTags;




};