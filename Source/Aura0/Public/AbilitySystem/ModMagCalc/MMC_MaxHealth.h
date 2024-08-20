// ����MMCʵ�ֶ����Ѫ��������������ڵȼ�����
//MMC ȫ�� Mod Magnitude Calculation��Ϊ�Զ��������ֵ���࣬����ʹ����ͼ�̳У�Ҳ����ʹ��c++�̳У��������Ŀ��������ʹ�õ�c++ʵ�֡�
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

	//��ȡAS�ڵĻ���ֵ
	FGameplayEffectAttributeCaptureDefinition VigorDef;



};
