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

	//能力系统组件
	//在基类初始化的属性同步模式(血量、蓝量、体力、声音、特效、技能冷却、等等)
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;
	//属性集
	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;


	virtual void InitAbilityActorInfo();


	//玩家属性添加方法之一
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category= "Attributes")
	TSubclassOf<UGameplayEffect>DefaultPrimaryAttributes;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect>DefaultSecondaryAttributes;

	//在角色身上添加GE系统可配置角色属性面板
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect>GameplayEffectClass, float Level) const;
	void InitializeDefaultAttributes()const;




};

