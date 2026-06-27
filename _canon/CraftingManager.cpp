#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from CraftingManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CraftingManager.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "Collections.h"
#include "CraftingRecipe.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "Material.h"
#include "MobSpawnerBase.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "RecipeSorter.h"
#include "RecipesArmor.h"
#include "RecipesCrafting.h"
#include "RecipesFood.h"
#include "RecipesIngots.h"
#include "RecipesTools.h"
#include "RecipesWeapons.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

CraftingManager* CraftingManager::s_instance;

CraftingManager::CraftingManager()
{
	this->recipes = JList<void*>();
	(new RecipesTools())->addRecipes(this);
	(new RecipesWeapons())->addRecipes(this);
	(new RecipesIngots())->addRecipes(this);
	(new RecipesFood())->addRecipes(this);
	(new RecipesCrafting())->addRecipes(this);
	(new RecipesArmor())->addRecipes(this);
	this->addRecipe(new ItemStack(Item::s_paper, 3), (JArrayInit<JObject >(), "###", Character::valueOf('#'), Item::s_reed));
	this->addRecipe(new ItemStack(Item::s_book, 1), (JArrayInit<JObject >(), "#", "#", "#", Character::valueOf('#'), Item::s_paper));
	this->addRecipe(new ItemStack(Block::s_fence, 2), (JArrayInit<JObject >(), "###", "###", Character::valueOf('#'), Item::s_stick));
	this->addRecipe(new ItemStack(Block::s_jukebox, 1), (JArrayInit<JObject >(), "###", "#X#", "###", Character::valueOf('#'), Block::s_planks, Character::valueOf('X'), Item::s_diamond));
	this->addRecipe(new ItemStack(Block::s_bookShelf, 1), (JArrayInit<JObject >(), "###", "XXX", "###", Character::valueOf('#'), Block::s_planks, Character::valueOf('X'), Item::s_book));
	this->addRecipe(new ItemStack(Block::s_blockSnow, 1), (JArrayInit<JObject >(), "##", "##", Character::valueOf('#'), Item::s_snowball));
	this->addRecipe(new ItemStack(Block::s_blockClay, 1), (JArrayInit<JObject >(), "##", "##", Character::valueOf('#'), Item::s_clay));
	this->addRecipe(new ItemStack(Block::s_brick, 1), (JArrayInit<JObject >(), "##", "##", Character::valueOf('#'), Item::s_brick));
	this->addRecipe(new ItemStack(Block::s_lightStone, 1), (JArrayInit<JObject >(), "###", "###", "###", Character::valueOf('#'), Item::s_lightStoneDust));
	this->addRecipe(new ItemStack(Block::s_cloth, 1), (JArrayInit<JObject >(), "###", "###", "###", Character::valueOf('#'), Item::s_silk));
	this->addRecipe(new ItemStack(Block::s_tnt, 1), (JArrayInit<JObject >(), "X#X", "#X#", "X#X", Character::valueOf('X'), Item::s_gunpowder, Character::valueOf('#'), Block::s_sand));
	this->addRecipe(new ItemStack(Block::s_stairSingle, 3), (JArrayInit<JObject >(), "###", Character::valueOf('#'), Block::s_cobblestone));
	this->addRecipe(new ItemStack(Block::s_ladder, 1), (JArrayInit<JObject >(), "# #", "###", "# #", Character::valueOf('#'), Item::s_stick));
	this->addRecipe(new ItemStack(Item::s_doorWood, 1), (JArrayInit<JObject >(), "##", "##", "##", Character::valueOf('#'), Block::s_planks));
	this->addRecipe(new ItemStack(Item::s_doorSteel, 1), (JArrayInit<JObject >(), "##", "##", "##", Character::valueOf('#'), Item::s_ingotIron));
	this->addRecipe(new ItemStack(Item::s_sign, 1), (JArrayInit<JObject >(), "###", "###", " X ", Character::valueOf('#'), Block::s_planks, Character::valueOf('X'), Item::s_stick));
	this->addRecipe(new ItemStack(Block::s_planks, 4), (JArrayInit<JObject >(), "#", Character::valueOf('#'), Block::s_wood));
	this->addRecipe(new ItemStack(Item::s_stick, 4), (JArrayInit<JObject >(), "#", "#", Character::valueOf('#'), Block::s_planks));
	this->addRecipe(new ItemStack(Block::s_torchWood, 4), (JArrayInit<JObject >(), "X", "#", Character::valueOf('X'), Item::s_coal, Character::valueOf('#'), Item::s_stick));
	this->addRecipe(new ItemStack(Item::s_bowlEmpty, 4), (JArrayInit<JObject >(), "# #", " # ", Character::valueOf('#'), Block::s_planks));
	this->addRecipe(new ItemStack(Block::s_minecartTrack, 16), (JArrayInit<JObject >(), "X X", "X#X", "X X", Character::valueOf('X'), Item::s_ingotIron, Character::valueOf('#'), Item::s_stick));
	this->addRecipe(new ItemStack(Item::s_minecartEmpty, 1), (JArrayInit<JObject >(), "# #", "###", Character::valueOf('#'), Item::s_ingotIron));
	this->addRecipe(new ItemStack(Block::s_pumpkinLantern, 1), (JArrayInit<JObject >(), "A", "B", Character::valueOf('A'), Block::s_pumpkin, Character::valueOf('B'), Block::s_torchWood));
	this->addRecipe(new ItemStack(Item::s_minecartCrate, 1), (JArrayInit<JObject >(), "A", "B", Character::valueOf('A'), Block::s_crate, Character::valueOf('B'), Item::s_minecartEmpty));
	this->addRecipe(new ItemStack(Item::s_minecartPowered, 1), (JArrayInit<JObject >(), "A", "B", Character::valueOf('A'), Block::s_stoneOvenIdle, Character::valueOf('B'), Item::s_minecartEmpty));
	this->addRecipe(new ItemStack(Item::s_boat, 1), (JArrayInit<JObject >(), "# #", "###", Character::valueOf('#'), Block::s_planks));
	this->addRecipe(new ItemStack(Item::s_bucketEmpty, 1), (JArrayInit<JObject >(), "# #", " # ", Character::valueOf('#'), Item::s_ingotIron));
	this->addRecipe(new ItemStack(Item::s_flintAndSteel, 1), (JArrayInit<JObject >(), "A ", " B", Character::valueOf('A'), Item::s_ingotIron, Character::valueOf('B'), Item::s_flint));
	this->addRecipe(new ItemStack(Item::s_bread, 1), (JArrayInit<JObject >(), "###", Character::valueOf('#'), Item::s_wheat));
	this->addRecipe(new ItemStack(Block::s_stairCompactPlanks, 4), (JArrayInit<JObject >(), "#  ", "## ", "###", Character::valueOf('#'), Block::s_planks));
	this->addRecipe(new ItemStack(Item::s_fishingRod, 1), (JArrayInit<JObject >(), "  #", " #X", "# X", Character::valueOf('#'), Item::s_stick, Character::valueOf('X'), Item::s_silk));
	this->addRecipe(new ItemStack(Block::s_stairCompactCobblestone, 4), (JArrayInit<JObject >(), "#  ", "## ", "###", Character::valueOf('#'), Block::s_cobblestone));
	this->addRecipe(new ItemStack(Item::s_painting, 1), (JArrayInit<JObject >(), "###", "#X#", "###", Character::valueOf('#'), Item::s_stick, Character::valueOf('X'), Block::s_cloth));
	this->addRecipe(new ItemStack(Item::s_appleGold, 1), (JArrayInit<JObject >(), "###", "#X#", "###", Character::valueOf('#'), Block::s_blockGold, Character::valueOf('X'), Item::s_appleRed));
	this->addRecipe(new ItemStack(Block::s_lever, 1), (JArrayInit<JObject >(), "X", "#", Character::valueOf('#'), Block::s_cobblestone, Character::valueOf('X'), Item::s_stick));
	this->addRecipe(new ItemStack(Block::s_torchRedstoneActive, 1), (JArrayInit<JObject >(), "X", "#", Character::valueOf('#'), Item::s_stick, Character::valueOf('X'), Item::s_redstone));
	this->addRecipe(new ItemStack(Item::s_pocketSundial, 1), (JArrayInit<JObject >(), " # ", "#X#", " # ", Character::valueOf('#'), Item::s_ingotGold, Character::valueOf('X'), Item::s_redstone));
	this->addRecipe(new ItemStack(Item::s_compass, 1), (JArrayInit<JObject >(), " # ", "#X#", " # ", Character::valueOf('#'), Item::s_ingotIron, Character::valueOf('X'), Item::s_redstone));
	this->addRecipe(new ItemStack(Block::s_button, 1), (JArrayInit<JObject >(), "#", "#", Character::valueOf('#'), Block::s_stone));
	this->addRecipe(new ItemStack(Block::s_pressurePlateStone, 1), (JArrayInit<JObject >(), "###", Character::valueOf('#'), Block::s_stone));
	this->addRecipe(new ItemStack(Block::s_pressurePlatePlanks, 1), (JArrayInit<JObject >(), "###", Character::valueOf('#'), Block::s_planks));
	Collections::sort(this->recipes, new RecipeSorter(this));
	System::out.println((JCat() + this->recipes.size() + " recipes"));
}

CraftingManager::~CraftingManager()
{
}

CraftingManager* CraftingManager::getInstance()
{
	return s_instance;
}

void CraftingManager::addRecipe(ItemStack* var1, JArray<JObject > var2)
{
	JavaString var3 = "";
	int var4 = 0;
	int var5 = 0;
	int var6 = 0;
	if(((var2[var4]).isStringArray())) {
	JArray<JavaString > var11 = ((var2[var4++]).strArray());
	for(int var8 = 0; var8 < var11.length; ++var8) {
	JavaString var9 = var11[var8];
	++var6;
	var5 = var9.length();
	var3 = var3 + var9;
	}
	} else {
	while(((var2[var4]).isString())) {
	JavaString var7 = (var2[var4++]).str();
	++var6;
	var5 = var7.length();
	var3 = var3 + var7;
	}
	}
	std::map<JObject, JObject> var12;
	int var15;
	for(var12 = std::map<JObject, JObject>(); var4 < var2.length; var4 += 2) {
	char var13 = (var2[var4]);
	var15 = 0;
	if(((var2[var4 + 1]).is<Item>())) {
	var15 = ((Item*)var2[var4 + 1])->shiftedIndex;
	} else if(((var2[var4 + 1]).is<Block>())) {
	var15 = ((Block*)var2[var4 + 1])->blockID;
	}
	var12[var13] = Integer::valueOf(var15);
	}
	JArray<int > var14 = JArray<int >(var5 * var6);
	for(var15 = 0; var15 < var5 * var6; ++var15) {
	char var10 = var3[var15];
	if(var12.count(Character::valueOf(var10))) {
	var14[var15] = ((var12[Character::valueOf(var10)])).intValue();
	} else {
	var14[var15] = -1;
	}
	}
	this->recipes.push_back(new CraftingRecipe(var5, var6, var14, var1));
}

ItemStack* CraftingManager::craft(JArray<int > var1)
{
	for(int var2 = 0; var2 < this->recipes.size(); ++var2) {
	CraftingRecipe* var3 = (CraftingRecipe*)this->recipes[var2];
	if(var3->matchRecipe(var1)) {
	return var3->createResult(var1);
	}
	}
	return NULL;
}

void CraftingManager::initStatics()
{
	CraftingManager::s_instance = new CraftingManager();
}
