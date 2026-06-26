#include "stdafx.h"
// Hand-fixed from ItemBucket.java (Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemBucket.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "WorldProvider.h"
#include "Block.h"
#include "Item.h"
#include "Material.h"
#include "MovingObjectPosition.h"
#include "Vec3D.h"
#include "EntityCow.h"

ItemBucket::ItemBucket()
	: m_isFull(0)
{
}

ItemBucket::ItemBucket(int var1, int var2)
	: Item(var1)
	, m_isFull(0)
{
	maxStackSize = 1;
	maxDamage = 64;
	m_isFull = var2;
}

ItemBucket::~ItemBucket() {
	// No dynamic resources
}

ItemStack* ItemBucket::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3) {
	float var4 = 1.0F;
	float var5 = var3->prevRotationPitch + (var3->rotationPitch - var3->prevRotationPitch) * var4;
	float var6 = var3->prevRotationYaw + (var3->rotationYaw - var3->prevRotationYaw) * var4;
	double var7 = var3->prevPosX + (var3->posX - var3->prevPosX) * (double)var4;
	double var9 = var3->prevPosY + (var3->posY - var3->prevPosY) * (double)var4 + 1.62 - (double)var3->yOffset;
	double var11 = var3->prevPosZ + (var3->posZ - var3->prevPosZ) * (double)var4;
	Vec3D* var13 = Vec3D::createVector(var7, var9, var11);
	float var14 = cosf(-var6 * (3.14159265358979323846f / 180.0F) - 3.14159265358979323846f);
	float var15 = sinf(-var6 * (3.14159265358979323846f / 180.0F) - 3.14159265358979323846f);
	float var16 = -cosf(-var5 * (3.14159265358979323846f / 180.0F));
	float var17 = sinf(-var5 * (3.14159265358979323846f / 180.0F));
	float var18 = var15 * var16;
	float var20 = var14 * var16;
	double var21 = 5.0;
	Vec3D* var23 = var13->addVector((double)var18 * var21, (double)var17 * var21, (double)var20 * var21);
	MovingObjectPosition* var24 = var2->rayTraceBlocks(var13, var23, m_isFull == 0);
	if(var24 == NULL) {
		return var1;
	} else {
		if(var24->m_typeOfHit == 0) {
			int var25 = var24->m_blockX;
			int var26 = var24->m_blockY;
			int var27 = var24->m_blockZ;
			if(!var2->func_6466_a(var3, var25, var26, var27)) {
				return var1;
			}

			if(m_isFull == 0) {
				if(var2->getBlockMaterial(var25, var26, var27) == Material::s_water && var2->getBlockMetadata(var25, var26, var27) == 0) {
					var2->setBlockWithNotify(var25, var26, var27, 0);
					return new ItemStack(Item::s_bucketWater);
				}

				if(var2->getBlockMaterial(var25, var26, var27) == Material::s_lava && var2->getBlockMetadata(var25, var26, var27) == 0) {
					var2->setBlockWithNotify(var25, var26, var27, 0);
					return new ItemStack(Item::s_bucketLava);
				}
			} else {
				if(m_isFull < 0) {
					return new ItemStack(Item::s_bucketEmpty);
				}

				if(var24->m_sideHit == 0) {
					--var26;
				}

				if(var24->m_sideHit == 1) {
					++var26;
				}

				if(var24->m_sideHit == 2) {
					--var27;
				}

				if(var24->m_sideHit == 3) {
					++var27;
				}

				if(var24->m_sideHit == 4) {
					--var25;
				}

				if(var24->m_sideHit == 5) {
					++var25;
				}

				if(var2->getBlockId(var25, var26, var27) == 0 || !var2->getBlockMaterial(var25, var26, var27)->func_878_a()) {
					if(var2->m_worldProvider->m_field_6479_d && m_isFull == Block::s_waterStill->blockID) {
						var2->playSoundEffect(var7 + 0.5, var9 + 0.5, var11 + 0.5, "random.fizz", 0.5F, 2.6F + (var2->rand->nextFloat() - var2->rand->nextFloat()) * 0.8F);

						for(int var28 = 0; var28 < 8; ++var28) {
							var2->spawnParticle("largesmoke", (double)var25 + ((float)::rand() / (float)RAND_MAX), (double)var26 + ((float)::rand() / (float)RAND_MAX), (double)var27 + ((float)::rand() / (float)RAND_MAX), 0.0, 0.0, 0.0);
						}
					} else {
						var2->setBlockAndMetadataWithNotify(var25, var26, var27, m_isFull, 0);
					}

					return new ItemStack(Item::s_bucketEmpty);
				}
			}
		} else if(m_isFull == 0 && dynamic_cast<EntityCow*>(var24->m_entityHit) != NULL) {
			return new ItemStack(Item::s_bucketMilk);
		}

		return var1;
	}
}
