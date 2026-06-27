#ifndef BLOCKPUMPKIN_H
#define BLOCKPUMPKIN_H

// Auto-generated from BlockPumpkin.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class EntityLiving;
class World;

class BlockPumpkin : public Block {
public:
	BlockPumpkin(int var1, int var2, bool var3);
	virtual ~BlockPumpkin();

	bool facingState;

	virtual int getBlockTextureFromSideAndMetadata(int var1, int var2);
	virtual int getBlockTextureFromSide(int var1);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onBlockPlacedBy(World* var1, int var2, int var3, int var4, EntityLiving* var5);

	static void initStatics();
};

#endif // BLOCKPUMPKIN_H