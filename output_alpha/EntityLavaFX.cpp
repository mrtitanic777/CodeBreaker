#include "stdafx.h"
// Auto-generated from EntityLavaFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityLavaFX.h"
#include "Common.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"
#include "Tesselator.h"

EntityLavaFX::EntityLavaFX()
	: m_field_674_a(0)
{
}

EntityLavaFX::EntityLavaFX(World* var1, double var2, double var4, double var6)
	: EntityFX(var1, var2, var4, var6, 0.0f, 0.0f, 0.0f)
	, m_field_674_a(0)
{
		motionX *= (double)0.8F;
		motionY *= (double)0.8F;
		motionZ *= (double)0.8F;
		motionY = (double)(rand->nextFloat() * 0.4F + 0.05F);
		m_field_663_i = m_field_662_j = m_field_661_k = 1.0F;
		m_field_665_g *= rand->nextFloat() * 2.0F + 0.2F;
		m_field_674_a = m_field_665_g;
		m_field_666_f = (int)(16.0f / (((float)::rand() / (float)RAND_MAX) * 0.8f + 0.2f));
		m_field_9314_ba = false;
		m_field_670_b = 49;
}

EntityLavaFX::~EntityLavaFX() {
	// No dynamic resources
}

float EntityLavaFX::getEntityBrightness(float var1) {
		return 1.0F;
}

void EntityLavaFX::func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = ((float)m_e + var2) / (float)m_field_666_f;
		m_field_665_g = m_field_674_a * (1.0F - var8 * var8);
		EntityFX::func_406_a(var1, var2, var3, var4, var5, var6, var7);
}

void EntityLavaFX::onUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		if(m_e++ >= m_field_666_f) {
			setEntityDead();
		}

		float var1 = (float)m_e / (float)m_field_666_f;
		if(rand->nextFloat() > var1) {
			worldObj->spawnParticle("smoke", posX, posY, posZ, motionX, motionY, motionZ);
		}

		motionY -= 0.03f;
		moveEntity(motionX, motionY, motionZ);
		motionX *= (double)0.999F;
		motionY *= (double)0.999F;
		motionZ *= (double)0.999F;
		if(onGround) {
			motionX *= (double)0.7F;
			motionZ *= (double)0.7F;
		}

}
