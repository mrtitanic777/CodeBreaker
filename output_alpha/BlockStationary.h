#ifndef BLOCKSTATIONARY_H
#define BLOCKSTATIONARY_H

// Auto-generated from BlockStationary.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFluids.h"

class Material;
class Random;
class World;

class BlockStationary : public BlockFluids {
public:
	BlockStationary();
	BlockStationary(int var1, Material* var2);
	virtual ~BlockStationary();

	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);

public: // was protected
public: // was private
	void func_15234_j(World* var1, int var2, int var3, int var4);
	bool func_301_k(World* var1, int var2, int var3, int var4);

};

#endif // BLOCKSTATIONARY_H