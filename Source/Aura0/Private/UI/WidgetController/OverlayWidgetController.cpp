// �����AuraWidgetContreller������


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem\AuraAttributeSet.h"
#include "AbilitySystem\AuraAbilitySystemComponent.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
	//��Ϊ����Ҫʹ�õ�AS������������Ҫ�õ�AS
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	//�����AS����Ѫ����ʼֵ�����ֵ,���ҿ���ʹ��Broadcast����ͼ�¼�
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	//�����AS���Է��������ֵ��ʼֵ,���ҿ���ʹ��Broadcast����ͼ�¼�
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	//����������ʽ����˼�ǣ�������ҵ����ԣ����������Է����仯���ͽ��µ�ֵ����ȥ���滻�ɵ�ֵ���Դﵽ��ֵ���µ�Ч��
	
	//GetGameplayAttributeValueChangeDelegate����ASC�Դ��ļ�����ֵ�仯�ĺ���
	//��������Ϊ(AuraAttributeSet->GetHealthAttribute())
	//lambda���ʽ���¼��仯������������������������Է����仯ʱ���ͻ�ִ��lambda���ʽ
	//[this],this��ʾ��ǰLambda���ʽ�����Է��ʵ��ò��޸����.h��.CPP������������к�����
	// �������this�Ͳ���ʹ�ã�OnHealthChanged.Broadcast���������
	//(const FOnAttributeChangeData& Data)����Lambda���ʽ��
	//FOnAttributeChangeData��ASC�ж���õĺ�������������������Ǵ洢�������ֵ����ֵ�;�ֵ
	//&����˼�Ƕ�FOnAttributeChangeData�������ã�Data������ ( & �������ҵ����������ָ��*������
	//FOnAttributeChangeData�洢������Ե���ֵ�;�ֵ���������ֵ����OnHealthChanged��
	//��ҿ���ͨ������ͼ�л�ȡ���¼������������Ѫ���ĸı�

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
	//ʹ��Castģ�庯�����Խ�AbilitySystemComponentת��ΪUAuraAbilitySystemComponent����
	//����UAuraAbilitySystemComponent�е�EffectAssetTags�����������һ��Lambda���ʽ�Ĳ���
	//[this],this��ʾ��ǰLambda���ʽ�����Է��ʵ��ò��޸����.h��.CPP������������к���
	//(const FGameplayTagContainer&AssetTags) ����Lambda���ʽ��
	//FGameplayTagContainer��һ���ṹ�壨���ࣩ�����ڴ洢�͹�����Ϸ�淨��ǩ��Gameplay Tags���ļ��ϡ�
	// ��Щ��ǩ����Ϸ���������ڱ����Ϸ����������Ч���ȣ��Ա��ڸ�����Щ��ǩ�����߼��жϡ������ɸѡ��
	//AssetTagsΪFGameplayTagContainer�����ã�&�������Ϊ*��

	//--B--//
	//for (const FGameplayTag& Tag : AssetTags) ����˼�ǣ������� AssetTags �����е�ÿһ�� FGameplayTag ����
	// ��������Ϊһ��&�������ø�ֵ�� Tag����ִ��ѭ�����ڵĴ��롱��
	//  : ���ڷָ�ѭ��������Ҫ������������
	
	//--C--//
	// RequestGameplayTag��������ǻ�ȡ����Ҵ����ı�ǩ����"Message"����ַ����ı�ǩ

	//--D--//
	//if (Tag.MatchesTag(MessageTag))��MatchesTag������������÷��ǣ����Tag�еı�ǩ�Ƿ����MessageTag����ı�ǩ�����MessageTag����һ����ǩ�����������ｫ�᲻ִ�У�
	//���ƥ���ִ��һϵ�в�����������ĳ�����ݱ�MessageWidgetDataTable���и��ݱ�ǩ���������У�
	// ��ͨ��ί�У�MessageWidgetRowDelegate���㲥��������С�
	//���ݱ�ǩ��MessageWidgetDataTable�м���FUIWidgetRow���͵������С�
	
	//--E--//
	//������ GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag) ʱ��
	// ��������� MessageWidgetDataTable �е������У����ÿ���Ƿ��� Tag ƥ�䡣
	//����ҵ�ƥ����У����е����ݽ��ᱻʶ�𵽲�����FUIWidgetRow��
	// ���ǰ�����������˴������ｫ�Ҳ�����ƥ����У�ͨ������ nullptr ��������ʾ��δ�ҵ�����ֵ
	// (������������ǣ�ƥ�䲻����ǩ�ͻ�ȡ��������ֵ��ֻҪ�ѱ�ǩTag���ֻ��������ݱ�������֣�����ȷ�˾��ָܻ���
	// ���ݱ�����ж����о�ƥ����ٴΣ�Ȼ������������е���һ�н���ƥ��
	//�ѻ�ȡ�����ݴ���Row��
	//�󶨵���̬�ಥ�У��������ͼ����󶨵���Ӧ�Ĳ�����


	//����������ڻ�ȡ��ң�ʰ����Ʒ����Ϣ��ͨ��MessageWidgetRowDelegate.Broadcast��������ͼ�¼�//
	// ����Ʒ����Ϣ���ݵ���ȥ��ͨ����Ұ�Widget�������Ϣ���ݳ�ȥ//
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


