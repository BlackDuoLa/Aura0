// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	//ѭ��������������Action���ã����ֵ�Action��
	for(const FAuraInputAction& Action: AbilityInputAction)

		//&&�ж����������Ƿ�Ϊ�棬ֻҪһ��������ͷ���false;
		//�ж������Ƿ�Ϊ�գ��ж������Tag��Ϊ���������Tag
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
