#ifndef CHUNKPROVIDERHELL_H
#define CHUNKPROVIDERHELL_H

// Auto-generated from ChunkProviderHell.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IChunkProvider.h"

class Chunk;
class IProgressUpdate;
class MapGenBase;
class NoiseGeneratorOctaves;
class Random;
class World;

class ChunkProviderHell : public IChunkProvider {
public:
	ChunkProviderHell();
	ChunkProviderHell(World* var1, long long var2);
	virtual ~ChunkProviderHell();

	NoiseGeneratorOctaves* m_field_4177_a;
	NoiseGeneratorOctaves* m_field_4176_b;

	virtual void func_4059_a(int var1, int var2, unsigned char* var3);
	virtual void func_4058_b(int var1, int var2, unsigned char* var3);
	virtual Chunk* provideChunk(int var1, int var2);
	virtual bool chunkExists(int var1, int var2);
	virtual void populate(IChunkProvider* var1, int var2, int var3);
	virtual bool saveChunks(bool var1, IProgressUpdate* var2);
	virtual bool func_532_a();
	virtual bool func_536_b();

public: // was private
	Random* m_field_4170_h;
	NoiseGeneratorOctaves* m_field_4169_i;
	NoiseGeneratorOctaves* m_field_4168_j;
	NoiseGeneratorOctaves* m_field_4167_k;
	NoiseGeneratorOctaves* m_field_4166_l;
	NoiseGeneratorOctaves* m_field_4165_m;
	World* m_field_4164_n;
	double* m_field_4163_o;
	double* m_field_4162_p;
	double* m_field_4161_q;
	double* m_field_4160_r;
	MapGenBase* m_field_4159_s;
	double* m_field_4175_c;
	double* m_field_4174_d;
	double* m_field_4173_e;
	double* m_field_4172_f;
	double* m_field_4171_g;

	double* func_4057_a(double* var1, int var2, int var3, int var4, int var5, int var6, int var7);

public:  // array-length companions
	int m_field_4163_o_count;
	int m_field_4162_p_count;
	int m_field_4161_q_count;
	int m_field_4160_r_count;
	int m_field_4175_c_count;
	int m_field_4174_d_count;
	int m_field_4173_e_count;
	int m_field_4172_f_count;
	int m_field_4171_g_count;

};

#endif // CHUNKPROVIDERHELL_H