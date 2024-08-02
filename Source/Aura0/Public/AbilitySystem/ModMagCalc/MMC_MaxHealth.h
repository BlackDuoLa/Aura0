// 创建MMC实现对最大血量和最大蓝量基于等级升级

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
