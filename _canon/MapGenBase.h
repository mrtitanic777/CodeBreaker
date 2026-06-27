#ifndef MAPGENBASE_H
#define MAPGENBASE_H

// Auto-generated from MapGenBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class IChunkProvider;
class Random;
class World;

class MapGenBase {
public:
	MapGenBase();
	virtual ~MapGenBase();

	int range;
	Random* rand;

	virtual void generate(IChunkProvider* var1, World* var2, int var3, int var4, JArray<unsigned char > var5);
	virtual void recursiveGenerate(World* var1, int var2, int var3, int var4, int var5, JArray<unsigned char > var6);

	static void initStatics();
};

#endif // MAPGENBASE_H