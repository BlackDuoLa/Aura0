// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA0_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	

public:



	//����һ������ģ����
	//������Զ���һ���������κ����͵ĺ���
	//�󶨿���/����/���ܰ���

	template<class UserClass,typename PressedFuncType,typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, HeldFuncType ReleasedFunc, HeldFuncType HeldFunc);

};


//���������˼�ǣ�ѭ�������Ҷ���İ�����Ϣ�����󶨲�����Ϣ
//template��ģ���࣬���洫����ʲô����������ʲô�ࡣ���ڱ�д�������õĴ��롣
template<class UserClass, typename PressedFuncType, typename HeldFuncType>
inline void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, HeldFuncType ReleasedFunc, HeldFuncType HeldFunc)
{

	check(InputConfig);
	//ѭ�����AbilityInputAction�еİ�����Ϣ������Action��
	for (const FAuraInputAction& Action : InputConfig->AbilityInputAction)
	{
		//�ж���Ҵ����İ�����ۣ��Ƿ񶼰��˰����Ͷ�Ӧ��Tag
		//&&�ж����������Ƿ�Ϊ�棬��Ϊ�棬����һ��Ϊ�٣���Ϊ��
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			//�Ѵ��ݽ�����������Ϣ�����а������ư�
			//���Ƽ��Ƿ�ѹ��
			if (PressedFunc)
			{
				
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			//���Ƽ��Ƿ��ɿ���ѹ��
			if (ReleasedFunc)
			{
				
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);

			}
			//��ѹ��ɰ�
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
