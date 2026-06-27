#ifndef ITEMAXE_H
#define ITEMAXE_H

// Auto-generated from ItemAxe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ItemTool.h"

class Block;

class ItemAxe : public ItemTool {
public:
	ItemAxe(int var1, int var2);
	virtual ~ItemAxe();

	static JArray<Block* > s_blocksEffectiveAgainst;

	static void initStatics();
};

#endif // ITEMAXE_H