#include "stdafx.h"
// Auto-generated from EntityFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include "Tesselator.h"
#include "World.h"
#include "Entity.h"
#include "Tesselator.h"

double EntityFX::s_field_660_l;
double EntityFX::s_field_659_m;
double EntityFX::s_field_658_n;

EntityFX::EntityFX()
	: m_field_670_b(0)
	, m_field_669_c(0)
	, m_field_668_d(0)
	, m_e(0)
	, m_field_666_f(0)
	, m_field_665_g(0)
	, m_field_664_h(0)
	, m_field_663_i(0)
	, m_field_662_j(0)
	, m_field_661_k(0)
{
}

EntityFX::EntityFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12)
	: Entity(var1)
	, m_field_670_b(0)
	, m_field_669_c(0)
	, m_field_668_d(0)
	, m_e(0)
	, m_field_666_f(0)
	, m_field_665_g(0)
	, m_field_664_h(0)
	, m_field_663_i(0)
	, m_field_662_j(0)
	, m_field_661_k(0)
{
		setSize(0.2F, 0.2F);
		yOffset = height / 2.0F;
		setPosition(var2, var4, var6);
		m_field_663_i = m_field_662_j = m_field_661_k = 1.0F;
		motionX = var8 + (double)((float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.4F);
		motionY = var10 + (double)((float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.4F);
		motionZ = var12 + (double)((float)(((float)::rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.4F);
		float var14 = (float)(((float)::rand() / (float)RAND_MAX) + ((float)::rand() / (float)RAND_MAX) + 1.0f) * 0.15F;
		float var15 = sqrt(motionX * motionX + motionY * motionY + motionZ * motionZ);
		motionX = motionX / (double)var15 * (double)var14 * (double)0.4F;
		motionY = motionY / (double)var15 * (double)var14 * (double)0.4F + (double)0.1F;
		motionZ = motionZ / (double)var15 * (double)var14 * (double)0.4F;
		m_field_669_c = rand->nextFloat() * 3.0F;
		m_field_668_d = rand->nextFloat() * 3.0F;
		m_field_665_g = (rand->nextFloat() * 0.5F + 0.5F) * 2.0F;
		m_field_666_f = (int)(4.0F / (rand->nextFloat() * 0.9F + 0.1F));
		m_e = 0;
		m_entityWalks = false;
}

EntityFX::~EntityFX() {
	// No dynamic resources
}

EntityFX* EntityFX::func_407_b(float var1) {
		motionX *= (double)var1;
		motionY = (motionY - (double)0.1F) * (double)var1 + (double)0.1F;
		motionZ *= (double)var1;
		return this;
}

EntityFX* EntityFX::func_405_d(float var1) {
		setSize(0.2F * var1, 0.2F * var1);
		m_field_665_g *= var1;
		return this;
}

void EntityFX::onUpdate() {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		if(m_e++ >= m_field_666_f) {
			setEntityDead();
		}

		motionY -= 0.04f * (double)m_field_664_h;
		moveEntity(motionX, motionY, motionZ);
		motionX *= (double)0.98F;
		motionY *= (double)0.98F;
		motionZ *= (double)0.98F;
		if(onGround) {
			motionX *= (double)0.7F;
			motionZ *= (double)0.7F;
		}

}

void EntityFX::func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		float var8 = (float)(m_field_670_b % 16) / 16.0F;
		float var9 = var8 + 0.999F / 16.0F;
		float var10 = (float)(m_field_670_b / 16) / 16.0F;
		float var11 = var10 + 0.999F / 16.0F;
		float var12 = 0.1F * m_field_665_g;
		float var13 = (float)(prevPosX + (posX - prevPosX) * (double)var2 - s_field_660_l);
		float var14 = (float)(prevPosY + (posY - prevPosY) * (double)var2 - s_field_659_m);
		float var15 = (float)(prevPosZ + (posZ - prevPosZ) * (double)var2 - s_field_658_n);
		float var16 = getEntityBrightness(var2);
		var1->color3f(m_field_663_i * var16, m_field_662_j * var16, m_field_661_k * var16);
		var1->vertexUV((double)(var13 - var3 * var12 - var6 * var12), (double)(var14 - var4 * var12), (double)(var15 - var5 * var12 - var7 * var12), (double)var8, (double)var11);
		var1->vertexUV((double)(var13 - var3 * var12 + var6 * var12), (double)(var14 + var4 * var12), (double)(var15 - var5 * var12 + var7 * var12), (double)var8, (double)var10);
		var1->vertexUV((double)(var13 + var3 * var12 + var6 * var12), (double)(var14 + var4 * var12), (double)(var15 + var5 * var12 + var7 * var12), (double)var9, (double)var10);
		var1->vertexUV((double)(var13 + var3 * var12 - var6 * var12), (double)(var14 - var4 * var12), (double)(var15 + var5 * var12 - var7 * var12), (double)var9, (double)var11);
}

int EntityFX::func_404_c() {
		return 0;
}

void EntityFX::writeEntityToNBT(NBTTagCompound* var1) {
	// TODO: implement
}

void EntityFX::readEntityFromNBT(NBTTagCompound* var1) {
	// TODO: implement
}
