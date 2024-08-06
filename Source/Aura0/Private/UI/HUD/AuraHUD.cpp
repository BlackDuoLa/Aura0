// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI\Widget\AuraUserWidget.h"
#include "UI\WidgetController\OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	//判断指针是有参数传入
	if (OverlayWidgetController == nullptr)
	{
		//如果没有参数传入，我就新建一个类
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		//把OverlayWidgetController类的参数，设置为WidgetControllerParams(WCParams)
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{

	if (AttributeMenuWidgetController == nullptr)
	{

		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();//绑定监听数值变化

	}

	return AttributeMenuWidgetController;
}

//这个函数是为了初始化四个值
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	//检查蓝图里是否添加了OverlayWidgetClass
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitalized,Please fill out BBP_AuraHUD"));
	//检查蓝图里是否添加了OverlayWidgetControllerClass
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitalized,Please fill out BBP_AuraHUD"));

	//把UI显示出来
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);
	
	//声明前面的结构体斌赋予初始值
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	//把四个值传入OverlayWidgetController中
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);

	WidgetController->BroadcastInitialValues();

	//把UI添加到屏幕上
	Widget->AddToViewport();

}


