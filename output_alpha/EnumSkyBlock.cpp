#include "stdafx.h"
#include "EnumSkyBlock.h"

EnumSkyBlock* EnumSkyBlock::s_Sky = NULL;
EnumSkyBlock* EnumSkyBlock::s_Block = NULL;
EnumSkyBlock* EnumSkyBlock::values_[2] = {0};
const int EnumSkyBlock::values_count = 2;

EnumSkyBlock::EnumSkyBlock() : ordinal(0) {}

EnumSkyBlock::EnumSkyBlock(int var3) : ordinal(0)
{
	field_1722_c = var3;
}

void EnumSkyBlock::initStatics()
{
	s_Sky = new EnumSkyBlock(15);
	s_Sky->ordinal = 0; s_Sky->nameStr = "Sky"; values_[0] = s_Sky;
	s_Block = new EnumSkyBlock(0);
	s_Block->ordinal = 1; s_Block->nameStr = "Block"; values_[1] = s_Block;
}
