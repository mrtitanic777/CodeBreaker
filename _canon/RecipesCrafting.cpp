#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RecipesCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RecipesCrafting.h"
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

RecipesCrafting::~RecipesCrafting()
{
}

void RecipesCrafting::addRecipes(CraftingManager* var1)
{
	var1->addRecipe(new ItemStack(Block::s_crate), (JArrayInit<JObject >(), "###", "# #", "###", Character::valueOf('#'), Block::s_planks));
	var1->addRecipe(new ItemStack(Block::s_stoneOvenIdle), (JArrayInit<JObject >(), "###", "# #", "###", Character::valueOf('#'), Block::s_cobblestone));
	var1->addRecipe(new ItemStack(Block::s_workbench), (JArrayInit<JObject >(), "##", "##", Character::valueOf('#'), Block::s_planks));
}

void RecipesCrafting::initStatics()
{
}
