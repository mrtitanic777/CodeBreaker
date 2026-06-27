#ifndef CHUNKCOORDINTPAIR_H
#define CHUNKCOORDINTPAIR_H

// Auto-generated from ChunkCoordIntPair.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"

class ChunkCoordIntPair {
public:
	ChunkCoordIntPair(int var1, int var2);
	virtual ~ChunkCoordIntPair();

	int chunkXPos;
	int chunkZPos;

	virtual int hashCode();
	virtual bool equals(JObject var1);

	static void initStatics();
};

#endif // CHUNKCOORDINTPAIR_H