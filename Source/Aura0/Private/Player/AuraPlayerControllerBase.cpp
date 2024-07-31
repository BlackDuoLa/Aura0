#include "Player/AuraPlayerControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


AAuraPlayerControllerBase::AAuraPlayerControllerBase()
{
	//���ø��ƣ�Ҫʹһ�� Actor ���и��ƣ���Ҫ����Ĺ��캯�������� bReplicates ����Ϊ true��
	bReplicates = true;
}

//ÿִ֡�к���
void AAuraPlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//ÿ֡��ִ��һ�Σ��������Ƿ���ڣ����Թ��ߵ�������
	CursorTrace();
}

//��귢�䵽��Ļ������
void AAuraPlayerControllerBase::CursorTrace()
{
	//FHitResult ��һ�����ڴ�����ײ������Ľṹ��
	// CursorHit ��һ����������ͨ�����ڱ��������ļ������
	// ͨ�����߼�⣬�����ȷ����ҵ����λ�ã�����ȡ����������ϸ��Ϣ��
	// ���ںܶ���Ϸ�����зǳ����ã���������ѡ�񡢽���������ȡ�
	FHitResult CursorHit;
	//GetHitResultUnderCursor �� Unreal Engine �������ڹ�꣨����꣩λ��ִ�����߼�Ⲣ�������н���ĺ�����
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	//�������û���䵽�κζ������ͷ���
	if (!CursorHit.GetActor())return;

	//��һ������ݴ浽������һ��������
	LastActor = ThisActor;
	//�����ոջ��е�������û��ʵ������ӿ�
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	//�ж���һ��״̬������
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
	//��������龰
	Subsystem->AddMappingContext(AuraContext,0);
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

//��ҿ�����
void AAuraPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
	//�ѿ�����ת���ɽ�ɫ����
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	//����Ұ��¿���������õ�WASD��ӳ�䵽Move������
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &AAuraPlayerControllerBase::Move);
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



