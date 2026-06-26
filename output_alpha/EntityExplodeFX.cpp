#include "stdafx.h"
// Auto-generated from EntityExplodeFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityExplodeFX.h"
#include "Common.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"
#include "Tesselator.h"

EntityExplodeFX::EntityExplodeFX()
{
}

EntityExplodeFX::EntityExplodeFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12)
	: EntityFX(var1, var2, var4, var6, var8, var10, var12)
{
		motionX = var8 + (double)((float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.05F);
		motionY = var10 + (double)((float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.05F);
		motionZ = var12 + (double)((float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.05F);
		m_field_663_i = m_field_662_j = m_field_661_k = rand->nextFloat() * 0.3F + 0.7F;
		m_field_665_g = rand->nextFloat() * rand->nextFloat() * 6.0F + 1.0F;
		m_field_666_f = (int)(16.0f / ((double)rand->nextFloat() * 0.8f + 0.2f)) + 2;
}

EntityExplodeFX::~EntityExplodeFX() {
	// No dynamic resources
}

void EntityExplodeFX::func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		EntityFX::func_406_a(var1, var2, var3, var4, var5, var6, var7);
}

void EntityExplodeFX::onUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		if(m_e++ >= m_field_666_f) {
			setEntityDead();
		}

		m_field_670_b = 7 - m_e * 8 / m_field_666_f;
		motionY += 0.004f;
		moveEntity(motionX, motionY, motionZ);
		motionX *= (double)0.9F;
		motionY *= (double)0.9F;
		motionZ *= (double)0.9F;
		if(onGround) {
			motionX *= (double)0.7F;
			motionZ *= (double)0.7F;
		}

}
