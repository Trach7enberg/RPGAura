// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RPGAuraCoreTypes.generated.h"

class UBaseUserWidget;
class UAttributeSet;
class UAbilitySystemComponent;

/// ---BaseWidgetController类 使用
/// Widget控制器需要的基本参数的结构体
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
    GENERATED_BODY()

    FWidgetControllerParams() {}

    FWidgetControllerParams(APlayerController* Pc,
                            APlayerState* Ps,
                            UAbilitySystemComponent* Asc,
                            UAttributeSet* As) :
        CurrentPlayerController(Pc), CurrentPlayerState(Ps),
        CurrentAbilitySystemComponent(Asc), CurrentAttributeSet(As) {}

    UPROPERTY(BlueprintReadOnly, Category="WidgetController")
    TObjectPtr<APlayerController> CurrentPlayerController = nullptr;

    UPROPERTY(BlueprintReadOnly, Category="WidgetController")
    TObjectPtr<APlayerState> CurrentPlayerState = nullptr;

    UPROPERTY(BlueprintReadOnly, Category="WidgetController")
    TObjectPtr<UAbilitySystemComponent> CurrentAbilitySystemComponent = nullptr;

    UPROPERTY(BlueprintReadOnly, Category="WidgetController")
    TObjectPtr<UAttributeSet> CurrentAttributeSet = nullptr;
};

/// ---BaseWidgetController类 使用

/// ---UAttributeInfo类使用
/// 用于广播GAS属性的结构体
USTRUCT(BlueprintType)
struct FRPGAuraAttributeInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag AttributeTag = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText AttributeName = FText();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText AttributeDescription = FText();

    UPROPERTY(BlueprintReadOnly)
    float AttributeValue = 0.f;
};

/// ---UAttributeInfo类使用

/// ---MainWidgetController使用
/// 用于显示Message Widget的结构体,当角色拾取到什么东西时就会弹出
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
    GENERATED_BODY()

    // 要显示的标签
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FGameplayTag Tag = FGameplayTag();

    // 要显示的消息
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FText Message = FText();

    // 要显示的UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UBaseUserWidget> MessageWidget;

    // 要显示的图片
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UTexture2D* Image = nullptr;
};

/// 给生命值、魔力值属性变化时的委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVitalAttributeChangedSignature,
                                             float,
                                             NewValue,
                                             bool,
                                             /* NewValue增加还是减少,增加则为true */
                                             BIsIncreased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowSignature, const FUIWidgetRow, Row);

/// ---MainWidgetController使用

/// ---UAttributeMenuWidgetController类使用
/// 用于广播GAS属性变化的结构体,更新属性菜单的属性值显示 (包括vital、primary、secondary属性)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, const FRPGAuraAttributeInfo, Info);


/// ---FRPGAuraGameplayTags结构类使用
UENUM(BlueprintType)
enum class EGameplayTagType:uint8
{
    AllGameplayTags UMETA(DisplayName = "全部标签(All)"),
    VitalGameplayTags UMETA(DisplayName = "重要标签(Vital)"),
    PrimaryGameplayTags UMETA(DisplayName = "主要标签(Primary)"),
    SecondaryGameplayTags UMETA(DisplayName = "次要标签(Secondary)")
};

/// --- BaseA