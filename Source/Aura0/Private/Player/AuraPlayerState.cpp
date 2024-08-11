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

	DOREPLIFETIME(AAuraPlayerState, Level);

}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//玩家等级同步
void AAuraPlayerState::OnRep_Level(int32 Oldlevel)
{


}
