
#include "AbilitySystem/Data/AttributeInfo.h"





FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{

	//把玩家创建的属性，循环绑定到Info中
	//想知道定义了哪些内容可以看蓝图，名字为:DA_AttributeInfo

	for (const FAuraAttributeInfo& Info:AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	//暂时不知道有什么用
	if (bLogNotFound)
	{

		UE_LOG(LogTemp, Error, TEXT("Can't find Info For AttributeTag %s in %s"), *AttributeTag.ToString(), *GetNameSafe(this));

	}

	//完成数据的绑定最终发回存入到FAuraAttributeInfo结构体中
	return FAuraAttributeInfo();
}
