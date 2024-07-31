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

		//��д��ÿִ֡��
		virtual void PlayerTick(float DeltaTime)override;
	



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




protected:
	//��д����Ϸ��ʼִ���¼�
	virtual void BeginPlay() override;
	//��ҿ�����
	virtual void SetupInputComponent() override;

	
};
