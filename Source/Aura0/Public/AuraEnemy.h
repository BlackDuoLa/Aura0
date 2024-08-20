#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "interaction\EnemyInterface.h"//��ӵĹ��߽ӿ�
#include "AuraEnemy.generated.h"


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

		protected:
			//virtual���ڶ����麯�����̳����б���д�����ǣ��ĺ���
			virtual void BeginPlay() override;
	
			virtual void InitAbilityActorInfo() override;


			//�������˵ĵȼ�
			UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
			int32 Level = 1;






};
