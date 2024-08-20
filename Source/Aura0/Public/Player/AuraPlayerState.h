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

	//��ȡ������������Ҫ���Ƶ�����
	// ����GetLifetimeReplicatedProps��ȷ��Level���Ա���ȷ����  
	//�� GetLifetimeReplicatedProps ������ʱ��UE4�ķ���ϵͳ���Զ�ʶ�����б����Ϊ Replicated �����ԣ�
	// ����������ӵ� OutLifetimeProps ������
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	//����ɫ�ȼ�����GetPlayerLevel��
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }

	protected:
	//��ȡ����ϵͳ���
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	//��ȡ�������
	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;


private:

	//�����ҵȼ�
	//Level�����Ѿ���UPROPERTY���е�Replicated��־���Ϊ��Ҫ����
	//ָ��OnRep_Level�ú����������Ա����縴��ʱ����
	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_Level)
	int32 Level = 1;


	//��ҵȼ�����ͬ��
	//OnRep_Level �����������ã�������ɵ�ֵ��Ϊ������
	UFUNCTION()
	void OnRep_Level(int32 Oldlevel);




};
