// // 这个是AuraWidgetContreller的子类


#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayEffectTypes.h" 
#include "OverlayWidgetController.generated.h"



//这个是玩家拾起物品的结构体
USTRUCT(BlueprintType)
struct FUIWidgetRow :public FTableRowBase
{
	GENERATED_BODY()

	//定义物品的Tag类
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	//定义物品的名字类
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	//定义物品传出的UI类
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget>MessageWidget;

	//定义物品的照片类
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

	//定义玩家拾起物品的广播
	UPROPERTY(BlueprintAssignable, Category = "GAS|Massages")
	FMessageWidgetRowSignatura MessageWidgetRowDelegate;


protected:

	//创建一个数据标签属性，玩家定义的拾起物品属性就存在MessageWidgetDataTable里面
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Widget Data")
	TObjectPtr<UDataTable>MessageWidgetDataTable;



	//这是一个函数模板类
	//定义一个可以是任何类型的函数T
	//T*GetDataTableRowByTag可以理解为T=GetDataTableRowByTag
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

};

//这里是对模板类添加添加一个返回值
//返回值里的意思是，DataTable把Tab中的首行数据和Tag做比较，如果两个相同就返回这一行的数据到T中保存（就是GetDataTableRowByTag）
//如果不相同就返回nullptr,继续检索下一行
	template<typename T>
	T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
	{
		return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	}


