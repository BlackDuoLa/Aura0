
#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem\AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "interaction/CombatInterface.h"

//����ṹ�岻��F����Ϊ�����ڲ��ṹ�壬����Ҫ�ⲿ��ȡ��Ҳ����Ҫ����ͼ��ʹ��
struct AuraDamageStatics
{
	//ͨ�����ö�Ӧ������
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);//����
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);//���״�͸��
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);//��
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);//����
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);//��������
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);//�����˺�




	AuraDamageStatics()
	{
		//����һ����ȡ���Ժ�
		//��ȡ�������˵Ļ���
		//������1.���Լ� 2.������ 3.Ŀ�껹������ 4.�Ƿ����ÿ��գ�trueΪ����ʱ��ȡ��falseΪӦ��ʱ��ȡ��
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);




	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;

}




UExecCalc_Damage::UExecCalc_Damage()
{

	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);


}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	FGameplayEffectCustomExecutionOutput& OutExeccutionOutput) const
{

	//��ȡԴ��Ŀ�������ϵͳ(��ȡ�Լ����յ������ߵ�����ϵͳ��
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	//��ȡAvatarActor
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	//��ȡ��ս���ӿ�
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);


	//��ȡ���ش����GEʵ��
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	//��ȡԴ��Ŀ��ı�ǩ(��ȡ�Լ����յ������ߵı�ǩ��
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	//��ȡ�˺�ֵ
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	





	//��ȡĿ��ĸ�ֵ�������Ƹ�ֵ����С��0
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	//ͨ��������ж��Ƿ��ֵ
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance ;
	Damage = bBlocked ? Damage / 2.f : Damage;


	//��ȡĿ��Ļ���ֵ�������ƻ���ֵ����С��0
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);



	//��ȡԴ���״�͸�������ƻ��״�͸����С��0
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);


	UCharacterClassInfo* CharactertClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	//��ȡ�����ݱ��ڵĻ��״�͸ϵ�������û��״�͸�ȼ��ܽ�ɫ�ȼ�Ӱ��
	const FRealCurve* ArmorPenetrationCurve = CharactertClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPentrationCoeffecient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	//���״�͸�����ձ�������Ŀ��Ļ���ֵ�����״�͸��ֵ���Ը���ĳ�����̼����ʵ�ʻ��״�͸�ʣ����Ը��ݵȼ����ȼ�Խ�ߣ�ÿһ�㻤�״�͸ֵ�ı���Խ�ͣ�
	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * ArmorPentrationCoeffecient) / 100.f;

	//��ȡ�����ݱ��ڵĻ���ϵ���������û��׵ȼ��ͽ�ɫ�ȼ��ҹ�
	const FRealCurve* EffectiveArmorCurve = CharactertClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmorr"), FString());
	const float EffectiveArmorCoeffecient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	//���Ժ�Ļ���ֵ����һ������Ӱ���˺�
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoeffecient) / 100.f;


	//��ȡԴ�����ʣ������Ʊ����ʲ���С��0
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);


	//��ȡĿ�걩�����ԣ������Ʊ������Բ���С��0
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);


	//��ȡԴ�����˺��������Ʊ������Բ���С��0
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	//��ȡ�����ݱ��ڵı�������ϵ�������ñ������Եȼ��ܽ�ɫ�ȼ�Ӱ��
	const FRealCurve* CriticalHitResistanceCurve = CharactertClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanCeffecient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());


	//Դ�ı����� - Ŀ��ı������� = ���ձ����ʣ�ͨ��������ж��Ƿ񱩻�
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanCeffecient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;


	//���������
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExeccutionOutput.AddOutputModifier(EvaluatedData);

}
