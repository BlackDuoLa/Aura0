
#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"


// ��ĳ���¼���������ҵ����ĳ��Ŀ�꣩����ʱ
// ���а󶨵����ί�еĻص��������ᱻ���ã����һ���յ�һ�� FGameplayAbilityTargetDataHandle ���͵Ĳ�����
// �ڽ�ɫ�ͷż���ʱ������ͨ��FGameplayAbilityTargetDataHandle�����ݼ��ܵ�Ŀ����Ϣ����Ŀ��λ�á�Ŀ�����ȣ�
// DataHandle�������ͨ�������˹��ڱ����Ŀ�����Ϣ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDaraSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);



UCLASS()
class AURA0_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	


public:

	//������ʾ��굱ǰָ��λ��Ŀ�����ݵ�UTargetDataUnderMouseʵ��
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility",DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly="true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	//ValidData�����������������
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDaraSignature ValidData; 


private:

	//�����
	virtual void Activate()override;

	//�������������
	void SendMouseCursorData();


	//�����λ����Ϣ����ͬ��
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,FGameplayTag ActivationTag);
};
