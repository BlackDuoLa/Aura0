// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"



UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	////创建表示鼠标当前指向位置目标数据的UTargetDataUnderMouse实例
	//并返回本身
	UTargetDataUnderMouse* MyObj = NewAbilityTask< UTargetDataUnderMouse>(OwningAbility);
	return MyObj;

}

void UTargetDataUnderMouse::Activate()
{

	//判断玩家是否别控制
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
	//获取鼠标点击的位置并广播

//将客户端生成的数据同步到服务端(发送鼠标光标数据)
void UTargetDataUnderMouse::SendMouseCursorData()
{

	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	//FHitResult是一个结构体，用于存储碰撞检测的结果，包括碰撞发生的点（位置）、碰撞的物体（可以是Actor、Component等）、碰撞的法线方向等信息。
	FHitResult CursorHit;
	//GetHitResultUnderCursor这个方法用于获取在光标当前位置下的碰撞结果
	//ECC_Visibility通常用于检查可见性碰撞，false：这是第二个参数，通常用于指定是否忽略某些类型的物体，false表示不忽略任何物体
	//CursorHit：这是第三个参数，这个参数会被填充为光标当前位置下的碰撞结果
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	//DataHandle这个参数通常包含了关于被点击目标的信息
	FGameplayAbilityTargetDataHandle DataHandle;

	//FGameplayAbilityTargetData_SingleTargetHit实例用于存储单个目标击中的结果，并将其添加到Data中
	//以便在Gameplay Abilities和Gameplay Effects之间传递
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();

	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	//客户端生成的火球，同步到服务器
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
