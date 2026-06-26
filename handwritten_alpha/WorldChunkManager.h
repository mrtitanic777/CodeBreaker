#ifndef WORLDCHUNKMANAGER_H
#define WORLDCHUNKMANAGER_H

// Auto-generated from WorldChunkManager.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: the biome buffer (m_field_4195_d) and loadBlockGeneratorData /
// func_4069_a return a BiomeGenBase[] (array of biome references) -> MobSpawnerBase**.
// func_4073_a / func_4074_a still return a single biome (MobSpawnerBase*).

class ChunkCoordIntPair;
class MobSpawnerBase;
class NoiseGeneratorOctaves2;
class World;

class WorldChunkManager {
public:
	WorldChunkManager();
	WorldChunkManager(World* var1);
	virtual ~WorldChunkManager();

	double* m_temperature;
	double* m_humidity;
	double* m_field_4196_c;
	MobSpawnerBase** m_field_4195_d;

	virtual MobSpawnerBase* func_4074_a(ChunkCoordIntPair* var1);
	virtual MobSpawnerBase* func_4073_a(int var1, int var2);
	virtual double func_4072_b(int var1, int var2);
	virtual MobSpawnerBase** func_4069_a(int var1, int var2, int var3, int var4);
	virtual double* getTemperatures(double* var1, int var2, int var3, int var4, int var5);
	virtual MobSpawnerBase** loadBlockGeneratorData(MobSpawnerBase** var1, int var2, int var3, int var4, int var5);

public: // was protected
public: // was private
	NoiseGeneratorOctaves2* m_field_4194_e;
	NoiseGeneratorOctaves2* m_field_4193_f;
	NoiseGeneratorOctaves2* m_field_4192_g;

public:  // array-length companions
	int m_temperature_count;
	int m_humidity_count;
	int m_field_4196_c_count;
	int m_field_4195_d_count;

};

#endif // WORLDCHUNKMANAGER_H
