#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemArmor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemArmor.h"
#include "Item.h"
#include "ItemStack.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "Random.h"

static int _ItemArmor_s_damageReduceAmmountArray_data[] = { 3, 8, 6, 3 };
JArray<int > ItemArmor::s_damageReduceAmmountArray(_ItemArmor_s_damageReduceAmmountArray_data, 4);
static int _ItemArmor_s_maxDamageArray_data[] = { 11, 16, 15, 13 };
JArray<int > ItemArmor::s_maxDamageArray(_ItemArmor_s_maxDamageArray_data, 4);

ItemArmor::ItemArmor(int var1, int var2, int var3, int var4)
	: Item(var1)
	, armorLevel(0)
	, armorType(0)
	, damageReduceAmmount(0)
	, renderIndex(0)
{
	this->armorLevel = var2;
	this->armorType = var4;
	this->renderIndex = var3;
	this->damageReduceAmmount = s_damageReduceAmmountArray[var4];
	this->maxDamage = (s_maxDamageArray[var4] * 3) << var2;
	this->maxStackSize = 1;
}

ItemArmor::~ItemArmor()
{
}

void ItemArmor::initStatics()
{
}
