//��ΪUAuraWidgetController������

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


class UAttributeInfo;
struct FAuraAttributeInfo;


//����������ԵĶ�̬�ಥ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);



UCLASS(BlueprintType, Blueprintable)
class AURA0_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:

	//����Ϊ�����к�����ʹ����Ҫoverride
	virtual void BindCallbacksToDependencies()override;
	virtual void BroadcastInitialValues()override;
	
	//����������Զ�̬�ಥ��Ϣ��
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;


protected:

	//������ҽӿڽṹ�壬��Ҫ����ͼ����Ӷ���
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo>AttributeInfo;

private:

	//void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute);




};
