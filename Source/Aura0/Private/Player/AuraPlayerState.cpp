#include "Player/AuraPlayerState.h"
#include "AbilitySystem\AuraAbilitySystemComponent.h"
#include "AbilitySystem\AuraAttributeSet.h"
#include <Net\UnrealNetwork.h>


AAuraPlayerState::AAuraPlayerState()
{
	//����ϵͳ���
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	//��������ͬ��ģʽ(Ѫ������������������������Ч��������ȴ���ȵ�)
	//������������˭�ܻ�ȡ����������˭����
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	//���Լ�
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME�������Զ��������Ը��Ƶ�һ���꣬�ֶ���������Ե�������
	//�� AAuraPlayerState ��� Level ���Ա��Ϊ��Ҫ�����縴�Ƶ�������������
	//ʵ�־��ǽ�Level����ע�ᵽ���������ڷ������иĶ�ʱ�����Զ����Ƶ�����
	DOREPLIFETIME(AAuraPlayerState, Level);

}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//�����ҵȼ�������ʱ��ͬʱ�����������
void AAuraPlayerState::OnRep_Level(int32 Oldlevel)
{


}
