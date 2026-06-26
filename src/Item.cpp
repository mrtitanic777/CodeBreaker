#include "stdafx.h"
// Auto-generated from Item.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"
#include "Common.h"
#include "Block.h"
#include "EntityPlayer.h"
#include "ItemArmor.h"
#include "ItemAxe.h"
#include "ItemBow.h"
#include "ItemFlintAndSteel.h"
#include "ItemFood.h"
#include "ItemHoe.h"
#include "ItemPickaxe.h"
#include "ItemSeeds.h"
#include "ItemSoup.h"
#include "ItemSpade.h"
#include "ItemStack.h"
#include "ItemSword.h"
#include "Random.h"
#include "World.h"

Random* Item::s_rand = new Random();
Item* Item::itemsList = new Item[1024];
Item* Item::s_shovel;
Item* Item::s_pickaxeSteel;
Item* Item::s_axeSteel;
Item* Item::s_striker;
Item* Item::s_bow;
Item* Item::s_arrow;
Item* Item::s_coal;
Item* Item::s_diamond;
Item* Item::s_ingotIron;
Item* Item::s_ingotGold;
Item* Item::s_swordSteel;
Item* Item::s_swordWood;
Item* Item::s_pickaxeWood;
Item* Item::s_axeWood;
Item* Item::s_swordStone;
Item* Item::s_shovelStone;
Item* Item::s_pickaxeStone;
Item* Item::s_axeStone;
Item* Item::s_swordDiamond;
Item* Item::s_shovelDiamond;
Item* Item::s_pickaxeDiamond;
Item* Item::s_axeDiamond;
Item* Item::s_stick;
Item* Item::s_bowlEmpty;
Item* Item::s_bowlSoup;
Item* Item::s_swordGold;
Item* Item::s_shovelGold;
Item* Item::s_pickaxeGold;
Item* Item::s_axeGold;
Item* Item::s_silk;
Item* Item::s_feather;
Item* Item::s_gunpowder;
Item* Item::s_hoeWood;
Item* Item::s_hoeStone;
Item* Item::s_hoeSteel;
Item* Item::s_hoeDiamond;
Item* Item::s_hoeGold;
Item* Item::s_seeds;
Item* Item::s_wheat;
Item* Item::s_bread;
Item* Item::s_helmetLeather;
Item* Item::s_plateLeather;
Item* Item::s_legsLeather;
Item* Item::s_bootsLeather;
Item* Item::s_helmetChain;
Item* Item::s_plateChain;
Item* Item::s_legsChain;
Item* Item::s_bootsChain;
Item* Item::s_helmetSteel;
Item* Item::s_plateSteel;
Item* Item::s_legsSteel;
Item* Item::s_bootsSteel;
Item* Item::s_helmetDiamond;
Item* Item::s_plateDiamond;
Item* Item::s_legsDiamond;
Item* Item::s_bootsDiamond;
Item* Item::s_helmetGold;
Item* Item::s_plateGold;
Item* Item::s_legsGold;
Item* Item::s_bootsGold;
Item* Item::s_flint;

Item::Item()
	: shiftedIndex(0)
	, maxStackSize(64)
	, maxDamage(32)
	, iconIndex(0)
	, m_var10000(new ItemSpade(0, 2))
	, m_var1(82)
	, m_var0(var10000)
	, m_var14(new ItemPickaxe(1, 2))
	, m_var2(var14)
	, m_var15(new ItemAxe(2, 2))
	, m_var3(var15)
	, m_var16(new ItemFlintAndSteel(3))
	, m_var4(var16)
	, m_var17(new ItemFood(4, 4))
	, m_var5(var17)
	, m_var18(new ItemBow(5))
	, m_var6(var18)
	, m_var19(new Item(6))
	, m_var7(var19)
	, m_var20(new ItemSword(11, 2))
	, m_var8(var20)
	, m_var21(new ItemSoup(26, 10))
	, m_var9(var21)
	, m_var22(new ItemHoe(34, 0))
	, m_var11(128)
	, m_var10(var22)
	, m_var23(new ItemSeeds(39, Block::s_crops->blockID))
	, m_var12(var23)
	, m_var24(new ItemArmor(42, 0, 0))
	, m_var13(var24)
{
}

Item::Item(int var1)
	: shiftedIndex(0)
	, maxStackSize(64)
	, maxDamage(32)
	, iconIndex(0)
	, m_var10000(new ItemSpade(0, 2))
	, m_var1(82)
	, m_var0(var10000)
	, m_var14(new ItemPickaxe(1, 2))
	, m_var2(var14)
	, m_var15(new ItemAxe(2, 2))
	, m_var3(var15)
	, m_var16(new ItemFlintAndSteel(3))
	, m_var4(var16)
	, m_var17(new ItemFood(4, 4))
	, m_var5(var17)
	, m_var18(new ItemBow(5))
	, m_var6(var18)
	, m_var19(new Item(6))
	, m_var7(var19)
	, m_var20(new ItemSword(11, 2))
	, m_var8(var20)
	, m_var21(new ItemSoup(26, 10))
	, m_var9(var21)
	, m_var22(new ItemHoe(34, 0))
	, m_var11(128)
	, m_var10(var22)
	, m_var23(new ItemSeeds(39, Block::s_crops->blockID))
	, m_var12(var23)
	, m_var24(new ItemArmor(42, 0, 0))
	, m_var13(var24)
{
		shiftedIndex = var1 + 256;
		itemsList[var1 + 256] = this;
}

Item::~Item() {
	m_var10000 = NULL;
	m_var0 = NULL;
	m_var14 = NULL;
	m_var2 = NULL;
	m_var15 = NULL;
	m_var3 = NULL;
	m_var16 = NULL;
	m_var4 = NULL;
	m_var17 = NULL;
	m_var5 = NULL;
	m_var18 = NULL;
	m_var6 = NULL;
	m_var19 = NULL;
	m_var7 = NULL;
	m_var20 = NULL;
	m_var8 = NULL;
	m_var21 = NULL;
	m_var9 = NULL;
	m_var22 = NULL;
	m_var10 = NULL;
	m_var23 = NULL;
	m_var12 = NULL;
	m_var24 = NULL;
	m_var13 = NULL;
}

Item* Item::setIconIndex(int var1) {
		iconIndex = var1;
		return this;
}

int Item::getIconIndex() {
		return iconIndex;
}

bool Item::onItemUse(const ItemStack& var1, World* var2, int var3, int var4, int var5, int var6) {
		return false;
}

float Item::getStrVsBlock(Block* var1) {
		return 1.0F;
}

ItemStack Item::onItemRightClick(const ItemStack& var1, World* var2, EntityPlayer* var3) {
		return var1;
}

int Item::getItemStackLimit() {
		return maxStackSize;
}

bool Item::onPlaced(World* var1, float var2, float var3, float var4) {
		return false;
}

int Item::getMaxDamage() {
		return maxDamage;
}

void Item::hitEntity(const ItemStack& var1) {
	// TODO: implement
}

void Item::onBlockDestroyed(const ItemStack& var1) {
	// TODO: implement
}

int Item::getDamageVsEntity() {
		return 1;
}

bool Item::canHarvestBlock(Block* var1) {
		return false;
}
