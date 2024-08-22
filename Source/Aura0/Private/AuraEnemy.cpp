#include "AuraEnemy.h"
#include "AbilitySystem\AuraAbilitySystemComponent.h"
#include "AbilitySystem\AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Aura0\Aura0.h"
#include "UI/Widget/AuraUserWidget.h"


AAuraEnemy::AAuraEnemy()
{
	//ASC系统在敌人中创建能力系统组件(血量、蓝量、体力、声音、特效、技能冷却、等等)
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	//设置属性同步模式(血量、蓝量、体力、声音、特效、技能冷却、等等)
	//想把相关属性让谁能获取到，就设在谁身上
	//与下面初始化Begin Play相关联
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	//属性集
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	//创建伤害显示UI
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}


//检查是否执行沟边
void AAuraEnemy::HightLightActor()
{
	//设置渲染自定义深度通道设置为true
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	//打开武器的渲染自定义深度通道
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

//检查是否执行不沟边
void AAuraEnemy::UnHightLightActor()
{
	//吗检查到设置为false;
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	//check(AbilitySystemComponent);
	//初始化，谁拥有这个代理，谁是代理都是本身This（敌人比较简单）
	InitAbilityActorInfo();

	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);

	}



	//给广播敌人的生命值
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
		//因为敌人的生命属性绑定的是玩家的生命属性，所以这里也获取定义好的玩家的生命属性
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());




	}





}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	//初始化敌人属性
	InitializeDefaultAttributes();

}
