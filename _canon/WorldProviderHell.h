#ifndef WORLDPROVIDERHELL_H
#define WORLDPROVIDERHELL_H

// Auto-generated from WorldProviderHell.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldProvider.h"

class File;
class IChunkLoader;
class IChunkProvider;
class Vec3D;

class WorldProviderHell : public WorldProvider {
public:
	virtual ~WorldProviderHell();

	virtual void registerWorldChunkManager();
	virtual Vec3D* getFogColor(float var1, float var2);
	virtual void generateLightBrightnessTable();
	virtual IChunkProvider* getChunkProvider();
	virtual IChunkLoader* getChunkLoader(File* var1);
	virtual bool canCoordinateBeSpawn(int var1, int var2);
	virtual float calculateCelestialAngle(long long var1, float var3);
	virtual bool isSurfaceWorld();

	static void initStatics();
};

#endif // WORLDPROVIDERHELL_H