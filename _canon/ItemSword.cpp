#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemSword.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSword.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityLiving.h"
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

ItemSword::ItemSword(int var1, int var2)
	: Item(var1)
	, weaponDamage(0)
{
	this->maxStackSize = 1;
	this->maxDamage = 32 << var2;
	if(var2 == 3) {
	this->maxDamage *= 4;
	}
	this->weaponDamage = 4 + var2 * 2;
}

ItemSword::~ItemSword()
{
}

float ItemSword::getStrVsBlock(ItemStack* var1, Block* var2)
{
	return 1.5F;
}

void ItemSword::hitEntity(ItemStack* var1, EntityLiving* var2)
{
	var1->damageItem(1);
}

void ItemSword::hitBlock(ItemStack* var1, int var2, int var3, int var4, int var5)
{
	var1->damageItem(2);
}

int ItemSword::getDamageVsEntity(Entity* var1)
{
	return this->weaponDamage;
}

bool ItemSword::isFull3D()
{
	return true;
}

void ItemSword::initStatics()
{
}
