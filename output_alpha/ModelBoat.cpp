#include "stdafx.h"
// Auto-generated from ModelBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBoat.h"
#include "Common.h"
#include "ModelRenderer.h"

ModelBoat::ModelBoat()
	: m_field_1287_a(new ModelRenderer[5])
{
		m_field_1287_a[0] = ModelRenderer(0, 8);
		m_field_1287_a[1] = ModelRenderer(0, 0);
		m_field_1287_a[2] = ModelRenderer(0, 0);
		m_field_1287_a[3] = ModelRenderer(0, 0);
		m_field_1287_a[4] = ModelRenderer(0, 0);
		unsigned char var1 = 24;
		unsigned char var2 = 6;
		unsigned char var3 = 20;
		unsigned char var4 = 4;
		m_field_1287_a[0].addBox((float)(-var1 / 2), (float)(-var3 / 2 + 2), -3.0F, var1, var3 - 4, 4, 0.0F);
		m_field_1287_a[0].setPosition(0.0F, (float)(0 + var4), 0.0F);
		m_field_1287_a[1].addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F);
		m_field_1287_a[1].setPosition((float)(-var1 / 2 + 1), (float)(0 + var4), 0.0F);
		m_field_1287_a[2].addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F);
		m_field_1287_a[2].setPosition((float)(var1 / 2 - 1), (float)(0 + var4), 0.0F);
		m_field_1287_a[3].addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F);
		m_field_1287_a[3].setPosition(0.0F, (float)(0 + var4), (float)(-var3 / 2 + 1));
		m_field_1287_a[4].addBox((float)(-var1 / 2 + 2), (float)(-var2 - 1), -1.0F, var1 - 4, var2, 2, 0.0F);
		m_field_1287_a[4].setPosition(0.0F, (float)(0 + var4), (float)(var3 / 2 - 1));
		m_field_1287_a[0].m_rotateAngleX = 3.14159265358979323846f * 0.5F;
		m_field_1287_a[1].m_rotateAngleY = 3.14159265358979323846f * 3.0F / 2.0F;
		m_field_1287_a[2].m_rotateAngleY = 3.14159265358979323846f * 0.5F;
		m_field_1287_a[3].m_rotateAngleY = 3.14159265358979323846f;
}

ModelBoat::~ModelBoat() {
	delete[] m_field_1287_a;
	m_field_1287_a = NULL;
}

void ModelBoat::render(float var1, float var2, float var3, float var4, float var5, float var6) {
		for(int var7 = 0; var7 < 5; ++var7) {
			m_field_1287_a[var7].render(var6);
		}

}

void ModelBoat::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
	// TODO: implement
}
