#include "stdafx.h"
// Auto-generated from BlockOreBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockOreBlock.h"
#include "Common.h"
#include "Block.h"
#include "Material.h"

BlockOreBlock::BlockOreBlock()
{
}

BlockOreBlock::BlockOreBlock(int var1, int var2)
	: Block(var1, Material::s_iron)
{
		blockIndexInTexture = var2;
}

BlockOreBlock::~BlockOreBlock() {
	// No dynamic resources
}

int BlockOreBlock::getBlockTextureFromSide(int var1) {
		return var1 == 1 ? blockIndexInTexture - 16 : (var1 == 0 ? blockIndexInTexture + 16 : blockIndexInTexture);
}
