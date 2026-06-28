#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemStack.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemStack.h"
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

ItemStack::ItemStack(Block* var1)
	: stackSize(0)
	, animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
	// inlined this((Block*)var1, 1) -- C++03 lacks ctor delegation
}

ItemStack::ItemStack(Block* var1, int var2)
	: stackSize(0)
	, animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
	// inlined this(var1->blockID, var2) -- C++03 lacks ctor delegation
}

ItemStack::ItemStack(Item* var1)
	: stackSize(0)
	, animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
	// inlined this((Item*)var1, 1) -- C++03 lacks ctor delegation
}

ItemStack::ItemStack(Item* var1, int var2)
	: stackSize(0)
	, animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
	// inlined this(var1->shiftedIndex, var2) -- C++03 lacks ctor delegation
}

ItemStack::ItemStack(int var1)
	: stackSize(0)
	, animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
	// inlined this(var1, 1) -- C++03 lacks ctor delegation
}

ItemStack::ItemStack(int var1, int var2)
	: stackSize(0)
	, animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
	this->stackSize = 0;
	this->itemID = var1;
	this->stackSize = var2;
}

ItemStack::ItemStack(int var1, int var2, int var3)
	: stackSize(0)
	, animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
	this->stackSize = 0;
	this->itemID = var1;
	this->stackSize = var2;
	this->itemDamage = var3;
}

ItemStack::ItemStack(NBTTagCompound* var1)
	: stackSize(0)
	, animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
	this->stackSize = 0;
	this->readFromNBT(var1);
}

ItemStack::~ItemStack()
{
}

ItemStack* ItemStack::splitStack(int var1)
{
	this->stackSize -= var1;
	return new ItemStack(this->itemID, var1, this->itemDamage);
}

Item* ItemStack::getItem()
{
	return Item::s_itemsList[this->itemID];
}

int ItemStack::getIconIndex()
{
	// PORT(★ SMP NULL item): getItem() = Item::s_itemsList[itemID] is NULL for an unregistered id
	// (e.g. a chest/inventory holding an item this Alpha build never registered). -1 = "no icon, skip".
	if(this->getItem() == NULL) { return -1; }
	return this->getItem()->getIconIndex(this);
}

bool ItemStack::useItem(EntityPlayer* var1, World* var2, int var3, int var4, int var5, int var6)
{
	if(this->getItem() == NULL) { return false; }   // PORT(★ SMP NULL item): unregistered id -> no-op
	return this->getItem()->onItemUse(this, var1, var2, var3, var4, var5, var6);
}

float ItemStack::getStrVsBlock(Block* var1)
{
	if(this->getItem() == NULL) { return 1.0F; }   // PORT(★ SMP NULL item): unregistered id -> base speed
	return this->getItem()->getStrVsBlock(this, var1);
}

ItemStack* ItemStack::useItemRightClick(World* var1, EntityPlayer* var2)
{
	if(this->getItem() == NULL) { return this; }   // PORT(★ SMP NULL item): unregistered id -> unchanged
	return this->getItem()->onItemRightClick(this, var1, var2);
}

NBTTagCompound* ItemStack::writeToNBT(NBTTagCompound* var1)
{
	var1->setShort("id", (short)this->itemID);
	var1->setByte("Count", (unsigned char)this->stackSize);
	var1->setShort("Damage", (short)this->itemDamage);
	return var1;
}

void ItemStack::readFromNBT(NBTTagCompound* var1)
{
	this->itemID = var1->getShort("id");
	this->stackSize = var1->getByte("Count");
	this->itemDamage = var1->getShort("Damage");
}

int ItemStack::getMaxStackSize()
{
	if(this->getItem() == NULL) { return 64; }   // PORT(★ SMP NULL item): unregistered id -> default stack
	return this->getItem()->getItemStackLimit();
}

int ItemStack::getMaxDamage()
{
	// PORT(★ SMP chest-open IP-0): this is THE confirmed chest crash. A chest holding a DAMAGED item with
	// an id this build never registered -> Item::s_itemsList[id] is NULL -> ->getMaxDamage() is a virtual
	// call through address 0 (IP-0), reached from RenderItem::renderItemOverlayIntoGUI's damage-bar draw.
	// 0 = "no durability bar" (the overlay caller is additionally guarded against the div-by-zero).
	if(Item::s_itemsList[this->itemID] == NULL) { return 0; }
	return Item::s_itemsList[this->itemID]->getMaxDamage();
}

void ItemStack::damageItem(int var1)
{
	this->itemDamage += var1;
	if(this->itemDamage > this->getMaxDamage()) {
	--this->stackSize;
	if(this->stackSize < 0) {
	this->stackSize = 0;
	}
	this->itemDamage = 0;
	}
}

void ItemStack::hitEntity(EntityLiving* var1)
{
	if (Item::s_itemsList[this->itemID] != NULL) { Item::s_itemsList[this->itemID]->hitEntity(this, var1); }
}

void ItemStack::hitBlock(int var1, int var2, int var3, int var4)
{
	if (Item::s_itemsList[this->itemID] != NULL) { Item::s_itemsList[this->itemID]->hitBlock(this, var1, var2, var3, var4); }
}

int ItemStack::getDamageVsEntity(Entity* var1)
{
	if(Item::s_itemsList[this->itemID] == NULL) { return 0; }   // PORT(★ SMP NULL item): unregistered id
	return Item::s_itemsList[this->itemID]->getDamageVsEntity(var1);
}

bool ItemStack::canHarvestBlock(Block* var1)
{
	if(Item::s_itemsList[this->itemID] == NULL) { return false; }   // PORT(★ SMP NULL item): unregistered id
	return Item::s_itemsList[this->itemID]->canHarvestBlock(var1);
}

void ItemStack::onItemDestroyedByUse(EntityPlayer* var1)
{
}

void ItemStack::useItemOnEntity(EntityLiving* var1)
{
	if (Item::s_itemsList[this->itemID] != NULL) { Item::s_itemsList[this->itemID]->useItemOnEntity(this, var1); }
}

ItemStack* ItemStack::copy()
{
	return new ItemStack(this->itemID, this->stackSize, this->itemDamage);
}

void ItemStack::initStatics()
{
}
