// 创建MMC实现对最大血量和最大蓝量基于等级升级
//MMC 全称 Mod Magnitude Calculation，为自定义计算数值的类，可以使用蓝图继承，也可以使用c++继承，在这个项目里面我们使用的c++实现。
#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"


UCLASS()
class AURA0_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()



public:
	UMMC_MaxHealth();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec)const override;


private:

	//获取AS内的活力值
	FGameplayEffectAttributeCaptureDefinition VigorDef;



};
