#ifndef ITEMSPADE_H
#define ITEMSPADE_H

// Auto-generated from ItemSpade.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemTool.h"

class Block;

class ItemSpade : public ItemTool {
public:
	ItemSpade();
	ItemSpade(int var1, int var2);
	virtual ~ItemSpade();

	virtual bool canHarvestBlock(Block* var1);

public: // was private
	static Block** s_blocksEffectiveAgainst;

};

#endif // ITEMSPADE_H