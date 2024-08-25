// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

UCLASS(Abstract)
class AURA0_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface ,public ICombatInterface
{
	GENERATED_BODY()

public:
	
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	
	virtual void BeginPlay() override;

	
	//智能指针
	UPROPERTY(EditAnywhere,Category = "Combat")
	TObjectPtr<USkeletalMeshComponent>Weapon;

	//武器插槽
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSockeName;

	//武器接口
	virtual FVector GetCombatSocketLocation()override;


	//能力系统组件
	//在基类初始化的属性同步模式(血量、蓝量、体力、声音、特效、技能冷却、等等)
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;
	//属性集
	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;


	virtual void InitAbilityActorInfo();


	//玩家默认主属性
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category= "Attributes")
	TSubclassOf<UGameplayEffect>DefaultPrimaryAttributes;

	//玩家默认次属性
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect>DefaultSecondaryAttributes;

	//玩家默认属性
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect>DefaultVitalAttributes;



	//在角色身上添加GE系统可配置角色属性面板
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect>GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes()const;


	//添加能力系统函数
	void AddCharacterAbilities();


private:

	//玩家角色的能力系统
	UPROPERTY(EditAnywhere,Category="Ability")
	TArray < TSubclassOf<UGameplayAbility>>StartupAbilities;





};

