// Copyright GGBAO 


#include "GameModes/RPGAuraGameModeBase.h"

#include "EngineUtils.h"
#include "Characters/AuraCharacter.h"
#include "Controller/BasePlayerController.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GameFramework/PlayerStart.h"
#include "GameInstance/BaseGameInstance.h"
#include "Interfaces/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Save/LoadScreen/LoadScreenSave.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/MVVM/ViewModel/MVVMLoadSlotSwitcher.h"

DEFINE_LOG_CATEGORY_STATIC(ARPGAuraGameModeBaseLog, All, All);

ARPGAuraGameModeBase::ARPGAuraGameModeBase()
{
	DefaultPawnClass      = AAuraCharacter::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();
}

AActor* ARPGAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	const auto Gi = GetMyGameInstance();
	if (!Gi) { return nullptr; }

	// 获取出生点
	const auto Name = Gi->PlayerStartTag;


	TArray<AActor*> Actors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	AActor* PlayerStart = nullptr;

	for (const auto& Actor : Actors)
	{
		APlayerStart* LocalPlayerStart = Cast<APlayerStart>(Actor);
		if (!LocalPlayerStart) { continue; }

		if (LocalPlayerStart->PlayerStartTag == Name)
		{
			PlayerStart = LocalPlayerStart;
			break;
		}
	}

	if (!PlayerStart)
	{
		UE_LOG(
		       ARPGAuraGameModeBaseLog,
		       Warning,
		       TEXT("[%s]无法找到名为[%s]指定的开始位置!"),
		       *FString::Printf(TEXT(__FUNCTION__)),
		       *Name.ToString());
		if (Actors.Num()) { PlayerStart = Actors[0]; }
	}

	return PlayerStart;
}

UBaseGameInstance* ARPGAuraGameModeBase::GetMyGameInstance()
{
	if (!MyGameInstance) { MyGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this)); }
	return MyGameInstance.Get();
}

void ARPGAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	const auto Level = LevelMaps.Find(StartingMapName);
	if (!Level) { UE_LOG(ARPGAuraGameModeBaseLog, Error, TEXT("[%s]无法找到关卡!"), *FString::Printf(TEXT(__FUNCTION__))); }
}


bool ARPGAuraGameModeBase::SaveLoadSlotData(UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel, const int32 SaveSlotIndex)
{
	if (!LoadSlotSwitcherViewModel) { return false; }

	// 要保持的存档如果已经存在,则删除
	DeleteSlotData(LoadSlotSwitcherViewModel);

	auto SaveGameObj = UGameplayStatics::CreateSaveGameObject(ULoadScreenSave::StaticClass());
	if (!SaveGameObj) { return false; }

	auto LoadScreenSaveGameObj = Cast<ULoadScreenSave>(SaveGameObj);
	if (!LoadScreenSaveGameObj) { return false; }

	LoadScreenSaveGameObj->SlotName                           = LoadSlotSwitcherViewModel->GetLoadSlotName();
	LoadScreenSaveGameObj->SlotIndex                          = SaveSlotIndex;
	LoadScreenSaveGameObj->MapName                            = LoadSlotSwitcherViewModel->GetMapName();
	LoadScreenSaveGameObj->MapAssetName                       = LoadSlotSwitcherViewModel->GetMapAssetName();
	LoadScreenSaveGameObj->PlayerName                         = LoadSlotSwitcherViewModel->GetPlayerName();
	LoadScreenSaveGameObj->PlayerStartTag                     = LoadSlotSwitcherViewModel->GetPlayerStartTag();
	LoadScreenSaveGameObj->CurrentLoadSlotSwitcherLoadSlotTag = FRPGAuraGameplayTags::Get().Widget_Menu_Load_Slot_Taken;
	return UGameplayStatics::SaveGameToSlot(
	                                        LoadScreenSaveGameObj,
	                                        LoadSlotSwitcherViewModel->GetLoadSlotName(),
	                                        SaveSlotIndex);
}

bool ARPGAuraGameModeBase::SaveLoadSlotData(ULoadScreenSave* SaveGameObj)
{
	if (!SaveGameObj || !UGameplayStatics::DoesSaveGameExist(SaveGameObj->SlotName, SaveGameObj->SlotIndex))
	{
		return false;
	}
	return UGameplayStatics::SaveGameToSlot(SaveGameObj, SaveGameObj->SlotName, SaveGameObj->SlotIndex);
}

void ARPGAuraGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName)
{
	if (!World) { return; }

	// 获取当前已加载的存档
	const auto CurrentLoadData = GetCurrentInGameSaveData();
	if (!CurrentLoadData)
	{
		UE_LOG(ARPGAuraGameModeBaseLog, Error, TEXT("[%s]无法获取当前关卡存档!"), *FString::Printf(TEXT(__FUNCTION__)));
		return;
	}

	if (DestinationMapAssetName != FString(""))
	{
		CurrentLoadData->MapAssetName = DestinationMapAssetName;
		CurrentLoadData->MapName      = GetLevelMapNameFromMapAssetName(DestinationMapAssetName);
		if (CurrentLoadData->MapName.IsEmpty())
		{
			UE_LOG(ARPGAuraGameModeBaseLog, Error, TEXT("[%s]无法获取关卡地图!"), *FString::Printf(TEXT(__FUNCTION__)));
			return;
		}
	}

	// 获取当前关卡名
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	auto SavedActors = CurrentLoadData->SaveMaps.Find(WorldName);

	// 如果存在则重置对应地图中保存的Actors,否则创建新的
	if (SavedActors) { SavedActors->Actors.Reset(); }
	else { SavedActors = &CurrentLoadData->SaveMaps.Add(WorldName, FSavedActors{}); }

	// 获取所有带有保存接口的检查点PlayerStart
	for (FActorIterator It(World); It; ++It)
	{
		AActor* Actor = *It;

		// 获取实现了存档接口的Actor
		if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) { continue; }

		FSavedActorInfo SavedActorInfo{};
		SavedActorInfo.ActorName      = Actor->GetFName();
		SavedActorInfo.ActorTransform = Actor->GetActorTransform();

		// 将SavedActorInfo.Bytes给内存写入器,用于下面的Archive将数据写入这个字节数组
		// FObjectAndNameAsStringProxyArchive是一个序列化代理（Proxy Archive）继承自 FArchiveProxy
		// 专门用于在序列化对象的过程中，将 FName 和 UObject 引用转换为字符串形式
		FMemoryWriter                      MemoryWriter(SavedActorInfo.Bytes);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true;

		// 将Actor序列化到SavedActorInfo.Bytes中!
		Actor->Serialize(Archive);
		SavedActors->Actors.AddUnique(SavedActorInfo);
	}
	UGameplayStatics::SaveGameToSlot(CurrentLoadData, CurrentLoadData->SlotName, CurrentLoadData->SlotIndex);
}

void ARPGAuraGameModeBase::LoadWorldState(UWorld* World)
{
	if (!World) { return; }

	// 获取当前已加载的存档
	const auto CurrentLoadData = GetCurrentInGameSaveData();
	if (!CurrentLoadData)
	{
		UE_LOG(ARPGAuraGameModeBaseLog, Warning, TEXT("[%s]无法获取当前关卡存档!"), *FString::Printf(TEXT(__FUNCTION__)));
		return;
	}

	// 获取当前关卡名
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	const auto CurrentMapSavedActorsFromData = CurrentLoadData->SaveMaps.Find(WorldName);
	if (!CurrentMapSavedActorsFromData)
	{
		UE_LOG(ARPGAuraGameModeBaseLog,
		       Error,
		       TEXT("[%s]存档没有地图[%s]所保存的信息"),
		       *FString::Printf(TEXT(__FUNCTION__)),
		       *WorldName);
		return;
	}

	for (FActorIterator It(World); It; ++It)
	{
		AActor* Actor = *It;

		const auto SaveIntFActor = Cast<ISaveInterface>(Actor);
		// 获取实现了存档接口的Actor
		if (!SaveIntFActor) { continue; }
		for (auto& ActorInfo : CurrentMapSavedActorsFromData->Actors)
		{
			if (ActorInfo.ActorName != Actor->GetFName()) { continue; }

			if (SaveIntFActor->ShouldLoadTransform()) { Actor->SetActorTransform(ActorInfo.ActorTransform); }

			// 内存读取器
			FMemoryReader                      MemoryReader(ActorInfo.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
			Archive.ArIsSaveGame = true;

			// 反序列化!它会读取Archive中的字节数组信息,并将这些数据重新赋值到Actor对象的相应属性上
			Actor->Serialize(Archive);
			SaveIntFActor->LoadActorSaveInfo();
		}
	}
}

ULoadScreenSave* ARPGAuraGameModeBase::GetLoadSlotData(const FString& Name, const int32 SlotIndex) const
{
	return Cast<ULoadScreenSave>(UGameplayStatics::LoadGameFromSlot(Name, SlotIndex));
}

ULoadScreenSave* ARPGAuraGameModeBase::GetLoadSlotData(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel) const
{
	if (!UGameplayStatics::DoesSaveGameExist(LoadSlotSwitcherViewModel->GetLoadSlotName(),
	                                         LoadSlotSwitcherViewModel->GetLoadSlotInSave())) { return nullptr; }
	return Cast<ULoadScreenSave>(UGameplayStatics::LoadGameFromSlot(LoadSlotSwitcherViewModel->GetLoadSlotName(),
	                                                                LoadSlotSwitcherViewModel->GetLoadSlotInSave()));
}

bool ARPGAuraGameModeBase::DeleteSlotData(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel)
{
	if (!IsValid(LoadSlotSwitcherViewModel)) { return false; }

	// 要保持的存档已经存在,则删除
	if (UGameplayStatics::DoesSaveGameExist(LoadSlotSwitcherViewModel->GetLoadSlotName(),
	                                        LoadSlotSwitcherViewModel->GetLoadSlotInSave()))
	{
		return UGameplayStatics::DeleteGameInSlot(LoadSlotSwitcherViewModel->GetLoadSlotName(),
		                                          LoadSlotSwitcherViewModel->GetLoadSlotInSave());
	}

	return false;
}

void ARPGAuraGameModeBase::OpenLevel(const FString& LevelName)
{
	const auto Level = LevelMaps.Find(LevelName);
	if (!Level)
	{
		UE_LOG(ARPGAuraGameModeBaseLog, Warning, TEXT("[%s]无法找到关卡!"), *FString::Printf(TEXT(__FUNCTION__)));
		return;
	}

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, *Level);
}

ULoadScreenSave* ARPGAuraGameModeBase::GetCurrentInGameSaveData()
{
	if (!GetMyGameInstance()) { return nullptr; }

	return GetLoadSlotData(GetMyGameInstance()->LoadSlotName, GetMyGameInstance()->LoadSlotIndex);
}

void ARPGAuraGameModeBase::CharacterDie(ACharacter* DeadCharacter)
{
	// 不是玩家的话不执行
	const auto Player = Cast<AAuraCharacter>(DeadCharacter);
	if (!Player) { return; }

	const auto CurrentLoadData = GetCurrentInGameSaveData();
	if (!CurrentLoadData)
	{
		UE_LOG(ARPGAuraGameModeBaseLog, Error, TEXT("[%s]无法获取当前关卡存档!"), TEXT(__FUNCTION__));
		return;
	}
	UGameplayStatics::OpenLevel(DeadCharacter, FName(CurrentLoadData->MapAssetName));
}

FString ARPGAuraGameModeBase::GetLevelMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Pair : LevelMaps)
	{
		if (Pair.Value.ToSoftObjectPath().GetAssetName() == MapAssetName) { return Pair.Key; }
	}
	UE_LOG(ARPGAuraGameModeBaseLog, Warning, TEXT("[%s]没有从配置里找到名为:\"%s\"的地图"), TEXT(__FUNCTION__), *MapAssetName);
	return FString{};
}

FString ARPGAuraGameModeBase::GetLevelMapAssetNameByString(const FString& KeyStr) const
{
	for ( auto& Pair : LevelMaps)
	{
		if(Pair.Key == KeyStr)
		{
			return Pair.Value.ToSoftObjectPath().GetAssetName();
		}
	}
	UE_LOG(ARPGAuraGameModeBaseLog, Warning, TEXT("[%s]没有从配置里找到名为:\"%s\"所对应的地图名字!"), TEXT(__FUNCTION__), *KeyStr);
	return FString{};
}
