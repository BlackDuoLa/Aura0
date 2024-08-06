// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayEffectTypes.h" 
#include "OverlayWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FUIWidgetRow :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget>MessageWidget;

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

	//������һ�ȡѪ������ �����ı�㲥
	UPROPERTY(BlueprintAssignable, Category = "GAS|Massages")
	FMessageWidgetRowSignatura MessageWidgetRowDelegate;


protected:

	//������ұ�ǩ���ԣ���ұ�ǩ���ڿ��ƽ�������
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Widget Data")
	TObjectPtr<UDataTable>MessageWidgetDataTable;


	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

};


	template<typename T>
	T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
	{
		return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	}


