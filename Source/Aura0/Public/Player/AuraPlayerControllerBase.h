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

		//��д��ÿִ֡��
		virtual void PlayerTick(float DeltaTime)override;
	

protected:
	//��д����Ϸ��ʼִ���¼�
	virtual void BeginPlay() override;
	//��ҿ�����
	virtual void SetupInputComponent() override;


private:
	//�������ӳ���龰
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext>AuraContext;
	//����������
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction>MoveAction;

	//�ƶ�����
	void Move(const FInputActionValue& InputActionValue);

	//��귢�䵽��Ļ������
	void CursorTrace();

	//����������һ��״̬������
	//���������һ���״̬
	IEnemyInterface* LastActor;
	//������굱ǰ��״̬
	IEnemyInterface* ThisActor;


	//���ÿ��Ƽ��Ƿ�ѹ����
	void AbilityInputTagPressed(FGameplayTag InputTag);
	//���ÿ��Ƽ��Ƿ��ɿ���ѹ
	void AbilityInputTagReleased(FGameplayTag InputTag);
	//���ÿ��Ƽ���ѹ���
	void AbilityInputTagHeld(FGameplayTag InputTag);

	

	//����������õĿ��Ʋ���
	UPROPERTY(EditDefaultsOnly,Category= "Input")
	TObjectPtr<UAuraInputConfig>InputConfig;

	//����UAuraAbilitySystemComponent������ָ��
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

	//����ƶ�����������
	UPROPERTY(VisibleAnywhere)
	TObjectPtr< USplineComponent>Spline;

	
};
