#include "stdafx.h"
// Auto-generated from EntityPortalFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPortalFX.h"
#include "Common.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"
#include "Tesselator.h"

EntityPortalFX::EntityPortalFX()
	: m_field_4083_a(0)
	, m_field_4086_p(0)
	, m_field_4085_q(0)
	, m_field_4084_r(0)
{
}

EntityPortalFX::EntityPortalFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12)
	: EntityFX(var1, var2, var4, var6, var8, var10, var12)
	, m_field_4083_a(0)
	, m_field_4086_p(0)
	, m_field_4085_q(0)
	, m_field_4084_r(0)
{
		motionX = var8;
		motionY = var10;
		motionZ = var12;
		m_field_4086_p = posX = var2;
		m_field_4085_q = posY = var4;
		m_field_4084_r = posZ = var6;
		float var14 = rand->nextFloat() * 0.6F + 0.4F;
		m_field_4083_a = m_field_665_g = rand->nextFloat() * 0.2F + 0.5F;
		m_field_663_i = m_field_662_j = m_field_661_k = 1.0F * var14;
		m_field_662_j *= 0.3F;
		m_field_663_i *= 0.9F;
		m_field_666_f = (int)(((float)::rand() / (float)RAND_MAX) * 10.0f) + 40;
		m_field_9314_ba = true;
		m_field_670_b = (int)(((float)::rand() / (float)RAND_MAX) * 8.0f);
}

EntityPortalFX::~EntityPortalFX() {
	// No dynamic resources
}

void EntityPortalFX::func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = ((float)m_e + var2) / (float)m_field_666_f;
		var8 = 1.0F - var8;
		var8 *= var8;
		var8 = 1.0F - var8;
		m_field_665_g = m_field_4083_a * var8;
		EntityFX::func_406_a(var1, var2, var3, var4, var5, var6, var7);
}

float EntityPortalFX::getEntityBrightness(float var1) {
		float var2 = EntityFX::getEntityBrightness(var1);
		float var3 = (float)m_e / (float)m_field_666_f;
		var3 *= var3;
		var3 *= var3;
		return var2 * (1.0F - var3) + var3;
}

void EntityPortalFX::onUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		float var1 = (float)m_e / (float)m_field_666_f;
		float var2 = var1;
		var1 = -var1 + var1 * var1 * 2.0F;
		var1 = 1.0F - var1;
		posX = m_field_4086_p + motionX * (double)var1;
		posY = m_field_4085_q + motionY * (double)var1 + (double)(1.0F - var2);
		posZ = m_field_4084_r + motionZ * (double)var1;
		if(m_e++ >= m_field_666_f) {
			setEntityDead();
		}

}
