#ifndef METADATACHUNKBLOCK_H
#define METADATACHUNKBLOCK_H

// Auto-generated from MetadataChunkBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

class Light;

class MetadataChunkBlock {
public:
	MetadataChunkBlock();
	MetadataChunkBlock(Light* var1, int var2, int var3, int var4, int var5, int var6, int var7);
	virtual ~MetadataChunkBlock();

	int m_x;
	int m_y;
	int m_z;
	int maxX;
	int maxY;
	int maxZ;


};

#endif // METADATACHUNKBLOCK_H