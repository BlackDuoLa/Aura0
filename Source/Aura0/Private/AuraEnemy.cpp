#include "AuraEnemy.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Aura0/Aura0.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"

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

	//�����˺���ʾUI
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
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

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::Die()
{
	//��ɫ�������ã�ʱ��ΪLifeSpan=5s������ɫ����������ͻ���5�������
	SetLifeSpan(LifeSpan);
	Super::Die();


}



void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	//��Ϸһ��ʼ���õ��˵��ƶ��ٶ�Ϊ
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	//check(AbilitySystemComponent);
	//��ʼ����˭ӵ���������˭�Ǵ����Ǳ���This�����˱Ƚϼ򵥣�
	InitAbilityActorInfo();

	//һ��ʼ�����ݴ����ȥ
	UAuraAbilitySystemLibrary::GiveStartupAbilites(this, AbilitySystemComponent);




	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);

	}



	//���㲥���˵�����ֵ
	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{

			
				OnHealthChanged.Broadcast(Data.NewValue);

			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
			
				OnMaxHealthChanged.Broadcast(Data.NewValue);
				
			}
		);


		//ע���ǩ�¼�,����ǩ�Ƴ�����Ӷ���ִ�а󶨵ĺ���
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraEnemy::HitReactTagChanged
		);



		//��Ϊ���˵��������԰󶨵�����ҵ��������ԣ���������Ҳ��ȡ����õ���ҵ���������
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());




	}
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{

	//����bHitReacting����Ƿ�Ϊ0
	bHitReacting = NewCount > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;


}





void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	//��ʼ����������
	InitializeDefaultAttributes();





}

void AAuraEnemy::InitializeDefaultAttributes() const
{

	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);


}
