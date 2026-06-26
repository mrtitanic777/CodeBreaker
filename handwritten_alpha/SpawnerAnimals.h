#ifndef SPAWNERANIMALS_H
#define SPAWNERANIMALS_H

// Auto-generated from SpawnerAnimals.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: java.util.Set<ChunkCoordIntPair> -> std::vector<void*>*.

#include <vector>

class ChunkPosition;
class World;

class SpawnerAnimals {
public:
	SpawnerAnimals();
	virtual ~SpawnerAnimals();

	static int performSpawning(World* var0);

public: // was protected
	ChunkPosition* getRandomSpawningPointInChunk(World* var0, int var1, int var2);

public: // was private
	static std::vector<void*>* s_eligibleChunksForSpawning;  // was Set

};

#endif // SPAWNERANIMALS_H
