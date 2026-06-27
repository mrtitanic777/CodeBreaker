#ifndef BLOCKCHEST_H
#define BLOCKCHEST_H

// Auto-generated from BlockChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "BlockContainer.h"

class EntityPlayer;
class IBlockAccess;
class Random;
class TileEntity;
class World;

class BlockChest : public BlockContainer {
public:
	BlockChest(int var1);
	virtual ~BlockChest();

	Random* rand;

	virtual int getBlockTexture(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual int getBlockTextureFromSide(int var1);
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual bool isThereANeighborChest(World* var1, int var2, int var3, int var4);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);
	virtual bool blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual TileEntity* SetBlockEntity();

	static void initStatics();
};

#endif // BLOCKCHEST_H