#include "stdafx.h"
// Auto-generated from BlockOre.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockOre.h"
#include "Common.h"
#include "Random.h"
#include "Block.h"
#include "Item.h"
#include "Material.h"

BlockOre::BlockOre()
{
}

BlockOre::BlockOre(int var1, int var2)
	: Block(var1, var2, Material::s_rock)
{
}

BlockOre::~BlockOre() {
	// No dynamic resources
}

int BlockOre::idDropped(int var1, Random* var2) {
		return blockID == Block::s_oreCoal->blockID ? Item::s_coal->shiftedIndex : (blockID == Block::s_oreDiamond->blockID ? Item::s_diamond->shiftedIndex : blockID);
}

int BlockOre::quantityDropped(Random* var1) {
		return 1;
}
