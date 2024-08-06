// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI\Widget\AuraUserWidget.h"
#include "UI\WidgetController\OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	//�ж�ָ�����в�������
	if (OverlayWidgetController == nullptr)
	{
		//���û�в������룬�Ҿ��½�һ����
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		//��OverlayWidgetController��Ĳ���������ΪWidgetControllerParams(WCParams)
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
		AttributeMenuWidgetController->BindCallbacksToDependencies();//�󶨼�����ֵ�仯

	}

	return AttributeMenuWidgetController;
}

//���������Ϊ�˳�ʼ���ĸ�ֵ
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	//�����ͼ���Ƿ������OverlayWidgetClass
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitalized,Please fill out BBP_AuraHUD"));
	//�����ͼ���Ƿ������OverlayWidgetControllerClass
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitalized,Please fill out BBP_AuraHUD"));

	//��UI��ʾ����
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);
	
	//����ǰ��Ľṹ������ʼֵ
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	//���ĸ�ֵ����OverlayWidgetController��
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);

	WidgetController->BroadcastInitialValues();

	//��UI��ӵ���Ļ��
	Widget->AddToViewport();

}


