// Copyright GGBAO 

#pragma once

/**
 *
 * 基础武器类
 */
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS(Abstract)
class RPGAURA_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();

	void SetWeaponMeshCollision(bool Enabled) const;
	virtual void HighLight();
	virtual void UnHighLight();

    /// 根据名字获取武器的插槽位置
    /// @param SocketName 
    /// @return 插槽位置
    FVector GetWeaponSocketLocByName(const FName& SocketName)const ;

	/// 设置武器的物理(模拟物理、重力、碰撞)
	/// @param Enable 是否开启物理
	void SetWeaponPhysics(bool Enable) const;

	// 设置武器的材质效果为溶解
	void SetWeaponMaterial(int I, UMaterialInstance* MaterialInstance) const;

	/// 设置武器的碰撞类型
	/// @param NewType 碰撞类型
	void SetCollisionEnabled(ECollisionEnabled::Type NewType) const;

	/// 获取武器骨骼的AnimInstance
	/// @return 
	UAnimInstance* GetWeaponAnimInstance() const;

	/// 给武器添加冲量
	/// @param Impulse 
	/// @param BoneName 
	/// @param bVelChange 
	void AddImpulse(const FVector& Impulse, FName BoneName = NAME_None, bool bVelChange = false) const;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	/// 当前武器能否被高亮
	/// @return 可以则是true
	virtual bool CanHighLight();
};
