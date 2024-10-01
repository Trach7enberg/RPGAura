// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGAuraBlueprintFunctionLibrary.generated.h"


struct FAbilityDescription;
struct FGameplayTag;
struct FAbilityDescElement;
class UAbilityDescriptionAsset;
/**
 * 当前项目的函数库
 */
UCLASS()
class RPGAURA_API URPGAuraBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	URPGAuraBlueprintFunctionLibrary();
	
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

	/// 检查两个Actor是否是友军
	/// @param Actor1 
	/// @param Actor2 
	/// @return 
	UFUNCTION(BlueprintCallable)
	static bool IsFriendly(const AActor* Actor1, AActor* Actor2);

	/// 获取描述技能信息的数据资产
	static UAbilityDescriptionAsset* GetAbilityDescriptionAsset();
	
	static UAbilityDescriptionAsset* GetAbilityDescriptionAsset(UObject* Outer);

	/// 从Actor身上获取给定技能标签的详细技能描述
	/// @param Actor 
	/// @param AbilityTag 
	/// @return
	UFUNCTION(BlueprintCallable)
	static FAbilityDescription GetAbilityDescription(AActor *Actor, const FGameplayTag& AbilityTag);
	

protected:
	static TObjectPtr<UAbilityDescriptionAsset> AbilityDescriptionAsset;

private:
	static void LoadAbilityDescriptionAsset(UObject* Outer);
};
