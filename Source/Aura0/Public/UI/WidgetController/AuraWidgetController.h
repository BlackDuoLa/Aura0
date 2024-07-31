//这个是视图部件的基类控制器
//基类定义PlayerController，PlayerState，AbilitySystemComponent，AttributeSet这四个值
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;



USTRUCT(BlueprintType)
//这个结构体故构造函数是为了持有PlayerController，PlayerState，AbilitySystemComponent，AttributeSet这四个值
//这四个值必须在结构体里又写一次
//用这个函数调用这个结构体，再通过结构体把参数都赋上
struct FWidgetControllerParams
{
	GENERATED_BODY()

	//不带参数的结构体函数
	FWidgetControllerParams(){}

	//带参数的结构体函数定义四个值
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}
	
	//需要把四个值重新在结构体中写一遍（不知道为什么）
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

	//设置
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();

protected:
	//创建了这四个值指针PlayerController，PlayerState，AbilitySystemComponent，AttributeSet（不知道为什么要创建）
	//用于上面结构体持有
	UPROPERTY(BlueprintReadOnly,Category ="WIdgetController")
	TObjectPtr<APlayerController>PlayerController;

	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<APlayerState>PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet>AttributeSet;
};
