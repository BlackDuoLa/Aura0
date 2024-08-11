
#include "Input/AuraInputConfig.h"







const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	//循环检测输入操作的Action设置，保持到Action中
	for(const FAuraInputAction& Action: AbilityInputAction)

		//判断玩家创建的按键插槽，是否都绑定了按键和对应的Tag
		//&&判断两个条件是否都为真，则为真，其中一个为假，则为假
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
