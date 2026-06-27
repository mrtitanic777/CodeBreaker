#ifndef BLOCKJUKEBOX_H
#define BLOCKJUKEBOX_H

// Auto-generated from BlockJukeBox.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class EntityPlayer;
class World;

class BlockJukeBox : public Block {
public:
	BlockJukeBox(int var1, int var2);
	virtual ~BlockJukeBox();

	virtual int getBlockTextureFromSide(int var1);
	virtual bool blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual void ejectRecord(World* var1, int var2, int var3, int var4, int var5);
	virtual void dropBlockAsItemWithChance(World* var1, int var2, int var3, int var4, int var5, float var6);

	static void initStatics();
};

#endif // BLOCKJUKEBOX_H