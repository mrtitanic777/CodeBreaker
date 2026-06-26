#ifndef METADATACHUNKBLOCK_H
#define METADATACHUNKBLOCK_H

// Auto-generated from MetadataChunkBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

struct EnumSkyBlock;
class World;

class MetadataChunkBlock {
public:
	MetadataChunkBlock();
	MetadataChunkBlock(EnumSkyBlock* var1, int var2, int var3, int var4, int var5, int var6, int var7);
	virtual ~MetadataChunkBlock();

	EnumSkyBlock* m_field_1299_a;
	int m_field_1298_b;
	int m_field_1304_c;
	int m_field_1303_d;
	int m_field_1302_e;
	int m_field_1301_f;
	int m_field_1300_g;

	virtual void func_4127_a(World* var1);
	virtual bool func_866_a(int var1, int var2, int var3, int var4, int var5, int var6);

};

#endif // METADATACHUNKBLOCK_H