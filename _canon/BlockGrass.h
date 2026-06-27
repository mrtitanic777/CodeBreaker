#ifndef BLOCKGRASS_H
#define BLOCKGRASS_H

// Auto-generated from BlockGrass.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class IBlockAccess;
class Random;
class World;

class BlockGrass : public Block {
public:
	BlockGrass(int var1);
	virtual ~BlockGrass();

	virtual int getBlockTexture(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual int colorMultiplier(IBlockAccess* var1, int var2, int var3, int var4);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual int idDropped(int var1, Random* var2);

	static void initStatics();
};

#endif // BLOCKGRASS_H