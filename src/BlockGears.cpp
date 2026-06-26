#include "stdafx.h"
// Auto-generated from BlockGears.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockGears.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Random.h"
#include "Block.h"
#include "Material.h"

BlockGears::BlockGears()
{
}

BlockGears::BlockGears(int var1, int var2)
	: Block(55, 62, Material.circuits)
{
}

BlockGears::~BlockGears() {
	// No dynamic resources
}

AxisAlignedBB BlockGears::getCollisionBoundingBoxFromPool(int var1, int var2, int var3) {
		return AxisAlignedBB();
}

bool BlockGears::isOpaqueCube() {
		return false;
}

bool BlockGears::renderAsNormalBlock() {
		return false;
}

int BlockGears::getRenderType() {
		return 5;
}

int BlockGears::quantityDropped(Random* var1) {
		return 1;
}

bool BlockGears::isCollidable() {
		return false;
}
