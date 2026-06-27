#ifndef ENUMART_H
#define ENUMART_H

// Auto-generated from EnumArt.java
// Target: Xbox 360 / C++03 / DirectX 9
//
// EnumArt in Java is a RICH enum: every constant carries (title, sizeX, sizeY,
// offsetX, offsetY) and the enum exposes values(). The first converter pass
// emitted it as a flat list of ordinal ints, which dropped that data and broke
// every consumer (EntityPainting / RenderPainting access art.sizeX / .title and
// call EnumArt.values()). This header restores the value-typed enum while
// keeping the original ordinal constants for any code that uses them as ints.

#include "JavaString.h"
#include "JArray.h"

struct EnumArt {
	// Original ordinal constants (kept so ordinal-based code still compiles).
	static const int Kebab = 0;
	static const int Aztec = 1;
	static const int Alban = 2;
	static const int Aztec2 = 3;
	static const int Bomb = 4;
	static const int Plant = 5;
	static const int Wasteland = 6;
	static const int Pool = 7;
	static const int Courbet = 8;
	static const int Sea = 9;
	static const int Sunset = 10;
	static const int Creebet = 11;
	static const int Wanderer = 12;
	static const int Graham = 13;
	static const int Match = 14;
	static const int Bust = 15;
	static const int Stage = 16;
	static const int Void = 17;
	static const int SkullAndRoses = 18;
	static const int Fighters = 19;
	static const int Pointer = 20;
	static const int Pigscene = 21;
	static const int Skeleton = 22;
	static const int DonkeyKong = 23;

	// Per-constant payload (matches the Java enum constructor:
	// EnumArt(String name, int id, String title, int sizeX, int sizeY, int offsetX, int offsetY)).
	int id;
	JavaString title;
	int sizeX;
	int sizeY;
	int offsetX;
	int offsetY;

	EnumArt() : id(0), title(), sizeX(0), sizeY(0), offsetX(0), offsetY(0) {}
	EnumArt(int aId, const JavaString& aTitle, int sx, int sy, int ox, int oy)
		: id(aId), title(aTitle), sizeX(sx), sizeY(sy), offsetX(ox), offsetY(oy) {}

	// Implicit ordinal so existing `int x = someEnumArt;` keeps working.
	operator int() const { return id; }

	// Java: EnumArt.values()
	static JArray<EnumArt> values() {
		JArray<EnumArt> a(24);
		a[0]  = EnumArt(Kebab,         "Kebab",         16, 16,   0,   0);
		a[1]  = EnumArt(Aztec,         "Aztec",         16, 16,  16,   0);
		a[2]  = EnumArt(Alban,         "Alban",         16, 16,  32,   0);
		a[3]  = EnumArt(Aztec2,        "Aztec2",        16, 16,  48,   0);
		a[4]  = EnumArt(Bomb,          "Bomb",          16, 16,  64,   0);
		a[5]  = EnumArt(Plant,         "Plant",         16, 16,  80,   0);
		a[6]  = EnumArt(Wasteland,     "Wasteland",     16, 16,  96,   0);
		a[7]  = EnumArt(Pool,          "Pool",          32, 16,   0,  32);
		a[8]  = EnumArt(Courbet,       "Courbet",       32, 16,  32,  32);
		a[9]  = EnumArt(Sea,           "Sea",           32, 16,  64,  32);
		a[10] = EnumArt(Sunset,        "Sunset",        32, 16,  96,  32);
		a[11] = EnumArt(Creebet,       "Creebet",       32, 16, 128,  32);
		a[12] = EnumArt(Wanderer,      "Wanderer",      16, 32,   0,  64);
		a[13] = EnumArt(Graham,        "Graham",        16, 32,  16,  64);
		a[14] = EnumArt(Match,         "Match",         32, 32,   0, 128);
		a[15] = EnumArt(Bust,          "Bust",          32, 32,  32, 128);
		a[16] = EnumArt(Stage,         "Stage",         32, 32,  64, 128);
		a[17] = EnumArt(Void,          "Void",          32, 32,  96, 128);
		a[18] = EnumArt(SkullAndRoses, "SkullAndRoses", 32, 32, 128, 128);
		a[19] = EnumArt(Fighters,      "Fighters",      64, 32,   0,  96);
		a[20] = EnumArt(Pointer,       "Pointer",       64, 64,   0, 192);
		a[21] = EnumArt(Pigscene,      "Pigscene",      64, 64,  64, 192);
		a[22] = EnumArt(Skeleton,      "Skeleton",      64, 48, 192,  64);
		a[23] = EnumArt(DonkeyKong,    "DonkeyKong",    64, 48, 192, 112);
		return a;
	}

	// Convenience: fetch a constant by ordinal (used where Java referenced a named constant).
	static EnumArt byId(int id) {
		JArray<EnumArt> a = values();
		return (id >= 0 && id < a.length) ? a[id] : a[0];
	}
};

#endif // ENUMART_H
