#ifndef ITEM_H
#define ITEM_H

// Auto-generated from Item.java
// Target: Xbox 360 / C++03 / DirectX 9

class Block;
class EntityPlayer;
class ItemStack;
class Random;
class World;

class Item {
public:
	Item();
	Item(int var1);
	virtual ~Item();
	static void initStatics();  // run once at startup (was Java static {})

	static Item** itemsList;
	static Item* s_shovel;
	static Item* s_pickaxeSteel;
	static Item* s_axeSteel;
	static Item* s_striker;
	static Item* s_bow;
	static Item* s_arrow;
	static Item* s_coal;
	static Item* s_diamond;
	static Item* s_ingotIron;
	static Item* s_ingotGold;
	static Item* s_swordSteel;
	static Item* s_swordWood;
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
	int shiftedIndex;

	virtual Item* setIconIndex(int var1);
	virtual int getIconIndex();
	virtual bool onItemUse(ItemStack* var1, World* var2, int var3, int var4, int var5, int var6);
	virtual float getStrVsBlock(Block* var1);
	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);
	virtual int getItemStackLimit();
	virtual bool onPlaced(World* var1, float var2, float var3, float var4);
	virtual int getMaxDamage();
	virtual void hitEntity(ItemStack* var1);
	virtual void onBlockDestroyed(ItemStack* var1);
	virtual int getDamageVsEntity();
	virtual bool canHarvestBlock(Block* var1);

public: // was protected
	static Random* s_rand;
	int maxStackSize;
	int maxDamage;
	int iconIndex;


};

#endif // ITEM_H