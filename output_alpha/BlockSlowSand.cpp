#include "stdafx.h"
// Auto-generated from BlockSlowSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSlowSand.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockSlowSand::BlockSlowSand()
{
}

BlockSlowSand::BlockSlowSand(int var1, int var2)
	: Block(var1, var2, Material::s_sand)
{
}

BlockSlowSand::~BlockSlowSand() {
	// No dynamic resources
}

AxisAlignedBB BlockSlowSand::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		float var5 = 2.0F / 16.0F;
		return AxisAlignedBB::getBoundingBoxFromPool((double)var2, (double)var3, (double)var4, (double)(var2 + 1), (double)((float)(var3 + 1) - var5), (double)(var4 + 1));
}

void BlockSlowSand::onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5) {
		var5->motionX *= 0.4f;
		var5->motionZ *= 0.4f;
}
