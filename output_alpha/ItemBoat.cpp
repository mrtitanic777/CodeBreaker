#include "stdafx.h"
// Auto-generated from ItemBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemBoat.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "Item.h"
#include "MovingObjectPosition.h"
#include "Vec3D.h"
#include "EntityBoat.h"

ItemBoat::ItemBoat()
{
}

ItemBoat::ItemBoat(int var1)
	: Item(var1)
{
		maxStackSize = 1;
}

ItemBoat::~ItemBoat() {
	// No dynamic resources
}

ItemStack* ItemBoat::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3) {
		float var4 = 1.0F;
		float var5 = var3->prevRotationPitch + (var3->rotationPitch - var3->prevRotationPitch) * var4;
		float var6 = var3->prevRotationYaw + (var3->rotationYaw - var3->prevRotationYaw) * var4;
		double var7 = var3->prevPosX + (var3->posX - var3->prevPosX) * (double)var4;
		double var9 = var3->prevPosY + (var3->posY - var3->prevPosY) * (double)var4 + 1.62f - (double)var3->yOffset;
		double var11 = var3->prevPosZ + (var3->posZ - var3->prevPosZ) * (double)var4;
		Vec3D* var13 = Vec3D::createVector(var7, var9, var11);
		float var14 = cosf(-var6 * (3.14159265358979323846f / 180.0F) - 3.14159265358979323846f);
		float var15 = sinf(-var6 * (3.14159265358979323846f / 180.0F) - 3.14159265358979323846f);
		float var16 = -cosf(-var5 * (3.14159265358979323846f / 180.0F));
		float var17 = sinf(-var5 * (3.14159265358979323846f / 180.0F));
		float var18 = var15 * var16;
		float var20 = var14 * var16;
		double var21 = 5.0f;
		Vec3D* var23 = var13->addVector((double)var18 * var21, (double)var17 * var21, (double)var20 * var21);
		MovingObjectPosition* var24 = var2->rayTraceBlocks(var13, var23, true);
		if(var24 == NULL) {
			return var1;
		} else {
			if(var24->m_typeOfHit == 0) {
				int var25 = var24->m_blockX;
				int var26 = var24->m_blockY;
				int var27 = var24->m_blockZ;
				if(!var2->m_multiplayerWorld) {
					var2->entityJoinedWorld(new EntityBoat(var2, (double)((float)var25 + 0.5F), (double)((float)var26 + 1.5F), (double)((float)var27 + 0.5F)));
				}

				--var1->stackSize;
			}

			return var1;
		}
}
