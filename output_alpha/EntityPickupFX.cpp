#include "stdafx.h"
// Auto-generated from EntityPickupFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPickupFX.h"
#include "Common.h"
#include "Entity.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"
#include "RenderManager.h"

EntityPickupFX::EntityPickupFX()
	: m_field_675_a(NULL)
	, m_field_679_o(NULL)
	, m_field_678_p(0)
	, m_field_677_q(0)
	, m_field_676_r(0)
{
}

EntityPickupFX::EntityPickupFX(World* var1, Entity* var2, Entity* var3, float var4)
	: EntityFX(var1, var2->posX, var2->posY, var2->posZ, var2->motionX, var2->motionY, var2->motionZ)
	, m_field_675_a(NULL)
	, m_field_679_o(NULL)
	, m_field_678_p(0)
	, m_field_677_q(0)
	, m_field_676_r(0)
{
		m_field_675_a = var2;
		m_field_679_o = var3;
		m_field_677_q = 3;
		m_field_676_r = var4;
}

EntityPickupFX::~EntityPickupFX() {
	m_field_675_a = NULL;
	m_field_679_o = NULL;
}

void EntityPickupFX::func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = ((float)m_field_678_p + var2) / (float)m_field_677_q;
		var8 *= var8;
		double var9 = m_field_675_a->posX;
		double var11 = m_field_675_a->posY;
		double var13 = m_field_675_a->posZ;
		double var15 = m_field_679_o->lastTickPosX + (m_field_679_o->posX - m_field_679_o->lastTickPosX) * (double)var2;
		double var17 = m_field_679_o->lastTickPosY + (m_field_679_o->posY - m_field_679_o->lastTickPosY) * (double)var2 + (double)m_field_676_r;
		double var19 = m_field_679_o->lastTickPosZ + (m_field_679_o->posZ - m_field_679_o->lastTickPosZ) * (double)var2;
		double var21 = var9 + (var15 - var9) * (double)var8;
		double var23 = var11 + (var17 - var11) * (double)var8;
		double var25 = var13 + (var19 - var13) * (double)var8;
		int var27 = (int)floor(var21);
		int var28 = (int)floor(var23 + (double)(yOffset / 2.0F));
		int var29 = (int)floor(var25);
		float var30 = worldObj->getLightBrightness(var27, var28, var29);
		var21 -= EntityFX::s_field_660_l;
		var23 -= EntityFX::s_field_659_m;
		var25 -= EntityFX::s_field_658_n;
  Tesselator::getInstance().color4f(var30, var30, var30, 1.0F);
		RenderManager::instance->func_853_a(m_field_675_a, (double)((float)var21), (double)((float)var23), (double)((float)var25), m_field_675_a->rotationYaw, var2);
}

void EntityPickupFX::onUpdate() {
		++m_field_678_p;
		if(m_field_678_p == m_field_677_q) {
			setEntityDead();
		}

}

int EntityPickupFX::func_404_c() {
		return 3;
}
