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

// 声明关于AS角色生命属性的动态多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

//声明玩家拾起物品UI界面的动态多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignatura, FUIWidgetRow, Row);






UCLASS(BlueprintType, Blueprintable)
class AURA0_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	//这两个函数在基类里定义了但是没有使用，在这子类里使用的话就要override一下
	virtual void BroadcastInitialValues()override;
	virtual void BindCallbacksToDependencies()override;

	//设置玩家生命属性发生改变广播
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	//设置玩家获取血量提升 发生改变广播
	UPROPERTY(BlueprintAssignable, Category = "GAS|Massages")
	FMessageWidgetRowSignatura MessageWidgetRowDelegate;


protected:

	//创建玩家标签属性，玩家标签需在控制界面设置
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


