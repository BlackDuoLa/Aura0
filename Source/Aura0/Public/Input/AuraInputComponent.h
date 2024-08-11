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



	//这是一个函数模板类
	//里面可以定义一个可以是任何类型的函数
	//绑定控制/能力/技能按键

	template<class UserClass,typename PressedFuncType,typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, HeldFuncType ReleasedFunc, HeldFuncType HeldFunc);

};


//这个函数意思是：循环检测玩家定义的按键消息，并绑定操作信息
//template是模板类，对面传过来什么参数它就是什么类。用于编写灵活、可重用的代码。
template<class UserClass, typename PressedFuncType, typename HeldFuncType>
inline void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, HeldFuncType ReleasedFunc, HeldFuncType HeldFunc)
{

	check(InputConfig);
	//循环检测AbilityInputAction中的按键信息并存入Action中
	for (const FAuraInputAction& Action : InputConfig->AbilityInputAction)
	{
		//判断玩家创建的按键插槽，是否都绑定了按键和对应的Tag
		//&&判断两个条件是否都为真，则为真，其中一个为假，则为假
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			//把传递进来按键的信息，进行按键控制绑定
			//控制键是否按压绑定
			if (PressedFunc)
			{
				
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			//控制键是否松开按压绑定
			if (ReleasedFunc)
			{
				
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);

			}
			//按压完成绑定
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
