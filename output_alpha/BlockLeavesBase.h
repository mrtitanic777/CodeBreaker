#ifndef BLOCKLEAVESBASE_H
#define BLOCKLEAVESBASE_H

// Auto-generated from BlockLeavesBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class IBlockAccess;
class Material;

class BlockLeavesBase : public Block {
public:
	BlockLeavesBase();
	BlockLeavesBase(int var1, int var2, Material* var3, bool var4);
	virtual ~BlockLeavesBase();

	virtual bool isOpaqueCube();
	virtual bool shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5);

public: // was protected
	bool m_graphicsLevel;


};

#endif // BLOCKLEAVESBASE_H