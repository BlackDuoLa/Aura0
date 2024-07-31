// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"


class UOverlayWidgetController;
class UAuraUserWidget;

class APlayerController;
class APlayerState;
class UAbilitySystemComponent;
class UAttributeSet;

struct FWidgetControllerParams;

UCLASS()
class AURA0_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
	

public:
	//AuraUI�����������ָ��
	UPROPERTY()
	TObjectPtr<UAuraUserWidget>OverlayWidget;

	//�õ�OverlayWidgetController���ָ�룬ָ���ﴫ�루����Ĳ���
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	//���ó�ʼ������InitOverlay��Ҫ�����Ĵ���APlayerController��APlayerState��UAbilitySystemComponent��UAttributeSet
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,UAttributeSet* AS);


protected:

private:
	//����AuraUI���������࣬Ҫ����ͼ�����õ���
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget>OverlayWidgetClass;

	//����AuraUI���������������(������Overlay)��Ҫ����ͼ�����õ���
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController>OverlayWidgetController;

	//AuraUI����������������ָ��(������Overlay)
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController>OverlayWidgetControllerClass;

};
