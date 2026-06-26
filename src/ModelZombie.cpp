#include "stdafx.h"
// Auto-generated from ModelZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelZombie.h"
#include "Common.h"
#include "ModelBiped.h"
#include "Zombie.h"
#include "ModelRenderer.h"

ModelZombie::ModelZombie()
{
}

ModelZombie::~ModelZombie() {
	// No dynamic resources
}

void ModelZombie::setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
		ModelBiped::setRotationAngles(var1, var2, var3, var4, var5, var6);
		var1 = sinf(0.0F);
		var2 = sinf(0.0F);
		m_a->m_rotateAngleZ = 0.0F;
		m_b->m_rotateAngleZ = 0.0F;
		m_a->m_rotateAngleY = -(0.1F - var1 * 0.6F);
		m_b->m_rotateAngleY = 0.1F - var1 * 0.6F;
		m_a->m_rotateAngleX = 3.14159265358979323846f * -0.5F;
		m_b->m_rotateAngleX = 3.14159265358979323846f * -0.5F;
		m_a->m_rotateAngleX -= var1 * 1.2F - var2 * 0.4F;
		m_b->m_rotateAngleX -= var1 * 1.2F - var2 * 0.4F;
		m_a->m_rotateAngleZ += cosf(var3 * 0.09F) * 0.05F + 0.05F;
		m_b->m_rotateAngleZ -= cosf(var3 * 0.09F) * 0.05F + 0.05F;
		m_a->m_rotateAngleX += sinf(var3 * 0.067F) * 0.05F;
		m_b->m_rotateAngleX -= sinf(var3 * 0.067F) * 0.05F;
}
