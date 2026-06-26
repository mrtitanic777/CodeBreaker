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
		float var7 = sinf(m_field_1244_k * 3.14159265358979323846f);
		float var8 = sinf((1.0F - (1.0F - m_field_1244_k) * (1.0F - m_field_1244_k)) * 3.14159265358979323846f);
		m_bipedRightArm->m_rotateAngleZ = 0.0F;
		m_bipedLeftArm->m_rotateAngleZ = 0.0F;
		m_bipedRightArm->m_rotateAngleY = -(0.1F - var7 * 0.6F);
		m_bipedLeftArm->m_rotateAngleY = 0.1F - var7 * 0.6F;
		m_bipedRightArm->m_rotateAngleX = 3.14159265358979323846f * -0.5F;
		m_bipedLeftArm->m_rotateAngleX = 3.14159265358979323846f * -0.5F;
		m_bipedRightArm->m_rotateAngleX -= var7 * 1.2F - var8 * 0.4F;
		m_bipedLeftArm->m_rotateAngleX -= var7 * 1.2F - var8 * 0.4F;
		m_bipedRightArm->m_rotateAngleZ += cosf(var3 * 0.09F) * 0.05F + 0.05F;
		m_bipedLeftArm->m_rotateAngleZ -= cosf(var3 * 0.09F) * 0.05F + 0.05F;
		m_bipedRightArm->m_rotateAngleX += sinf(var3 * 0.067F) * 0.05F;
		m_bipedLeftArm->m_rotateAngleX -= sinf(var3 * 0.067F) * 0.05F;
}
