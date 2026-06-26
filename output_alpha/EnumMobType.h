#ifndef ENUMMOBTYPE_H
#define ENUMMOBTYPE_H

// Auto-generated from EnumMobType.java (Java enum)
// Target: Xbox 360 / C++03 / DirectX 9

// Java enum -> struct of static singleton instances (compared by pointer identity).
struct EnumMobType {
	int ordinal;        // Java ordinal()
	std::string nameStr; // Java name()

	EnumMobType();

	static EnumMobType* s_everything;
	static EnumMobType* s_mobs;
	static EnumMobType* s_players;

	static EnumMobType* values_[3];
	static const int values_count;
	static void initStatics();  // call once at startup (creates the singletons)
};

#endif // ENUMMOBTYPE_H