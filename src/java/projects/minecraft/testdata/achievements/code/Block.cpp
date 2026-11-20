#include "stdafx.h"
#include "Block.h"

void Block::init()
{
	Block::s_stone = (new BlockStone(1, 1))->setHardness(1.5F);
	Block::s_wood = (new Block(5, 4, Material::s_wood))->setHardness(2.0F);
	Block::s_workbench = (new BlockWorkbench(58))->setHardness(2.5F);
}
