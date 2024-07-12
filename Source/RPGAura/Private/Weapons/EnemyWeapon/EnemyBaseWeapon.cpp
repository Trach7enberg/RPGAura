// Copyright GGBAO 


#include "Weapons/EnemyWeapon/EnemyBaseWeapon.h"

#include "RPGAura/RPGAura.h"

void AEnemyBaseWeapon::HighLightActor()
{
	if (!WeaponMesh) { return; }
	WeaponMesh->SetRenderCustomDepth(true);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEnemyBaseWeapon::UnHighLightActor()
{
	if (!WeaponMesh) { return; }
	WeaponMesh->SetRenderCustomDepth(false);
}
