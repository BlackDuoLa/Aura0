#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"

#include "AuraCharacter.generated.h"


UCLASS()
class AURA0_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:

		AAuraCharacter();

		//��Ҫ��ʼ������ͬ��ģʽ(Ѫ������������������������Ч��������ȴ���ȵ�)�ĺ���
		//��AuraAbilitySystemComponent���
		virtual void PossessedBy(AController*NewController) override;
		virtual void OnRep_PlayerState()override;

private:
	//��ʼ������ͬ��ģʽ(Ѫ������������������������Ч��������ȴ���ȵ�)
		//��AuraAbilitySystemComponent���
	virtual void InitAbilityActorInfo() override;


};
