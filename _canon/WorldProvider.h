#ifndef WORLDPROVIDER_H
#define WORLDPROVIDER_H

// Auto-generated from WorldProvider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class File;
class IChunkLoader;
class IChunkProvider;
class Vec3D;
class World;
class WorldChunkManager;

class WorldProvider {
public:
	WorldProvider();
	virtual ~WorldProvider();

	World* worldObj;
	WorldChunkManager* worldChunkMgr;
	bool isHellWorld;
	bool doesWaterVaporize;
	bool hasNoSky;
	JArray<float > lightBrightnessTable;
	int dimensionId;
	JArray<float > colorsSunriseSunset;

	virtual void registerWorld(World* var1);
	virtual void generateLightBrightnessTable();
	virtual void registerWorldChunkManager();
	virtual IChunkProvider* getChunkProvider();
	virtual IChunkLoader* getChunkLoader(File* var1);
	virtual bool canCoordinateBeSpawn(int var1, int var2);
	virtual float calculateCelestialAngle(long long var1, float var3);
	virtual JArray<float > calcSunriseSunsetColors(float var1, float var2);
	virtual Vec3D* getFogColor(float var1, float var2);
	virtual bool isSurfaceWorld();
	static WorldProvider* getProviderForDimension(int var0);

	static void initStatics();
};

#endif // WORLDPROVIDER_H