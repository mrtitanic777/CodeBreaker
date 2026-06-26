#include "stdafx.h"
// Hand-ported from WorldProviderHell.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// The Nether ("Hell") dimension provider: bright/red sky tint, flat half-day
// celestial angle, a dedicated WorldChunkManagerHell biome source, a
// ChunkProviderHell terrain generator and a ChunkLoader rooted at a "DIM-1"
// sub-directory of the world save.
//
// PORT NOTES:
//  - The Java line `new WorldChunkManagerHell(MobSpawnerBase.hell, 1.0F, 0.0F)`
//    references the static hell-biome singleton `MobSpawnerBase.hell`. That
//    static (and MobSpawnerBase.h itself) is not yet ported project-wide, so we
//    pass NULL for the biome here. WorldChunkManagerHell only stashes the
//    pointer in m_field_4201_e; nothing in the Nether path dereferences it.
//  - getChunkLoader's argument was a java.io.File; the converted interface
//    carries it as std::string* (a save-root path). We append "DIM-1" with the
//    JavaIO File bridge, mkdirs() it, and hand a heap std::string* to ChunkLoader.

#include "WorldProviderHell.h"
#include "Common.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "Vec3D.h"
#include <string>
#include "Block.h"
#include "World.h"
#include "ChunkLoader.h"
#include "ChunkProviderHell.h"
#include "WorldChunkManagerHell.h"
#include "JavaIO.h"

WorldProviderHell::WorldProviderHell()
{
}

WorldProviderHell::~WorldProviderHell() {
	// No dynamic resources
}

void WorldProviderHell::registerWorldChunkManager() {
	// Java: new WorldChunkManagerHell(MobSpawnerBase.hell, 1.0F, 0.0F)
	// PORT: MobSpawnerBase.hell singleton unavailable -> NULL biome (see header note).
	m_worldChunkMgr = new WorldChunkManagerHell((MobSpawnerBase*)NULL, 1.0, 0.0);
	m_field_4220_c = true;
	m_field_6479_d = true;
	m_field_6478_e = true;
	m_field_4218_e = -1;
}

Vec3D* WorldProviderHell::func_4096_a(float var1, float var2) {
	return Vec3D::createVector((double)0.2F, (double)0.03F, (double)0.03F);
}

void WorldProviderHell::generateLightBrightnessTable() {
	float var1 = 0.1F;

	for(int var2 = 0; var2 <= 15; ++var2) {
		float var3 = 1.0F - (float)var2 / 15.0F;
		m_lightBrightnessTable[var2] = (1.0F - var3) / (var3 * 3.0F + 1.0F) * (1.0F - var1) + var1;
	}

}

IChunkProvider* WorldProviderHell::getChunkProvider() {
	return new ChunkProviderHell(worldObj, worldObj->m_randomSeed);
}

IChunkLoader* WorldProviderHell::getChunkLoader(std::string /* PORT: was java.io.File */* var1) {
	// Java: File var2 = new File(var1, "DIM-1"); var2.mkdirs();
	//       return new ChunkLoader(var2, true);
	File var2(var1 != NULL ? *var1 : std::string(), "DIM-1");
	var2.mkdirs();
	std::string* var3 = new std::string(var2.getPath());
	return new ChunkLoader(var3, true);
}

bool WorldProviderHell::canCoordinateBeSpawn(int var1, int var2) {
	int var3 = worldObj->func_614_g(var1, var2);
	return var3 == Block::s_bedrock->blockID ? false : (var3 == 0 ? false : Block::s_opaqueCubeLookup[var3]);
}

float WorldProviderHell::calculateCelestialAngle(long long var1, float var3) {
	return 0.5F;
}

bool WorldProviderHell::func_6477_d() {
	return false;
}
