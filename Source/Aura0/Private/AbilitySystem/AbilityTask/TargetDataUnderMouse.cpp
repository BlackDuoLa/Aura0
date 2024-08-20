// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"



UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	////������ʾ��굱ǰָ��λ��Ŀ�����ݵ�UTargetDataUnderMouseʵ��
	//�����ر���
	UTargetDataUnderMouse* MyObj = NewAbilityTask< UTargetDataUnderMouse>(OwningAbility);
	return MyObj;

}

void UTargetDataUnderMouse::Activate()
{

	//�ж�����Ƿ�����
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();

	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);

		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}
	//��ȡ�������λ�ò��㲥

//���ͻ������ɵ�����ͬ���������(�������������)
void UTargetDataUnderMouse::SendMouseCursorData()
{

	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	//FHitResult��һ���ṹ�壬���ڴ洢��ײ���Ľ����������ײ�����ĵ㣨λ�ã�����ײ�����壨������Actor��Component�ȣ�����ײ�ķ��߷������Ϣ��
	FHitResult CursorHit;
	//GetHitResultUnderCursor����������ڻ�ȡ�ڹ�굱ǰλ���µ���ײ���
	//ECC_Visibilityͨ�����ڼ��ɼ�����ײ��false�����ǵڶ���������ͨ������ָ���Ƿ����ĳЩ���͵����壬false��ʾ�������κ�����
	//CursorHit�����ǵ�������������������ᱻ���Ϊ��굱ǰλ���µ���ײ���
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	//DataHandle�������ͨ�������˹��ڱ����Ŀ�����Ϣ
	FGameplayAbilityTargetDataHandle DataHandle;

	//FGameplayAbilityTargetData_SingleTargetHitʵ�����ڴ洢����Ŀ����еĽ������������ӵ�Data��
	//�Ա���Gameplay Abilities��Gameplay Effects֮�䴫��
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();

	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	//�ͻ������ɵĻ���ͬ����������
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), 
		DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}

}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{

	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());


}
