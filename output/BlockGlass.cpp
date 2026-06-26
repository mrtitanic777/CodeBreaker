#include "stdafx.h"
// Auto-generated from BlockGlass.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockGlass.h"
#include "Common.h"
#include "Material.h"
#include "Random.h"

BlockGlass::BlockGlass()
{
}

BlockGlass::BlockGlass(int var1, int var2, Material* var3, bool var4)
	: BlockBreakable(20, 49, var3, false)
{
}

BlockGlass::~BlockGlass() {
	// No dynamic resources
}

int BlockGlass::quantityDropped(Random* var1) {
		return 0;
}
