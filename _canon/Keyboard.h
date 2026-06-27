#ifndef KEYBOARD_H
#define KEYBOARD_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for org.lwjgl.input.Keyboard.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

namespace Keyboard {
	const int KEY_ESCAPE = 1;
	const int KEY_1 = 2;
	const int KEY_2 = 3;
	const int KEY_3 = 4;
	const int KEY_4 = 5;
	const int KEY_5 = 6;
	const int KEY_6 = 7;
	const int KEY_7 = 8;
	const int KEY_8 = 9;
	const int KEY_9 = 10;
	const int KEY_0 = 11;
	const int KEY_MINUS = 12;
	const int KEY_EQUALS = 13;
	const int KEY_BACK = 14;
	const int KEY_TAB = 15;
	const int KEY_Q = 16;
	const int KEY_W = 17;
	const int KEY_E = 18;
	const int KEY_R = 19;
	const int KEY_T = 20;
	const int KEY_Y = 21;
	const int KEY_U = 22;
	const int KEY_I = 23;
	const int KEY_O = 24;
	const int KEY_P = 25;
	const int KEY_LBRACKET = 26;
	const int KEY_RBRACKET = 27;
	const int KEY_RETURN = 28;
	const int KEY_LCONTROL = 29;
	const int KEY_A = 30;
	const int KEY_S = 31;
	const int KEY_D = 32;
	const int KEY_F = 33;
	const int KEY_G = 34;
	const int KEY_H = 35;
	const int KEY_J = 36;
	const int KEY_K = 37;
	const int KEY_L = 38;
	const int KEY_SEMICOLON = 39;
	const int KEY_APOSTROPHE = 40;
	const int KEY_GRAVE = 41;
	const int KEY_LSHIFT = 42;
	const int KEY_BACKSLASH = 43;
	const int KEY_Z = 44;
	const int KEY_X = 45;
	const int KEY_C = 46;
	const int KEY_V = 47;
	const int KEY_B = 48;
	const int KEY_N = 49;
	const int KEY_M = 50;
	const int KEY_COMMA = 51;
	const int KEY_PERIOD = 52;
	const int KEY_SLASH = 53;
	const int KEY_RSHIFT = 54;
	const int KEY_MULTIPLY = 55;
	const int KEY_LMENU = 56;
	const int KEY_SPACE = 57;
	const int KEY_CAPITAL = 58;
	const int KEY_F1 = 59;
	const int KEY_F2 = 60;
	const int KEY_F3 = 61;
	const int KEY_F4 = 62;
	const int KEY_F5 = 63;
	const int KEY_F6 = 64;
	const int KEY_F7 = 65;
	const int KEY_F8 = 66;
	const int KEY_F9 = 67;
	const int KEY_F10 = 68;
	const int KEY_NUMLOCK = 69;
	const int KEY_SCROLL = 70;
	const int KEY_F11 = 87;
	const int KEY_F12 = 88;
	const int KEY_NUMPAD0 = 82;
	const int KEY_NUMPAD1 = 79;
	const int KEY_NUMPAD2 = 80;
	const int KEY_NUMPAD3 = 81;
	const int KEY_NUMPAD4 = 75;
	const int KEY_NUMPAD5 = 76;
	const int KEY_NUMPAD6 = 77;
	const int KEY_NUMPAD7 = 71;
	const int KEY_NUMPAD8 = 72;
	const int KEY_NUMPAD9 = 73;
	const int KEY_ADD = 78;
	const int KEY_SUBTRACT = 74;
	const int KEY_DECIMAL = 83;
	const int KEY_RCONTROL = 157;
	const int KEY_RMENU = 184;
	const int KEY_HOME = 199;
	const int KEY_UP = 200;
	const int KEY_PRIOR = 201;
	const int KEY_LEFT = 203;
	const int KEY_RIGHT = 205;
	const int KEY_END = 207;
	const int KEY_DOWN = 208;
	const int KEY_NEXT = 209;
	const int KEY_INSERT = 210;
	const int KEY_DELETE = 211;
	const int CHAR_NONE = 0;
	const int KEY_NONE = 0;
	inline void create(...) {}
	inline void destroy(...) {}
	inline void enableRepeatEvents(...) {}
	inline int getEventKey(...) { return 0; }
	inline bool getEventKeyState(...) { return false; }
	inline char getEventCharacter(...) { return 0; }
	inline bool isKeyDown(...) { return false; }
	inline bool next(...) { return false; }
	inline std::string getKeyName(...) { return std::string(); }
}
#endif // KEYBOARD_H
