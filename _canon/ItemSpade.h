#ifndef ITEMSPADE_H
#define ITEMSPADE_H

// Auto-generated from ItemSpade.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ItemTool.h"

class Block;

class ItemSpade : public ItemTool {
public:
	ItemSpade(int var1, int var2);
	virtual ~ItemSpade();

	static JArray<Block* > s_blocksEffectiveAgainst;

	virtual bool canHarvestBlock(Block* var1);

	static void initStatics();
};

#endif // ITEMSPADE_H