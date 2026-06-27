#ifndef IBLOCKACCESS_H
#define IBLOCKACCESS_H

// Auto-generated from IBlockAccess.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Material;
class TileEntity;
class WorldChunkManager;

class IBlockAccess {
public:

	virtual int getBlockId(int var1, int var2, int var3) = 0;
	virtual TileEntity* getBlockTileEntity(int var1, int var2, int var3) = 0;
	virtual float getLightBrightness(int var1, int var2, int var3) = 0;
	virtual int getBlockMetadata(int var1, int var2, int var3) = 0;
	virtual Material* getBlockMaterial(int var1, int var2, int var3) = 0;
	virtual bool isBlockOpaqueCube(int var1, int var2, int var3) = 0;
	virtual WorldChunkManager* getWorldChunkManager() = 0;
};

#endif // IBLOCKACCESS_H