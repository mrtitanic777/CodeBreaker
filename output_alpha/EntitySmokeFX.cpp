#include "stdafx.h"
// Auto-generated from EntitySmokeFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySmokeFX.h"
#include "Common.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"
#include "Tesselator.h"

EntitySmokeFX::EntitySmokeFX()
	: m_field_671_a(0)
{
}

EntitySmokeFX::EntitySmokeFX(World* var1, double var2, double var4, double var6)
	: m_field_671_a(0)
{
  // PORT: constructor delegation  // was: this(var1, var2, var4, var6, 1.0F);
}

EntitySmokeFX::EntitySmokeFX(World* var1, double var2, double var4, double var6, float var8)
	: EntityFX(var1, var2, var4, var6, 0.0f, 0.0f, 0.0f)
	, m_field_671_a(0)
{
		motionX *= (double)0.1F;
		motionY *= (double)0.1F;
		motionZ *= (double)0.1F;
		m_field_663_i = m_field_662_j = m_field_661_k = (float)(((float)::rand() / (float)RAND_MAX) * (double)0.3F);
		m_field_665_g *= 12.0F / 16.0F;
		m_field_665_g *= var8;
		m_field_671_a = m_field_665_g;
		m_field_666_f = (int)(8.0f / (((float)::rand() / (float)RAND_MAX) * 0.8f + 0.2f));
		m_field_666_f = (int)((float)m_field_666_f * var8);
		m_field_9314_ba = false;
}

EntitySmokeFX::~EntitySmokeFX() {
	// No dynamic resources
}

void EntitySmokeFX::func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = ((float)m_e + var2) / (float)m_field_666_f * 32.0F;
		if(var8 < 0.0F) {
			var8 = 0.0F;
		}

		if(var8 > 1.0F) {
			var8 = 1.0F;
		}

		m_field_665_g = m_field_671_a * var8;
		EntityFX::func_406_a(var1, var2, var3, var4, var5, var6, var7);
}

void EntitySmokeFX::onUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		if(m_e++ >= m_field_666_f) {
			setEntityDead();
		}

		m_field_670_b = 7 - m_e * 8 / m_field_666_f;
		motionY += 0.004f;
		moveEntity(motionX, motionY, motionZ);
		if(posY == prevPosY) {
			motionX *= 1.1f;
			motionZ *= 1.1f;
		}

		motionX *= (double)0.96F;
		motionY *= (double)0.96F;
		motionZ *= (double)0.96F;
		if(onGround) {
			motionX *= (double)0.7F;
			motionZ *= (double)0.7F;
		}

}
