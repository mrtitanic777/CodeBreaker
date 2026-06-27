#ifndef BLOCKREDSTONEORE_H
#define BLOCKREDSTONEORE_H

// Auto-generated from BlockRedstoneOre.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class Entity;
class EntityPlayer;
class Random;
class World;

class BlockRedstoneOre : public Block {
public:
	BlockRedstoneOre(int var1, int var2, bool var3);
	virtual ~BlockRedstoneOre();

	bool glowing;

	virtual int tickRate();
	virtual void onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual void onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5);
	virtual bool blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual void glow(World* var1, int var2, int var3, int var4);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual int idDropped(int var1, Random* var2);
	virtual int quantityDropped(Random* var1);
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void sparkle(World* var1, int var2, int var3, int var4);

	static void initStatics();
};

#endif // BLOCKREDSTONEORE_H