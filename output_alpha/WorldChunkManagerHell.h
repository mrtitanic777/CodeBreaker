#ifndef WORLDCHUNKMANAGERHELL_H
#define WORLDCHUNKMANAGERHELL_H

// Auto-generated from WorldChunkManagerHell.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: func_4069_a / loadBlockGeneratorData return MobSpawnerBase**
// (BiomeGenBase[]) to match the WorldChunkManager base overrides.

#include "WorldChunkManager.h"

class ChunkCoordIntPair;
class MobSpawnerBase;

class WorldChunkManagerHell : public WorldChunkManager {
public:
	WorldChunkManagerHell();
	WorldChunkManagerHell(MobSpawnerBase* var1, double var2, double var4);
	virtual ~WorldChunkManagerHell();

	virtual MobSpawnerBase* func_4074_a(ChunkCoordIntPair* var1);
	virtual MobSpawnerBase* func_4073_a(int var1, int var2);
	virtual double func_4072_b(int var1, int var2);
	virtual MobSpawnerBase** func_4069_a(int var1, int var2, int var3, int var4);
	virtual double* getTemperatures(double* var1, int var2, int var3, int var4, int var5);
	virtual MobSpawnerBase** loadBlockGeneratorData(MobSpawnerBase** var1, int var2, int var3, int var4, int var5);

public: // was private
	MobSpawnerBase* m_field_4201_e;
	double m_field_4200_f;
	double m_field_4199_g;

};

#endif // WORLDCHUNKMANAGERHELL_H
