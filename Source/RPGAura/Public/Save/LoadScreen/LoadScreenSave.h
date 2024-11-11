// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "LoadScreenSave.generated.h"


/**
 * 用于加载屏幕widget所需要持久化到磁盘的数据
 */
UCLASS()
class RPGAURA_API ULoadScreenSave : public USaveGame
{
	GENERATED_BODY()

public:
	// 分辨持久化数据所需要的名称
	UPROPERTY()
	FString SlotName = FString();

	// 关卡默认的初始位置的标识
	UPROPERTY()
	FName PlayerStartTag{};

	// 持久化数据所需要的索引
	UPROPERTY()
	int32 SlotIndex = 0;

	// 玩家存档的名字
	UPROPERTY()
	FString PlayerName = FString(TEXT("无"));

	// 默认地图名字
	UPROPERTY()
	FString MapName = FString(TEXT("无"));

	// 当前插槽切换器所在的插槽索引
	UPROPERTY()
	FGameplayTag CurrentLoadSlotSwitcherLoadSlotTag = FGameplayTag::EmptyTag;

	/// 用于记录是否是第一次加载
	UPROPERTY()
	bool BisFirstLoad = true;

	/* 与玩家属性有关的信息 */
	/// 玩家等级
	UPROPERTY()
	int32 PlayerLevel = 1;

	// 默认经验为1级的经验,300
	UPROPERTY()
	int32 PlayerXP = 300;

	/// 当前角色可以分配的属性点
	UPROPERTY()
	int32 AssignableAttributePoints = 0;

	/// 当前角色可以分配的法术点
	UPROPERTY()
	int32 AssignableSpellPoints = 0;

	// 主属性 力量
	UPROPERTY()
	float Strength;

	// 主属性 智力
	UPROPERTY()
	float Intelligence;

	// 主属性 耐力
	UPROPERTY()
	float Resilience;

	// 主属性 活力
	UPROPERTY()
	float Vigor;

	/// 包含需要序列化角色能力的信息数组
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities{};

	// 地图资产的名字与地图所对应的Actors列表
	UPROPERTY()
	TMap<FString, FSavedActors> SaveMaps{};
};
