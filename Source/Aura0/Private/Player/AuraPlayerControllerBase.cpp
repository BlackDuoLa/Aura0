#include "Player/AuraPlayerControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Components/SplineComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"





AAuraPlayerControllerBase::AAuraPlayerControllerBase()
{
	//启用复制：要使一个 Actor 进行复制，需要在类的构造函数中设置 bReplicates 属性为 true。
	bReplicates = true;


	Spline = CreateDefaultSubobject<USplineComponent>("Spline");



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

//控制键是否按压
void AAuraPlayerControllerBase::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;

	}
	
}

//控制键是否松开按压
void AAuraPlayerControllerBase::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)return;
	GetASC()->AbilityInputTagReleased(InputTag);

}
//按压完成
void AAuraPlayerControllerBase::AbilityInputTagHeld(FGameplayTag InputTag)
{

	//判断传入的按键，绑定的Tag，是否为InputTag_LMB
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
	
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);

		}
		return;
	}
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);

		}

	}
	else
	{

		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult Hit;
		//获取到鼠标点击的位置
		if(GetHitResultUnderCursor(ECC_Visibility,false,Hit))
		{
			CacheDestination = Hit.ImpactPoint;
		}

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



