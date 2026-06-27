#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from CraftingRecipe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CraftingRecipe.h"
#include "Item.h"
#include "ItemStack.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "Random.h"

CraftingRecipe::CraftingRecipe(int var1, int var2, JArray<int > var3, ItemStack* var4)
	: width(0)
	, height(0)
	, resultStack(NULL)
	, resultId(0)
{
	this->resultId = var4->itemID;
	this->width = var1;
	this->height = var2;
	this->ingredientMap = var3;
	this->resultStack = var4;
}

CraftingRecipe::~CraftingRecipe()
{
}

bool CraftingRecipe::matchRecipe(JArray<int > var1)
{
	for(int var2 = 0; var2 <= 3 - this->width; ++var2) {
	for(int var3 = 0; var3 <= 3 - this->height; ++var3) {
	if(this->matches(var1, var2, var3, true)) {
	return true;
	}
	if(this->matches(var1, var2, var3, false)) {
	return true;
	}
	}
	}
	return false;
}

bool CraftingRecipe::matches(JArray<int > var1, int var2, int var3, bool var4)
{
	for(int var5 = 0; var5 < 3; ++var5) {
	for(int var6 = 0; var6 < 3; ++var6) {
	int var7 = var5 - var2;
	int var8 = var6 - var3;
	int var9 = -1;
	if(var7 >= 0 && var8 >= 0 && var7 < this->width && var8 < this->height) {
	if(var4) {
	var9 = this->ingredientMap[this->width - var7 - 1 + var8 * this->width];
	} else {
	var9 = this->ingredientMap[var7 + var8 * this->width];
	}
	}
	if(var1[var5 + var6 * 3] != var9) {
	return false;
	}
	}
	}
	return true;
}

ItemStack* CraftingRecipe::createResult(JArray<int > var1)
{
	return new ItemStack(this->resultStack->itemID, this->resultStack->stackSize);
}

int CraftingRecipe::getRecipeSize()
{
	return this->width * this->height;
}

void CraftingRecipe::initStatics()
{
}
