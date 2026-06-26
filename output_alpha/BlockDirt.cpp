#include "stdafx.h"
// Auto-generated from BlockDirt.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockDirt.h"
#include "Common.h"
#include "Block.h"
#include "Material.h"

BlockDirt::BlockDirt()
{
}

BlockDirt::BlockDirt(int var1, int var2)
	: Block(var1, var2, Material::s_ground)
{
}

BlockDirt::~BlockDirt() {
	// No dynamic resources
}
