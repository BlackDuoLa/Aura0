#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"




UMMC_MaxHealth::UMMC_MaxHealth()
{
	//获取的Vigor属性对象
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	//设置VigorDef的应用为Target
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	//添加到捕获VigorDef数值，只有添加到列表
	RelevantAttributesToCapture.Add(VigorDef);


}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//获取SourceTags和TargetTags
	//SourceTags自身|TargetTags敌方
	//可以通过Tag的挂载，影响属性数值的计算
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	//生成Vigor体力
	float Vigor = 0.f;
	//从Spec中获取到VigorDef的数值，使用EvaluationParameters获取到对象的TargetTag，最后把相应的数据传入到Vigor中
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	//设置Vigor的数值，不能小于等于0
	Vigor = FMath::Max<float>(Vigor, 0.f);

	
	//Spec.GetContext().GetSourceObject()获取触发或应用当前效果的对象
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	//通过获取到的数值，计算最大血量
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;

}
