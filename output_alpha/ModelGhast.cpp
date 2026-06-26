#include "stdafx.h"
// Auto-generated from ModelGhast.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelGhast.h"
#include "Common.h"
#include "ModelRenderer.h"
#include "Random.h"

ModelGhast::ModelGhast()
	: body(NULL)
	, m_tentacles(new ModelRenderer[9])
{
		unsigned char var1 = -16;
		body = new ModelRenderer(0, 0);
		body->func_921_a(-8.0F, -8.0F, -8.0F, 16, 16, 16);
		body->m_offsetY += (float)(24 + var1);
		Random* var2 = new Random(1660L);

		for(int var3 = 0; var3 < m_tentacles_count /* PORT: set this from allocation */; ++var3) {
			m_tentacles[var3] = ModelRenderer(0, 0);
			float var4 = (((float)(var3 % 3) - (float)(var3 / 3 % 2) * 0.5F + 0.25F) / 2.0F * 2.0F - 1.0F) * 5.0F;
			float var5 = ((float)(var3 / 3) / 2.0F * 2.0F - 1.0F) * 5.0F;
			int var6 = var2->nextInt(7) + 8;
			m_tentacles[var3].func_921_a(-1.0F, 0.0F, -1.0F, 2, var6, 2);
			m_tentacles[var3].m_offsetX = var4;
			m_tentacles[var3].m_offsetZ = var5;
			m_tentacles[var3].m_offsetY = (float)(31 + var1);
		}

}

ModelGhast::~ModelGhast() {
	body = NULL;
	delete[] m_tentacles;
	m_tentacles = NULL;
}

void ModelGhast::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
		for(int var7 = 0; var7 < m_tentacles_count /* PORT: set this from allocation */; ++var7) {
			m_tentacles[var7].m_rotateAngleX = 0.2F * sinf(var3 * 0.3F + (float)var7) + 0.4F;
		}

}

void ModelGhast::render(float var1, float var2, float var3, float var4, float var5, float var6) {
		setRotationAngles(var1, var2, var3, var4, var5, var6);
		body->render(var6);

		for(int var7 = 0; var7 < m_tentacles_count /* PORT: set this from allocation */; ++var7) {
			m_tentacles[var7].render(var6);
		}

}
