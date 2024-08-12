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
	//�Ƿ����ݴ��ͷ���������
	bReplicates = true;
	LastActor = nullptr;
	ThisActor = nullptr;


	//���������ߣ���ɫ��������ƶ����
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");


}

//ÿִ֡�к���
void AAuraPlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//���λ��׷���Ƿ���ͣ�ڵ�������
	CursorTrace();

	//�Զ�Ѱ·
	AutoRun();

}

//����ҵ�����棬��ɫ�ͻ���������ĵط��ƶ���ȥ
void AAuraPlayerControllerBase::AutoRun()
{
	
	//�ж��Ƿ��Զ��ƶ�
	if (!bAutoRunning)return;

	//�Զ�Ѱ·ϵͳ���ж�ǰ�����ϰ������������ײ������һ���ƹ��ϰ����·��
	if (APawn* ControlledPawn = GetPawn())
	{
		//FindLocationClosestToWorldLocation�ڸ�������������ϵ�е�λ�ã�WorldLocation���ҵ�һ���������������ĵ�
		//�ҵ��������������λ��
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		//��ȡ���λ���������ϵķ���
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		//���㵽Ŀ��λ�õľ���
		const float DistanceToDestination = (LocationOnSpline - CacheDestination).Length();
		// �������С�ڻ����ĳ�����ܰ뾶����ֹͣ�Զ����� 
		if (DistanceToDestination <= AutoRunAccceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}


//��귢�䵽��Ļ������
void AAuraPlayerControllerBase::CursorTrace()
{
	
	//GetHitResultUnderCursor �� Unreal Engine �������ڹ�꣨����꣩λ��ִ�����߼�Ⲣ�������н���ĺ�����
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	//�������û���䵽�κζ������ͷ���
	if (!CursorHit.bBlockingHit)return;

	//��һ������ݴ浽������һ��������
	LastActor = ThisActor;
	//�����ոջ��е�������û��ʵ������ӿ�
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	//�ж���һ��״̬������
	if (LastActor != ThisActor)
	{
		if (ThisActor)ThisActor->HightLightActor();
		if (LastActor)LastActor->UnHightLightActor();
		
	}
}

//���Ƽ��Ƿ�ѹ
void AAuraPlayerControllerBase::AbilityInputTagPressed(FGameplayTag InputTag)
{
	
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor != nullptr;
		bAutoRunning = false;
		FollowTime = 0.f;//����ͳ�Ƶ�ʱ��

	}
	
}

//���Ƽ��Ƿ��ɿ���ѹ
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
			//���õĲ��ȥʰȡ�Զ�Ѱ·��·�ߣ�����������ʼ�㣬�����㣩
			//�����ѯ�ĵ�������һ��UNavigationPath����NavPath��
			//FindPathToLocationSynchronously �� Unreal Engine 5 ������ͬ�����Ҵ�һ�㵽��һ��·���ĺ���
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CacheDestination))
			{
				//������������еĵ�
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					//���µ�λ����ӵ�����������
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);

				}
				//�Զ�Ѱ·������Ŀ�ĵ�����Ϊ�������յ㣬����ֹͣ����
				CacheDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				//���õ�ǰ�����Զ�Ѱ·״̬������tick�и���λ��
				bAutoRunning = true;

			}
		}
		
	}

}


//��ѹ���
void AAuraPlayerControllerBase::AbilityInputTagHeld(FGameplayTag InputTag)
{

	//�жϴ���İ������󶨵�Tag���Ƿ�ΪInputTag_LMB
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
		//ͳ����ͣʱ�����ж��Ƿ�Ϊ���
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		//��ȡ���������λ��
		if(CursorHit.bBlockingHit)CacheDestination = CursorHit.ImpactPoint;
		

		//������ȡ����λ���ƶ�
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CacheDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}

}


UAuraAbilitySystemComponent* AAuraPlayerControllerBase::GetASC()
{
	//ȷ�������ת��UAuraAbilitySystemComponent��ʱ��ִֻ��һ��
	//����ÿ��ִ�ж�ת��һ�Σ������Ż�����
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}


//��Ϸһ��ʼִ���¼�
void AAuraPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	//���Լ��һ���Ƿ���ڣ������ھͱ���
	check(AuraContext);

	//�õ����������ǿ����ϵͳ
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) 
	{
		//��������龰
		Subsystem->AddMappingContext(AuraContext, 0);

	}
	
	//��ʾ��겢�Ҳ�Ҫ�������ӽ��м�
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//FInputModeGameAndUI���ڶ�������ģʽ������ʹ�������Ϸ���û����棨UI��֮���޷��л����롣
	// ����������Ϸ������ͬʱ������Ϸ�����UI���롣
	FInputModeGameAndUI InputModeData;
	//EMouseLockMode::DoNotLock��ָ����������굽�ӿڡ�������ƶ����ʱ��
	//�����������ƶ�����Ϸ���ڡ�������Ҫ������ҷ�������Ӧ�ó��������ʱ�ǳ����á�
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//�����ڲ�������ʱ�Ƿ���������꣺
	InputModeData.SetHideCursorDuringCapture(false);
	//�����úõ�����ģʽ InputModeData Ӧ�õ���ǰ��ҿ�����SetInputMode
	SetInputMode(InputModeData);

}

//��ȡ����ҵ����ð�����Ϣ�����԰������г�ʼ��ƥ��
void AAuraPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	//����UAuraInputComponent������ָ�룬�����InputComponent�Ƿ�Ҳ��UAuraInputComponent������ָ��
	UAuraInputComponent * AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	//����Ұ��¿���������õ�WASD��ӳ�䵽Move������
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &AAuraPlayerControllerBase::Move);

	//����Ұ��²�������������Ӧ�Ĳ���
	//��ȡ��ҵİ���������ִ��AuraInputComponent�е�BindAbilityActions����,����Ҫִ����Ϣ���ݹ�ȥ��������Ӧ�ĺ���
	AuraInputComponent->BindAbilityActions(InputConfig,this,&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);
	 

}

	//����ҿ������������и�ֵ
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



