#include "stdafx.h"
// Auto-generated from BlockMobSpawner.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockMobSpawner.h"
#include "Common.h"
#include "Random.h"
#include "TileEntity.h"
#include "Material.h"
// PORT: removed spurious "MobSpawner.h" (no such class; nothing here uses it)
#include "TileEntityMobSpawner.h"

BlockMobSpawner::BlockMobSpawner()
{
}

BlockMobSpawner::BlockMobSpawner(int var1, int var2)
	: BlockContainer(var1, var2, Material::s_rock)
{
}

BlockMobSpawner::~BlockMobSpawner() {
	// No dynamic resources
}

TileEntity* BlockMobSpawner::SetBlockEntity() {
		return new TileEntityMobSpawner();
}

int BlockMobSpawner::idDropped(int var1, Random* var2) {
		return 0;
}

int BlockMobSpawner::quantityDropped(Random* var1) {
		return 0;
}

bool BlockMobSpawner::isOpaqueCube() {
		return false;
}
