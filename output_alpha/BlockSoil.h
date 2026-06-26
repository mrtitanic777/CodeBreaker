#ifndef BLOCKSOIL_H
#define BLOCKSOIL_H

// Auto-generated from BlockSoil.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

struct AxisAlignedBB;
class Entity;
class Random;
class World;

class BlockSoil : public Block {
public:
	BlockSoil();
	BlockSoil(int var1);
	virtual ~BlockSoil();

	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getBlockTextureFromSideAndMetadata(int var1, int var2);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual int idDropped(int var1, Random* var2);

public: // was protected
public: // was private
	bool func_275_h(World* var1, int var2, int var3, int var4);
	bool func_274_i(World* var1, int var2, int var3, int var4);

};

#endif // BLOCKSOIL_H