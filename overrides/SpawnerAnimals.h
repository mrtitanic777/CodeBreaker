#ifndef SPAWNERANIMALS_H
#define SPAWNERANIMALS_H

// Auto-generated from SpawnerAnimals.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <set>

class ChunkPosition;
class World;

class SpawnerAnimals {
public:
	virtual ~SpawnerAnimals();

	static std::set<void*> s_eligibleChunksForSpawning;

	static ChunkPosition* getRandomSpawningPointInChunk(World* var0, int var1, int var2);
	static int performSpawning(World* var0);
};

#endif // SPAWNERANIMALS_H