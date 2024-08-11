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

UCLASS()
class AURA0_API AAuraPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()


public:
		AAuraPlayerControllerBase();

		//重写的每帧执行
		virtual void PlayerTick(float DeltaTime)override;
	

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

	//移动函数
	void Move(const FInputActionValue& InputActionValue);

	//鼠标发射到屏幕的射线
	void CursorTrace();

	//创建储存上一秒状态的名称
	//储存鼠标上一秒的状态
	IEnemyInterface* LastActor;
	//储存鼠标当前的状态
	IEnemyInterface* ThisActor;


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



	FVector CacheDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAccceptanceRadius = 50.f;

	//玩家移动的样条变量
	UPROPERTY(VisibleAnywhere)
	TObjectPtr< USplineComponent>Spline;

	
};
