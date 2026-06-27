#ifndef CHUNKCOORDINATES_H
#define CHUNKCOORDINATES_H

// Auto-generated from ChunkCoordinates.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"

class ChunkCoordinates {
public:
	ChunkCoordinates(int var1, int var2);
	virtual ~ChunkCoordinates();

	int posX;
	int posZ;

	virtual bool equals(JObject var1);
	virtual int hashCode();

	static void initStatics();
};

#endif // CHUNKCOORDINATES_H