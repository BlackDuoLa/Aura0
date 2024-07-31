#include "Player/AuraPlayerControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


AAuraPlayerControllerBase::AAuraPlayerControllerBase()
{
	//启用复制：要使一个 Actor 进行复制，需要在类的构造函数中设置 bReplicates 属性为 true。
	bReplicates = true;
}

//每帧执行函数
void AAuraPlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//每帧都执行一次，检查鼠标是否放在，可以沟边的物体上
	CursorTrace();
}

//鼠标发射到屏幕的射线
void AAuraPlayerControllerBase::CursorTrace()
{
	//FHitResult 是一个用于处理碰撞检测结果的结构体
	// CursorHit 是一个变量名，通常用于保存光标点击的检测结果。
	// 通过射线检测，你可以确定玩家点击的位置，并获取点击对象的详细信息。
	// 这在很多游戏机制中非常有用，例如物体选择、交互和射击等。
	FHitResult CursorHit;
	//GetHitResultUnderCursor 是 Unreal Engine 中用于在光标（或鼠标）位置执行射线检测并返回命中结果的函数。
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	//如果射线没有射到任何东西，就返回
	if (!CursorHit.GetActor())return;

	//上一秒的数据存到现在这一秒数据里
	LastActor = ThisActor;
	//看看刚刚击中的物体有没有实现这个接口
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	//判断上一秒状态并储存
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HightLightActor();
		}
		else
		{
			//
		}
	}
	else
	{
		if(ThisActor==nullptr)
		{
			//
			LastActor->UnHightLightActor();

		}
		else
		{
			if (LastActor == ThisActor)
			{
				//
			}
			else
			{
				LastActor->UnHightLightActor();
				ThisActor->HightLightActor();
			}
		}
	}
	

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
	//添加输入情景
	Subsystem->AddMappingContext(AuraContext,0);
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

//玩家控制器
void AAuraPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
	//把控制器转换成角色控制
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	//当玩家按下控制器定义好的WASD就映射到Move函数上
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &AAuraPlayerControllerBase::Move);
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



