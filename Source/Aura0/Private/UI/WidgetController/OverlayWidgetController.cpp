// 这个是AuraWidgetContreller的子类


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem\AuraAttributeSet.h"
#include "AbilitySystem\AuraAbilitySystemComponent.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
	//因为下面要使用到AS，所以这里需要拿到AS
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	//绑定玩家AS属性血量初始值和最大值,并且可以使用Broadcast绑定蓝图事件
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	//绑定玩家AS属性法力和最大值初始值,并且可以使用Broadcast绑定蓝图事件
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	//这个函数表达式的意思是，监听玩家的属性，如果玩家属性发生变化，就将新的值传过去，替换旧的值，以达到数值更新的效果
	
	//GetGameplayAttributeValueChangeDelegate这是ASC自带的监听数值变化的函数
	//监听对象为(AuraAttributeSet->GetHealthAttribute())
	//lambda表达式是事件变化处理器。当监听对象玩家属性发生变化时，就会执行lambda表达式
	//[this],this表示当前Lambda表达式，可以访问调用并修改这个.h和.CPP中所定义的所有函数，
	// 如果不加this就不能使用（OnHealthChanged.Broadcast）这个函数
	//(const FOnAttributeChangeData& Data)传入Lambda表达式中
	//FOnAttributeChangeData是ASC中定义好的函数，这个函数的作用是存储玩家属性值的新值和旧值
	//&的意思是对FOnAttributeChangeData常量引用，Data是名字 ( & 在这里我的理解类似于指针*的作用
	//FOnAttributeChangeData存储玩家属性的新值和旧值，这里把新值传入OnHealthChanged中
	//玩家可以通过在蓝图中获取此事件，用来对玩家血量的改变

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);

		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);

	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);

		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);

		});


	
	
	//--A--//
	//使用Cast模板函数尝试将AbilitySystemComponent转换为UAuraAbilitySystemComponent类型
	//调用UAuraAbilitySystemComponent中的EffectAssetTags函数并添加了一个Lambda表达式的操作
	//[this],this表示当前Lambda表达式，可以访问调用并修改这个.h和.CPP中所定义的所有函数
	//(const FGameplayTagContainer&AssetTags) 传入Lambda表达式中
	//FGameplayTagContainer是一个结构体（或类），用于存储和管理游戏玩法标签（Gameplay Tags）的集合。
	// 这些标签在游戏开发中用于标记游戏对象、能力、效果等，以便于根据这些标签进行逻辑判断、分类和筛选。
	//AssetTags为FGameplayTagContainer的引用（&可以理解为*）

	//--B--//
	//for (const FGameplayTag& Tag : AssetTags) 的意思是：“对于 AssetTags 容器中的每一个 FGameplayTag 对象，
	// 都将其作为一个&常量引用赋值给 Tag，并执行循环体内的代码”。
	//  : 用于分隔循环变量和要遍历的容器。
	
	//--C--//
	// RequestGameplayTag这个函数是获取在玩家创建的标签中有"Message"相关字符串的标签

	//--D--//
	//if (Tag.MatchesTag(MessageTag))，MatchesTag函数在这里的用法是：检查Tag中的标签是否包含MessageTag里面的标签（如果MessageTag里有一个标签不包含，这里将会不执行）
	//如果匹配后，执行一系列操作，包括从某个数据表（MessageWidgetDataTable）中根据标签检索数据行，
	// 并通过委托（MessageWidgetRowDelegate）广播这个数据行。
	//根据标签从MessageWidgetDataTable中检索FUIWidgetRow类型的数据行。
	
	//--E--//
	//当调用 GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag) 时，
	// 函数会遍历 MessageWidgetDataTable 中的所有行，检查每行是否与 Tag 匹配。
	//如果找到匹配的行，这行的数据将会被识别到并传入FUIWidgetRow中
	// 如果前面命名出现了错误，这里将找不到到匹配的行，通常返回 nullptr 或其他表示“未找到”的值
	// (遇到的情况就是，匹配不到标签就获取不到属性值，只要把标签Tag名字或者是数据表的行名字，改正确了就能恢复）
	// 数据表格里有多少行就匹配多少次，然后继续对数据中的下一行进行匹配
	//把获取的数据存入Row中
	//绑定到动态多播中，玩家在蓝图界面绑定到相应的操作中


	//这个函数用于获取玩家，拾起物品的信息，通过MessageWidgetRowDelegate.Broadcast函数绑定蓝图事件//
	// 把物品的信息传递到出去，通过玩家绑定Widget界面把消息传递出去//
			//A
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer&AssetTags)
		{
			//B
			for (const FGameplayTag& Tag : AssetTags)
			{
				//C
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				//D
				if (Tag.MatchesTag(MessageTag))
				{
					//E
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);

				}
			}
		}
	);
}


