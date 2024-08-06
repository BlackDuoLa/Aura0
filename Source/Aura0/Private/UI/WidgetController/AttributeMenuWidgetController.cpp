
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"



void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	//�������ӳ��˵�������ͬ��
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair,AS](const FOnAttributeChangeData& Data)
			{
				//��Ϊ���溯��������
				//BroadcastAttributeInfo(Pair.Key, Pair.Value());

				FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key, false);
				Info.AttributeValue = Pair.Value().GetNumericValue(AS);
				AttributeInfoDelegate.Broadcast(Info);



			}
		);
	}
}


void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	//ʵ��������Ե��˵���ӳ��
	for (auto& Pair : AS->TagsToAttributes)
	{
		//��Ϊ���溯��������
		//BroadcastAttributeInfo(Pair.Key, Pair.Value());

		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key, false);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	


	}
}





//������������þ��Ǽ�д����For����������
//Ӧ��������������Ҫд֮���к�������������̫�鷳�ˣ������ۣ��Ͷ��������������Ҫ�õ�ʱ�����þ�����
//void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,const FGameplayAttribute& Attribute)
//{
//	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag,false);
//	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
//	AttributeInfoDelegate.Broadcast(Info);
//}


