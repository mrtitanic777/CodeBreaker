#ifndef ENUMSKYBLOCK_H
#define ENUMSKYBLOCK_H

// Auto-generated from EnumSkyBlock.java (Java enum)
// Target: Xbox 360 / C++03 / DirectX 9

// Java enum -> struct of static singleton instances (compared by pointer identity).
struct EnumSkyBlock {
	int field_1722_c;
	int ordinal;        // Java ordinal()
	std::string nameStr; // Java name()

	EnumSkyBlock();
	EnumSkyBlock(int var3);

	static EnumSkyBlock* s_Sky;
	static EnumSkyBlock* s_Block;

	static EnumSkyBlock* values_[2];
	static const int values_count;
	static void initStatics();  // call once at startup (creates the singletons)
};

#endif // ENUMSKYBLOCK_H