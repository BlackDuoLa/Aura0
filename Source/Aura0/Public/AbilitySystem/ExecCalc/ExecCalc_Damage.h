// Execution Calculations是Unreal Engine中Gameplay Effects系统的一部分，用于在Gameplay Effect执行期间进行自定义的计算和逻辑操作。它允许开发者根据特定的游戏需求，灵活地处理和修改游戏中的属性（Attributes）。

//功能强大且灵活：与ModifierMagnitudeCalculation类似，Execution Calculations可以捕获Attribute，并可选择性地为其创建Snapshot。不过，与MMC不同的是，Execution Calculations可以修改多个Attribute，基本上可以处理程序员想要做的任何事。
//不可预测性：这种强大和灵活性的代价是它是不可预测的（Modifiers是可以预测的），且必须在C++中实现。这是因为Execution Calculations提供了很高的自由度，允许开发者根据特定的游戏逻辑进行自定义计算。
//使用场景：Execution Calculations最普遍的应用场景是计算一个来自很多源（Source）和目标（Target）中Attribute伤害值的复杂公式。例如，在计算伤害时，可以考虑攻击者的攻击力、防御者的防御力、各种增益和减益效果等多个因素。
//限制：需要注意的是，Execution Calculations只能由即刻（Instant）和周期性（Periodic）Gameplay Effect使用。功能强大也带来了劣势就是它不支持预测（Prediction）。属性获取时不会运行PreAttributeChange函数，所以，我们还需要在其内部实现一遍属性限制。只能够在服务器运行并且在Net Execution Policies设置为 Local Predicted，Server Initiated和Server Only时。
//它和GameplayModMagnitudeCalculation很像，但是可以一次性修改多个属性。它不是只返回一个值，而是直接在函数内部对属性进行了修改。
//由于Execution Calculations的复杂性，建议进行充分的测试和调试，以确保计算逻辑的正确性。




#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class AURA0_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()



public:
	UExecCalc_Damage();


	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExeccutionOutput)const override;






	
};
