#include "stdafx.h"
// Auto-generated from BlockLightStone.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockLightStone.h"
#include "Common.h"
#include "Material.h"
#include "Random.h"
#include "Block.h"
#include "Item.h"

BlockLightStone::BlockLightStone()
{
}

BlockLightStone::BlockLightStone(int var1, int var2, Material* var3)
	: Block(var1, var2, var3)
{
}

BlockLightStone::~BlockLightStone() {
	// No dynamic resources
}

int BlockLightStone::idDropped(int var1, Random* var2) {
		return Item::s_lightStoneDust->shiftedIndex;
}
