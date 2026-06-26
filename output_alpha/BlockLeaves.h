#ifndef BLOCKLEAVES_H
#define BLOCKLEAVES_H

// Auto-generated from BlockLeaves.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockLeavesBase.h"

class Entity;
class IBlockAccess;
class Random;
class World;

class BlockLeaves : public BlockLeavesBase {
public:
	BlockLeaves();
	BlockLeaves(int var1, int var2);
	virtual ~BlockLeaves();

	virtual int colorMultiplier(IBlockAccess* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void func_6361_f(World* var1, int var2, int var3, int var4, int var5);
	virtual void func_308_h(World* var1, int var2, int var3, int var4);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual int quantityDropped(Random* var1);
	virtual int idDropped(int var1, Random* var2);
	virtual bool isOpaqueCube();
	virtual void setGraphicsLevel(bool var1);
	virtual void onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5);

public: // was protected
public: // was private
	int m_baseIndexInPNG;
	int m_field_464_c;

	int func_6362_g(World* var1, int var2, int var3, int var4, int var5);
	void func_6360_i(World* var1, int var2, int var3, int var4);

};

#endif // BLOCKLEAVES_H