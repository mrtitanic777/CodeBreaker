#ifndef ITEMAXE_H
#define ITEMAXE_H

// Auto-generated from ItemAxe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemTool.h"

class Block;

class ItemAxe : public ItemTool {
public:
	ItemAxe();
	ItemAxe(int var1, int var2);
	virtual ~ItemAxe();


private:
	static Block* s_blocksEffectiveAgainst;

};

#endif // ITEMAXE_H