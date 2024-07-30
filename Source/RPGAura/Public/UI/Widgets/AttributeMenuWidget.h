// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/BaseUserWidget.h"
#include "AttributeMenuWidget.generated.h"

enum class EGameplayTagType : uint8;
class UVerticalBox;
/**
 * 属性菜单Widget
 */
UCLASS()
class RPGAURA_API UAttributeMenuWidget : public UBaseUserWidget
{
    GENERATED_BODY()

public:
    /// 初始化垂直框区域,注意需要在控制器设置之后运行
    UFUNCTION(BlueprintCallable)
    void InitVerticalArea();

    /// 根据BaseUserWidget有的GT查找当前AttriMenu的Widget
    /// 每一个显示属性的Widget都不重复,因此找到的Widget是唯一的,不会有多个
    /// @param GameplayTagType 标签类型(重要、主要、次要)
    /// @param Tag GameplayTag
    /// @return BaseUserWidget,找不到则为nullptr
    UFUNCTION(BlueprintCallable)
    UBaseUserWidget* GetWidgetByGameplayTag(EGameplayTagType GameplayTagType, FGameplayTag Tag);

protected:
    /// 可加属性点数区域,该区域只有一行内容
    UPROPERTY(meta=(BindWidget))
    UVerticalBox* CurrentAttributePointArea;

    UPROPERTY(meta=(BindWidget))
    UVerticalBox* VerticalBox_PrimaryAttriArea;


    UPROPERTY(meta=(BindWidget))
    UVerticalBox* VerticalBox_SecondaryAttriArea;

    /// 用于生成TextValueRowButtonWidget的类
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AttriMenu|WdigetClass")
    TSubclassOf<UBaseUserWidget> TextValueRowButtonWidgetClass;

    /// 用于生成 TextValueRowWidget的类
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AttriMenu|WdigetClass")
    TSubclassOf<UBaseUserWidget> TextValueRowWidgetClass;

    /// 保存属性面板中显示Vital属性的Widget指针
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AttriMenu|VerticalAreaElement")
    TArray<UBaseUserWidget*> VitalAttriAreaWidgetArray{};

    /// 保存属性面板中显示Primary属性的Widget指针
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AttriMenu|VerticalAreaElement")
    TArray<UBaseUserWidget*> PrimaryAttriAreaWidgetArray{};

    /// 保存属性面板中显示Secondary属性的Widget指针
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AttriMenu|VerticalAreaElement")
    TArray<UBaseUserWidget*> SecondaryAttriAreaWidgetArray{};

    /// 保存属性面板中所有通过CreateVerticalAreaWidgets函数创建的widget指针(该数组包括Vital、Primary、Secondary等widget数组所拥有的内容)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AttriMenu|VerticalAreaElement")
    TArray<UBaseUserWidget*> AllAttriAreaWidgetArray{};

private:
    UPROPERTY()
    UBaseUserWidget* CurrentAttributePointRow = nullptr;

    /// 根据标签查找当前属性面板中的Widget (每一个BaseUserWidget都有一个标签)
    /// @param Array 标签数组指针
    /// @param Tag 标签
    /// @return 对应的widget,可能为nullptr
    UBaseUserWidget* FindWidgetByTagFromArray(const TArray<UBaseUserWidget*>& Array, const FGameplayTag& Tag);

    /// 创建属性面板的垂直区域里的widget
    /// @param Array 标签数组指针
    /// @param GameplayTagType 标签类型
    /// @param VerticalArea 垂直区域
    void CreateVerticalAreaWidgets(const TArray<FGameplayTag*>* Array,
                                   const EGameplayTagType GameplayTagType,
                                   UVerticalBox* VerticalArea);
};