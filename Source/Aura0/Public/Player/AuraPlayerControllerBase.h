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

		//��д��ÿִ֡��
		virtual void PlayerTick(float DeltaTime)override;

		//������ͼ���ݵĵ����ѵ��˺�������
		UFUNCTION(Client,Reliable)
		void ShowDamageNumber(float DamageAmount,ACharacter * TargetCharacter, bool bBlocckedHit, bool bCriticalHit);
	

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
	//����л�����
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction>ShiftAction;


	void ShiftPressed() { bShiftKeyDown = true; }
	void ShifReleased() { bShiftKeyDown = false; }
	bool bShiftKeyDown = false;

	//�ƶ�����
	void Move(const FInputActionValue& InputActionValue);

	//��귢�䵽��Ļ������
	void CursorTrace();

	//����������һ��״̬������
	//���������һ���״̬
	IEnemyInterface* LastActor;
	//������굱ǰ��״̬
	IEnemyInterface* ThisActor;

	//FHitResult ��һ�����ڴ�����ײ������Ľṹ��
	// ͨ�����߼�⣬�����ȷ����ҵ����λ�ã�����ȡ����������ϸ��Ϣ��
	// ���ںܶ���Ϸ�����зǳ����ã���������ѡ�񡢽���������ȡ�
	FHitResult CursorHit;

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



	//�洢�������λ��
	FVector CacheDestination = FVector::ZeroVector;
	// ���ڲ鿴��ס�˶��
	float FollowTime = 0.f;
	//��ǰ�Ƿ��Զ��ƶ�
	bool bAutoRunning = false;
	//��ǰ����Ƿ�ѡ�е���
	bool bTargeting = false;
	UPROPERTY(EditDefaultsOnly)
	float ShortPressThreshold = 0.5f;//���������ͣ�೤ʱ���������¼�
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAccceptanceRadius = 50.f; //����ɫ��Ŀ������ڴ˰뾶��ʱ�����ر��Զ�Ѱ·

	//�Զ�Ѱ·ʱ���ɵ�������
	UPROPERTY(VisibleAnywhere)
	TObjectPtr< USplineComponent>Spline;


	void AutoRun();


	//����������˺�UI����
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent>DamageTextComponentClass;
	
};
