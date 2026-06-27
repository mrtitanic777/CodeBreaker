#ifndef CHUNKPOSITION_H
#define CHUNKPOSITION_H

// Auto-generated from ChunkPosition.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"

class ChunkPosition {
public:
	ChunkPosition(int var1, int var2, int var3);
	virtual ~ChunkPosition();

	int x;
	int y;
	int z;

	virtual bool equals(JObject var1);
	virtual int hashCode();

	static void initStatics();
};

#endif // CHUNKPOSITION_H