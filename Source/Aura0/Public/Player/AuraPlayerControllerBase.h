#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "interaction\EnemyInterface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "AuraPlayerControllerBase.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;

UCLASS()
class AURA0_API AAuraPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()


public:
		AAuraPlayerControllerBase();

		//重写的每帧执行
		virtual void PlayerTick(float DeltaTime)override;

		//创建蓝图传递的敌人搜到伤害的数据
		UFUNCTION(Client,Reliable)
		void ShowDamageNumber(float DamageAmount,ACharacter * TargetCharacter, bool bBlocckedHit, bool bCriticalHit);
	

protected:
	//重写的游戏开始执行事件
	virtual void BeginPlay() override;
	//玩家控制器
	virtual void SetupInputComponent() override;


private:
	//添加输入映射情景
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext>AuraContext;
	//添加输入操作
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction>MoveAction;
	//添加切换操作
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction>ShiftAction;


	void ShiftPressed() { bShiftKeyDown = true; }
	void ShifReleased() { bShiftKeyDown = false; }
	bool bShiftKeyDown = false;

	//移动函数
	void Move(const FInputActionValue& InputActionValue);

	//鼠标发射到屏幕的射线
	void CursorTrace();

	//创建储存上一秒状态的名称
	//储存鼠标上一秒的状态
	IEnemyInterface* LastActor;
	//储存鼠标当前的状态
	IEnemyInterface* ThisActor;

	//FHitResult 是一个用于处理碰撞检测结果的结构体
	// 通过射线检测，你可以确定玩家点击的位置，并获取点击对象的详细信息。
	// 这在很多游戏机制中非常有用，例如物体选择、交互和射击等。
	FHitResult CursorHit;

	//设置控制键是否按压函数
	void AbilityInputTagPressed(FGameplayTag InputTag);
	//设置控制键是否松开按压
	void AbilityInputTagReleased(FGameplayTag InputTag);
	//设置控制键按压完成
	void AbilityInputTagHeld(FGameplayTag InputTag);

	

	//传入玩家设置的控制操作
	UPROPERTY(EditDefaultsOnly,Category= "Input")
	TObjectPtr<UAuraInputConfig>InputConfig;

	//创建UAuraAbilitySystemComponent的智能指针
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent>AuraAbilitySystemComponent;
	UAuraAbilitySystemComponent* GetASC();



	//存储鼠标点击的位置
	FVector CacheDestination = FVector::ZeroVector;
	// 用于查看按住了多久
	float FollowTime = 0.f;
	//当前是否自动移动
	bool bAutoRunning = false;
	//当前鼠标是否选中敌人
	bool bTargeting = false;
	UPROPERTY(EditDefaultsOnly)
	float ShortPressThreshold = 0.5f;//定义鼠标悬停多长时间内算点击事件
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAccceptanceRadius = 50.f; //当角色和目标距离在此半径内时，将关闭自动寻路

	//自动寻路时生成的样条线
	UPROPERTY(VisibleAnywhere)
	TObjectPtr< USplineComponent>Spline;


	void AutoRun();


	//创建传入的伤害UI界面
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent>DamageTextComponentClass;
	
};
