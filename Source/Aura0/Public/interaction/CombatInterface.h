// 这个接口为敌人专门使用的，玩家角色无法使用

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA0_API ICombatInterface
{
	GENERATED_BODY()

	
public:

	//虚拟函数，用于获取等级
	virtual int32 GetPlayerLevel();
	//获取鼠标点击的位置
	virtual FVector GetCombatSocketLocation();

	//设置角色攻击时旋转函数（该函数在角色中作为自定义函数使用）
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);
};
