
#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem\AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "interaction/CombatInterface.h"

//这里结构体不加F是因为它是内部结构体，不需要外部获取，也不需要在蓝图中使用
struct AuraDamageStatics
{
	//通过宏获得对应的属性
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);//护甲
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);//护甲穿透率
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);//格挡
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);//暴击
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);//暴击抗性
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);//暴击伤害




	AuraDamageStatics()
	{
		//这是一个获取属性宏
		//获取攻击敌人的护甲
		//参数：1.属性集 2.属性名 3.目标还是自身 4.是否设置快照（true为创建时获取，false为应用时获取）
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

	//获取源和目标的能力系统(获取自己和收到攻击者的能力系统）
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	//获取AvatarActor
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	//获取到战斗接口
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);


	//获取挂载此类的GE实例
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	//获取源和目标的标签(获取自己和收到攻击者的标签）
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	//获取伤害值
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	





	//获取目标的格挡值，并控制格挡值不能小于0
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	//通过随机数判断是否格挡值
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance ;
	Damage = bBlocked ? Damage / 2.f : Damage;


	//获取目标的护甲值，并控制护甲值不能小于0
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);



	//获取源护甲穿透，并控制护甲穿透不能小于0
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);


	UCharacterClassInfo* CharactertClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	//获取到数据表内的护甲穿透系数，设置护甲穿透等级受角色等级影响
	const FRealCurve* ArmorPenetrationCurve = CharactertClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPentrationCoeffecient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	//护甲穿透将按照比例忽略目标的护甲值，护甲穿透的值可以根据某个方程计算出实际护甲穿透率（可以根据等级，等级越高，每一点护甲穿透值的比例越低）
	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * ArmorPentrationCoeffecient) / 100.f;

	//获取到数据表内的护甲系数，并设置护甲等级和角色等级挂钩
	const FRealCurve* EffectiveArmorCurve = CharactertClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmorr"), FString());
	const float EffectiveArmorCoeffecient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	//忽略后的护甲值将以一定比例影响伤害
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoeffecient) / 100.f;


	//获取源暴击率，并控制暴击率不能小于0
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);


	//获取目标暴击抗性，并控制暴击抗性不能小于0
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);


	//获取源暴击伤害，并控制暴击抗性不能小于0
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	//获取到数据表内的暴击抗性系数，设置暴击抗性等级受角色等级影响
	const FRealCurve* CriticalHitResistanceCurve = CharactertClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanCeffecient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());


	//源的暴击率 - 目标的暴击抗性 = 最终暴击率，通过随机数判断是否暴击
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanCeffecient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;


	//输出计算结果
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExeccutionOutput.AddOutputModifier(EvaluatedData);

}
