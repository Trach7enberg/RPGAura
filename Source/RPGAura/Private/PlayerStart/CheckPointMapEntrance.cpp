// Copyright GGBAO 


#include "PlayerStart/CheckPointMapEntrance.h"

#include "Components/BoxComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GameModes/RPGAuraGameModeBase.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(ACheckPointMapEntranceLog, All, All);

void ACheckPointMapEntrance::LoadActorSaveInfo()
{
	// 从磁盘加载时,什么都不做
}

void ACheckPointMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
                                             AActor*              OtherActor,
                                             UPrimitiveComponent* OtherComp,
                                             int32                OtherBodyIndex,
                                             bool                 bFromSweep,
                                             const FHitResult&    SweepResult)
{
	if (!OtherActor || !OtherActor->ActorHasTag(FRPGAuraGameplayTags::Get().Player)) { return; }

	const auto PlayerIntF = Cast<IPlayerInterface>(OtherActor);
	if (!PlayerIntF) { return; }

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (DestinationMap.IsNull() || DestinationPlayerStartTag.IsNone())
	{
		// 软指针对象只有使用时才会延迟加载,因此即使设置了引用,判断DestinationMap.Get()也为空,应该用IsNull判断 
		UE_LOG(ACheckPointMapEntranceLog, Error, TEXT("[%s]旅行目的地的检查点无效!!"), TEXT(__FUNCTION__));
		return;
	}
	Reached = true;
	// 保存进度,并且设置当前存档的重生点为下一个目的地的重生位置 TODO 同时也要设置Gi的playerStartTag  
	PlayerIntF->SaveProgress(DestinationPlayerStartTag);

	if (const auto MyGameMode = Cast<ARPGAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		 
		MyGameMode->SaveWorldState(GetWorld(),DestinationMap.ToSoftObjectPath().GetAssetName());
	}
	else
	{
		UE_LOG(ACheckPointMapEntranceLog,
		       Error,
		       TEXT("[%s]检查点 %s:无法保存世界状态!!"),
		       TEXT(__FUNCTION__),
		       *PlayerStartTag.ToString());
		return;
	}

	UGameplayStatics::OpenLevelBySoftObjectPtr(this,DestinationMap);
}
