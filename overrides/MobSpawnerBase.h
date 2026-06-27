#ifndef MOBSPAWNERBASE_H
#define MOBSPAWNERBASE_H

// Auto-generated from MobSpawnerBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EnumCreatureType.h"

class Class;

class MobSpawnerBase {
public:
	MobSpawnerBase();   // synthesized: Java field initializers (override had no ctor)
	static void initStatics();   // run the deferred biome static inits in order (j2x_initAll)
	virtual ~MobSpawnerBase();

	JavaString biomeName;
	int color;
	unsigned char topBlock;
	unsigned char fillerBlock;
	int grassColor;
	JArray<Class* > biomeMonsters;
	JArray<Class* > biomeCreatures;
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
	static JArray<MobSpawnerBase* > s_biomeLookupTable;

	static void generateBiomeLookup();
	virtual MobSpawnerBase* setEnableSnow();
	virtual MobSpawnerBase* setBiomeName(const JavaString& var1);
	virtual MobSpawnerBase* setGrassColor(int var1);
	virtual MobSpawnerBase* setColor(int var1);
	static MobSpawnerBase* getBiomeFromLookup(double var0, double var2);
	static MobSpawnerBase* getBiome(float var0, float var1);
	virtual int getSkyColorByTemp(float var1);
	virtual JArray<Class* > getEntitiesForType(int var1);
};

#endif // MOBSPAWNERBASE_H