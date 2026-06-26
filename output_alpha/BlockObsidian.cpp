#include "stdafx.h"
// Auto-generated from BlockObsidian.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockObsidian.h"
#include "Common.h"
#include "Random.h"
#include "Block.h"

BlockObsidian::BlockObsidian()
{
}

BlockObsidian::BlockObsidian(int var1, int var2)
	: BlockStone(var1, var2)
{
}

BlockObsidian::~BlockObsidian() {
	// No dynamic resources
}

int BlockObsidian::quantityDropped(Random* var1) {
		return 1;
}

int BlockObsidian::idDropped(int var1, Random* var2) {
		return Block::s_obsidian->blockID;
}
