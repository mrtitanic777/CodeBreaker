#ifndef CHUNKPROVIDERGENERATE_H
#define CHUNKPROVIDERGENERATE_H

// Auto-generated from ChunkProviderGenerate.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: the per-chunk biome buffer is a BiomeGenBase[] (array of biome
// references) -> MobSpawnerBase**. generateTerrain / replaceBlocksForBiome take it as **.

#include "IChunkProvider.h"

class Chunk;
class IProgressUpdate;
class MapGenBase;
class MobSpawnerBase;
class NoiseGeneratorOctaves;
class Random;
class World;

class ChunkProviderGenerate : public IChunkProvider {
public:
	ChunkProviderGenerate();
	ChunkProviderGenerate(World* var1, long long var2);
	virtual ~ChunkProviderGenerate();

	NoiseGeneratorOctaves* m_field_922_a;
	NoiseGeneratorOctaves* m_field_921_b;
	NoiseGeneratorOctaves* m_field_920_c;

	virtual void generateTerrain(int var1, int var2, unsigned char* var3, MobSpawnerBase** var4, double* var5);
	virtual void replaceBlocksForBiome(int var1, int var2, unsigned char* var3, MobSpawnerBase** var4);
	virtual Chunk* provideChunk(int var1, int var2);
	virtual bool chunkExists(int var1, int var2);
	virtual void populate(IChunkProvider* var1, int var2, int var3);
	virtual bool saveChunks(bool var1, IProgressUpdate* var2);
	virtual bool func_532_a();
	virtual bool func_536_b();

public: // was private
	Random* rand;
	NoiseGeneratorOctaves* m_field_912_k;
	NoiseGeneratorOctaves* m_field_911_l;
	NoiseGeneratorOctaves* m_field_910_m;
	NoiseGeneratorOctaves* m_field_909_n;
	NoiseGeneratorOctaves* m_field_908_o;
	World* worldObj;
	double* m_field_4180_q;
	double* m_field_905_r;
	double* m_field_904_s;
	double* m_field_903_t;
	MapGenBase* m_field_902_u;
	MobSpawnerBase** m_biomesForGeneration;
	double* m_field_4185_d;
	double* m_field_4184_e;
	double* m_field_4183_f;
	double* m_field_4182_g;
	double* m_field_4181_h;
	int** m_field_914_i;
	double* m_field_4178_w;

	double* func_4061_a(double* var1, int var2, int var3, int var4, int var5, int var6, int var7);

public:  // array-length companions
	int m_field_4180_q_count;
	int m_field_905_r_count;
	int m_field_904_s_count;
	int m_field_903_t_count;
	int m_biomesForGeneration_count;
	int m_field_4185_d_count;
	int m_field_4184_e_count;
	int m_field_4183_f_count;
	int m_field_4182_g_count;
	int m_field_4181_h_count;
	int m_field_914_i_count;
	int m_field_4178_w_count;

};

#endif // CHUNKPROVIDERGENERATE_H
