#include "stdafx.h"
// Auto-generated from BlockMinecartTrack.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockMinecartTrack.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "IBlockAccess.h"
#include "MovingObjectPosition.h"
#include "Random.h"
#include "Vec3D.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "MinecartTrackLogic.h"

BlockMinecartTrack::BlockMinecartTrack()
{
}

BlockMinecartTrack::BlockMinecartTrack(int var1, int var2)
	: Block(var1, var2, Material::s_circuits)
{
		setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 2.0F / 16.0F, 1.0F);
}

BlockMinecartTrack::~BlockMinecartTrack() {
	// No dynamic resources
}

AxisAlignedBB BlockMinecartTrack::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB();
}

bool BlockMinecartTrack::isOpaqueCube() {
		return false;
}

MovingObjectPosition* BlockMinecartTrack::collisionRayTrace(World* var1, int var2, int var3, int var4, Vec3D* var5, Vec3D* var6) {
		setBlockBoundsBasedOnState(var1, var2, var3, var4);
		return Block::collisionRayTrace(var1, var2, var3, var4, var5, var6);
}

void BlockMinecartTrack::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockMetadata(var2, var3, var4);
		if(var5 >= 2 && var5 <= 5) {
			setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 10.0F / 16.0F, 1.0F);
		} else {
			setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 2.0F / 16.0F, 1.0F);
		}

}

int BlockMinecartTrack::getBlockTextureFromSideAndMetadata(int var1, int var2) {
		return var2 >= 6 ? blockIndexInTexture - 16 : blockIndexInTexture;
}

bool BlockMinecartTrack::renderAsNormalBlock() {
		return false;
}

int BlockMinecartTrack::getRenderType() {
		return 9;
}

int BlockMinecartTrack::quantityDropped(Random* var1) {
		return 1;
}

bool BlockMinecartTrack::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return var1->isBlockOpaqueCube(var2, var3 - 1, var4);
}

void BlockMinecartTrack::onBlockAdded(World* var1, int var2, int var3, int var4) {
		if(!var1->m_multiplayerWorld) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 15);
			func_4031_h(var1, var2, var3, var4);
		}

}

void BlockMinecartTrack::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(!var1->m_multiplayerWorld) {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			bool var7 = false;
			if(!var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
				var7 = true;
			}

			if(var6 == 2 && !var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
				var7 = true;
			}

			if(var6 == 3 && !var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
				var7 = true;
			}

			if(var6 == 4 && !var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
				var7 = true;
			}

			if(var6 == 5 && !var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
				var7 = true;
			}

			if(var7) {
				dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
				var1->setBlockWithNotify(var2, var3, var4, 0);
			} else if(var5 > 0 && Block::blocksList[var5]->canProvidePower() && MinecartTrackLogic::func_791_a(new MinecartTrackLogic(this, var1, var2, var3, var4)) == 3) {
				func_4031_h(var1, var2, var3, var4);
			}

		}
}

void BlockMinecartTrack::func_4031_h(World* var1, int var2, int var3, int var4) {
		if(!var1->m_multiplayerWorld) {
			(new MinecartTrackLogic(this, var1, var2, var3, var4))->func_792_a(var1->isBlockIndirectlyGettingPowered(var2, var3, var4));
		}
}
