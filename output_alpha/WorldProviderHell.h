#ifndef WORLDPROVIDERHELL_H
#define WORLDPROVIDERHELL_H

// Auto-generated from WorldProviderHell.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldProvider.h"

class IChunkLoader;
class IChunkProvider;
class Vec3D;

class WorldProviderHell : public WorldProvider {
public:
	WorldProviderHell();
	virtual ~WorldProviderHell();

	virtual void registerWorldChunkManager();
	virtual Vec3D* func_4096_a(float var1, float var2);
	virtual IChunkProvider* getChunkProvider();
	virtual IChunkLoader* getChunkLoader(std::string /* PORT: was java.io.File */* var1);
	virtual bool canCoordinateBeSpawn(int var1, int var2);
	virtual float calculateCelestialAngle(long long var1, float var3);
	virtual bool func_6477_d();

public: // was protected
	virtual void generateLightBrightnessTable();

};

#endif // WORLDPROVIDERHELL_H