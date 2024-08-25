// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

UCLASS(Abstract)
class AURA0_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface ,public ICombatInterface
{
	GENERATED_BODY()

public:
	
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	
	virtual void BeginPlay() override;

	
	//����ָ��
	UPROPERTY(EditAnywhere,Category = "Combat")
	TObjectPtr<USkeletalMeshComponent>Weapon;

	//�������
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSockeName;

	//�����ӿ�
	virtual FVector GetCombatSocketLocation()override;


	//����ϵͳ���
	//�ڻ����ʼ��������ͬ��ģʽ(Ѫ������������������������Ч��������ȴ���ȵ�)
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;
	//���Լ�
	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;


	virtual void InitAbilityActorInfo();


	//���Ĭ��������
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category= "Attributes")
	TSubclassOf<UGameplayEffect>DefaultPrimaryAttributes;

	//���Ĭ�ϴ�����
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect>DefaultSecondaryAttributes;

	//���Ĭ������
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect>DefaultVitalAttributes;



	//�ڽ�ɫ�������GEϵͳ�����ý�ɫ�������
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect>GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes()const;


	//�������ϵͳ����
	void AddCharacterAbilities();


private:

	//��ҽ�ɫ������ϵͳ
	UPROPERTY(EditAnywhere,Category="Ability")
	TArray < TSubclassOf<UGameplayAbility>>StartupAbilities;





};

