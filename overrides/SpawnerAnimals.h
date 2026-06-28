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
class Chunk;

class SpawnerAnimals {
public:
	virtual ~SpawnerAnimals();

	static std::set<void*> s_eligibleChunksForSpawning;

	static ChunkPosition* getRandomSpawningPointInChunk(World* var0, int var1, int var2);
	static int performSpawning(World* var0);
	// CONSOLE: Alpha 1.2.6 has no world-gen animal spawning -> add herds (cow/pig/sheep/chicken) to newly
	// generated grass chunks so a fresh world isn't barren. Spawns directly into the Chunk.
	static void performWorldGenSpawning(World* var0, Chunk* var1, int var2, int var3);
};

#endif // SPAWNERANIMALS_H