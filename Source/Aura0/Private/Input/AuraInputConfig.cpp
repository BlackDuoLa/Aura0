
#include "Input/AuraInputConfig.h"







const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	//ѭ��������������Action���ã����ֵ�Action��
	for(const FAuraInputAction& Action: AbilityInputAction)

		//�ж���Ҵ����İ�����ۣ��Ƿ񶼰��˰����Ͷ�Ӧ��Tag
		//&&�ж����������Ƿ�Ϊ�棬��Ϊ�棬����һ��Ϊ�٣���Ϊ��
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
