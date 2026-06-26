#ifndef WORLDGENLAKES_H
#define WORLDGENLAKES_H

// Auto-generated from WorldGenLakes.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"

class Random;
class World;

class WorldGenLakes : public WorldGenerator {
public:
	WorldGenLakes();
	WorldGenLakes(int var1);
	virtual ~WorldGenLakes();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

public: // was private
	int m_field_15235_a;

};

#endif // WORLDGENLAKES_H