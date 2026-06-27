#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RecipesTools.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RecipesTools.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "CraftingManager.h"
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
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

RecipesTools::RecipesTools()
{
	this->recipePatterns = (JArrayInit<JArray<JavaString > >(), (JArrayInit<JavaString >(), "XXX", " # ", " # "), (JArrayInit<JavaString >(), "X", "#", "#"), (JArrayInit<JavaString >(), "XX", "X#", " #"), (JArrayInit<JavaString >(), "XX", " #", " #"));
	this->recipeItems = (JArrayInit<JArray<JObject > >(), (JArrayInit<JObject >(), Block::s_planks, Block::s_cobblestone, Item::s_ingotIron, Item::s_diamond, Item::s_ingotGold), (JArrayInit<JObject >(), Item::s_pickaxeWood, Item::s_pickaxeStone, Item::s_pickaxeSteel, Item::s_pickaxeDiamond, Item::s_pickaxeGold), (JArrayInit<JObject >(), Item::s_shovelWood, Item::s_shovelStone, Item::s_shovelSteel, Item::s_shovelDiamond, Item::s_shovelGold), (JArrayInit<JObject >(), Item::s_axeWood, Item::s_axeStone, Item::s_axeSteel, Item::s_axeDiamond, Item::s_axeGold), (JArrayInit<JObject >(), Item::s_hoeWood, Item::s_hoeStone, Item::s_hoeSteel, Item::s_hoeDiamond, Item::s_hoeGold));
}

RecipesTools::~RecipesTools()
{
}

void RecipesTools::addRecipes(CraftingManager* var1)
{
	for(int var2 = 0; var2 < this->recipeItems[0].length; ++var2) {
	JObject var3 = this->recipeItems[0][var2];
	for(int var4 = 0; var4 < this->recipeItems.length - 1; ++var4) {
	Item* var5 = (Item*)this->recipeItems[var4 + 1][var2];
	var1->addRecipe(new ItemStack(var5), (JArrayInit<JObject >(), this->recipePatterns[var4], Character::valueOf('#'), Item::s_stick, Character::valueOf('X'), var3));
	}
	}
}

void RecipesTools::initStatics()
{
}
