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

		//需要初始化属性同步模式(血量、蓝量、体力、声音、特效、技能冷却、等等)的函数
		//与AuraAbilitySystemComponent相关
		virtual void PossessedBy(AController*NewController) override;
		virtual void OnRep_PlayerState()override;

private:
	//初始化属性同步模式(血量、蓝量、体力、声音、特效、技能冷却、等等)
		//与AuraAbilitySystemComponent相关
	virtual void InitAbilityActorInfo() override;


};
