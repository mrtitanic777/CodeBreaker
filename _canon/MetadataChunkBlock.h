#ifndef METADATACHUNKBLOCK_H
#define METADATACHUNKBLOCK_H

// Auto-generated from MetadataChunkBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EnumSkyBlock.h"

class World;

class MetadataChunkBlock {
public:
	MetadataChunkBlock(int var1, int var2, int var3, int var4, int var5, int var6, int var7);
	virtual ~MetadataChunkBlock();

	int skyBlock;
	int minX;
	int minY;
	int minZ;
	int maxX;
	int maxY;
	int maxZ;

	virtual void updateSkylight(World* var1);
	virtual bool expandToContain(int var1, int var2, int var3, int var4, int var5, int var6);

	static void initStatics();
};

#endif // METADATACHUNKBLOCK_H