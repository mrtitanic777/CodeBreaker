#include "stdafx.h"
// Auto-generated from EntitySlimeFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySlimeFX.h"
#include "Common.h"
#include "Item.h"
#include "Tesselator.h"
#include "World.h"
#include "EntityFX.h"
#include "ItemStack.h"
#include "Block.h"

EntitySlimeFX::EntitySlimeFX()
{
}

EntitySlimeFX::EntitySlimeFX(World* var1, double var2, double var4, double var6, Item* var8)
	: EntityFX(var1, var2, var4, var6, 0.0f, 0.0f, 0.0f)
{
		m_field_670_b = var8->getIconIndex(NULL);
		m_field_663_i = m_field_662_j = m_field_661_k = 1.0F;
		m_field_664_h = Block::s_blockSnow->m_field_357_bm;
		m_field_665_g /= 2.0F;
}

EntitySlimeFX::~EntitySlimeFX() {
	// No dynamic resources
}

int EntitySlimeFX::func_404_c() {
		return 2;
}

void EntitySlimeFX::func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = ((float)(m_field_670_b % 16) + m_field_669_c / 4.0F) / 16.0F;
		float var9 = var8 + 0.999F / 64.0F;
		float var10 = ((float)(m_field_670_b / 16) + m_field_668_d / 4.0F) / 16.0F;
		float var11 = var10 + 0.999F / 64.0F;
		float var12 = 0.1F * m_field_665_g;
		float var13 = (float)(prevPosX + (posX - prevPosX) * (double)var2 - EntityFX::s_field_660_l);
		float var14 = (float)(prevPosY + (posY - prevPosY) * (double)var2 - EntityFX::s_field_659_m);
		float var15 = (float)(prevPosZ + (posZ - prevPosZ) * (double)var2 - EntityFX::s_field_658_n);
		float var16 = getEntityBrightness(var2);
		var1->color3f(var16 * m_field_663_i, var16 * m_field_662_j, var16 * m_field_661_k);
		var1->vertexUV((double)(var13 - var3 * var12 - var6 * var12), (double)(var14 - var4 * var12), (double)(var15 - var5 * var12 - var7 * var12), (double)var8, (double)var11);
		var1->vertexUV((double)(var13 - var3 * var12 + var6 * var12), (double)(var14 + var4 * var12), (double)(var15 - var5 * var12 + var7 * var12), (double)var8, (double)var10);
		var1->vertexUV((double)(var13 + var3 * var12 + var6 * var12), (double)(var14 + var4 * var12), (double)(var15 + var5 * var12 + var7 * var12), (double)var9, (double)var10);
		var1->vertexUV((double)(var13 + var3 * var12 - var6 * var12), (double)(var14 - var4 * var12), (double)(var15 + var5 * var12 - var7 * var12), (double)var9, (double)var11);
}
