#include "stdafx.h"
// Auto-generated from BlockWorkbench.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockWorkbench.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockWorkbench::BlockWorkbench()
{
}

BlockWorkbench::BlockWorkbench(int var1)
	: Block(var1, Material::s_wood)
{
		blockIndexInTexture = 59;
}

BlockWorkbench::~BlockWorkbench() {
	// No dynamic resources
}

int BlockWorkbench::getBlockTextureFromSide(int var1) {
		return var1 == 1 ? blockIndexInTexture - 16 : (var1 == 0 ? Block::s_planks->getBlockTextureFromSide(0) : (var1 != 2 && var1 != 4 ? blockIndexInTexture : blockIndexInTexture + 1));
}

bool BlockWorkbench::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		var5->displayWorkbenchGUI();
		return true;
}
