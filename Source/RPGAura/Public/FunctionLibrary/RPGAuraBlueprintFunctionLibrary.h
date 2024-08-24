// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGAuraBlueprintFunctionLibrary.generated.h"

/**
 * 当前项目的函数库
 */
UCLASS()
class RPGAURA_API URPGAuraBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/// 在给定球体范围内查找没有死亡的敌方角色
	/// @param Causer 发起者
	/// @param OutOverlappingActors 返回找到的没有死亡并且发生重叠的角色
	/// @param IgnoreActors 不进行重叠的角色
	/// @param Radius 球体半径
	/// @param SphereOrigin 球体中心点位
	/// @param IgnoreSelf 是否忽略自己
	UFUNCTION(BlueprintCallable, Category="Overlap")
	static void FindLivePlayersWithinRadius(const AActor* Causer,
	                                 TArray<AActor*>& OutOverlappingActors,
	                                 const TArray<AActor*>& IgnoreActors, float Radius,
	                                 const FVector& SphereOrigin,bool IgnoreSelf);
};
