// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class AURA0_API AAuraPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	public:
	AAuraPlayerState();

	//获取生命周期内需要复制的属性
	// 覆盖GetLifetimeReplicatedProps来确保Level属性被正确复制  
	//当 GetLifetimeReplicatedProps 被调用时，UE4的反射系统会自动识别所有被标记为 Replicated 的属性，
	// 并将它们添加到 OutLifetimeProps 数组中
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	//将角色等级传入GetPlayerLevel中
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }

	protected:
	//获取能力系统组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	//获取玩家属性
	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;


private:

	//添加玩家等级
	//Level属性已经被UPROPERTY宏中的Replicated标志标记为需要复制
	//指定OnRep_Level该函数将在属性被网络复制时调用
	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_Level)
	int32 Level = 1;


	//玩家等级网络同步
	//OnRep_Level 函数将被调用，并传入旧的值作为参数。
	UFUNCTION()
	void OnRep_Level(int32 Oldlevel);




};
