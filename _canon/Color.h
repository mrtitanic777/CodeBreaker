#ifndef COLOR_H
#define COLOR_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.awt.Color.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

class Color {
public:
	Color(...) {}
	virtual ~Color() {}
	virtual int getRGB(...) { return 0; }
	virtual int getRed(...) { return 0; }
	virtual int getGreen(...) { return 0; }
	virtual int getBlue(...) { return 0; }
	virtual int getAlpha(...) { return 0; }
	static int HSBtoRGB(...) { return 0; }
	static Color getHSBColor(...) { return Color(); }
	static const int BLACK = (int)0xFF000000;
	static const int WHITE = (int)0xFFFFFFFF;
	static const int GRAY = (int)0xFF808080;
	static const int LIGHT_GRAY = (int)0xFFC0C0C0;
	static const int DARK_GRAY = (int)0xFF404040;
	static const int RED = (int)0xFFFF0000;
	static const int GREEN = (int)0xFF00FF00;
	static const int BLUE = (int)0xFF0000FF;
	static const int YELLOW = (int)0xFFFFFF00;
	static const int CYAN = (int)0xFF00FFFF;
	static const int MAGENTA = (int)0xFFFF00FF;
	static const int ORANGE = (int)0xFFFFC800;
	static const int PINK = (int)0xFFFFAFAF;
	static const int black = (int)0xFF000000;
	static const int white = (int)0xFFFFFFFF;
	static const int gray = (int)0xFF808080;
	static const int lightGray = (int)0xFFC0C0C0;
	static const int darkGray = (int)0xFF404040;
	static const int red = (int)0xFFFF0000;
	static const int green = (int)0xFF00FF00;
	static const int blue = (int)0xFF0000FF;
	static const int yellow = (int)0xFFFFFF00;
	static const int cyan = (int)0xFF00FFFF;
	static const int magenta = (int)0xFFFF00FF;
	static const int orange = (int)0xFFFFC800;
	static const int pink = (int)0xFFFFAFAF;
};
#endif // COLOR_H
