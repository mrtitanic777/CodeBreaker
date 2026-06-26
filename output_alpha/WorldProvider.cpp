#include "stdafx.h"
// Auto-generated from WorldProvider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldProvider.h"
#include "Common.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include <string>
#include "Block.h"
#include "ChunkLoader.h"
#include "ChunkProviderGenerate.h"
#include "WorldProviderHell.h"

WorldProvider::WorldProvider()
	: worldObj(NULL)
	, m_worldChunkMgr(NULL)
	, m_field_4220_c(false)
	, m_field_6479_d(false)
	, m_field_6478_e(false)
	, m_lightBrightnessTable(new float[16])
	, m_field_4218_e(0)
	, m_field_4217_f(new float[4])
{
}

WorldProvider::~WorldProvider() {
	worldObj = NULL;
	m_worldChunkMgr = NULL;
	delete[] m_lightBrightnessTable;
	m_lightBrightnessTable = NULL;
	delete[] m_field_4217_f;
	m_field_4217_f = NULL;
}

void WorldProvider::registerWorld(World* var1) {
		worldObj = var1;
		registerWorldChunkManager();
		generateLightBrightnessTable();
}

void WorldProvider::generateLightBrightnessTable() {
		float var1 = 0.05F;

		for(int var2 = 0; var2 <= 15; ++var2) {
			float var3 = 1.0F - (float)var2 / 15.0F;
			m_lightBrightnessTable[var2] = (1.0F - var3) / (var3 * 3.0F + 1.0F) * (1.0F - var1) + var1;
		}

}

void WorldProvider::registerWorldChunkManager() {
		m_worldChunkMgr = new WorldChunkManager(worldObj);
}

IChunkProvider* WorldProvider::getChunkProvider() {
		return new ChunkProviderGenerate(worldObj, worldObj->m_randomSeed);
}

IChunkLoader* WorldProvider::getChunkLoader(std::string /* PORT: was java.io.File */* var1) {
		return new ChunkLoader(var1, true);
}

bool WorldProvider::canCoordinateBeSpawn(int var1, int var2) {
		int var3 = worldObj->func_614_g(var1, var2);
		return var3 == Block::s_sand->blockID;
}

float WorldProvider::calculateCelestialAngle(long long var1, float var3) {
		int var4 = (int)(var1 % 24000L);
		float var5 = ((float)var4 + var3) / 24000.0F - 0.25F;
		if(var5 < 0.0F) {
			++var5;
		}

		if(var5 > 1.0F) {
			--var5;
		}

		float var6 = var5;
		var5 = 1.0F - (float)((cosf((double)var5 * 3.14159265358979323846f) + 1.0f) / 2.0f);
		var5 = var6 + (var5 - var6) / 3.0F;
		return var5;
}

float* WorldProvider::func_4097_b(float var1, float var2) {
		float var3 = 0.4F;
		float var4 = cosf(var1 * 3.14159265358979323846f * 2.0F) - 0.0F;
		float var5 = -0.0F;
		if(var4 >= var5 - var3 && var4 <= var5 + var3) {
			float var6 = (var4 - var5) / var3 * 0.5F + 0.5F;
			float var7 = 1.0F - (1.0F - sinf(var6 * 3.14159265358979323846f)) * 0.99F;
			var7 *= var7;
			m_field_4217_f[0] = var6 * 0.3F + 0.7F;
			m_field_4217_f[1] = var6 * var6 * 0.7F + 0.2F;
			m_field_4217_f[2] = var6 * var6 * 0.0F + 0.2F;
			m_field_4217_f[3] = var7;
			return m_field_4217_f;
		} else {
			return NULL;
		}
}

Vec3D* WorldProvider::func_4096_a(float var1, float var2) {
		float var3 = cosf(var1 * 3.14159265358979323846f * 2.0F) * 2.0F + 0.5F;
		if(var3 < 0.0F) {
			var3 = 0.0F;
		}

		if(var3 > 1.0F) {
			var3 = 1.0F;
		}

		float var4 = 192.0F / 255.0F;
		float var5 = 216.0F / 255.0F;
		float var6 = 1.0F;
		var4 *= var3 * 0.94F + 0.06F;
		var5 *= var3 * 0.94F + 0.06F;
		var6 *= var3 * 0.91F + 0.09F;
		return Vec3D::createVector((double)var4, (double)var5, (double)var6);
}

bool WorldProvider::func_6477_d() {
		return true;
}

WorldProvider* WorldProvider::func_4101_a(int var0) {
		return (WorldProvider*)(var0 == 0 ? new WorldProvider() : (var0 == -1 ? new WorldProviderHell() : NULL));
}
