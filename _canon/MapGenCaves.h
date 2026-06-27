#ifndef MAPGENCAVES_H
#define MAPGENCAVES_H

// Auto-generated from MapGenCaves.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "MapGenBase.h"

class World;

class MapGenCaves : public MapGenBase {
public:
	virtual ~MapGenCaves();

	virtual void generateLargeCaveNode(int var1, int var2, JArray<unsigned char > var3, double var4, double var6, double var8);
	virtual void generateCaveNode(int var1, int var2, JArray<unsigned char > var3, double var4, double var6, double var8, float var10, float var11, float var12, int var13, int var14, double var15);
	virtual void recursiveGenerate(World* var1, int var2, int var3, int var4, int var5, JArray<unsigned char > var6);

	static void initStatics();
};

#endif // MAPGENCAVES_H