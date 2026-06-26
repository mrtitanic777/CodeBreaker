#include "stdafx.h"
// Auto-generated from WorldChunkManagerHell.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE:
//  - biome buffer is MobSpawnerBase** (BiomeGenBase[]).
//  - Java Arrays.fill(arr,0,n,val) was mis-translated to a 5-arg std::fill -> plain loops.
//  - declared the missing var1_count (Java `var1.length`).

#include "WorldChunkManagerHell.h"
#include "Common.h"
#include "ChunkCoordIntPair.h"
#include "MobSpawnerBase.h"

WorldChunkManagerHell::WorldChunkManagerHell()
	: m_field_4201_e(NULL)
	, m_field_4200_f(0)
	, m_field_4199_g(0)
{
}

WorldChunkManagerHell::WorldChunkManagerHell(MobSpawnerBase* var1, double var2, double var4)
	: m_field_4201_e(NULL)
	, m_field_4200_f(0)
	, m_field_4199_g(0)
{
		m_field_4201_e = var1;
		m_field_4200_f = var2;
		m_field_4199_g = var4;
}

WorldChunkManagerHell::~WorldChunkManagerHell() {
	m_field_4201_e = NULL;
}

MobSpawnerBase* WorldChunkManagerHell::func_4074_a(ChunkCoordIntPair* var1) {
		return m_field_4201_e;
}

MobSpawnerBase* WorldChunkManagerHell::func_4073_a(int var1, int var2) {
		return m_field_4201_e;
}

double WorldChunkManagerHell::func_4072_b(int var1, int var2) {
		return m_field_4200_f;
}

MobSpawnerBase** WorldChunkManagerHell::func_4069_a(int var1, int var2, int var3, int var4) {
		m_field_4195_d = loadBlockGeneratorData(m_field_4195_d, var1, var2, var3, var4);
		return m_field_4195_d;
}

double* WorldChunkManagerHell::getTemperatures(double* var1, int var2, int var3, int var4, int var5) {
		int var1_count = 0;
		if(var1 == NULL || var1_count < var4 * var5) {
			var1 = new double[var4 * var5];
		}

		// Arrays.fill(var1, 0, var4 * var5, m_field_4200_f);
		for(int _i = 0; _i < var4 * var5; ++_i) {
			var1[_i] = m_field_4200_f;
		}
		return var1;
}

MobSpawnerBase** WorldChunkManagerHell::loadBlockGeneratorData(MobSpawnerBase** var1, int var2, int var3, int var4, int var5) {
		int var1_count = m_field_4195_d_count;
		if(var1 == NULL || var1_count < var4 * var5) {
			var1 = new MobSpawnerBase*[var4 * var5];
			m_field_4195_d_count = var4 * var5;
			delete[] m_temperature;
			m_temperature = new double[var4 * var5];
			m_temperature_count = var4 * var5;
			delete[] m_humidity;
			m_humidity = new double[var4 * var5];
			m_humidity_count = var4 * var5;
		}

		// Arrays.fill(var1, 0, var4 * var5, m_field_4201_e);
		for(int _i = 0; _i < var4 * var5; ++_i) {
			var1[_i] = m_field_4201_e;
		}
		// Arrays.fill(m_humidity, 0, var4 * var5, m_field_4199_g);
		for(int _j = 0; _j < var4 * var5; ++_j) {
			m_humidity[_j] = m_field_4199_g;
		}
		// Arrays.fill(m_temperature, 0, var4 * var5, m_field_4200_f);
		for(int _k = 0; _k < var4 * var5; ++_k) {
			m_temperature[_k] = m_field_4200_f;
		}
		return var1;
}
