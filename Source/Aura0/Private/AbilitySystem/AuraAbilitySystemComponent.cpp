// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{

	//OnGameplayEffectAppliedDelegateToSelf����ASC�Դ��Ĺ㲥����
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);

	



}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{

	//���� StartupAbilities �����е�ÿһ��Ԫ�أ�����ÿ��Ԫ�ظ�ֵ�� AbilityClass ����
	for (TSubclassOf<UGameplayAbility>AbilityClass:StartupAbilities)
	{
		//�����ȼ�Ϊ1������
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		//GiveAbility(AbilitySpec);
		
		//�����ɫ����������������һ��
		GiveAbilityAndActivateOnce(AbilitySpec);

	}



}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//FGameplayTagContainer ��һ�������࣬���ڴ洢�͹���һ�� FGameplayTag��
	// FGameplayTag ��һ��������Ϸ�߼��б�Ǻͷ�����ϷԪ�أ����ɫ����Ʒ���¼��ȣ�����������ʶ����
	
	//����TagContainer����
	FGameplayTagContainer TagContainer;
	//��EffectSpec�����м���������֮�������ʲ���ǩ��������Щ��ǩ�洢��TagContainerָ���������С�
	EffectSpec.GetAllAssetTags(TagContainer);

	//Ȼ�󽫻�ȡ���ı�ǩ������EffectAssetTags��̬�ಥί����
	EffectAssetTags.Broadcast(TagContainer);



}
