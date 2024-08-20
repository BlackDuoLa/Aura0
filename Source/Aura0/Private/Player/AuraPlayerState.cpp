#include "Player/AuraPlayerState.h"
#include "AbilitySystem\AuraAbilitySystemComponent.h"
#include "AbilitySystem\AuraAttributeSet.h"
#include <Net\UnrealNetwork.h>


AAuraPlayerState::AAuraPlayerState()
{
	//能力系统组件
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	//设置属性同步模式(血量、蓝量、体力、声音、特效、技能冷却、等等)
	//想把相关属性让谁能获取到，就设在谁身上
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	//属性集
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME是用于自动处理属性复制的一个宏，手动添加了属性到数组中
	//将 AAuraPlayerState 类的 Level 属性标记为需要在网络复制的生命周期属性
	//实现就是将Level属性注册到服务器，在服务器有改动时，会自动复制到本地
	DOREPLIFETIME(AAuraPlayerState, Level);

}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//玩家玩家等级被复制时会同时运行这个函数
void AAuraPlayerState::OnRep_Level(int32 Oldlevel)
{


}
