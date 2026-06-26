#ifndef ENUMCREATURETYPE_H
#define ENUMCREATURETYPE_H

// Auto-generated from EnumCreatureType.java (Java enum)
// Target: Xbox 360 / C++03 / DirectX 9

class Class;

// Java enum -> struct of static singleton instances (compared by pointer identity).
struct EnumCreatureType {
	void* field_4278_c;
	int maxNumberOfEntityType;
	int ordinal;        // Java ordinal()
	std::string nameStr; // Java name()

	EnumCreatureType();
	EnumCreatureType(void* var3, int var4);

	static EnumCreatureType* s_monster;
	static EnumCreatureType* s_creature;

	static EnumCreatureType* values_[2];
	static const int values_count;
	static void initStatics();  // call once at startup (creates the singletons)
};

#endif // ENUMCREATURETYPE_H