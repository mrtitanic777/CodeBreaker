#ifndef ENUMOS1_H
#define ENUMOS1_H

// Auto-generated from EnumOS1.java (Java enum)
// Target: Xbox 360 / C++03 / DirectX 9

// Java enum -> struct of static singleton instances (compared by pointer identity).
struct EnumOS1 {
	int ordinal;        // Java ordinal()
	std::string nameStr; // Java name()

	EnumOS1();

	static EnumOS1* s_linux;
	static EnumOS1* s_solaris;
	static EnumOS1* s_windows;
	static EnumOS1* s_macos;
	static EnumOS1* s_unknown;

	static EnumOS1* values_[5];
	static const int values_count;
	static void initStatics();  // call once at startup (creates the singletons)
};

#endif // ENUMOS1_H