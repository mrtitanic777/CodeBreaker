#include "stdafx.h"
// Auto-generated from BlockBloodStone.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockBloodStone.h"
#include "Common.h"
#include "Block.h"
#include "Material.h"

BlockBloodStone::BlockBloodStone()
{
}

BlockBloodStone::BlockBloodStone(int var1, int var2)
	: Block(var1, var2, Material::s_rock)
{
}

BlockBloodStone::~BlockBloodStone() {
	// No dynamic resources
}
