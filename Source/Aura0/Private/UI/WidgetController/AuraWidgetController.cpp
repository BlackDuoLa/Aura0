// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	//�����涨����ĸ���������������︳ֵ������ṹ��ĺ���
	//��ֵ����ʵ�ʲ���һ��
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;

}

void UAuraWidgetController::BroadcastInitialValues()
{


}

void UAuraWidgetController::BindCallbacksToDependencies()
{

}
