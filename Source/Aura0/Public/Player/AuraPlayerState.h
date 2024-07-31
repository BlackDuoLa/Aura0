// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class AURA0_API AAuraPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	public:
	AAuraPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	protected:
	//����ϵͳ���
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;
	//���Լ�
	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;

};
