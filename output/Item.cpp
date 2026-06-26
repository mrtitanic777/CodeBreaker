#include "stdafx.h"
// Auto-generated from Item::s_java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"
#include "Common.h"
#include "Block.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "Random.h"
#include "World.h"
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
#include "ItemSword.h"

Random* Item::s_rand = new Random();
Item** Item::itemsList = new Item*[1024]();
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
{
}

Item::Item(int var1)
	: shiftedIndex(0)
	, maxStackSize(64)
	, maxDamage(32)
	, iconIndex(0)
{
		shiftedIndex = var1 + 256;
		itemsList[var1 + 256] = this;
}

Item::~Item() {
	// No dynamic resources
}

Item* Item::setIconIndex(int var1) {
		iconIndex = var1;
		return this;
}

int Item::getIconIndex() {
		return iconIndex;
}

bool Item::onItemUse(ItemStack* var1, World* var2, int var3, int var4, int var5, int var6) {
		return false;
}

float Item::getStrVsBlock(Block* var1) {
		return 1.0F;
}

ItemStack* Item::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3) {
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

void Item::hitEntity(ItemStack* var1) {
	// TODO: implement
}

void Item::onBlockDestroyed(ItemStack* var1) {
	// TODO: implement
}

int Item::getDamageVsEntity() {
		return 1;
}

bool Item::canHarvestBlock(Block* var1) {
		return false;
}

void Item::initStatics() {
		ItemSpade* var10000 = new ItemSpade(0, 2);
		unsigned char var1 = 82;
		ItemSpade* var0 = var10000;
		var0->iconIndex = var1;
		s_shovel = var0;
		ItemPickaxe* var14 = new ItemPickaxe(1, 2);
		var1 = 98;
		ItemPickaxe* var2 = var14;
		var2->iconIndex = var1;
		s_pickaxeSteel = var2;
		ItemAxe* var15 = new ItemAxe(2, 2);
		var1 = 114;
		ItemAxe* var3 = var15;
		var3->iconIndex = var1;
		s_axeSteel = var3;
		ItemFlintAndSteel* var16 = new ItemFlintAndSteel(3);
		var1 = 5;
		ItemFlintAndSteel* var4 = var16;
		var4->iconIndex = var1;
		s_striker = var4;
		ItemFood* var17 = new ItemFood(4, 4);
		var1 = 4;
		ItemFood* var5 = var17;
		var5->iconIndex = var1;
		ItemBow* var18 = new ItemBow(5);
		var1 = 21;
		ItemBow* var6 = var18;
		var6->iconIndex = var1;
		s_bow = var6;
		Item* var19 = new Item(6);
		var1 = 37;
		Item* var7 = var19;
		var7->iconIndex = var1;
		s_arrow = var7;
		var19 = new Item(7);
		var1 = 7;
		var7 = var19;
		var7->iconIndex = var1;
		s_coal = var7;
		var19 = new Item(8);
		var1 = 55;
		var7 = var19;
		var7->iconIndex = var1;
		s_diamond = var7;
		var19 = new Item(9);
		var1 = 23;
		var7 = var19;
		var7->iconIndex = var1;
		s_ingotIron = var7;
		var19 = new Item(10);
		var1 = 39;
		var7 = var19;
		var7->iconIndex = var1;
		s_ingotGold = var7;
		ItemSword* var20 = new ItemSword(11, 2);
		var1 = 66;
		ItemSword* var8 = var20;
		var8->iconIndex = var1;
		s_swordSteel = var8;
		var20 = new ItemSword(12, 0);
		var1 = 64;
		var8 = var20;
		var8->iconIndex = var1;
		s_swordWood = var8;
		var10000 = new ItemSpade(13, 0);
		var1 = 80;
		var0 = var10000;
		var0->iconIndex = var1;
		s_pickaxeWood = var0;
		var14 = new ItemPickaxe(14, 0);
		var1 = 96;
		var2 = var14;
		var2->iconIndex = var1;
		s_axeWood = var2;
		var15 = new ItemAxe(15, 0);
		var1 = 112;
		var3 = var15;
		var3->iconIndex = var1;
		s_swordStone = var3;
		var20 = new ItemSword(16, 1);
		var1 = 65;
		var8 = var20;
		var8->iconIndex = var1;
		s_shovelStone = var8;
		var10000 = new ItemSpade(17, 1);
		var1 = 81;
		var0 = var10000;
		var0->iconIndex = var1;
		s_pickaxeStone = var0;
		var14 = new ItemPickaxe(18, 1);
		var1 = 97;
		var2 = var14;
		var2->iconIndex = var1;
		s_axeStone = var2;
		var15 = new ItemAxe(19, 1);
		var1 = 113;
		var3 = var15;
		var3->iconIndex = var1;
		s_swordDiamond = var3;
		var20 = new ItemSword(20, 3);
		var1 = 67;
		var8 = var20;
		var8->iconIndex = var1;
		s_shovelDiamond = var8;
		var10000 = new ItemSpade(21, 3);
		var1 = 83;
		var0 = var10000;
		var0->iconIndex = var1;
		s_pickaxeDiamond = var0;
		var14 = new ItemPickaxe(22, 3);
		var1 = 99;
		var2 = var14;
		var2->iconIndex = var1;
		s_axeDiamond = var2;
		var15 = new ItemAxe(23, 3);
		var1 = 115;
		var3 = var15;
		var3->iconIndex = var1;
		s_stick = var3;
		var19 = new Item(24);
		var1 = 53;
		var7 = var19;
		var7->iconIndex = var1;
		s_bowlEmpty = var7;
		var19 = new Item(25);
		var1 = 71;
		var7 = var19;
		var7->iconIndex = var1;
		s_bowlSoup = var7;
		ItemSoup* var21 = new ItemSoup(26, 10);
		var1 = 72;
		ItemSoup* var9 = var21;
		var9->iconIndex = var1;
		s_swordGold = var9;
		var20 = new ItemSword(27, 0);
		var1 = 68;
		var8 = var20;
		var8->iconIndex = var1;
		s_shovelGold = var8;
		var10000 = new ItemSpade(28, 0);
		var1 = 84;
		var0 = var10000;
		var0->iconIndex = var1;
		s_pickaxeGold = var0;
		var14 = new ItemPickaxe(29, 0);
		var1 = 100;
		var2 = var14;
		var2->iconIndex = var1;
		s_axeGold = var2;
		var15 = new ItemAxe(30, 0);
		var1 = 116;
		var3 = var15;
		var3->iconIndex = var1;
		s_silk = var3;
		var19 = new Item(31);
		var1 = 8;
		var7 = var19;
		var7->iconIndex = var1;
		s_feather = var7;
		var19 = new Item(32);
		var1 = 24;
		var7 = var19;
		var7->iconIndex = var1;
		s_gunpowder = var7;
		var19 = new Item(33);
		var1 = 40;
		var7 = var19;
		var7->iconIndex = var1;
		s_hoeWood = var7;
		ItemHoe* var22 = new ItemHoe(34, 0);
		short var11 = 128;
		ItemHoe* var10 = var22;
		var10->iconIndex = var11;
		s_hoeStone = var10;
		var22 = new ItemHoe(35, 1);
		var11 = 129;
		var10 = var22;
		var10->iconIndex = var11;
		s_hoeSteel = var10;
		var22 = new ItemHoe(36, 2);
		var11 = 130;
		var10 = var22;
		var10->iconIndex = var11;
		s_hoeDiamond = var10;
		var22 = new ItemHoe(37, 3);
		var11 = 131;
		var10 = var22;
		var10->iconIndex = var11;
		s_hoeGold = var10;
		var22 = new ItemHoe(38, 4);
		var11 = 132;
		var10 = var22;
		var10->iconIndex = var11;
		s_seeds = var10;
		ItemSeeds* var23 = new ItemSeeds(39, Block::s_crops->blockID);
		var1 = 9;
		ItemSeeds* var12 = var23;
		var12->iconIndex = var1;
		s_wheat = var12;
		var19 = new Item(40);
		var1 = 25;
		var7 = var19;
		var7->iconIndex = var1;
		s_bread = var7;
		var17 = new ItemFood(41, 5);
		var1 = 41;
		var5 = var17;
		var5->iconIndex = var1;
		s_helmetLeather = var5;
		ItemArmor* var24 = new ItemArmor(42, 0, 0);
		var1 = 0;
		ItemArmor* var13 = var24;
		var13->iconIndex = var1;
		s_plateLeather = var13;
		var24 = new ItemArmor(43, 0, 1);
		var1 = 16;
		var13 = var24;
		var13->iconIndex = var1;
		s_legsLeather = var13;
		var24 = new ItemArmor(44, 0, 2);
		var1 = 32;
		var13 = var24;
		var13->iconIndex = var1;
		s_bootsLeather = var13;
		var24 = new ItemArmor(45, 0, 3);
		var1 = 48;
		var13 = var24;
		var13->iconIndex = var1;
		s_helmetChain = var13;
		var24 = new ItemArmor(46, 1, 0);
		var1 = 1;
		var13 = var24;
		var13->iconIndex = var1;
		s_plateChain = var13;
		var24 = new ItemArmor(47, 1, 1);
		var1 = 17;
		var13 = var24;
		var13->iconIndex = var1;
		s_legsChain = var13;
		var24 = new ItemArmor(48, 1, 2);
		var1 = 33;
		var13 = var24;
		var13->iconIndex = var1;
		s_bootsChain = var13;
		var24 = new ItemArmor(49, 1, 3);
		var1 = 49;
		var13 = var24;
		var13->iconIndex = var1;
		s_helmetSteel = var13;
		var24 = new ItemArmor(50, 2, 0);
		var1 = 2;
		var13 = var24;
		var13->iconIndex = var1;
		s_plateSteel = var13;
		var24 = new ItemArmor(51, 2, 1);
		var1 = 18;
		var13 = var24;
		var13->iconIndex = var1;
		s_legsSteel = var13;
		var24 = new ItemArmor(52, 2, 2);
		var1 = 34;
		var13 = var24;
		var13->iconIndex = var1;
		s_bootsSteel = var13;
		var24 = new ItemArmor(53, 2, 3);
		var1 = 50;
		var13 = var24;
		var13->iconIndex = var1;
		s_helmetDiamond = var13;
		var24 = new ItemArmor(54, 3, 0);
		var1 = 3;
		var13 = var24;
		var13->iconIndex = var1;
		s_plateDiamond = var13;
		var24 = new ItemArmor(55, 3, 1);
		var1 = 19;
		var13 = var24;
		var13->iconIndex = var1;
		s_legsDiamond = var13;
		var24 = new ItemArmor(56, 3, 2);
		var1 = 35;
		var13 = var24;
		var13->iconIndex = var1;
		s_bootsDiamond = var13;
		var24 = new ItemArmor(57, 3, 3);
		var1 = 51;
		var13 = var24;
		var13->iconIndex = var1;
		s_helmetGold = var13;
		var24 = new ItemArmor(58, 1, 0);
		var1 = 4;
		var13 = var24;
		var13->iconIndex = var1;
		s_plateGold = var13;
		var24 = new ItemArmor(59, 1, 1);
		var1 = 20;
		var13 = var24;
		var13->iconIndex = var1;
		s_legsGold = var13;
		var24 = new ItemArmor(60, 1, 2);
		var1 = 36;
		var13 = var24;
		var13->iconIndex = var1;
		s_bootsGold = var13;
		var24 = new ItemArmor(61, 1, 3);
		var1 = 52;
		var13 = var24;
		var13->iconIndex = var1;
		s_flint = var13;
}
