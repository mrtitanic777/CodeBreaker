#include "stdafx.h"
// Auto-generated from BlockDoor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockDoor.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "EntityPlayer.h"
#include "IBlockAccess.h"
#include "Material.h"
#include "MovingObjectPosition.h"
#include "Random.h"
#include "Vec3D.h"
#include "World.h"
#include "Block.h"
#include "Item.h"

BlockDoor::BlockDoor()
{
}

BlockDoor::BlockDoor(int var1, Material* var2)
	: Block(var1, var2)
{
		blockIndexInTexture = 97;
		if(var2 == Material::s_iron) {
			++blockIndexInTexture;
		}

		float var3 = 0.5F;
		float var4 = 1.0F;
		setBlockBounds(0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, var4, 0.5F + var3);
}

BlockDoor::~BlockDoor() {
	// No dynamic resources
}

int BlockDoor::getBlockTextureFromSideAndMetadata(int var1, int var2) {
		if(var1 != 0 && var1 != 1) {
			int var3 = func_312_c(var2);
			if((var3 == 0 || var3 == 2) ^ (var1 <= 3)) {
				return blockIndexInTexture;
			} else {
				int var4 = var3 / 2 + (var1 & 1 ^ var3);
				var4 += (var2 & 4) / 4;
				int var5 = blockIndexInTexture - (var2 & 8) * 2;
				if((var4 & 1) != 0) {
					var5 = -var5;
				}

				return var5;
			}
		} else {
			return blockIndexInTexture;
		}
}

bool BlockDoor::isOpaqueCube() {
		return false;
}

bool BlockDoor::renderAsNormalBlock() {
		return false;
}

int BlockDoor::getRenderType() {
		return 7;
}

AxisAlignedBB BlockDoor::getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		setBlockBoundsBasedOnState(var1, var2, var3, var4);
		return Block::getSelectedBoundingBoxFromPool(var1, var2, var3, var4);
}

AxisAlignedBB BlockDoor::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		setBlockBoundsBasedOnState(var1, var2, var3, var4);
		return Block::getCollisionBoundingBoxFromPool(var1, var2, var3, var4);
}

void BlockDoor::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4) {
		func_313_b(func_312_c(var1->getBlockMetadata(var2, var3, var4)));
}

void BlockDoor::func_313_b(int var1) {
		float var2 = 3.0F / 16.0F;
		setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 2.0F, 1.0F);
		if(var1 == 0) {
			setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, var2);
		}

		if(var1 == 1) {
			setBlockBounds(1.0F - var2, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
		}

		if(var1 == 2) {
			setBlockBounds(0.0F, 0.0F, 1.0F - var2, 1.0F, 1.0F, 1.0F);
		}

		if(var1 == 3) {
			setBlockBounds(0.0F, 0.0F, 0.0F, var2, 1.0F, 1.0F);
		}

}

void BlockDoor::onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		blockActivated(var1, var2, var3, var4, var5);
}

bool BlockDoor::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		if(m_blockMaterial == Material::s_iron) {
			return true;
		} else {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			if((var6 & 8) != 0) {
				if(var1->getBlockId(var2, var3 - 1, var4) == blockID) {
					blockActivated(var1, var2, var3 - 1, var4, var5);
				}

				return true;
			} else {
				if(var1->getBlockId(var2, var3 + 1, var4) == blockID) {
					var1->setBlockMetadataWithNotify(var2, var3 + 1, var4, (var6 ^ 4) + 8);
				}

				var1->setBlockMetadataWithNotify(var2, var3, var4, var6 ^ 4);
				var1->func_701_b(var2, var3 - 1, var4, var2, var3, var4);
				if(((float)::rand() / (float)RAND_MAX) < 0.5f) {
					var1->playSoundEffect((double)var2 + 0.5f, (double)var3 + 0.5f, (double)var4 + 0.5f, "random.door_open", 1.0F, var1->rand->nextFloat() * 0.1F + 0.9F);
				} else {
					var1->playSoundEffect((double)var2 + 0.5f, (double)var3 + 0.5f, (double)var4 + 0.5f, "random.door_close", 1.0F, var1->rand->nextFloat() * 0.1F + 0.9F);
				}

				return true;
			}
		}
}

void BlockDoor::func_311_a(World* var1, int var2, int var3, int var4, bool var5) {
		int var6 = var1->getBlockMetadata(var2, var3, var4);
		if((var6 & 8) != 0) {
			if(var1->getBlockId(var2, var3 - 1, var4) == blockID) {
				func_311_a(var1, var2, var3 - 1, var4, var5);
			}

		} else {
			bool var7 = (var1->getBlockMetadata(var2, var3, var4) & 4) > 0;
			if(var7 != var5) {
				if(var1->getBlockId(var2, var3 + 1, var4) == blockID) {
					var1->setBlockMetadataWithNotify(var2, var3 + 1, var4, (var6 ^ 4) + 8);
				}

				var1->setBlockMetadataWithNotify(var2, var3, var4, var6 ^ 4);
				var1->func_701_b(var2, var3 - 1, var4, var2, var3, var4);
				if(((float)::rand() / (float)RAND_MAX) < 0.5f) {
					var1->playSoundEffect((double)var2 + 0.5f, (double)var3 + 0.5f, (double)var4 + 0.5f, "random.door_open", 1.0F, var1->rand->nextFloat() * 0.1F + 0.9F);
				} else {
					var1->playSoundEffect((double)var2 + 0.5f, (double)var3 + 0.5f, (double)var4 + 0.5f, "random.door_close", 1.0F, var1->rand->nextFloat() * 0.1F + 0.9F);
				}

			}
		}
}

void BlockDoor::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = var1->getBlockMetadata(var2, var3, var4);
		if((var6 & 8) != 0) {
			if(var1->getBlockId(var2, var3 - 1, var4) != blockID) {
				var1->setBlockWithNotify(var2, var3, var4, 0);
			}

			if(var5 > 0 && Block::blocksList[var5]->canProvidePower()) {
				onNeighborBlockChange(var1, var2, var3 - 1, var4, var5);
			}
		} else {
			bool var7 = false;
			if(var1->getBlockId(var2, var3 + 1, var4) != blockID) {
				var1->setBlockWithNotify(var2, var3, var4, 0);
				var7 = true;
			}

			if(!var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
				var1->setBlockWithNotify(var2, var3, var4, 0);
				var7 = true;
				if(var1->getBlockId(var2, var3 + 1, var4) == blockID) {
					var1->setBlockWithNotify(var2, var3 + 1, var4, 0);
				}
			}

			if(var7) {
				dropBlockAsItem(var1, var2, var3, var4, var6);
			} else if(var5 > 0 && Block::blocksList[var5]->canProvidePower()) {
				bool var8 = var1->isBlockIndirectlyGettingPowered(var2, var3, var4) || var1->isBlockIndirectlyGettingPowered(var2, var3 + 1, var4);
				func_311_a(var1, var2, var3, var4, var8);
			}
		}

}

int BlockDoor::idDropped(int var1, Random* var2) {
		return (var1 & 8) != 0 ? 0 : (m_blockMaterial == Material::s_iron ? Item::s_doorSteel->shiftedIndex : Item::s_doorWood->shiftedIndex);
}

MovingObjectPosition* BlockDoor::collisionRayTrace(World* var1, int var2, int var3, int var4, Vec3D* var5, Vec3D* var6) {
		setBlockBoundsBasedOnState(var1, var2, var3, var4);
		return Block::collisionRayTrace(var1, var2, var3, var4, var5, var6);
}

int BlockDoor::func_312_c(int var1) {
		return (var1 & 4) == 0 ? var1 - 1 & 3 : var1 & 3;
}

bool BlockDoor::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return var3 >= 127 ? false : var1->isBlockOpaqueCube(var2, var3 - 1, var4) && Block::canPlaceBlockAt(var1, var2, var3, var4) && Block::canPlaceBlockAt(var1, var2, var3 + 1, var4);
}
