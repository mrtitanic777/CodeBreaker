#ifndef BLOCKSTATIONARY_H
#define BLOCKSTATIONARY_H

// Auto-generated from BlockStationary.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFluid.h"

class Material;
class Random;
class World;

class BlockStationary : public BlockFluid {
public:
	BlockStationary();
	BlockStationary(int var1, Material* var2);
	virtual ~BlockStationary();

	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);

public: // was protected
};

#endif // BLOCKSTATIONARY_H