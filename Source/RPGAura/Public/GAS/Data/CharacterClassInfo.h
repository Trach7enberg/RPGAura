// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"


/**
 *  游戏角色职业信息资产
 */
UCLASS()
class RPGAURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	// 角色类别映射到对应的角色默认信息结构Map
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Character Class Defaults")
	TMap<ECharacterClass,FCharacterClassDefaultInfo> CharacterClassInformation;

	// 角色(敌人)的共同属性(Secondary)影响初始化(注意,当前敌人的次要属性不会受到主属性的升级影响,即只是Instant,不是无限效果)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributesEnemy;

	// 角色(玩家)的共同属性(Secondary)影响初始化(注意,当前玩家的次要属性会受到主属性影响)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributesPlayer;

	// 角色的共同属性(Vital)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	/// 根据角色职业枚举查找对应角色的默认职业数据
	/// @param CharacterClass 角色职业枚举
	/// @return 角色的默认职业数据结构
	FCharacterClassDefaultInfo FindClassDefaultInfo(const ECharacterClass & CharacterClass);
};
