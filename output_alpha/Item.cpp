#include "stdafx.h"
// Auto-generated from Item::s_java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"
#include "Common.h"
#include "Block.h"
#include "Entity.h"
#include "EntityLiving.h"
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
#include "Material.h"
#include "ItemBoat.h"
#include "ItemBucket.h"
#include "ItemDoor.h"
#include "ItemFishingRod.h"
#include "ItemMinecart.h"
#include "ItemPainting.h"
#include "ItemRecord.h"
#include "ItemRedstone.h"
#include "ItemReed.h"
#include "ItemSaddle.h"
#include "ItemSign.h"
#include "ItemSnowball.h"

Random* Item::s_itemRand = new Random();
Item** Item::itemsList = new Item*[32000]();
Item* Item::s_shovelSteel = (new ItemSpade(0, 2))->setIconIndex(82);
Item* Item::s_pickaxeSteel = (new ItemPickaxe(1, 2))->setIconIndex(98);
Item* Item::s_axeSteel = (new ItemAxe(2, 2))->setIconIndex(114);
Item* Item::s_flintAndSteel = (new ItemFlintAndSteel(3))->setIconIndex(5);
Item* Item::s_appleRed = (new ItemFood(4, 4))->setIconIndex(10);
Item* Item::s_bow = (new ItemBow(5))->setIconIndex(21);
Item* Item::s_arrow = (new Item(6))->setIconIndex(37);
Item* Item::s_coal = (new Item(7))->setIconIndex(7);
Item* Item::s_diamond = (new Item(8))->setIconIndex(55);
Item* Item::s_ingotIron = (new Item(9))->setIconIndex(23);
Item* Item::s_ingotGold = (new Item(10))->setIconIndex(39);
Item* Item::s_swordSteel = (new ItemSword(11, 2))->setIconIndex(66);
Item* Item::s_swordWood = (new ItemSword(12, 0))->setIconIndex(64);
Item* Item::s_shovelWood = (new ItemSpade(13, 0))->setIconIndex(80);
Item* Item::s_pickaxeWood = (new ItemPickaxe(14, 0))->setIconIndex(96);
Item* Item::s_axeWood = (new ItemAxe(15, 0))->setIconIndex(112);
Item* Item::s_swordStone = (new ItemSword(16, 1))->setIconIndex(65);
Item* Item::s_shovelStone = (new ItemSpade(17, 1))->setIconIndex(81);
Item* Item::s_pickaxeStone = (new ItemPickaxe(18, 1))->setIconIndex(97);
Item* Item::s_axeStone = (new ItemAxe(19, 1))->setIconIndex(113);
Item* Item::s_swordDiamond = (new ItemSword(20, 3))->setIconIndex(67);
Item* Item::s_shovelDiamond = (new ItemSpade(21, 3))->setIconIndex(83);
Item* Item::s_pickaxeDiamond = (new ItemPickaxe(22, 3))->setIconIndex(99);
Item* Item::s_axeDiamond = (new ItemAxe(23, 3))->setIconIndex(115);
Item* Item::s_stick = (new Item(24))->setIconIndex(53)->setFull3D();
Item* Item::s_bowlEmpty = (new Item(25))->setIconIndex(71);
Item* Item::s_bowlSoup = (new ItemSoup(26, 10))->setIconIndex(72);
Item* Item::s_swordGold = (new ItemSword(27, 0))->setIconIndex(68);
Item* Item::s_shovelGold = (new ItemSpade(28, 0))->setIconIndex(84);
Item* Item::s_pickaxeGold = (new ItemPickaxe(29, 0))->setIconIndex(100);
Item* Item::s_axeGold = (new ItemAxe(30, 0))->setIconIndex(116);
Item* Item::s_silk = (new Item(31))->setIconIndex(8);
Item* Item::s_feather = (new Item(32))->setIconIndex(24);
Item* Item::s_gunpowder = (new Item(33))->setIconIndex(40);
Item* Item::s_hoeWood = (new ItemHoe(34, 0))->setIconIndex(128);
Item* Item::s_hoeStone = (new ItemHoe(35, 1))->setIconIndex(129);
Item* Item::s_hoeSteel = (new ItemHoe(36, 2))->setIconIndex(130);
Item* Item::s_hoeDiamond = (new ItemHoe(37, 3))->setIconIndex(131);
Item* Item::s_hoeGold = (new ItemHoe(38, 1))->setIconIndex(132);
Item* Item::s_seeds = (new ItemSeeds(39, Block::s_crops->blockID))->setIconIndex(9);
Item* Item::s_wheat = (new Item(40))->setIconIndex(25);
Item* Item::s_bread = (new ItemFood(41, 5))->setIconIndex(41);
Item* Item::s_helmetLeather = (new ItemArmor(42, 0, 0, 0))->setIconIndex(0);
Item* Item::s_plateLeather = (new ItemArmor(43, 0, 0, 1))->setIconIndex(16);
Item* Item::s_legsLeather = (new ItemArmor(44, 0, 0, 2))->setIconIndex(32);
Item* Item::s_bootsLeather = (new ItemArmor(45, 0, 0, 3))->setIconIndex(48);
Item* Item::s_helmetChain = (new ItemArmor(46, 1, 1, 0))->setIconIndex(1);
Item* Item::s_plateChain = (new ItemArmor(47, 1, 1, 1))->setIconIndex(17);
Item* Item::s_legsChain = (new ItemArmor(48, 1, 1, 2))->setIconIndex(33);
Item* Item::s_bootsChain = (new ItemArmor(49, 1, 1, 3))->setIconIndex(49);
Item* Item::s_helmetSteel = (new ItemArmor(50, 2, 2, 0))->setIconIndex(2);
Item* Item::s_plateSteel = (new ItemArmor(51, 2, 2, 1))->setIconIndex(18);
Item* Item::s_legsSteel = (new ItemArmor(52, 2, 2, 2))->setIconIndex(34);
Item* Item::s_bootsSteel = (new ItemArmor(53, 2, 2, 3))->setIconIndex(50);
Item* Item::s_helmetDiamond = (new ItemArmor(54, 3, 3, 0))->setIconIndex(3);
Item* Item::s_plateDiamond = (new ItemArmor(55, 3, 3, 1))->setIconIndex(19);
Item* Item::s_legsDiamond = (new ItemArmor(56, 3, 3, 2))->setIconIndex(35);
Item* Item::s_bootsDiamond = (new ItemArmor(57, 3, 3, 3))->setIconIndex(51);
Item* Item::s_helmetGold = (new ItemArmor(58, 1, 4, 0))->setIconIndex(4);
Item* Item::s_plateGold = (new ItemArmor(59, 1, 4, 1))->setIconIndex(20);
Item* Item::s_legsGold = (new ItemArmor(60, 1, 4, 2))->setIconIndex(36);
Item* Item::s_bootsGold = (new ItemArmor(61, 1, 4, 3))->setIconIndex(52);
Item* Item::s_flint = (new Item(62))->setIconIndex(6);
Item* Item::s_porkRaw = (new ItemFood(63, 3))->setIconIndex(87);
Item* Item::s_porkCooked = (new ItemFood(64, 8))->setIconIndex(88);
Item* Item::s_painting = (new ItemPainting(65))->setIconIndex(26);
Item* Item::s_appleGold = (new ItemFood(66, 42))->setIconIndex(11);
Item* Item::s_sign = (new ItemSign(67))->setIconIndex(42);
Item* Item::s_doorWood = (new ItemDoor(68, Material::s_wood))->setIconIndex(43);
Item* Item::s_bucketEmpty = (new ItemBucket(69, 0))->setIconIndex(74);
Item* Item::s_bucketWater = (new ItemBucket(70, Block::s_waterStill->blockID))->setIconIndex(75);
Item* Item::s_bucketLava = (new ItemBucket(71, Block::s_lavaStill->blockID))->setIconIndex(76);
Item* Item::s_minecartEmpty = (new ItemMinecart(72, 0))->setIconIndex(135);
Item* Item::s_saddle = (new ItemSaddle(73))->setIconIndex(104);
Item* Item::s_doorSteel = (new ItemDoor(74, Material::s_iron))->setIconIndex(44);
Item* Item::s_redstone = (new ItemRedstone(75))->setIconIndex(56);
Item* Item::s_snowball = (new ItemSnowball(76))->setIconIndex(14);
Item* Item::s_boat = (new ItemBoat(77))->setIconIndex(136);
Item* Item::s_leather = (new Item(78))->setIconIndex(103);
Item* Item::s_bucketMilk = (new ItemBucket(79, -1))->setIconIndex(77);
Item* Item::s_brick = (new Item(80))->setIconIndex(22);
Item* Item::s_clay = (new Item(81))->setIconIndex(57);
Item* Item::s_reed = (new ItemReed(82, Block::s_reed))->setIconIndex(27);
Item* Item::s_paper = (new Item(83))->setIconIndex(58);
Item* Item::s_book = (new Item(84))->setIconIndex(59);
Item* Item::s_slimeBall = (new Item(85))->setIconIndex(30);
Item* Item::s_minecartCrate = (new ItemMinecart(86, 1))->setIconIndex(151);
Item* Item::s_minecartPowered = (new ItemMinecart(87, 2))->setIconIndex(167);
Item* Item::s_egg = (new Item(88))->setIconIndex(12);
Item* Item::s_compass = (new Item(89))->setIconIndex(54);
Item* Item::s_fishingRod = (new ItemFishingRod(90))->setIconIndex(69);
Item* Item::s_pocketSundial = (new Item(91))->setIconIndex(70);
Item* Item::s_lightStoneDust = (new Item(92))->setIconIndex(73);
Item* Item::s_fishRaw = (new ItemFood(93, 2))->setIconIndex(89);
Item* Item::s_fishCooked = (new ItemFood(94, 5))->setIconIndex(90);
Item* Item::s_record13 = (new ItemRecord(2000, "13"))->setIconIndex(240);
Item* Item::s_recordCat = (new ItemRecord(2001, "cat"))->setIconIndex(241);

Item::Item()
	: shiftedIndex(0)
	, maxStackSize(64)
	, maxDamage(32)
	, iconIndex(0)
	, m_bFull3D(false)
{
}

Item::Item(int var1)
	: shiftedIndex(0)
	, maxStackSize(64)
	, maxDamage(32)
	, iconIndex(0)
	, m_bFull3D(false)
{
		shiftedIndex = 256 + var1;
		if(itemsList[256 + var1] != NULL) {
			DbgPrintF("%s\n", "CONFLICT @ " + var1);
		}

		itemsList[256 + var1] = this;
}

Item::~Item() {
	// No dynamic resources
}

Item* Item::setIconIndex(int var1) {
		iconIndex = var1;
		return this;
}

int Item::getIconIndex(ItemStack* var1) {
		return iconIndex;
}

bool Item::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7) {
		return false;
}

float Item::getStrVsBlock(ItemStack* var1, Block* var2) {
		return 1.0F;
}

ItemStack* Item::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3) {
		return var1;
}

int Item::getItemStackLimit() {
		return maxStackSize;
}

int Item::getMaxDamage() {
		return maxDamage;
}

void Item::hitEntity(ItemStack* var1, EntityLiving* var2) {
	// TODO: implement
}

void Item::hitBlock(ItemStack* var1, int var2, int var3, int var4, int var5) {
	// TODO: implement
}

int Item::getDamageVsEntity(Entity* var1) {
		return 1;
}

bool Item::canHarvestBlock(Block* var1) {
		return false;
}

void Item::func_4019_b(ItemStack* var1, EntityLiving* var2) {
	// TODO: implement
}

Item* Item::setFull3D() {
		m_bFull3D = true;
		return this;
}

bool Item::isFull3D() {
		return m_bFull3D;
}

bool Item::shouldRotateAroundWhenRendering() {
		return false;
}
