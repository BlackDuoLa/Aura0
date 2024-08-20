
#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"


// 当某个事件（比如玩家点击了某个目标）发生时
// 所有绑定到这个委托的回调函数都会被调用，并且会接收到一个 FGameplayAbilityTargetDataHandle 类型的参数，
// 在角色释放技能时，可以通过FGameplayAbilityTargetDataHandle来传递技能的目标信息（如目标位置、目标对象等）
// DataHandle这个参数通常包含了关于被点击目标的信息
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDaraSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);



UCLASS()
class AURA0_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	


public:

	//创建表示鼠标当前指向位置目标数据的UTargetDataUnderMouse实例
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility",DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly="true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	//ValidData存放鼠标点击对象的数据
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDaraSignature ValidData; 


private:

	//激活函数
	virtual void Activate()override;

	//发送鼠标光标数据
	void SendMouseCursorData();


	//鼠标光标位置信息网络同步
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,FGameplayTag ActivationTag);
};
