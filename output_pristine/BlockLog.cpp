#include "stdafx.h"
// Auto-generated from BlockLog.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockLog.h"
#include "Common.h"
#include "Random.h"
#include "Block.h"
#include "Material.h"

BlockLog::BlockLog()
{
}

BlockLog::BlockLog(int var1)
	: Block(17, Material::s_wood)
{
		blockIndexInTexture = 20;
}

BlockLog::~BlockLog() {
	// No dynamic resources
}

int BlockLog::quantityDropped(Random* var1) {
		return var1->nextInt(3) + 3;
}

int BlockLog::idDropped(int var1) {
		return Block::s_planks->blockID;
}

int BlockLog::getBlockTextureFromSide(int var1) {
		return var1 == 1 ? 21 : (var1 == 0 ? 21 : 20);
}
