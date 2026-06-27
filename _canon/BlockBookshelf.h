#ifndef BLOCKBOOKSHELF_H
#define BLOCKBOOKSHELF_H

// Auto-generated from BlockBookshelf.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class Random;

class BlockBookshelf : public Block {
public:
	BlockBookshelf(int var1, int var2);
	virtual ~BlockBookshelf();

	virtual int getBlockTextureFromSide(int var1);
	virtual int quantityDropped(Random* var1);

	static void initStatics();
};

#endif // BLOCKBOOKSHELF_H