#include "stdafx.h"
// Auto-generated from ModelMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelMinecart.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelMinecart::ModelMinecart()
	: m_field_1256_a(new ModelRenderer[7])
{
		m_field_1256_a[0] = ModelRenderer(0, 10);
		m_field_1256_a[1] = ModelRenderer(0, 0);
		m_field_1256_a[2] = ModelRenderer(0, 0);
		m_field_1256_a[3] = ModelRenderer(0, 0);
		m_field_1256_a[4] = ModelRenderer(0, 0);
		m_field_1256_a[5] = ModelRenderer(44, 10);
		unsigned char var1 = 20;
		unsigned char var2 = 8;
		unsigned char var3 = 16;
		unsigned char var4 = 4;
		m_field_1256_a[0].addBox((float)(-var1 / 2), (float)(-var3 / 2), -1.0F, var1, var3, 2, 0.0F);
		m_field_1256_a[0].setPosition(0.0F, (float)(0 + var4), 0.0F);
		m_field_1256_a[5].addBox((float)(-var1 / 2 + 1), (float)(-var3 / 2 + 1), -1.0F, var1 - 2, var3 - 2, 1, 0.0F);
		m_field_1256_a[5].setPosition(0.0F, (float)(0 + var4), 0.0F);
		m_field_1256_a[1].addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F);
		m_field_1256_a[1].setPosition((float)(-var1 / 2 + 1), (float)(0 + var4), 0.0F);
		m_field_1256_a[2].addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F);
		m_field_1256_a[2].setPosition((float)(var1 / 2 - 1), (float)(0 + var4), 0.0F);
		m_field_1256_a[3].addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F);
		m_field_1256_a[3].setPosition(0.0F, (float)(0 + var4), (float)(-var3 / 2 + 1));
		m_field_1256_a[4].addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F);
		m_field_1256_a[4].setPosition(0.0F, (float)(0 + var4), (float)(var3 / 2 - 1));
		m_field_1256_a[0].m_rotateAngleX = 3.14159265358979323846f * 0.5F;
		m_field_1256_a[1].m_rotateAngleY = 3.14159265358979323846f * 3.0F / 2.0F;
		m_field_1256_a[2].m_rotateAngleY = 3.14159265358979323846f * 0.5F;
		m_field_1256_a[3].m_rotateAngleY = 3.14159265358979323846f;
		m_field_1256_a[5].m_rotateAngleX = 3.14159265358979323846f * -0.5F;
}

ModelMinecart::~ModelMinecart() {
	delete[] m_field_1256_a;
	m_field_1256_a = NULL;
}

void ModelMinecart::render(float var1, float var2, float var3, float var4, float var5, float var6) {
		m_field_1256_a[5].m_offsetY = 4.0F - var3;

		for(int var7 = 0; var7 < 6; ++var7) {
			m_field_1256_a[var7].render(var6);
		}

}

void ModelMinecart::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
	// TODO: implement
}
