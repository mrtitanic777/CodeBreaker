#include "stdafx.h"
#include "OsMap.h"

// Hand-ported from EnumOSMappingHelper.java (converter named the class OsMap).
// Target: Xbox 360 / C++03 / DirectX 9
//
// Java original built an int[] indexed by the EnumOS enum ordinal:
//     osValues[EnumOS.linux.ordinal()]   = 1;
//     osValues[EnumOS.solaris.ordinal()] = 2;
//     osValues[EnumOS.windows.ordinal()] = 3;
//     osValues[EnumOS.macos.ordinal()]   = 4;
// The per-assignment try/catch(NoSuchFieldError) guards are a no-op in C++03
// (the ordinals are compile-time constants and can never throw), so they are
// dropped. The converter mis-named the enum "EnumOS1"; the real enum in this
// codebase is EnumOS2 with declaration order { linux, solaris, windows, macos,
// unknown } -> ordinals 0..4 (length 5). To keep this small helper
// self-contained we use those literal ordinals rather than depending on an
// EnumOS header that does not exist in output_alpha.

// EnumOS ordinals (declaration order from EnumOS.java)
enum { OSMAP_LINUX = 0, OSMAP_SOLARIS = 1, OSMAP_WINDOWS = 2, OSMAP_MACOS = 3, OSMAP_UNKNOWN = 4 };
static const int OSMAP_COUNT = 5; // EnumOS.values().length

int* OsMap::s_field_1193_a = new int[OSMAP_COUNT];

OsMap::OsMap()
{
}

OsMap::~OsMap() {
	// No dynamic resources owned by an instance; s_field_1193_a is static.
}

void OsMap::initStatics() {
	// new[] zero-init isn't guaranteed in C++03, so clear first.
	for(int var0 = 0; var0 < OSMAP_COUNT; ++var0) {
		s_field_1193_a[var0] = 0;
	}

	s_field_1193_a[OSMAP_LINUX]   = 1;
	s_field_1193_a[OSMAP_SOLARIS] = 2;
	s_field_1193_a[OSMAP_WINDOWS] = 3;
	s_field_1193_a[OSMAP_MACOS]   = 4;
}
