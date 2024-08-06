//此为UAuraWidgetController的子类

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


class UAttributeInfo;
struct FAuraAttributeInfo;


//创建玩家属性的动态多播
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);



UCLASS(BlueprintType, Blueprintable)
class AURA0_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:

	//函数为父类中函数，使用需要override
	virtual void BindCallbacksToDependencies()override;
	virtual void BroadcastInitialValues()override;
	
	//创建玩家属性动态多播消息绑定
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;


protected:

	//创建玩家接口结构体，需要在蓝图中添加定义
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo>AttributeInfo;

private:

	//void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute);




};
