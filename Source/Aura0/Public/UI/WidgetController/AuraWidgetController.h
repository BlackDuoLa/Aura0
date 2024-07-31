//�������ͼ�����Ļ��������
//���ඨ��PlayerController��PlayerState��AbilitySystemComponent��AttributeSet���ĸ�ֵ
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;



USTRUCT(BlueprintType)
//����ṹ��ʹ��캯����Ϊ�˳���PlayerController��PlayerState��AbilitySystemComponent��AttributeSet���ĸ�ֵ
//���ĸ�ֵ�����ڽṹ������дһ��
//�����������������ṹ�壬��ͨ���ṹ��Ѳ���������
struct FWidgetControllerParams
{
	GENERATED_BODY()

	//���������Ľṹ�庯��
	FWidgetControllerParams(){}

	//�������Ľṹ�庯�������ĸ�ֵ
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}
	
	//��Ҫ���ĸ�ֵ�����ڽṹ����дһ�飨��֪��Ϊʲô��
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerController>PlayerController = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerState>PlayerState = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAttributeSet>AttributeSet = nullptr;



}; 

/**
 * 
 */
UCLASS()
class AURA0_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()


public:

	//����
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();

protected:
	//���������ĸ�ֵָ��PlayerController��PlayerState��AbilitySystemComponent��AttributeSet����֪��ΪʲôҪ������
	//��������ṹ�����
	UPROPERTY(BlueprintReadOnly,Category ="WIdgetController")
	TObjectPtr<APlayerController>PlayerController;

	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<APlayerState>PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet>AttributeSet;
};
