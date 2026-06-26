#include "stdafx.h"
// Auto-generated from WorldChunkManager.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE:
//  - biome buffer m_field_4195_d / loadBlockGeneratorData is MobSpawnerBase** (BiomeGenBase[]).
//  - declared the missing var1_count (Java `biomes.length`); reuse m_field_4195_d_count.
//  - initialized the _count companions so the null/grow guard is well-defined.

#include "WorldChunkManager.h"
#include "Common.h"
#include "ChunkCoordIntPair.h"
#include "MobSpawnerBase.h"
#include "NoiseGeneratorOctaves2.h"
#include "World.h"
#include "Random.h"

WorldChunkManager::WorldChunkManager()
	: m_temperature(NULL)
	, m_humidity(NULL)
	, m_field_4196_c(NULL)
	, m_field_4195_d(NULL)
	, m_field_4194_e(NULL)
	, m_field_4193_f(NULL)
	, m_field_4192_g(NULL)
	, m_temperature_count(0)
	, m_humidity_count(0)
	, m_field_4196_c_count(0)
	, m_field_4195_d_count(0)
{
}

WorldChunkManager::WorldChunkManager(World* var1)
	: m_temperature(NULL)
	, m_humidity(NULL)
	, m_field_4196_c(NULL)
	, m_field_4195_d(NULL)
	, m_field_4194_e(NULL)
	, m_field_4193_f(NULL)
	, m_field_4192_g(NULL)
	, m_temperature_count(0)
	, m_humidity_count(0)
	, m_field_4196_c_count(0)
	, m_field_4195_d_count(0)
{
		m_field_4194_e = new NoiseGeneratorOctaves2(new Random(var1->m_randomSeed * 9871L), 4);
		m_field_4193_f = new NoiseGeneratorOctaves2(new Random(var1->m_randomSeed * 39811L), 4);
		m_field_4192_g = new NoiseGeneratorOctaves2(new Random(var1->m_randomSeed * 543321L), 2);
}

WorldChunkManager::~WorldChunkManager() {
	m_field_4194_e = NULL;
	m_field_4193_f = NULL;
	m_field_4192_g = NULL;
	delete[] m_temperature;
	m_temperature = NULL;
	delete[] m_humidity;
	m_humidity = NULL;
	delete[] m_field_4196_c;
	m_field_4196_c = NULL;
	delete[] m_field_4195_d;
	m_field_4195_d = NULL;
}

MobSpawnerBase* WorldChunkManager::func_4074_a(ChunkCoordIntPair* var1) {
		return func_4073_a(var1->m_chunkXPos, var1->m_chunkZPos);
}

MobSpawnerBase* WorldChunkManager::func_4073_a(int var1, int var2) {
		return func_4069_a(var1, var2, 1, 1)[0];
}

double WorldChunkManager::func_4072_b(int var1, int var2) {
		m_temperature = m_field_4194_e->func_4112_a(m_temperature, (double)var1, (double)var2, 1, 1, (double)0.025F, (double)0.025F, 0.5f);
		return m_temperature[0];
}

MobSpawnerBase** WorldChunkManager::func_4069_a(int var1, int var2, int var3, int var4) {
		m_field_4195_d = loadBlockGeneratorData(m_field_4195_d, var1, var2, var3, var4);
		return m_field_4195_d;
}

double* WorldChunkManager::getTemperatures(double* var1, int var2, int var3, int var4, int var5) {
		int var1_count = 0;
		if(var1 == NULL || var1_count < var4 * var5) {
			var1 = new double[var4 * var5];
		}

		var1 = m_field_4194_e->func_4112_a(var1, (double)var2, (double)var3, var4, var4, (double)0.025F, (double)0.025F, 0.25f);
		m_field_4196_c = m_field_4192_g->func_4112_a(m_field_4196_c, (double)var2, (double)var3, var4, var4, 0.25f, 0.25f, 0.5882352941176471f);
		int var6 = 0;

		for(int var7 = 0; var7 < var4; ++var7) {
			for(int var8 = 0; var8 < var5; ++var8) {
				double var9 = m_field_4196_c[var6] * 1.1f + 0.5f;
				double var11 = 0.01f;
				double var13 = 1.0f - var11;
				double var15 = (var1[var6] * 0.15f + 0.7f) * var13 + var9 * var11;
				var15 = 1.0f - (1.0f - var15) * (1.0f - var15);
				if(var15 < 0.0f) {
					var15 = 0.0f;
				}

				if(var15 > 1.0f) {
					var15 = 1.0f;
				}

				var1[var6] = var15;
				++var6;
			}
		}

		return var1;
}

MobSpawnerBase** WorldChunkManager::loadBlockGeneratorData(MobSpawnerBase** var1, int var2, int var3, int var4, int var5) {
		// Java: if (biomes == null || biomes.length < width*length) biomes = new BiomeGenBase[width*length];
		if(var1 == NULL || m_field_4195_d_count < var4 * var5) {
			var1 = new MobSpawnerBase*[var4 * var5];
			m_field_4195_d_count = var4 * var5;
		}

		m_temperature = m_field_4194_e->func_4112_a(m_temperature, (double)var2, (double)var3, var4, var4, (double)0.025F, (double)0.025F, 0.25f);
		m_humidity = m_field_4193_f->func_4112_a(m_humidity, (double)var2, (double)var3, var4, var4, (double)0.05F, (double)0.05F, 1.0f / 3.0f);
		m_field_4196_c = m_field_4192_g->func_4112_a(m_field_4196_c, (double)var2, (double)var3, var4, var4, 0.25f, 0.25f, 0.5882352941176471f);
		int var6 = 0;

		for(int var7 = 0; var7 < var4; ++var7) {
			for(int var8 = 0; var8 < var5; ++var8) {
				double var9 = m_field_4196_c[var6] * 1.1f + 0.5f;
				double var11 = 0.01f;
				double var13 = 1.0f - var11;
				double var15 = (m_temperature[var6] * 0.15f + 0.7f) * var13 + var9 * var11;
				var11 = 0.002f;
				var13 = 1.0f - var11;
				double var17 = (m_humidity[var6] * 0.15f + 0.5f) * var13 + var9 * var11;
				var15 = 1.0f - (1.0f - var15) * (1.0f - var15);
				if(var15 < 0.0f) {
					var15 = 0.0f;
				}

				if(var17 < 0.0f) {
					var17 = 0.0f;
				}

				if(var15 > 1.0f) {
					var15 = 1.0f;
				}

				if(var17 > 1.0f) {
					var17 = 1.0f;
				}

				m_temperature[var6] = var15;
				m_humidity[var6] = var17;
				var1[var6++] = MobSpawnerBase::getBiomeFromLookup(var15, var17);
			}
		}

		return var1;
}
