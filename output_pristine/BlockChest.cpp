#include "stdafx.h"
// Auto-generated from BlockChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockChest.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "Random.h"
#include "TileEntity.h"
#include "World.h"
#include "Block.h"
#include "EntityItem.h"
#include "InventoryLargeChest.h"
#include "ItemStack.h"
#include "Material.h"
#include "TileEntityChest.h"

BlockChest::BlockChest()
	: m_random(new Random())
{
}

BlockChest::BlockChest(int var1)
	: BlockContainer(54, Material::s_wood)
	, m_random(new Random())
{
		blockIndexInTexture = 26;
}

BlockChest::~BlockChest() {
	m_random = NULL;
}

int BlockChest::getBlockTexture(World* var1, int var2, int var3, int var4, int var5) {
		if(var5 == 1) {
			return blockIndexInTexture - 1;
		} else if(var5 == 0) {
			return blockIndexInTexture - 1;
		} else {
			int var6 = var1->getBlockId(var2, var3, var4 - 1);
			int var7 = var1->getBlockId(var2, var3, var4 + 1);
			int var8 = var1->getBlockId(var2 - 1, var3, var4);
			int var9 = var1->getBlockId(var2 + 1, var3, var4);
			int var10;
			int var11;
			int var12;
			unsigned char var13;
			if(var6 != blockID && var7 != blockID) {
				if(var8 != blockID && var9 != blockID) {
					unsigned char var14 = 3;
					if(Block::s_opaqueCubeLookup[var6] && !Block::s_opaqueCubeLookup[var7]) {
						var14 = 3;
					}

					if(Block::s_opaqueCubeLookup[var7] && !Block::s_opaqueCubeLookup[var6]) {
						var14 = 2;
					}

					if(Block::s_opaqueCubeLookup[var8] && !Block::s_opaqueCubeLookup[var9]) {
						var14 = 5;
					}

					if(Block::s_opaqueCubeLookup[var9] && !Block::s_opaqueCubeLookup[var8]) {
						var14 = 4;
					}

					return var5 == var14 ? blockIndexInTexture + 1 : blockIndexInTexture;
				} else if(var5 != 4 && var5 != 5) {
					var10 = 0;
					if(var8 == blockID) {
						var10 = -1;
					}

					var11 = var1->getBlockId(var8 == blockID ? var2 - 1 : var2 + 1, var3, var4 - 1);
					var12 = var1->getBlockId(var8 == blockID ? var2 - 1 : var2 + 1, var3, var4 + 1);
					if(var5 == 3) {
						var10 = -1 - var10;
					}

					var13 = 3;
					if((Block::s_opaqueCubeLookup[var6] || Block::s_opaqueCubeLookup[var11]) && !Block::s_opaqueCubeLookup[var7] && !Block::s_opaqueCubeLookup[var12]) {
						var13 = 3;
					}

					if((Block::s_opaqueCubeLookup[var7] || Block::s_opaqueCubeLookup[var12]) && !Block::s_opaqueCubeLookup[var6] && !Block::s_opaqueCubeLookup[var11]) {
						var13 = 2;
					}

					return (var5 == var13 ? blockIndexInTexture + 16 : blockIndexInTexture + 32) + var10;
				} else {
					return blockIndexInTexture;
				}
			} else if(var5 != 2 && var5 != 3) {
				var10 = 0;
				if(var6 == blockID) {
					var10 = -1;
				}

				var11 = var1->getBlockId(var2 - 1, var3, var6 == blockID ? var4 - 1 : var4 + 1);
				var12 = var1->getBlockId(var2 + 1, var3, var6 == blockID ? var4 - 1 : var4 + 1);
				if(var5 == 4) {
					var10 = -1 - var10;
				}

				var13 = 5;
				if((Block::s_opaqueCubeLookup[var8] || Block::s_opaqueCubeLookup[var11]) && !Block::s_opaqueCubeLookup[var9] && !Block::s_opaqueCubeLookup[var12]) {
					var13 = 5;
				}

				if((Block::s_opaqueCubeLookup[var9] || Block::s_opaqueCubeLookup[var12]) && !Block::s_opaqueCubeLookup[var8] && !Block::s_opaqueCubeLookup[var11]) {
					var13 = 4;
				}

				return (var5 == var13 ? blockIndexInTexture + 16 : blockIndexInTexture + 32) + var10;
			} else {
				return blockIndexInTexture;
			}
		}
}

int BlockChest::getBlockTextureFromSide(int var1) {
		return var1 == 1 ? blockIndexInTexture - 1 : (var1 == 0 ? blockIndexInTexture - 1 : (var1 == 3 ? blockIndexInTexture + 1 : blockIndexInTexture));
}

bool BlockChest::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		int var5 = 0;
		if(var1->getBlockId(var2 - 1, var3, var4) == blockID) {
			++var5;
		}

		if(var1->getBlockId(var2 + 1, var3, var4) == blockID) {
			++var5;
		}

		if(var1->getBlockId(var2, var3, var4 - 1) == blockID) {
			++var5;
		}

		if(var1->getBlockId(var2, var3, var4 + 1) == blockID) {
			++var5;
		}

		return var5 > 1 ? false : (isThereANeighborChest(var1, var2 - 1, var3, var4) ? false : (isThereANeighborChest(var1, var2 + 1, var3, var4) ? false : (isThereANeighborChest(var1, var2, var3, var4 - 1) ? false : !isThereANeighborChest(var1, var2, var3, var4 + 1))));
}

bool BlockChest::isThereANeighborChest(World* var1, int var2, int var3, int var4) {
		return var1->getBlockId(var2, var3, var4) != blockID ? false : (var1->getBlockId(var2 - 1, var3, var4) == blockID ? true : (var1->getBlockId(var2 + 1, var3, var4) == blockID ? true : (var1->getBlockId(var2, var3, var4 - 1) == blockID ? true : var1->getBlockId(var2, var3, var4 + 1) == blockID)));
}

void BlockChest::onBlockRemoval(World* var1, int var2, int var3, int var4) {
		TileEntityChest var5 = (TileEntityChest)var1->getBlockTileEntity(var2, var3, var4);

		for(int var6 = 0; var6 < var5.getSizeInventory(); ++var6) {
			ItemStack var7 = var5.getStackInSlot(var6);
			if(var7 != NULL) {
				float var8 = m_random->nextFloat() * 0.8F + 0.1F;
				float var9 = m_random->nextFloat() * 0.8F + 0.1F;
				float var10 = m_random->nextFloat() * 0.8F + 0.1F;

				while(var7.stackSize > 0) {
					int var11 = m_random->nextInt(21) + 10;
					if(var11 > var7.stackSize) {
						var11 = var7.stackSize;
					}

					var7.stackSize -= var11;
					EntityItem* var12 = new EntityItem(var1, (float)var2 + var8, (float)var3 + var9, (float)var4 + var10, new ItemStack(var7.itemID, var11, var7.itemDamage));
					var12->motionX = (float)m_random->nextGaussian() * 0.05F;
					var12->motionY = (float)m_random->nextGaussian() * 0.05F + 0.2F;
					var12->motionZ = (float)m_random->nextGaussian() * 0.05F;
					var1->spawnEntityInWorld(var12);
				}
			}
		}

		BlockContainer::onBlockRemoval(var1, var2, var3, var4);
}

bool BlockChest::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		void* var6 = (TileEntityChest)var1->getBlockTileEntity(var2, var3, var4);
		if(var1->isBlockNormalCube(var2, var3 + 1, var4)) {
			return true;
		} else if(var1->getBlockId(var2 - 1, var3, var4) == blockID && var1->isBlockNormalCube(var2 - 1, var3 + 1, var4)) {
			return true;
		} else if(var1->getBlockId(var2 + 1, var3, var4) == blockID && var1->isBlockNormalCube(var2 + 1, var3 + 1, var4)) {
			return true;
		} else if(var1->getBlockId(var2, var3, var4 - 1) == blockID && var1->isBlockNormalCube(var2, var3 + 1, var4 - 1)) {
			return true;
		} else if(var1->getBlockId(var2, var3, var4 + 1) == blockID && var1->isBlockNormalCube(var2, var3 + 1, var4 + 1)) {
			return true;
		} else {
			if(var1->getBlockId(var2 - 1, var3, var4) == blockID) {
				var6 = new InventoryLargeChest("Large chest", (TileEntityChest)var1->getBlockTileEntity(var2 - 1, var3, var4), (IInventory)var6);
			}

			if(var1->getBlockId(var2 + 1, var3, var4) == blockID) {
				var6 = new InventoryLargeChest("Large chest", (IInventory)var6, (TileEntityChest)var1->getBlockTileEntity(var2 + 1, var3, var4));
			}

			if(var1->getBlockId(var2, var3, var4 - 1) == blockID) {
				var6 = new InventoryLargeChest("Large chest", (TileEntityChest)var1->getBlockTileEntity(var2, var3, var4 - 1), (IInventory)var6);
			}

			if(var1->getBlockId(var2, var3, var4 + 1) == blockID) {
				var6 = new InventoryLargeChest("Large chest", (IInventory)var6, (TileEntityChest)var1->getBlockTileEntity(var2, var3, var4 + 1));
			}

			var5->displayGUIChest((IInventory)var6);
			return true;
		}
}

TileEntity* BlockChest::getBlockEntity() {
		return new TileEntityChest();
}
