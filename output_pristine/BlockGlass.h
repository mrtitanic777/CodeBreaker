#ifndef BLOCKGLASS_H
#define BLOCKGLASS_H

// Auto-generated from BlockGlass.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockBreakable.h"

class Material;
class Random;

class BlockGlass : public BlockBreakable {
public:
	BlockGlass();
	BlockGlass(int var1, int var2, Material* var3, bool var4);
	virtual ~BlockGlass();

	virtual int quantityDropped(Random* var1);

};

#endif // BLOCKGLASS_H