#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

// Auto-generated from WorldGenerator.java
// Target: Xbox 360 / C++03 / DirectX 9

class Random;
class World;

class WorldGenerator {
public:
	WorldGenerator();
	virtual ~WorldGenerator();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5) = 0;
	virtual void func_517_a(double var1, double var3, double var5);

};

#endif // WORLDGENERATOR_H