#ifndef ICHUNKLOADER_H
#define ICHUNKLOADER_H

// Auto-generated from IChunkLoader.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Chunk;
class World;

class IChunkLoader {
public:

	virtual Chunk* loadChunk(World* var1, int var2, int var3) = 0;
	virtual void saveChunk(World* var1, Chunk* var2) = 0;
	virtual void saveExtraChunkData(World* var1, Chunk* var2) = 0;
	virtual void func_814_a() = 0;
	virtual void saveExtraData() = 0;
};

#endif // ICHUNKLOADER_H