#include "stdafx.h"
// Auto-generated from BlockClay.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockClay.h"
#include "Common.h"
#include "Random.h"
#include "Block.h"
#include "Item.h"
#include "Material.h"

BlockClay::BlockClay()
{
}

BlockClay::BlockClay(int var1, int var2)
	: Block(var1, var2, Material::s_clay)
{
}

BlockClay::~BlockClay() {
	// No dynamic resources
}

int BlockClay::idDropped(int var1, Random* var2) {
		return Item::s_clay->shiftedIndex;
}

int BlockClay::quantityDropped(Random* var1) {
		return 4;
}
