#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "interaction\EnemyInterface.h"//添加的沟边接口
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
	
		//接口中定义好的函数，在引用的文件中在写一遍，并添加返回override
		//沟边
		virtual  void HightLightActor() override;
		//不沟边
		virtual  void UnHightLightActor() override;

		
		//覆盖CombatInterface类的获取等级虚拟函数
		virtual int32 GetPlayerLevel() override;

		//创建敌人生命属性
		UPROPERTY(BlueprintAssignable)
		FOnAttributeChangedSignature OnHealthChanged;
		//创建敌人最大生命属性
		UPROPERTY(BlueprintAssignable)
		FOnAttributeChangedSignature OnMaxHealthChanged;


		protected:
			//virtual用于定义虚函数、继承类中被重写（覆盖）的函数
			virtual void BeginPlay() override;
	
			virtual void InitAbilityActorInfo() override;

			virtual void InitializeDefaultAttributes()const override;


			//创建敌人的等级
			UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
			int32 Level = 1;

			//创建敌人的等级
			UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
			ECharacterClass CharacterClass = ECharacterClass::Warrior;


			//添加敌人雪胎哦
			UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
			TObjectPtr<UWidgetComponent>HealthBar;




};
