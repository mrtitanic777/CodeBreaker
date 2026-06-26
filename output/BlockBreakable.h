#ifndef BLOCKBREAKABLE_H
#define BLOCKBREAKABLE_H

// Auto-generated from BlockBreakable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class Material;
class World;

class BlockBreakable : public Block {
public:
	BlockBreakable();
	BlockBreakable(int var1, int var2, Material* var3, bool var4);
	virtual ~BlockBreakable();

	virtual bool isOpaqueCube();
	virtual bool shouldSideBeRendered(World* var1, int var2, int var3, int var4, int var5);

public: // was protected
public: // was private
	bool m_localFlag;

};

#endif // BLOCKBREAKABLE_H