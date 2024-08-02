// // 创建MMC实现对最大血量和最大蓝量基于等级升级

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"




UCLASS()
class AURA0_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:

	UMMC_MaxMana();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec)const override;


private:

	//获取AS内的智慧值
	FGameplayEffectAttributeCaptureDefinition IntDef;

};
