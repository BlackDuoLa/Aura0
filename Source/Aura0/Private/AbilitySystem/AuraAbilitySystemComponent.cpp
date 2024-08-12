// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//OnGameplayEffectAppliedDelegateToSelf����ASC�Դ��Ĺ㲥����
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);

}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{

	//���� StartupAbilities �����е�ÿһ��Ԫ�أ�����ÿ��Ԫ�ظ�ֵ�� AbilityClass ����
	for (TSubclassOf<UGameplayAbility>AbilityClass : StartupAbilities)
	{
		//�����ȼ�Ϊ1�ļ���
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);

		}

	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	//���°�������ⰴ���󶨵�Tag��������Tag���󶨵ļ���
	//�ж��Ƿ�����Ч�ģ�����Ч�ľ�ͨ������Ч�ľͷ���
	if (!InputTag.IsValid())return;
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//�жϴ�������Tag�Ƿ��Ѿ�����
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{

			AbilitySpecInputPressed(AbilitySpec);
			//�жϰ����Ƿ񼤻�
			if (!AbilitySpec.IsActive())

			{
				//��������
				TryActivateAbility(AbilitySpec.Handle);

			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{

	if (!InputTag.IsValid())return;
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//�жϴ�������Tag�Ƿ��Ѿ�����
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{

			AbilitySpecInputReleased(AbilitySpec);

		}
	}


}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
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
