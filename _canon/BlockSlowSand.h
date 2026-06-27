#ifndef BLOCKSLOWSAND_H
#define BLOCKSLOWSAND_H

// Auto-generated from BlockSlowSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class AxisAlignedBB;
class Entity;
class World;

class BlockSlowSand : public Block {
public:
	BlockSlowSand(int var1, int var2);
	virtual ~BlockSlowSand();

	virtual AxisAlignedBB* getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual void onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5);

	static void initStatics();
};

#endif // BLOCKSLOWSAND_H