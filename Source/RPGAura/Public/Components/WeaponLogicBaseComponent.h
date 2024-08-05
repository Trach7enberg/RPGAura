// Copyright GGBAO 

#pragma once

/**
 *	武器逻辑组件
 */

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponLogicBaseComponent.generated.h"


class ACharacterBase;
class ABaseWeapon;

/**
 * 控制角色武器的逻辑类
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPGAURA_API UWeaponLogicBaseComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponLogicBaseComponent();


    /// 高亮武器
    void HighLight() const;

    /// 解除高亮武器
    void UnHighLight() const;

    FORCEINLINE FName GetWeaponAttachSocketName() const
    {
        return WeaponAttachSocketName;
    }

    FORCEINLINE FName GetWeaponTipSocketName() const
    {
        return WeaponTipSocketName;
    }

    /// 根据名字获取武器插槽的位置
    /// @param SocketName 武器插槽名字
    /// @return 武器插槽的位置
    FVector GetWeaponSocketLocByName(const FName& SocketName) const;

protected:
    virtual void BeginPlay() override;

    /// 武器蓝图
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    TSubclassOf<ABaseWeapon> WeaponBp;

    /// 武器附着在角色部位的那个插槽名字
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    FName WeaponAttachSocketName = "WeaponHandSocket";

    /// 武器攻击部位的插槽名字(比如子弹、火球发射的位置
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    FName WeaponTipSocketName = "TipSocket";

private:
    /// 当前持有的武器
    UPROPERTY()
    ABaseWeapon* CurrentWeapon = nullptr;

    void AttachWeaponToSocket(ACharacterBase* Character, FName& SocketName);
};