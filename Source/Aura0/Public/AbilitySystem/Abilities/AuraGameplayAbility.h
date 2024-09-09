// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA0_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	

public:


	//创建设置玩家的输入Tag
	UPROPERTY(EditDefaultsOnly,Category = "Input")
	FGameplayTag StartupInputTag;

	//创建火球的曲线伤害
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	FScalableFloat Damage;





};
