#include "stdafx.h"
// Auto-generated from Frustrum.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Frustrum.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "ClippingHelperFaces.h"
#include "EntityLiving.h"

Frustrum::Frustrum()
	: m_clippingHelper(new ClippingHelperFaces[6])
{
}

Frustrum::Frustrum(EntityLiving* var1, float var2, float var3)
	: m_clippingHelper(new ClippingHelperFaces[6])
{
		float var4 = var1->prevRotationYaw + (var1->rotationYaw - var1->prevRotationYaw) * var3;
		float var5 = var1->prevRotationPitch + (var1->rotationPitch - var1->prevRotationPitch) * var3;
		float var6 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * var3;
		float var7 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * var3;
		float var10 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * var3;
		var3 = sinf(var4 / 180.0F * 3.14159265358979323846f) * cosf(var5 / 180.0F * 3.14159265358979323846f);
		float var8 = -cosf(var4 / 180.0F * 3.14159265358979323846f) * cosf(var5 / 180.0F * 3.14159265358979323846f);
		float var9 = -sinf(var5 / 180.0F * 3.14159265358979323846f);
		m_clippingHelper[0] = ClippingHelperFaces(var6, var7, var10, var4, var5);
		m_clippingHelper[1] = ClippingHelperFaces(var6, var7, var10, var4 + 30.0F, var5);
		m_clippingHelper[2] = ClippingHelperFaces(var6, var7, var10, var4 - 30.0F, var5);
		m_clippingHelper[3] = ClippingHelperFaces(var6, var7, var10, var4, var5 + 45.0F);
		m_clippingHelper[4] = ClippingHelperFaces(var6, var7, var10, var4, var5 - 45.0F);
		m_clippingHelper[5] = ClippingHelperFaces(var6 + var3 * var2, var7 + var9 * var2, var10 + var8 * var2, var4 + 180.0F, -var5);
}

Frustrum::~Frustrum() {
	delete[] m_clippingHelper;
	m_clippingHelper = NULL;
}

bool Frustrum::isBoundingBoxInFrustrum(AxisAlignedBB var1) {
		float var2;
		float var3;
		float var4;
		float var5;
		float var6;
		float var7;
		float var8;
		float var11;
		float var12;
		float var13;
		float var10000;
		label63: {
			var7 = var1.maxZ;
			var6 = var1.maxY;
			var5 = var1.maxX;
			var4 = var1.minZ;
			var3 = var1.minY;
			var2 = var1.minX;
			var8 = (var5 - var2) / 2.0F;
			float var9 = (var6 - var3) / 2.0F;
			float var10 = (var7 - var4) / 2.0F;
			var11 = var2 + var8;
			var12 = var3 + var9;
			var13 = var4 + var10;
			if(var8 > var9) {
				if(var8 <= var10) {
					var10000 = var8;
					break label63;
				}
			} else if(var9 > var10) {
				var10000 = var9;
				break label63;
			}

			var10000 = var10;
		}

		var8 = var10000 * 1.5F;
		return !m_clippingHelper[0].isVisible(var11, var12, var13, var8) ? false : (!m_clippingHelper[1].isVisible(var11, var12, var13, var8) ? false : (!m_clippingHelper[2].isVisible(var11, var12, var13, var8) ? false : (!m_clippingHelper[3].isVisible(var11, var12, var13, var8) ? false : (!m_clippingHelper[4].isVisible(var11, var12, var13, var8) ? false : (!m_clippingHelper[5].isVisible(var11, var12, var13, var8) ? false : (!m_clippingHelper[0].isVisible(var2, var3, var4, var5, var6, var7) ? false : (!m_clippingHelper[1].isVisible(var2, var3, var4, var5, var6, var7) ? false : (!m_clippingHelper[2].isVisible(var2, var3, var4, var5, var6, var7) ? false : (!m_clippingHelper[3].isVisible(var2, var3, var4, var5, var6, var7) ? false : (!m_clippingHelper[4].isVisible(var2, var3, var4, var5, var6, var7) ? false : m_clippingHelper[5].isVisible(var2, var3, var4, var5, var6, var7)))))))))));
}
