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
	//AuraUI控制器基类的指针
	UPROPERTY()
	TObjectPtr<UAuraUserWidget>OverlayWidget;

	//得到OverlayWidgetController类的指针，指针里传入（）里的参数
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	//设置初始化函数InitOverlay，要持有四大函数APlayerController，APlayerState，UAbilitySystemComponent，UAttributeSet
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,UAttributeSet* AS);


protected:

private:
	//加载AuraUI控制器基类，要在蓝图中设置的类
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget>OverlayWidgetClass;

	//加载AuraUI控制器基类的子类(名字是Overlay)，要在蓝图中设置的类
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController>OverlayWidgetController;

	//AuraUI控制器基类的子类的指针(名字是Overlay)
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController>OverlayWidgetControllerClass;

};
