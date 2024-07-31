#include "AuraEnemy.h"
#include "AbilitySystem\AuraAbilitySystemComponent.h"
#include "AbilitySystem\AuraAttributeSet.h"
#include "Aura0\Aura0.h"


AAuraEnemy::AAuraEnemy()
{
	//ASCϵͳ�ڵ����д�������ϵͳ���(Ѫ������������������������Ч��������ȴ���ȵ�)
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	//��������ͬ��ģʽ(Ѫ������������������������Ч��������ȴ���ȵ�)
	//������������˭�ܻ�ȡ����������˭����
	//�������ʼ��Begin Play�����
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	//���Լ�
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

}


//����Ƿ�ִ�й���
void AAuraEnemy::HightLightActor()
{
	//������Ⱦ�Զ������ͨ������Ϊtrue
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	//����������Ⱦ�Զ������ͨ��
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

//����Ƿ�ִ�в�����
void AAuraEnemy::UnHightLightActor()
{
	//���鵽����Ϊfalse;
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	//check(AbilitySystemComponent);
	//��ʼ����˭ӵ���������˭�Ǵ����Ǳ���This�����˱Ƚϼ򵥣�

	InitAbilityActorInfo();

}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

}
