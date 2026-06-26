#ifndef ITEM_H
#define ITEM_H

// Auto-generated from Item.java
// Target: Xbox 360 / C++03 / DirectX 9

class Block;
class Entity;
class EntityLiving;
class EntityPlayer;
class ItemStack;
class Random;
class World;

class Item {
public:
	Item();
	Item(int var1);
	virtual ~Item();

	static Item** itemsList;
	static Item* s_shovelSteel;
	static Item* s_pickaxeSteel;
	static Item* s_axeSteel;
	static Item* s_flintAndSteel;
	static Item* s_appleRed;
	static Item* s_bow;
	static Item* s_arrow;
	static Item* s_coal;
	static Item* s_diamond;
	static Item* s_ingotIron;
	static Item* s_ingotGold;
	static Item* s_swordSteel;
	static Item* s_swordWood;
	static Item* s_shovelWood;
	static Item* s_pickaxeWood;
	static Item* s_axeWood;
	static Item* s_swordStone;
	static Item* s_shovelStone;
	static Item* s_pickaxeStone;
	static Item* s_axeStone;
	static Item* s_swordDiamond;
	static Item* s_shovelDiamond;
	static Item* s_pickaxeDiamond;
	static Item* s_axeDiamond;
	static Item* s_stick;
	static Item* s_bowlEmpty;
	static Item* s_bowlSoup;
	static Item* s_swordGold;
	static Item* s_shovelGold;
	static Item* s_pickaxeGold;
	static Item* s_axeGold;
	static Item* s_silk;
	static Item* s_feather;
	static Item* s_gunpowder;
	static Item* s_hoeWood;
	static Item* s_hoeStone;
	static Item* s_hoeSteel;
	static Item* s_hoeDiamond;
	static Item* s_hoeGold;
	static Item* s_seeds;
	static Item* s_wheat;
	static Item* s_bread;
	static Item* s_helmetLeather;
	static Item* s_plateLeather;
	static Item* s_legsLeather;
	static Item* s_bootsLeather;
	static Item* s_helmetChain;
	static Item* s_plateChain;
	static Item* s_legsChain;
	static Item* s_bootsChain;
	static Item* s_helmetSteel;
	static Item* s_plateSteel;
	static Item* s_legsSteel;
	static Item* s_bootsSteel;
	static Item* s_helmetDiamond;
	static Item* s_plateDiamond;
	static Item* s_legsDiamond;
	static Item* s_bootsDiamond;
	static Item* s_helmetGold;
	static Item* s_plateGold;
	static Item* s_legsGold;
	static Item* s_bootsGold;
	static Item* s_flint;
	static Item* s_porkRaw;
	static Item* s_porkCooked;
	static Item* s_painting;
	static Item* s_appleGold;
	static Item* s_sign;
	static Item* s_doorWood;
	static Item* s_bucketEmpty;
	static Item* s_bucketWater;
	static Item* s_bucketLava;
	static Item* s_minecartEmpty;
	static Item* s_saddle;
	static Item* s_doorSteel;
	static Item* s_redstone;
	static Item* s_snowball;
	static Item* s_boat;
	static Item* s_leather;
	static Item* s_bucketMilk;
	static Item* s_brick;
	static Item* s_clay;
	static Item* s_reed;
	static Item* s_paper;
	static Item* s_book;
	static Item* s_slimeBall;
	static Item* s_minecartCrate;
	static Item* s_minecartPowered;
	static Item* s_egg;
	static Item* s_compass;
	static Item* s_fishingRod;
	static Item* s_pocketSundial;
	static Item* s_lightStoneDust;
	static Item* s_fishRaw;
	static Item* s_fishCooked;
	static Item* s_record13;
	static Item* s_recordCat;
	int shiftedIndex;

	virtual Item* setIconIndex(int var1);
	virtual int getIconIndex(ItemStack* var1);
	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);
	virtual float getStrVsBlock(ItemStack* var1, Block* var2);
	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);
	virtual int getItemStackLimit();
	virtual int getMaxDamage();
	virtual void hitEntity(ItemStack* var1, EntityLiving* var2);
	virtual void hitBlock(ItemStack* var1, int var2, int var3, int var4, int var5);
	virtual int getDamageVsEntity(Entity* var1);
	virtual bool canHarvestBlock(Block* var1);
	virtual void func_4019_b(ItemStack* var1, EntityLiving* var2);
	virtual Item* setFull3D();
	virtual bool isFull3D();
	virtual bool shouldRotateAroundWhenRendering();

public: // was protected
	static Random* s_itemRand;
	int maxStackSize;
	int maxDamage;
	int iconIndex;
	bool m_bFull3D;


};

#endif // ITEM_H