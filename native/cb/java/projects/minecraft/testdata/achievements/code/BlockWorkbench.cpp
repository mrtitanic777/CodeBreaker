#include "stdafx.h"
#include "BlockWorkbench.h"

BlockWorkbench::BlockWorkbench(int var1)
	: Block(var1, Material::s_wood)
{
	this->blockIndexInTexture = 59;
}
