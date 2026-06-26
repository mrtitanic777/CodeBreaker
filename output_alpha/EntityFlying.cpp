#include "stdafx.h"
// Auto-generated from EntityFlying.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFlying.h"
#include "Common.h"
#include "World.h"
#include "Block.h"
#include "EntityLiving.h"

EntityFlying::EntityFlying()
{
}

EntityFlying::EntityFlying(World* var1)
	: EntityLiving(var1)
{
}

EntityFlying::~EntityFlying() {
	// No dynamic resources
}

void EntityFlying::fall(float var1) {
	// TODO: implement
}

void EntityFlying::func_435_b(float var1, float var2) {
		if(handleWaterMovement()) {
			func_351_a(var1, var2, 0.02F);
			moveEntity(motionX, motionY, motionZ);
			motionX *= (double)0.8F;
			motionY *= (double)0.8F;
			motionZ *= (double)0.8F;
		} else if(handleLavaMovement()) {
			func_351_a(var1, var2, 0.02F);
			moveEntity(motionX, motionY, motionZ);
			motionX *= 0.5f;
			motionY *= 0.5f;
			motionZ *= 0.5f;
		} else {
			float var3 = 0.91F;
			if(onGround) {
				var3 = 546.0F * 0.1F * 0.1F * 0.1F;
				int var4 = worldObj->getBlockId((int)floor(posX), (int)floor(boundingBox.minY) - 1, (int)floor(posZ));
				if(var4 > 0) {
					var3 = Block::blocksList[var4]->slipperiness * 0.91F;
				}
			}

			float var8 = 0.16277136F / (var3 * var3 * var3);
			func_351_a(var1, var2, onGround ? 0.1F * var8 : 0.02F);
			var3 = 0.91F;
			if(onGround) {
				var3 = 546.0F * 0.1F * 0.1F * 0.1F;
				int var5 = worldObj->getBlockId((int)floor(posX), (int)floor(boundingBox.minY) - 1, (int)floor(posZ));
				if(var5 > 0) {
					var3 = Block::blocksList[var5]->slipperiness * 0.91F;
				}
			}

			moveEntity(motionX, motionY, motionZ);
			motionX *= (double)var3;
			motionY *= (double)var3;
			motionZ *= (double)var3;
		}

		m_field_705_Q = m_field_704_R;
		double var10 = posX - prevPosX;
		double var9 = posZ - prevPosZ;
		float var7 = sqrt(var10 * var10 + var9 * var9) * 4.0F;
		if(var7 > 1.0F) {
			var7 = 1.0F;
		}

		m_field_704_R += (var7 - m_field_704_R) * 0.4F;
		m_field_703_S += m_field_704_R;
}

bool EntityFlying::isOnLadder() {
		return false;
}
