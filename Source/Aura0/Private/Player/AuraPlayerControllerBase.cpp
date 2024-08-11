#include "Player/AuraPlayerControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Components/SplineComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"





AAuraPlayerControllerBase::AAuraPlayerControllerBase()
{
	//���ø��ƣ�Ҫʹһ�� Actor ���и��ƣ���Ҫ����Ĺ��캯�������� bReplicates ����Ϊ true��
	bReplicates = true;


	Spline = CreateDefaultSubobject<USplineComponent>("Spline");



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

//���Ƽ��Ƿ�ѹ
void AAuraPlayerControllerBase::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;

	}
	
}

//���Ƽ��Ƿ��ɿ���ѹ
void AAuraPlayerControllerBase::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)return;
	GetASC()->AbilityInputTagReleased(InputTag);

}
//��ѹ���
void AAuraPlayerControllerBase::AbilityInputTagHeld(FGameplayTag InputTag)
{

	//�жϴ���İ������󶨵�Tag���Ƿ�ΪInputTag_LMB
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
		//��ȡ���������λ��
		if(GetHitResultUnderCursor(ECC_Visibility,false,Hit))
		{
			CacheDestination = Hit.ImpactPoint;
		}

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



