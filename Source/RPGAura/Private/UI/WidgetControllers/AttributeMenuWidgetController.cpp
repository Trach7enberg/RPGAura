// Copyright GGBAO 


#include "UI/WidgetControllers/AttributeMenuWidgetController.h"

#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "GAS/Data/AttributeInfo.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"


DEFINE_LOG_CATEGORY_STATIC(UAttributeMenuWidgetControllerLog, All, All);


void UAttributeMenuWidgetController::BindCallBack()
{
	if (!IsWidgetControllerParamsValid())
	{
		UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("控制器的基本参数无效!"));
		return;
	}

	const auto MyAs = Cast<UBaseAttributeSet>(GetWidgetControllerParams().CurrentAttributeSet);
	if (!MyAs) { return; }

	const auto GameInstanceSubSystem = GetWidgetControllerParams().CurrentPlayerController->GetGameInstance()->
	                                                               GetSubsystem<URPGAuraGameInstanceSubsystem>();
	if (!GameInstanceSubSystem)
	{
		UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("GameInstance子系统获取失败!"));
		return;
	}
	for (const auto& Pair : MyAs->TagToAttributeMap) { HandleAnyAttributeChange(GameInstanceSubSystem,MyAs, Pair.Key, Pair.Value()); }
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	if (!AttributeInfo)
	{
		UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("映射数据资产表信息不能为nullptr!"));
		return;
	}

	if (!IsWidgetControllerParamsValid())
	{
		UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("控制器的基本参数无效!"));
		return;
	}

	const auto MyAs = Cast<UBaseAttributeSet>(GetWidgetControllerParams().CurrentAttributeSet);
	if (!MyAs) { return; }

	const auto MyAcs = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);
	if (!MyAcs) { return; }

	// UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("ALL:%d"), FRPGAuraGameplayTags::GetGameplayTagsMap().Num());
	// UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("ALL:%d"), MyAs->TagToAttributeMap.Num());

	// 遍历属性集的主要和次要属性,广播属性信息
	// DoFirstBroadcast(MyAs, FRPGAuraGameplayTags::PrimaryGameplayTagsArray);
	// DoFirstBroadcast(MyAs, FRPGAuraGameplayTags::SecondaryGameplayTagsArray);
	for (const auto& Pair : MyAs->TagToAttributeMap) { BroadcastAttributeInfoStruct(GetWidgetControllerParams().GameInstanceSubsystem,MyAs, Pair.Key, Pair.Value()); }
}

void UAttributeMenuWidgetController::HandleAnyAttributeChange(const URPGAuraGameInstanceSubsystem* Subsystem,const UBaseAttributeSet* MyAs,
                                                              const FGameplayTag& GameplayTag,
                                                              const FGameplayAttribute& Attribute) const
{
	GetWidgetControllerParams().CurrentAbilitySystemComponent->
	                            GetGameplayAttributeValueChangeDelegate(Attribute)
	                            .AddLambda([this,Subsystem,MyAs, GameplayTag, Attribute](const FOnAttributeChangeData& Data)
	                            {
		                            BroadcastAttributeInfoStruct(Subsystem,MyAs, GameplayTag, Attribute);
	                            });
}

void UAttributeMenuWidgetController::BroadcastAttributeInfoStruct(const URPGAuraGameInstanceSubsystem* Subsystem,
                                                                  const UBaseAttributeSet* MyAs,
                                                                  const FGameplayTag& GameplayTag,
                                                                  const FGameplayAttribute& Attribute) const
{
	if (!AttributeInfo || !Subsystem)
	{
		UE_LOG(UAttributeMenuWidgetControllerLog, Warning, TEXT("致命错误"));
		return;
	}
	auto AttributeInfoStruct = AttributeInfo->FindAttributeInfoByTag(GameplayTag);
	// FRPGAuraAttributeInfo{};

	if (!AttributeInfoStruct.AttributeTag.IsValid())
	{
		UE_LOG(UAttributeMenuWidgetControllerLog, Warning, TEXT("找不到GT:[%s]对应的属性信息结构体"), *GameplayTag.ToString());
		return;
	}

	AttributeInfoStruct.AttributeValue = Attribute.GetNumericValue(MyAs);


	Subsystem->OnAttributeChanged.Broadcast(AttributeInfoStruct);
}
