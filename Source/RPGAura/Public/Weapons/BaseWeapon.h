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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	/// 当前武器能否被高亮
	/// @return 可以则是true
	virtual bool CanHighLight();
};
