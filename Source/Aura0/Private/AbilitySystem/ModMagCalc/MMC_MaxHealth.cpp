#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"




UMMC_MaxHealth::UMMC_MaxHealth()
{
	//��ȡ��Vigor���Զ���
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	//����VigorDef��Ӧ��ΪTarget
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	//��ӵ�����VigorDef��ֵ��ֻ����ӵ��б�
	RelevantAttributesToCapture.Add(VigorDef);


}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//��ȡSourceTags��TargetTags
	//SourceTags����|TargetTags�з�
	//����ͨ��Tag�Ĺ��أ�Ӱ��������ֵ�ļ���
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	//����Vigor����
	float Vigor = 0.f;
	//��Spec�л�ȡ��VigorDef����ֵ��ʹ��EvaluationParameters��ȡ�������TargetTag��������Ӧ�����ݴ��뵽Vigor��
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	//����Vigor����ֵ������С�ڵ���0
	Vigor = FMath::Max<float>(Vigor, 0.f);

	
	//Spec.GetContext().GetSourceObject()��ȡ������Ӧ�õ�ǰЧ���Ķ���
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	//ͨ����ȡ������ֵ���������Ѫ��
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;

}
