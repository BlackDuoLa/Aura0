#include "Player/AuraPlayerState.h"
#include "AbilitySystem\AuraAbilitySystemComponent.h"
#include "AbilitySystem\AuraAttributeSet.h"


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

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
