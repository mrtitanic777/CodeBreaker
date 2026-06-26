#ifndef BLOCKREDSTONEORE_H
#define BLOCKREDSTONEORE_H

// Auto-generated from BlockRedstoneOre.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class Entity;
class EntityPlayer;
class Random;
class World;

class BlockRedstoneOre : public Block {
public:
	BlockRedstoneOre();
	BlockRedstoneOre(int var1, int var2, bool var3);
	virtual ~BlockRedstoneOre();

	virtual int tickRate();
	virtual void onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual void onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5);
	virtual bool blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual int idDropped(int var1, Random* var2);
	virtual int quantityDropped(Random* var1);
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);

public: // was private
	bool m_field_468_a;

	void func_320_h(World* var1, int var2, int var3, int var4);
	void func_319_i(World* var1, int var2, int var3, int var4);

};

#endif // BLOCKREDSTONEORE_H