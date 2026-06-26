#ifndef BLOCKCHEST_H
#define BLOCKCHEST_H

// Auto-generated from BlockChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockContainer.h"

class EntityPlayer;
class Random;
class TileEntity;
class World;

class BlockChest : public BlockContainer {
public:
	BlockChest();
	BlockChest(int var1);
	virtual ~BlockChest();

	virtual int getBlockTexture(World* var1, int var2, int var3, int var4, int var5);
	virtual int getBlockTextureFromSide(int var1);
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);
	virtual bool blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5);

public: // was protected
	virtual TileEntity* getBlockEntity();

public: // was private
	Random* m_random;

	bool isThereANeighborChest(World* var1, int var2, int var3, int var4);

};

#endif // BLOCKCHEST_H