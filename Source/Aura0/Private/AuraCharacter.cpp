// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerControllerBase.h" 
#include "UI/HUD/AuraHUD.h"





AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}


//与AuraAbilitySystemComponent相关
void AAuraCharacter::PossessedBy(AController* NewController)
{
	//这个函数是在引擎本身带有的，我们重构之后就要Super一下
	Super::PossessedBy(NewController);

	//初始化属性同步模式(血量、蓝量、体力、声音、特效、技能冷却、等等)
	//已经在敌人身上定义完成，这是操作是为了实现在玩家身上定义
	InitAbilityActorInfo();
	//初始化角色能力
	AddCharacterAbilities();
}


//与AuraAbilitySystemComponent相关
void AAuraCharacter::OnRep_PlayerState()
{
	//这个函数是在引擎本身带有的，我们重构之后就要Super一下
	Super::OnRep_PlayerState();

	//初始化属性同步模式(血量、蓝量、体力、声音、特效、技能冷却、等等)
	//已经在敌人身上定义完成，这是操作是为了实现在玩家身上定义
	InitAbilityActorInfo();

}

int32 AAuraCharacter::GetPlayerLevel()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//获取PlayerState，然后调用函数返回等级数值
	return AuraPlayerState->GetPlayerLevel();
}

//初始化属性同步模式(血量、蓝量、体力、声音、特效、技能冷却、等等)
//这就是玩家的属性同步模式(血量、蓝量、体力、声音、特效、技能冷却、等等)
//与AuraAbilitySystemComponent相关
void AAuraCharacter::InitAbilityActorInfo()
{
	//指认实际拥有者是谁
	AAuraPlayerState*AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	//判断它存不存在
	check(AuraPlayerState);
	//从AbilitySystemComponent组件上,获取初始化函数，把定义的实际使用者AuraPlayerState传过去，代理是玩家自己this
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);

	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	//把基类Base初始化的AbilitySystemComponent定义这是可以说是复制为=后面的参数
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	//也把基类Base的AttributeSet，定义为AuraPlayerState->GetAttributeSet();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	//成功获取到 AAuraPlayerControllerBase 和 AAuraHUD 后，调用 InitOverlay 函数初始化HUD的Overlay部分。
	if (AAuraPlayerControllerBase* AuraPlayerController = Cast<AAuraPlayerControllerBase>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}
