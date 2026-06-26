#ifndef LIGHT_H
#define LIGHT_H

// Auto-generated from Light.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MetadataChunkBlock.h"
#include <string>
#include <vector>

class World;

class Light {
public:
	Light();
	Light(World* var1);
	virtual ~Light();

	virtual void updateSkylight(int var1, int var2, int var3, int var4);
	virtual void updateDaylightCycle(int var1);
	virtual void updateBlockLight(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual void updateLight();
	virtual const std::string& debugSkylightUpdates();

private:
	int m_lightingUpdateCounter;
	std::vector<void*> /* PORT: determine element type */ m_lightingUpdateList;
	World* worldObj;
	int m_worldWidth;
	int m_worldLength;
	int m_worldHeight;
	unsigned char* m_blocks;
	unsigned char* m_data;
	int* m_heightMap;
	std::vector<void*> /* PORT: determine element type */ m_skyLightList;
	std::vector<void*> /* PORT: determine element type */ m_blockLightList;
	std::vector<void*> /* PORT: determine element type */ m_lightingUpdateList1;
	std::vector<void*> /* PORT: determine element type */ m_lightingUpdateList2;
	int* m_lightingUpdateList3;
	unsigned char* m_chunks;
	MetadataChunkBlock m_metadataChunkBlock;
	bool m_hasMetadataChunkBlock;
	int m_lightValue;
	int m_skylightSubtracted;

	int* getLightingUpdates();
	void updateLight(int var1);
	void updateLists(int var1, int var2, int var3, int var4, int var5, int var6);

};

#endif // LIGHT_H