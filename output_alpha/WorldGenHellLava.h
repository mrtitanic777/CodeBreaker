#ifndef WORLDGENHELLLAVA_H
#define WORLDGENHELLLAVA_H

// Auto-generated from WorldGenHellLava.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"

class Random;
class World;

class WorldGenHellLava : public WorldGenerator {
public:
	WorldGenHellLava();
	WorldGenHellLava(int var1);
	virtual ~WorldGenHellLava();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

public: // was private
	int m_field_4158_a;

};

#endif // WORLDGENHELLLAVA_H