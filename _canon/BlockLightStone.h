#ifndef BLOCKLIGHTSTONE_H
#define BLOCKLIGHTSTONE_H

// Auto-generated from BlockLightStone.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class Material;
class Random;

class BlockLightStone : public Block {
public:
	BlockLightStone(int var1, int var2, Material* var3);
	virtual ~BlockLightStone();

	virtual int idDropped(int var1, Random* var2);

	static void initStatics();
};

#endif // BLOCKLIGHTSTONE_H