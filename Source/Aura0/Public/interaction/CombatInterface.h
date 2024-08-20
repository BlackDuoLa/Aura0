// ����ӿ�Ϊ����ר��ʹ�õģ���ҽ�ɫ�޷�ʹ��

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA0_API ICombatInterface
{
	GENERATED_BODY()

	
public:

	//���⺯�������ڻ�ȡ�ȼ�
	virtual int32 GetPlayerLevel();
	//��ȡ�������λ��
	virtual FVector GetCombatSocketLocation();

	//���ý�ɫ����ʱ��ת�������ú����ڽ�ɫ����Ϊ�Զ��庯��ʹ�ã�
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);
};
