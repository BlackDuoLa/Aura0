// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{

	//OnGameplayEffectAppliedDelegateToSelf这是ASC自带的广播函数
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);

	



}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{

	//遍历 StartupAbilities 容器中的每一个元素，并将每个元素赋值给 AbilityClass 变量
	for (TSubclassOf<UGameplayAbility>AbilityClass:StartupAbilities)
	{
		//创建等级为1的能力
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		//GiveAbility(AbilitySpec);
		
		//授予角色该能力并立即激活一次
		GiveAbilityAndActivateOnce(AbilitySpec);

	}



}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//FGameplayTagContainer 是一个容器类，用于存储和管理一组 FGameplayTag。
	// FGameplayTag 是一种用于游戏逻辑中标记和分类游戏元素（如角色、物品、事件等）的轻量级标识符。
	
	//创建TagContainer函数
	FGameplayTagContainer TagContainer;
	//从EffectSpec对象中检索所有与之关联的资产标签，并将这些标签存储在TagContainer指定的容器中。
	EffectSpec.GetAllAssetTags(TagContainer);

	//然后将获取到的标签，绑定在EffectAssetTags动态多播委托中
	EffectAssetTags.Broadcast(TagContainer);



}
