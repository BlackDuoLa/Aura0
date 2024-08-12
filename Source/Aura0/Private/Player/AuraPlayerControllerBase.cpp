#include "Player/AuraPlayerControllerBase.h"

#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Components/SplineComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Interaction/EnemyInterface.h"







AAuraPlayerControllerBase::AAuraPlayerControllerBase()
{
	//是否将数据传送服务器更新
	bReplicates = true;
	LastActor = nullptr;
	ThisActor = nullptr;


	//创建样条线，角色点击地面移动相关
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");


}

//每帧执行函数
void AAuraPlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//鼠标位置追踪是否悬停在敌人身上
	CursorTrace();

	//自动寻路
	AutoRun();

}

//当玩家点击地面，角色就会向鼠标点击的地方移动过去
void AAuraPlayerControllerBase::AutoRun()
{
	
	//判断是否自动移动
	if (!bAutoRunning)return;

	//自动寻路系统，判断前面有障碍物，根据物体碰撞，创造一条绕过障碍物的路线
	if (APawn* ControlledPawn = GetPawn())
	{
		//FindLocationClosestToWorldLocation在给定的世界坐标系中的位置（WorldLocation）找到一个与这个曲线最近的点
		//找到距离样条最近的位置
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		//获取这个位置在样条上的方向
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		//计算到目标位置的距离
		const float DistanceToDestination = (LocationOnSpline - CacheDestination).Length();
		// 如果距离小于或等于某个接受半径，则停止自动运行 
		if (DistanceToDestination <= AutoRunAccceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}


//鼠标发射到屏幕的射线
void AAuraPlayerControllerBase::CursorTrace()
{
	
	//GetHitResultUnderCursor 是 Unreal Engine 中用于在光标（或鼠标）位置执行射线检测并返回命中结果的函数。
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	//如果射线没有射到任何东西，就返回
	if (!CursorHit.bBlockingHit)return;

	//上一秒的数据存到现在这一秒数据里
	LastActor = ThisActor;
	//看看刚刚击中的物体有没有实现这个接口
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	//判断上一秒状态并储存
	if (LastActor != ThisActor)
	{
		if (ThisActor)ThisActor->HightLightActor();
		if (LastActor)LastActor->UnHightLightActor();
		
	}
}

//控制键是否按压
void AAuraPlayerControllerBase::AbilityInputTagPressed(FGameplayTag InputTag)
{
	
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor != nullptr;
		bAutoRunning = false;
		FollowTime = 0.f;//重置统计的时间

	}
	
}

//控制键是否松开按压
void AAuraPlayerControllerBase::AbilityInputTagReleased(FGameplayTag InputTag)
{

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{

		if (GetASC())GetASC()->AbilityInputTagReleased(InputTag);

		return;
	}

	if (bTargeting)
	{
		if (GetASC())GetASC()->AbilityInputTagReleased(InputTag);
		
	}
	else

	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold&&ControlledPawn)
		{
			//内置的插件去拾取自动寻路的路线（传入自身，起始点，结束点）
			//如果查询的到将返回一个UNavigationPath存入NavPath中
			//FindPathToLocationSynchronously 是 Unreal Engine 5 中用于同步查找从一点到另一点路径的函数
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CacheDestination))
			{
				//清除样条内现有的点
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					//将新的位置添加到样条曲线中
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);

				}
				//自动寻路将最终目的地设置为导航的终点，方便停止导航
				CacheDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				//设置当前正常自动寻路状态，将在tick中更新位置
				bAutoRunning = true;

			}
		}
		
	}

}


//按压完成
void AAuraPlayerControllerBase::AbilityInputTagHeld(FGameplayTag InputTag)
{

	//判断传入的按键，绑定的Tag，是否为InputTag_LMB
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
	
		if (GetASC())GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	if (bTargeting)
	{
		if (GetASC())GetASC()->AbilityInputTagHeld(InputTag);

	}
	else
	{
		//统计悬停时间来判断是否为点击
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		//获取到鼠标点击的位置
		if(CursorHit.bBlockingHit)CacheDestination = CursorHit.ImpactPoint;
		

		//玩家向获取到的位置移动
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CacheDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}

}


UAuraAbilitySystemComponent* AAuraPlayerControllerBase::GetASC()
{
	//确保玩家在转换UAuraAbilitySystemComponent的时候只执行一次
	//不用每次执行都转换一次，可以优化代码
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}


//游戏一开始执行事件
void AAuraPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	//断言检查一下是否存在，不存在就崩溃
	check(AuraContext);

	//得到本地玩家增强输入系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) 
	{
		//添加输入情景
		Subsystem->AddMappingContext(AuraContext, 0);

	}
	
	//显示鼠标并且不要锁定在视角中间
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//FInputModeGameAndUI用于定义输入模式，可以使玩家在游戏和用户界面（UI）之间无缝切换输入。
	// 它允许在游戏窗口中同时处理游戏输入和UI输入。
	FInputModeGameAndUI InputModeData;
	//EMouseLockMode::DoNotLock：指定不锁定鼠标到视口。当玩家移动鼠标时，
	//鼠标可以自由移动出游戏窗口。这在需要允许玩家访问其他应用程序或桌面时非常有用。
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//设置在捕获输入时是否隐藏鼠标光标：
	InputModeData.SetHideCursorDuringCapture(false);
	//将配置好的输入模式 InputModeData 应用到当前玩家控制器SetInputMode
	SetInputMode(InputModeData);

}

//获取到玩家的设置按键消息，并对按键进行初始化匹配
void AAuraPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	//创建UAuraInputComponent的智能指针，并检测InputComponent是否也是UAuraInputComponent的智能指针
	UAuraInputComponent * AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	//当玩家按下控制器定义好的WASD就映射到Move函数上
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &AAuraPlayerControllerBase::Move);

	//当玩家按下操作键，激活相应的操作
	//获取玩家的按键操作并执行AuraInputComponent中的BindAbilityActions函数,把需要执行消息传递过去并激活相应的函数
	AuraInputComponent->BindAbilityActions(InputConfig,this,&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);
	 

}

	//对玩家控制器按键进行赋值
void AAuraPlayerControllerBase::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControllerPawn = GetPawn())
	{
		ControllerPawn->AddMovementInput(ForwardDir, InputAxisVector.Y);
		ControllerPawn->AddMovementInput(RightDir, InputAxisVector.X);
	}

}



