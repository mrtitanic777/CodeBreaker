// LWJGL Keyboard / Mouse / Controllers on the PC, fed by SDL2 events (Display::update pumps them in).
// Key codes are LWJGL's (its DirectInput set); SDL scancodes are mapped onto them in Input.cpp.
#ifndef INPUT_H
#define INPUT_H
#include "stdafx.h"
#include <string>
#include "JavaString.h"

union SDL_Event;

namespace Input {
	void onEvent(const SDL_Event* e);       // Display.cpp: every SDL event
	void setDisplaySize(int w, int h);      // window size, for Mouse's bottom-left origin
}

namespace Keyboard {
	const int CHAR_NONE = 0, KEY_NONE = 0, KEYBOARD_SIZE = 256;
	const int KEY_ESCAPE = 1;
	const int KEY_1 = 2, KEY_2 = 3, KEY_3 = 4, KEY_4 = 5, KEY_5 = 6;
	const int KEY_6 = 7, KEY_7 = 8, KEY_8 = 9, KEY_9 = 10, KEY_0 = 11;
	const int KEY_MINUS = 12, KEY_EQUALS = 13, KEY_BACK = 14, KEY_TAB = 15;
	const int KEY_Q = 16, KEY_W = 17, KEY_E = 18, KEY_R = 19, KEY_T = 20;
	const int KEY_Y = 21, KEY_U = 22, KEY_I = 23, KEY_O = 24, KEY_P = 25;
	const int KEY_LBRACKET = 26, KEY_RBRACKET = 27, KEY_RETURN = 28, KEY_LCONTROL = 29;
	const int KEY_A = 30, KEY_S = 31, KEY_D = 32, KEY_F = 33, KEY_G = 34;
	const int KEY_H = 35, KEY_J = 36, KEY_K = 37, KEY_L = 38, KEY_SEMICOLON = 39;
	const int KEY_APOSTROPHE = 40, KEY_GRAVE = 41, KEY_LSHIFT = 42, KEY_BACKSLASH = 43;
	const int KEY_Z = 44, KEY_X = 45, KEY_C = 46, KEY_V = 47, KEY_B = 48;
	const int KEY_N = 49, KEY_M = 50, KEY_COMMA = 51, KEY_PERIOD = 52, KEY_SLASH = 53;
	const int KEY_RSHIFT = 54, KEY_MULTIPLY = 55, KEY_LMENU = 56, KEY_SPACE = 57, KEY_CAPITAL = 58;
	const int KEY_F1 = 59, KEY_F2 = 60, KEY_F3 = 61, KEY_F4 = 62, KEY_F5 = 63;
	const int KEY_F6 = 64, KEY_F7 = 65, KEY_F8 = 66, KEY_F9 = 67, KEY_F10 = 68;
	const int KEY_NUMLOCK = 69, KEY_SCROLL = 70, KEY_NUMPAD7 = 71, KEY_NUMPAD8 = 72, KEY_NUMPAD9 = 73;
	const int KEY_SUBTRACT = 74, KEY_NUMPAD4 = 75, KEY_NUMPAD5 = 76, KEY_NUMPAD6 = 77, KEY_ADD = 78;
	const int KEY_NUMPAD1 = 79, KEY_NUMPAD2 = 80, KEY_NUMPAD3 = 81, KEY_NUMPAD0 = 82, KEY_DECIMAL = 83;
	const int KEY_F11 = 87, KEY_F12 = 88, KEY_NUMPADENTER = 156, KEY_RCONTROL = 157, KEY_DIVIDE = 181;
	const int KEY_RMENU = 184, KEY_PAUSE = 197, KEY_HOME = 199, KEY_UP = 200, KEY_PRIOR = 201;
	const int KEY_LEFT = 203, KEY_RIGHT = 205, KEY_END = 207, KEY_DOWN = 208, KEY_NEXT = 209;
	const int KEY_INSERT = 210, KEY_DELETE = 211, KEY_LMETA = 219, KEY_LWIN = 219, KEY_RMETA = 220;
	const int KEY_RWIN = 220, KEY_APPS = 221;
	void create();
	void destroy();
	bool isCreated();
	void enableRepeatEvents(bool enable);
	bool isKeyDown(int key);
	bool next();
	int  getEventKey();
	bool getEventKeyState();
	char getEventCharacter();
	bool isRepeatEvent();
	JavaString getKeyName(int key);
	int  getKeyIndex(const std::string& name);
	inline void poll() {}
}

namespace Mouse {
	void create();
	void destroy();
	bool isCreated();
	inline void setNativeCursor(...) {}
	void setGrabbed(bool grabbed);
	bool isGrabbed();
	void setCursorPosition(int x, int y);
	int getDX();
	int getDY();
	int getDWheel();
	int getX();
	int getY();
	bool isButtonDown(int button);
	bool next();
	int  getEventButton();
	bool getEventButtonState();
	int  getEventX();
	int  getEventY();
	int  getEventDWheel();
	int  getButtonCount();
	inline void poll() {}
}

namespace Controllers {
	inline void create(...) {}
	inline void destroy(...) {}
	inline void poll(...) {}
	inline void clearEvents(...) {}
	inline bool isCreated(...) { return false; }
	inline int  getControllerCount(...) { return 0; }
	inline void* getController(...) { return 0; }
	inline bool next(...) { return false; }
	inline void* getEventSource(...) { return 0; }
	inline int  getEventControlIndex(...) { return 0; }
	inline bool getEventButtonState(...) { return false; }
}
#endif // INPUT_H
