#ifndef BLOCKLOG_H
#define BLOCKLOG_H

// Auto-generated from BlockLog.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class Random;

class BlockLog : public Block {
public:
	BlockLog();
	BlockLog(int var1);
	virtual ~BlockLog();

	virtual int quantityDropped(Random* var1);
	virtual int idDropped(int var1);
	virtual int getBlockTextureFromSide(int var1);

public: // was protected
};

#endif // BLOCKLOG_H