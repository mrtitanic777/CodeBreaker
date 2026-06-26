#ifndef MOBSPAWNERBASE_H
#define MOBSPAWNERBASE_H

// Auto-generated from MobSpawnerBase.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: s_biomeLookupTable is a BiomeGenBase[] in Java (array of biome
// references), so it must be MobSpawnerBase** (array of biome pointers), not a single
// MobSpawnerBase*. getBiomeFromLookup therefore returns one element (MobSpawnerBase*).

#include <string>

class Class;
struct EnumCreatureType;

class MobSpawnerBase {
public:
	MobSpawnerBase();
	virtual ~MobSpawnerBase();
	static void initStatics();  // run once at startup (was Java static {})

	static MobSpawnerBase* s_rainforest;
	static MobSpawnerBase* s_swampland;
	static MobSpawnerBase* s_seasonalForest;
	static MobSpawnerBase* s_forest;
	static MobSpawnerBase* s_savanna;
	static MobSpawnerBase* s_shrubland;
	static MobSpawnerBase* s_taiga;
	static MobSpawnerBase* s_desert;
	static MobSpawnerBase* s_plains;
	static MobSpawnerBase* s_iceDesert;
	static MobSpawnerBase* s_tundra;
	static MobSpawnerBase* s_hell;
	std::string m_biomeName;
	int m_field_6503_n;
	unsigned char m_topBlock;
	unsigned char m_fillerBlock;
	int m_field_6502_q;

	static void generateBiomeLookup();
	static MobSpawnerBase* getBiomeFromLookup(double var0, double var2);
	static MobSpawnerBase* getBiome(float var0, float var1);
	virtual int getSkyColorByTemp(float var1);
	virtual void* getEntitiesForType(EnumCreatureType* var1);

public: // was protected
	void* m_biomeMonsters;
	void* m_biomeCreatures;

	virtual MobSpawnerBase* func_4122_b();
	virtual MobSpawnerBase* setBiomeName(const std::string& var1);
	virtual MobSpawnerBase* func_4124_a(int var1);
	virtual MobSpawnerBase* func_4123_b(int var1);

public: // was private
	// was MobSpawnerBase* (single) -- it is really BiomeGenBase[] (array of biome pointers).
	static MobSpawnerBase** s_biomeLookupTable;

public:  // array-length companions
	int m_biomeMonsters_count;
	int m_biomeCreatures_count;

};

#endif // MOBSPAWNERBASE_H
