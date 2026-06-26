#include "stdafx.h"
// Auto-generated from BlockBookshelf.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockBookshelf.h"
#include "Common.h"
#include "Random.h"
#include "Block.h"
#include "Material.h"

BlockBookshelf::BlockBookshelf()
{
}

BlockBookshelf::BlockBookshelf(int var1, int var2)
	: Block(47, 35, Material::s_wood)
{
}

BlockBookshelf::~BlockBookshelf() {
	// No dynamic resources
}

int BlockBookshelf::getBlockTextureFromSide(int var1) {
		return var1 <= 1 ? 4 : blockIndexInTexture;
}

int BlockBookshelf::quantityDropped(Random* var1) {
		return 0;
}
