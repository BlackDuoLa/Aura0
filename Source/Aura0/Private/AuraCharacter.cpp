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


//��AuraAbilitySystemComponent���
void AAuraCharacter::PossessedBy(AController* NewController)
{
	//��������������汾����еģ������ع�֮���ҪSuperһ��
	Super::PossessedBy(NewController);

	//��ʼ������ͬ��ģʽ(Ѫ������������������������Ч��������ȴ���ȵ�)
	//�Ѿ��ڵ������϶�����ɣ����ǲ�����Ϊ��ʵ����������϶���
	InitAbilityActorInfo();
	//��ʼ����ɫ����
	AddCharacterAbilities();
}


//��AuraAbilitySystemComponent���
void AAuraCharacter::OnRep_PlayerState()
{
	//��������������汾����еģ������ع�֮���ҪSuperһ��
	Super::OnRep_PlayerState();

	//��ʼ������ͬ��ģʽ(Ѫ������������������������Ч��������ȴ���ȵ�)
	//�Ѿ��ڵ������϶�����ɣ����ǲ�����Ϊ��ʵ����������϶���
	InitAbilityActorInfo();

}

int32 AAuraCharacter::GetPlayerLevel()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//��ȡPlayerState��Ȼ����ú������صȼ���ֵ
	return AuraPlayerState->GetPlayerLevel();
}

//��ʼ������ͬ��ģʽ(Ѫ������������������������Ч��������ȴ���ȵ�)
//�������ҵ�����ͬ��ģʽ(Ѫ������������������������Ч��������ȴ���ȵ�)
//��AuraAbilitySystemComponent���
void AAuraCharacter::InitAbilityActorInfo()
{
	//ָ��ʵ��ӵ������˭
	AAuraPlayerState*AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	//�ж����治����
	check(AuraPlayerState);
	//��AbilitySystemComponent�����,��ȡ��ʼ���������Ѷ����ʵ��ʹ����AuraPlayerState����ȥ������������Լ�this
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);

	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	//�ѻ���Base��ʼ����AbilitySystemComponent�������ǿ���˵�Ǹ���Ϊ=����Ĳ���
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	//Ҳ�ѻ���Base��AttributeSet������ΪAuraPlayerState->GetAttributeSet();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	//�ɹ���ȡ�� AAuraPlayerControllerBase �� AAuraHUD �󣬵��� InitOverlay ������ʼ��HUD��Overlay���֡�
	if (AAuraPlayerControllerBase* AuraPlayerController = Cast<AAuraPlayerControllerBase>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}
