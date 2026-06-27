#ifndef BLOCKBREAKABLE_H
#define BLOCKBREAKABLE_H

// Auto-generated from BlockBreakable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class IBlockAccess;
class Material;

class BlockBreakable : public Block {
public:
	BlockBreakable(int var1, int var2, Material* var3, bool var4);
	virtual ~BlockBreakable();

	bool renderAllSides;

	virtual bool isOpaqueCube();
	virtual bool shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // BLOCKBREAKABLE_H