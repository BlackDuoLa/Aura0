
#include "AbilitySystem/Data/AttributeInfo.h"





FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{

	//����Ҵ��������ԣ�ѭ���󶨵�Info��
	//��֪����������Щ���ݿ��Կ���ͼ������Ϊ:DA_AttributeInfo

	for (const FAuraAttributeInfo& Info:AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	//��ʱ��֪����ʲô��
	if (bLogNotFound)
	{

		UE_LOG(LogTemp, Error, TEXT("Can't find Info For AttributeTag %s in %s"), *AttributeTag.ToString(), *GetNameSafe(this));

	}

	//������ݵİ����շ��ش��뵽FAuraAttributeInfo�ṹ����
	return FAuraAttributeInfo();
}
