#ifndef BLOCKFURNACE_H
#define BLOCKFURNACE_H

// Auto-generated from BlockFurnace.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockContainer.h"

class EntityLiving;
class EntityPlayer;
class IBlockAccess;
class Random;
class TileEntity;
class World;

class BlockFurnace : public BlockContainer {
public:
	BlockFurnace();
	BlockFurnace(int var1, bool var2);
	virtual ~BlockFurnace();

	virtual int idDropped(int var1, Random* var2);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual int getBlockTexture(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual int getBlockTextureFromSide(int var1);
	virtual bool blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	static void updateFurnaceBlockState(bool var0, World* var1, int var2, int var3, int var4);
	virtual void onBlockPlacedBy(World* var1, int var2, int var3, int var4, EntityLiving* var5);

public: // was protected
	virtual TileEntity* SetBlockEntity();

public: // was private
	bool m_isActive;

	void func_284_h(World* var1, int var2, int var3, int var4);

};

#endif // BLOCKFURNACE_H