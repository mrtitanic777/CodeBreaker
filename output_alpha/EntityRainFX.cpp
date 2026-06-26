#include "stdafx.h"
// Auto-generated from EntityRainFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityRainFX.h"
#include "Common.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"
#include "Material.h"
#include "Tesselator.h"
#include "BlockFluids.h"

EntityRainFX::EntityRainFX()
{
}

EntityRainFX::EntityRainFX(World* var1, double var2, double var4, double var6)
	: EntityFX(var1, var2, var4, var6, 0.0f, 0.0f, 0.0f)
{
		motionX *= (double)0.3F;
		motionY = (double)((float)((float)::rand() / (float)RAND_MAX) * 0.2F + 0.1F);
		motionZ *= (double)0.3F;
		m_field_663_i = 1.0F;
		m_field_662_j = 1.0F;
		m_field_661_k = 1.0F;
		m_field_670_b = 19 + rand->nextInt(4);
		setSize(0.01F, 0.01F);
		m_field_664_h = 0.06F;
		m_field_666_f = (int)(8.0f / (((float)::rand() / (float)RAND_MAX) * 0.8f + 0.2f));
}

EntityRainFX::~EntityRainFX() {
	// No dynamic resources
}

void EntityRainFX::func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		EntityFX::func_406_a(var1, var2, var3, var4, var5, var6, var7);
}

void EntityRainFX::onUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		motionY -= (double)m_field_664_h;
		moveEntity(motionX, motionY, motionZ);
		motionX *= (double)0.98F;
		motionY *= (double)0.98F;
		motionZ *= (double)0.98F;
		if(m_field_666_f-- <= 0) {
			setEntityDead();
		}

		if(onGround) {
			if(((float)::rand() / (float)RAND_MAX) < 0.5f) {
				setEntityDead();
			}

			motionX *= (double)0.7F;
			motionZ *= (double)0.7F;
		}

		Material* var1 = worldObj->getBlockMaterial((int)floor(posX), (int)floor(posY), (int)floor(posZ));
		if(var1->getIsLiquid() || var1->func_878_a()) {
			double var2 = (double)((float)((int)floor(posY) + 1) - BlockFluids::func_288_b(worldObj->getBlockMetadata((int)floor(posX), (int)floor(posY), (int)floor(posZ))));
			if(posY < var2) {
				setEntityDead();
			}
		}

}
