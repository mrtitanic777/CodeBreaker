#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RecipesIngots.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RecipesIngots.h"
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

RecipesIngots::RecipesIngots()
{
	this->recipeItems = (JArrayInit<JArray<JObject > >(), (JArrayInit<JObject >(), Block::s_blockGold, Item::s_ingotGold), (JArrayInit<JObject >(), Block::s_blockSteel, Item::s_ingotIron), (JArrayInit<JObject >(), Block::s_blockDiamond, Item::s_diamond));
}

RecipesIngots::~RecipesIngots()
{
}

void RecipesIngots::addRecipes(CraftingManager* var1)
{
	for(int var2 = 0; var2 < this->recipeItems.length; ++var2) {
	Block* var3 = (Block*)this->recipeItems[var2][0];
	Item* var4 = (Item*)this->recipeItems[var2][1];
	var1->addRecipe(new ItemStack(var3), (JArrayInit<JObject >(), "###", "###", "###", Character::valueOf('#'), var4));
	var1->addRecipe(new ItemStack(var4, 9), (JArrayInit<JObject >(), "#", Character::valueOf('#'), var3));
	}
}

void RecipesIngots::initStatics()
{
}
