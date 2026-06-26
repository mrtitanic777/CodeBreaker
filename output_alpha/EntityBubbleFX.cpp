#include "stdafx.h"
// Auto-generated from EntityBubbleFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityBubbleFX.h"
#include "Common.h"
#include "World.h"
#include "EntityFX.h"
#include "Material.h"

EntityBubbleFX::EntityBubbleFX()
{
}

EntityBubbleFX::EntityBubbleFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12)
	: EntityFX(var1, var2, var4, var6, var8, var10, var12)
{
		m_field_663_i = 1.0F;
		m_field_662_j = 1.0F;
		m_field_661_k = 1.0F;
		m_field_670_b = 32;
		setSize(0.02F, 0.02F);
		m_field_665_g *= rand->nextFloat() * 0.6F + 0.2F;
		motionX = var8 * (double)0.2F + (double)((float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.02F);
		motionY = var10 * (double)0.2F + (double)((float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.02F);
		motionZ = var12 * (double)0.2F + (double)((float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.02F);
		m_field_666_f = (int)(8.0f / (((float)::rand() / (float)RAND_MAX) * 0.8f + 0.2f));
}

EntityBubbleFX::~EntityBubbleFX() {
	// No dynamic resources
}

void EntityBubbleFX::onUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		motionY += 0.002f;
		moveEntity(motionX, motionY, motionZ);
		motionX *= (double)0.85F;
		motionY *= (double)0.85F;
		motionZ *= (double)0.85F;
		if(worldObj->getBlockMaterial((int)floor(posX), (int)floor(posY), (int)floor(posZ)) != Material::s_water) {
			setEntityDead();
		}

		if(m_field_666_f-- <= 0) {
			setEntityDead();
		}

}
