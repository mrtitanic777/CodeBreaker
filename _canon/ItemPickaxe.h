#ifndef ITEMPICKAXE_H
#define ITEMPICKAXE_H

// Auto-generated from ItemPickaxe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ItemTool.h"

class Block;

class ItemPickaxe : public ItemTool {
public:
	ItemPickaxe(int var1, int var2);
	virtual ~ItemPickaxe();

	int toolMaterialLevel;
	static JArray<Block* > s_blocksEffectiveAgainst;

	virtual bool canHarvestBlock(Block* var1);

	static void initStatics();
};

#endif // ITEMPICKAXE_H