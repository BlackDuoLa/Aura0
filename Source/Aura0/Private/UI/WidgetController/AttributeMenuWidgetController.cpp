
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"



void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	//玩家属性映射菜单的网络同步
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair,AS](const FOnAttributeChangeData& Data)
			{
				//此为下面函数的引用
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

	//实现玩家属性到菜单的映射
	for (auto& Pair : AS->TagsToAttributes)
	{
		//此为下面函数的引用
		//BroadcastAttributeInfo(Pair.Key, Pair.Value());

		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key, false);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	


	}
}





//这个函数的作用就是简写上面For函数的内容
//应该是两个函数都要写之几行函数，博主觉得太麻烦了，不美观，就定义了这个函数，要用的时候引用就是了
//void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,const FGameplayAttribute& Attribute)
//{
//	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag,false);
//	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
//	AttributeInfoDelegate.Broadcast(Info);
//}


