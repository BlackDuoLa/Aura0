// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	//循环检测输入操作的Action设置，保持到Action中
	for(const FAuraInputAction& Action: AbilityInputAction)

		//&&判断两个条件是否都为真，只要一个不满足就返回false;
		//判断输入是否为空，判断输入的Tag，为输入进来的Tag
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			
			return Action.InputAction;

		}
	
	if (bLogNotFound)
	{

		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInput for InputTag[%s], On InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));

	}
	return nullptr;

}
