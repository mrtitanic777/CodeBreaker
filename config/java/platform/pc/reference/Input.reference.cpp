// Keyboard / Mouse on SDL2 events (see Input.reference.h).
#include "stdafx.h"
#include "Input.h"
#include <SDL2/SDL.h>
#include <deque>
#include <cstring>

namespace {

struct KeyEvent { int key; bool down; char ch; bool repeat; };
struct MouseEvent { int button; bool down; int x, y, wheel; };

std::deque<KeyEvent> g_keys;
std::deque<MouseEvent> g_mice;
KeyEvent g_key = { 0, false, 0, false };
MouseEvent g_mouse = { -1, false, 0, 0, 0 };
bool g_down[256];
bool g_repeat = false;
int g_w = 854, g_h = 480;
int g_x = 0, g_y = 0, g_dx = 0, g_dy = 0, g_wheel = 0;
bool g_grabbed = false;

// SDL scancode -> LWJGL key code (0: none)
int lwjgl(SDL_Scancode s) {
	if (s >= SDL_SCANCODE_A && s <= SDL_SCANCODE_Z) {
		static const int letters[26] = {30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38, 50, 49, 24, 25, 16,
		                                19, 31, 20, 22, 47, 17, 45, 21, 44};
		return letters[s - SDL_SCANCODE_A];
	}
	if (s >= SDL_SCANCODE_1 && s <= SDL_SCANCODE_9) return 2 + (s - SDL_SCANCODE_1);
	if (s >= SDL_SCANCODE_F1 && s <= SDL_SCANCODE_F10) return 59 + (s - SDL_SCANCODE_F1);
	switch (s) {
	case SDL_SCANCODE_0: return 11;
	case SDL_SCANCODE_ESCAPE: return 1;
	case SDL_SCANCODE_MINUS: return 12;
	case SDL_SCANCODE_EQUALS: return 13;
	case SDL_SCANCODE_BACKSPACE: return 14;
	case SDL_SCANCODE_TAB: return 15;
	case SDL_SCANCODE_LEFTBRACKET: return 26;
	case SDL_SCANCODE_RIGHTBRACKET: return 27;
	case SDL_SCANCODE_RETURN: return 28;
	case SDL_SCANCODE_LCTRL: return 29;
	case SDL_SCANCODE_SEMICOLON: return 39;
	case SDL_SCANCODE_APOSTROPHE: return 40;
	case SDL_SCANCODE_GRAVE: return 41;
	case SDL_SCANCODE_LSHIFT: return 42;
	case SDL_SCANCODE_BACKSLASH: return 43;
	case SDL_SCANCODE_COMMA: return 51;
	case SDL_SCANCODE_PERIOD: return 52;
	case SDL_SCANCODE_SLASH: return 53;
	case SDL_SCANCODE_RSHIFT: return 54;
	case SDL_SCANCODE_KP_MULTIPLY: return 55;
	case SDL_SCANCODE_LALT: return 56;
	case SDL_SCANCODE_SPACE: return 57;
	case SDL_SCANCODE_CAPSLOCK: return 58;
	case SDL_SCANCODE_NUMLOCKCLEAR: return 69;
	case SDL_SCANCODE_SCROLLLOCK: return 70;
	case SDL_SCANCODE_KP_7: return 71;
	case SDL_SCANCODE_KP_8: return 72;
	case SDL_SCANCODE_KP_9: return 73;
	case SDL_SCANCODE_KP_MINUS: return 74;
	case SDL_SCANCODE_KP_4: return 75;
	case SDL_SCANCODE_KP_5: return 76;
	case SDL_SCANCODE_KP_6: return 77;
	case SDL_SCANCODE_KP_PLUS: return 78;
	case SDL_SCANCODE_KP_1: return 79;
	case SDL_SCANCODE_KP_2: return 80;
	case SDL_SCANCODE_KP_3: return 81;
	case SDL_SCANCODE_KP_0: return 82;
	case SDL_SCANCODE_KP_PERIOD: return 83;
	case SDL_SCANCODE_F11: return 87;
	case SDL_SCANCODE_F12: return 88;
	case SDL_SCANCODE_KP_ENTER: return 156;
	case SDL_SCANCODE_RCTRL: return 157;
	case SDL_SCANCODE_KP_DIVIDE: return 181;
	case SDL_SCANCODE_RALT: return 184;
	case SDL_SCANCODE_PAUSE: return 197;
	case SDL_SCANCODE_HOME: return 199;
	case SDL_SCANCODE_UP: return 200;
	case SDL_SCANCODE_PAGEUP: return 201;
	case SDL_SCANCODE_LEFT: return 203;
	case SDL_SCANCODE_RIGHT: return 205;
	case SDL_SCANCODE_END: return 207;
	case SDL_SCANCODE_DOWN: return 208;
	case SDL_SCANCODE_PAGEDOWN: return 209;
	case SDL_SCANCODE_INSERT: return 210;
	case SDL_SCANCODE_DELETE: return 211;
	case SDL_SCANCODE_LGUI: return 219;
	case SDL_SCANCODE_RGUI: return 220;
	case SDL_SCANCODE_APPLICATION: return 221;
	default: return 0;
	}
}

// the character a key event carries (LWJGL reports it on the press): printable ASCII from the keycode,
// shifted; the few control characters text screens test for
char character(const SDL_Keysym& k) {
	bool shift = (k.mod & KMOD_SHIFT) != 0;
	SDL_Keycode c = k.sym;
	if (c == SDLK_RETURN || c == SDLK_KP_ENTER) return '\r';
	if (c == SDLK_BACKSPACE) return '\b';
	if (c == SDLK_TAB) return '\t';
	if (c < 32 || c > 126) return 0;
	if (c >= 'a' && c <= 'z') return (char)(shift != ((k.mod & KMOD_CAPS) != 0) ? c - 32 : c);
	if (!shift) return (char)c;
	static const char from[] = "1234567890-=[];'\\,./`";
	static const char to[]   = "!@#$%^&*()_+{}:\"|<>?~";
	const char* p = std::strchr(from, (int)c);
	return p ? to[p - from] : (char)c;
}

int toMouseButton(Uint8 b) {
	return b == SDL_BUTTON_LEFT ? 0 : b == SDL_BUTTON_RIGHT ? 1 : b == SDL_BUTTON_MIDDLE ? 2 : (int)b - 1;
}

} // namespace

namespace Input {

void setDisplaySize(int w, int h) {
	if (w > 0) g_w = w;
	if (h > 0) g_h = h;
}

void onEvent(const SDL_Event* e) {
	switch (e->type) {
	case SDL_KEYDOWN:
	case SDL_KEYUP: {
		int key = lwjgl(e->key.keysym.scancode);
		bool down = e->type == SDL_KEYDOWN;
		if (e->key.repeat && !g_repeat) break;
		if (key > 0 && key < 256) g_down[key] = down;
		KeyEvent k = { key, down, down ? character(e->key.keysym) : (char)0, e->key.repeat != 0 };
		g_keys.push_back(k);
		break;
	}
	case SDL_MOUSEMOTION:
		g_x = e->motion.x;
		g_y = g_h - 1 - e->motion.y;                     // LWJGL's origin is bottom-left
		g_dx += e->motion.xrel;
		g_dy -= e->motion.yrel;
		{
			MouseEvent m = { -1, false, g_x, g_y, 0 };
			g_mice.push_back(m);
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP: {
		MouseEvent m = { toMouseButton(e->button.button), e->type == SDL_MOUSEBUTTONDOWN, g_x, g_y, 0 };
		g_mice.push_back(m);
		break;
	}
	case SDL_MOUSEWHEEL: {
		int notch = e->wheel.y * 120;                      // LWJGL reports wheel deltas in 120ths
		g_wheel += notch;
		MouseEvent m = { -1, false, g_x, g_y, notch };
		g_mice.push_back(m);
		break;
	}
	default:
		break;
	}
}

} // namespace Input

namespace Keyboard {

void create() {}
void destroy() {}
bool isCreated() { return true; }
void enableRepeatEvents(bool enable) { g_repeat = enable; }
bool isKeyDown(int key) { return key > 0 && key < 256 && g_down[key]; }

bool next() {
	if (g_keys.empty()) return false;
	g_key = g_keys.front();
	g_keys.pop_front();
	return true;
}

int getEventKey() { return g_key.key; }
bool getEventKeyState() { return g_key.down; }
char getEventCharacter() { return g_key.ch; }
bool isRepeatEvent() { return g_key.repeat; }

JavaString getKeyName(int key) {
	static const char* names[256] = {0};
	static bool init = false;
	if (!init) {
		const char* n[] = {"NONE", "ESCAPE", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "MINUS", "EQUALS",
		                   "BACK", "TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "LBRACKET",
		                   "RBRACKET", "RETURN", "LCONTROL", "A", "S", "D", "F", "G", "H", "J", "K", "L",
		                   "SEMICOLON", "APOSTROPHE", "GRAVE", "LSHIFT", "BACKSLASH", "Z", "X", "C", "V", "B",
		                   "N", "M", "COMMA", "PERIOD", "SLASH", "RSHIFT", "MULTIPLY", "LMENU", "SPACE",
		                   "CAPITAL", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUMLOCK",
		                   "SCROLL", "NUMPAD7", "NUMPAD8", "NUMPAD9", "SUBTRACT", "NUMPAD4", "NUMPAD5",
		                   "NUMPAD6", "ADD", "NUMPAD1", "NUMPAD2", "NUMPAD3", "NUMPAD0", "DECIMAL"};
		for (int i = 0; i < (int)(sizeof(n) / sizeof(n[0])); ++i) names[i] = n[i];
		names[87] = "F11"; names[88] = "F12"; names[156] = "NUMPADENTER"; names[157] = "RCONTROL";
		names[181] = "DIVIDE"; names[184] = "RMENU"; names[197] = "PAUSE"; names[199] = "HOME";
		names[200] = "UP"; names[201] = "PRIOR"; names[203] = "LEFT"; names[205] = "RIGHT";
		names[207] = "END"; names[208] = "DOWN"; names[209] = "NEXT"; names[210] = "INSERT";
		names[211] = "DELETE"; names[219] = "LMETA"; names[220] = "RMETA"; names[221] = "APPS";
		init = true;
	}
	return JavaString((key >= 0 && key < 256 && names[key]) ? names[key] : "NONE");
}

int getKeyIndex(const std::string& name) {
	for (int i = 0; i < 256; ++i)
		if (getKeyName(i) == JavaString(name)) return i;
	return 0;
}

} // namespace Keyboard

namespace Mouse {

void create() {}
void destroy() {}
bool isCreated() { return true; }

void setGrabbed(bool grabbed) {
	g_grabbed = grabbed;
	SDL_SetRelativeMouseMode(grabbed ? SDL_TRUE : SDL_FALSE);
	g_dx = g_dy = 0;
}

bool isGrabbed() { return g_grabbed; }

void setCursorPosition(int x, int y) {
	g_x = x;
	g_y = y;
	SDL_Window* w = SDL_GetMouseFocus();
	if (w && !g_grabbed) SDL_WarpMouseInWindow(w, x, g_h - 1 - y);
}

int getDX() { int d = g_dx; g_dx = 0; return d; }
int getDY() { int d = g_dy; g_dy = 0; return d; }
int getDWheel() { int d = g_wheel; g_wheel = 0; return d; }
int getX() { return g_x; }
int getY() { return g_y; }

bool isButtonDown(int button) {
	Uint32 s = SDL_GetMouseState(0, 0);
	Uint32 mask = button == 0 ? SDL_BUTTON_LMASK : button == 1 ? SDL_BUTTON_RMASK : button == 2 ? SDL_BUTTON_MMASK : 0;
	return (s & mask) != 0;
}

bool next() {
	if (g_mice.empty()) return false;
	g_mouse = g_mice.front();
	g_mice.pop_front();
	return true;
}

int getEventButton() { return g_mouse.button; }
bool getEventButtonState() { return g_mouse.down; }
int getEventX() { return g_mouse.x; }
int getEventY() { return g_mouse.y; }
int getEventDWheel() { return g_mouse.wheel; }
int getButtonCount() { return 3; }

} // namespace Mouse
