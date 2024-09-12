#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "interaction/EnemyInterface.h"//��ӵĹ��߽ӿ�
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"


class UWidgetComponent;


UCLASS()
class AURA0_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()


public:


		AAuraEnemy();
	
		//�ӿ��ж���õĺ����������õ��ļ�����дһ�飬����ӷ���override
		//����
		virtual  void HightLightActor() override;
		//������
		virtual  void UnHightLightActor() override;

		
		//����CombatInterface��Ļ�ȡ�ȼ����⺯��
		virtual int32 GetPlayerLevel() override;
		virtual void Die()override;

		//����������������
		UPROPERTY(BlueprintAssignable)
		FOnAttributeChangedSignature OnHealthChanged;
		//�������������������
		UPROPERTY(BlueprintAssignable)
		FOnAttributeChangedSignature OnMaxHealthChanged;


		void HitReactTagChanged(const FGameplayTag CallbackTag ,int32 NewCount);


		//
		UPROPERTY(BlueprintReadOnly,Category = "Combat")
		bool bHitReacting = false;

		//�������˳�ʼ�ٶ�Ϊ250
		UPROPERTY(BlueprintReadOnly, Category = "Combat")
		float BaseWalkSpeed = 250.f;

		//��ɫ�������������ʧ
		UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat")
		float LifeSpan = 5.f;

		protected:
			//virtual���ڶ����麯�����̳����б���д�����ǣ��ĺ���
			virtual void BeginPlay() override;
	
			virtual void InitAbilityActorInfo() override;

			virtual void InitializeDefaultAttributes()const override;


			//�������˵ĵȼ�
			UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
			int32 Level = 1;

			//�������˵ĵȼ�
			UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
			ECharacterClass CharacterClass = ECharacterClass::Warrior;


			//��ӵ���ѩ̥Ŷ
			UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
			TObjectPtr<UWidgetComponent>HealthBar;


		

};
