#include "stdafx.h"
// Auto-generated from BlockStone.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockStone.h"
#include "Common.h"
#include "Block.h"
#include "Material.h"

BlockStone::BlockStone()
{
}

BlockStone::BlockStone(int var1, int var2)
	: Block(var1, var2, Material::s_rock)
{
}

BlockStone::~BlockStone() {
	// No dynamic resources
}

int BlockStone::idDropped(int var1) {
		return Block::s_cobblestone->blockID;
}
