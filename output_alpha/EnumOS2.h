#ifndef ENUMOS2_H
#define ENUMOS2_H

// Auto-generated from EnumOS2.java (Java enum)
// Target: Xbox 360 / C++03 / DirectX 9

// Java enum -> struct of static singleton instances (compared by pointer identity).
struct EnumOS2 {
	int ordinal;        // Java ordinal()
	std::string nameStr; // Java name()

	EnumOS2();

	static EnumOS2* s_a;
	static EnumOS2* s_b;
	static EnumOS2* s_c;
	static EnumOS2* s_d;
	static EnumOS2* s_unknown;

	static EnumOS2* values_[5];
	static const int values_count;
	static void initStatics();  // call once at startup (creates the singletons)
};

#endif // ENUMOS2_H