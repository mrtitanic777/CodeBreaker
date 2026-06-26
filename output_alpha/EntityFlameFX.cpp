#include "stdafx.h"
// Auto-generated from EntityFlameFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFlameFX.h"
#include "Common.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"
#include "Tesselator.h"

EntityFlameFX::EntityFlameFX()
	: m_field_672_a(0)
{
}

EntityFlameFX::EntityFlameFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12)
	: EntityFX(var1, var2, var4, var6, var8, var10, var12)
	, m_field_672_a(0)
{
		motionX = motionX * (double)0.01F + var8;
		motionY = motionY * (double)0.01F + var10;
		motionZ = motionZ * (double)0.01F + var12;
		double var10000 = var2 + (double)((rand->nextFloat() - rand->nextFloat()) * 0.05F);
		var10000 = var4 + (double)((rand->nextFloat() - rand->nextFloat()) * 0.05F);
		var10000 = var6 + (double)((rand->nextFloat() - rand->nextFloat()) * 0.05F);
		m_field_672_a = m_field_665_g;
		m_field_663_i = m_field_662_j = m_field_661_k = 1.0F;
		m_field_666_f = (int)(8.0f / (((float)::rand() / (float)RAND_MAX) * 0.8f + 0.2f)) + 4;
		m_field_9314_ba = true;
		m_field_670_b = 48;
}

EntityFlameFX::~EntityFlameFX() {
	// No dynamic resources
}

void EntityFlameFX::func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = ((float)m_e + var2) / (float)m_field_666_f;
		m_field_665_g = m_field_672_a * (1.0F - var8 * var8 * 0.5F);
		EntityFX::func_406_a(var1, var2, var3, var4, var5, var6, var7);
}

float EntityFlameFX::getEntityBrightness(float var1) {
		float var2 = ((float)m_e + var1) / (float)m_field_666_f;
		if(var2 < 0.0F) {
			var2 = 0.0F;
		}

		if(var2 > 1.0F) {
			var2 = 1.0F;
		}

		float var3 = EntityFX::getEntityBrightness(var1);
		return var3 * var2 + (1.0F - var2);
}

void EntityFlameFX::onUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		if(m_e++ >= m_field_666_f) {
			setEntityDead();
		}

		moveEntity(motionX, motionY, motionZ);
		motionX *= (double)0.96F;
		motionY *= (double)0.96F;
		motionZ *= (double)0.96F;
		if(onGround) {
			motionX *= (double)0.7F;
			motionZ *= (double)0.7F;
		}

}
