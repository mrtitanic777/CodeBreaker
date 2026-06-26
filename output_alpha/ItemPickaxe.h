#ifndef ITEMPICKAXE_H
#define ITEMPICKAXE_H

// Auto-generated from ItemPickaxe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemTool.h"

class Block;

class ItemPickaxe : public ItemTool {
public:
	ItemPickaxe();
	ItemPickaxe(int var1, int var2);
	virtual ~ItemPickaxe();

	virtual bool canHarvestBlock(Block* var1);

public: // was private
	static Block** s_blocksEffectiveAgainst;
	int m_field_328_aY;

};

#endif // ITEMPICKAXE_H