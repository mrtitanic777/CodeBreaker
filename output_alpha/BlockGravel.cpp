#include "stdafx.h"
// Auto-generated from BlockGravel.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockGravel.h"
#include "Common.h"
#include "Random.h"
#include "Item.h"

BlockGravel::BlockGravel()
{
}

BlockGravel::BlockGravel(int var1, int var2)
	: BlockSand(var1, var2)
{
}

BlockGravel::~BlockGravel() {
	// No dynamic resources
}

int BlockGravel::idDropped(int var1, Random* var2) {
		return var2->nextInt(10) == 0 ? Item::s_flint->shiftedIndex : blockID;
}
