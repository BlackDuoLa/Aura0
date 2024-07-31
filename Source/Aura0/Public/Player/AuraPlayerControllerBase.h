#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "interaction\EnemyInterface.h"
#include "AuraPlayerControllerBase.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;

UCLASS()
class AURA0_API AAuraPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()


	public:
		AAuraPlayerControllerBase();

		//重写的每帧执行
		virtual void PlayerTick(float DeltaTime)override;
	



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




protected:
	//重写的游戏开始执行事件
	virtual void BeginPlay() override;
	//玩家控制器
	virtual void SetupInputComponent() override;

	
};
