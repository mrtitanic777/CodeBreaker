#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RecipesArmor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RecipesArmor.h"
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

RecipesArmor::RecipesArmor()
{
	this->craftingPatterns = (JArrayInit<JArray<JavaString > >(), (JArrayInit<JavaString >(), "XXX", "X X"), (JArrayInit<JavaString >(), "X X", "XXX", "XXX"), (JArrayInit<JavaString >(), "XXX", "X X", "X X"), (JArrayInit<JavaString >(), "X X", "X X"));
	this->armorItems = (JArrayInit<JArray<JObject > >(), (JArrayInit<JObject >(), Item::s_leather, Block::s_fire, Item::s_ingotIron, Item::s_diamond, Item::s_ingotGold), (JArrayInit<JObject >(), Item::s_helmetLeather, Item::s_helmetChain, Item::s_helmetSteel, Item::s_helmetDiamond, Item::s_helmetGold), (JArrayInit<JObject >(), Item::s_plateLeather, Item::s_plateChain, Item::s_plateSteel, Item::s_plateDiamond, Item::s_plateGold), (JArrayInit<JObject >(), Item::s_legsLeather, Item::s_legsChain, Item::s_legsSteel, Item::s_legsDiamond, Item::s_legsGold), (JArrayInit<JObject >(), Item::s_bootsLeather, Item::s_bootsChain, Item::s_bootsSteel, Item::s_bootsDiamond, Item::s_bootsGold));
}

RecipesArmor::~RecipesArmor()
{
}

void RecipesArmor::addRecipes(CraftingManager* var1)
{
	for(int var2 = 0; var2 < this->armorItems[0].length; ++var2) {
	JObject var3 = this->armorItems[0][var2];
	for(int var4 = 0; var4 < this->armorItems.length - 1; ++var4) {
	Item* var5 = (Item*)this->armorItems[var4 + 1][var2];
	var1->addRecipe(new ItemStack(var5), (JArrayInit<JObject >(), this->craftingPatterns[var4], Character::valueOf('X'), var3));
	}
	}
}

void RecipesArmor::initStatics()
{
}
