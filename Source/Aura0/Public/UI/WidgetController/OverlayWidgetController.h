// // �����AuraWidgetContreller������


#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayEffectTypes.h" 
#include "OverlayWidgetController.generated.h"



//��������ʰ����Ʒ�Ľṹ��
USTRUCT(BlueprintType)
struct FUIWidgetRow :public FTableRowBase
{
	GENERATED_BODY()

	//������Ʒ��Tag��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	//������Ʒ��������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	//������Ʒ������UI��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget>MessageWidget;

	//������Ʒ����Ƭ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

class UAuraUserWidget;

// ��������AS��ɫ�������ԵĶ�̬�ಥί��
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

//�������ʰ����ƷUI����Ķ�̬�ಥί��
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignatura, FUIWidgetRow, Row);




UCLASS(BlueprintType, Blueprintable)
class AURA0_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	//�����������ڻ����ﶨ���˵���û��ʹ�ã�����������ʹ�õĻ���Ҫoverrideһ��
	virtual void BroadcastInitialValues()override;
	virtual void BindCallbacksToDependencies()override;

	//��������������Է����ı�㲥
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	//�������ʰ����Ʒ�Ĺ㲥
	UPROPERTY(BlueprintAssignable, Category = "GAS|Massages")
	FMessageWidgetRowSignatura MessageWidgetRowDelegate;


protected:

	//����һ�����ݱ�ǩ���ԣ���Ҷ����ʰ����Ʒ���Ծʹ���MessageWidgetDataTable����
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Widget Data")
	TObjectPtr<UDataTable>MessageWidgetDataTable;



	//����һ������ģ����
	//����һ���������κ����͵ĺ���T
	//T*GetDataTableRowByTag�������ΪT=GetDataTableRowByTag
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

};

//�����Ƕ�ģ����������һ������ֵ
//����ֵ�����˼�ǣ�DataTable��Tab�е��������ݺ�Tag���Ƚϣ����������ͬ�ͷ�����һ�е����ݵ�T�б��棨����GetDataTableRowByTag��
//�������ͬ�ͷ���nullptr,����������һ��
	template<typename T>
	T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
	{
		return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	}


