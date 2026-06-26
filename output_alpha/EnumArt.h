#ifndef ENUMART_H
#define ENUMART_H

// Auto-generated from EnumArt.java (Java enum)
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

// Java enum -> struct of static singleton instances (compared by pointer identity).
struct EnumArt {
	std::string field_1624_y;
	int field_1623_z;
	int field_1636_A;
	int field_1634_B;
	int field_1632_C;
	int ordinal;        // Java ordinal()
	std::string nameStr; // Java name()

	EnumArt();
	EnumArt(const std::string& var3, int var4, int var5, int var6, int var7);

	static EnumArt* s_Kebab;
	static EnumArt* s_Aztec;
	static EnumArt* s_Alban;
	static EnumArt* s_Aztec2;
	static EnumArt* s_Bomb;
	static EnumArt* s_Plant;
	static EnumArt* s_Wasteland;
	static EnumArt* s_Pool;
	static EnumArt* s_Courbet;
	static EnumArt* s_Sea;
	static EnumArt* s_Sunset;
	static EnumArt* s_Creebet;
	static EnumArt* s_Wanderer;
	static EnumArt* s_Graham;
	static EnumArt* s_Match;
	static EnumArt* s_Bust;
	static EnumArt* s_Stage;
	static EnumArt* s_Void;
	static EnumArt* s_SkullAndRoses;
	static EnumArt* s_Fighters;
	static EnumArt* s_Pointer;
	static EnumArt* s_Pigscene;
	static EnumArt* s_Skeleton;
	static EnumArt* s_DonkeyKong;

	static EnumArt* values_[24];
	static const int values_count;
	static void initStatics();  // call once at startup (creates the singletons)
};

#endif // ENUMART_H