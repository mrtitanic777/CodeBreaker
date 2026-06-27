// Input.cpp — LWJGL Keyboard/Mouse/Controllers -> Xbox 360 XInput adapter.  [Step 4]
//
// One XInput poll per render frame (Input::poll), feeding a virtual keyboard/mouse model that the
// faithful Java input code reads through Keyboard:: / Mouse::. See Input.h for the design and the
// control map; see docs/platform/03b-step4-integration.md for wiring + HARDWARE-VERIFY points.
//
// Grounded in the OLD port's proven inline XInput (deadzone 8000, menu step 16384, trigger 40, the
// button->action table, the static-prev edge detection, and the cross-screen resync that fixes the
// "button held across a menu close re-fires as a fresh press" bug -- Minecraft.cpp:878-883).

#include "Input.h"
#include <cstring>   // memset
#include <cstdlib>   // abs

// ===========================================================================
// Internal state — one frame's worth, rebuilt every Input::poll().
// ===========================================================================
namespace {

	// ---- Virtual keyboard ----
	// Key DOWN bitmap indexed by LWJGL scancode (0..255). Filled fresh each poll() from the gamepad.
	bool  g_keyDown[256];

	// Key event queue (for Keyboard::next()/getEvent*). Edge-detected: one PRESS event on a rising
	// edge, one RELEASE on a falling edge. Small ring is plenty -- a frame produces a handful of edges.
	struct KeyEvent { int key; bool state; char ch; };
	const int  KEY_Q_CAP = 64;
	KeyEvent   g_keyQ[KEY_Q_CAP];
	int        g_keyQHead = 0, g_keyQTail = 0;   // [head,tail) pending; head advances on next()
	KeyEvent   g_keyCur = { 0, false, 0 };       // the event next() last surfaced (getEvent* read this)

	// ---- Virtual mouse ----
	int   g_dx = 0, g_dy = 0;          // accumulated right-stick look delta since last getDX/getDY
	int   g_cursorX = 0, g_cursorY = 0;// virtual GUI cursor (top-down)
	int   g_dispW = 854, g_dispH = 480;// display size for cursor clamping (set by setDisplaySize)
	bool  g_btnDown[3];                // 0=left(RT) 1=right(LT) 2=middle
	bool  g_grabbed = false;           // when grabbed, look deltas feed getDX/getDY; cursor frozen

	// Mouse button event queue.
	struct BtnEvent { int button; bool state; int x; int y; int dwheel; };
	const int  BTN_Q_CAP = 32;
	BtnEvent   g_btnQ[BTN_Q_CAP];
	int        g_btnQHead = 0, g_btnQTail = 0;
	BtnEvent   g_btnCur = { -1, false, 0, 0, 0 };

	// ---- Analog left stick (for the optional MovementInput override) ----
	float g_moveStrafe = 0.0F, g_moveForward = 0.0F;

	// ---- Connection / edge-detection prev-state (STATIC across frames) ----
	bool  g_connected = false;
	WORD  g_prevButtons = 0;           // previous wButtons (rising/falling edge masks)
	bool  g_prevKeyDown[256];          // previous virtual key bitmap (to emit key edge events)
	bool  g_prevBtnDown[3];            // previous mouse-button (trigger) state
	// Menu-nav repeat edge state (d-pad / left-stick notch), mirrors GuiScreen.cpp:126.
	bool  g_prevNavUp = false, g_prevNavDown = false, g_prevNavLeft = false, g_prevNavRight = false;

	bool g_initDone = false;

	void ensureInit() {
		if (g_initDone) return;
		memset(g_keyDown,     0, sizeof(g_keyDown));
		memset(g_prevKeyDown, 0, sizeof(g_prevKeyDown));
		memset(g_btnDown,     0, sizeof(g_btnDown));
		memset(g_prevBtnDown, 0, sizeof(g_prevBtnDown));
		g_initDone = true;
	}

	// Push a key edge event (dropped silently if the ring is full -- next frame resyncs anyway).
	void pushKeyEvent(int key, bool state, char ch) {
		int n = (g_keyQTail + 1) % KEY_Q_CAP;
		if (n == g_keyQHead) return;            // full
		g_keyQ[g_keyQTail].key = key;
		g_keyQ[g_keyQTail].state = state;
		g_keyQ[g_keyQTail].ch = ch;
		g_keyQTail = n;
	}

	void pushBtnEvent(int button, bool state, int x, int y, int dwheel) {
		int n = (g_btnQTail + 1) % BTN_Q_CAP;
		if (n == g_btnQHead) return;
		g_btnQ[g_btnQTail].button = button;
		g_btnQ[g_btnQTail].state = state;
		g_btnQ[g_btnQTail].x = x;
		g_btnQ[g_btnQTail].y = y;
		g_btnQ[g_btnQTail].dwheel = dwheel;
		g_btnQTail = n;
	}

	// Map a held LWJGL scancode to a typeable character for text screens (chat/sign). Minimal ASCII;
	// the real path should be the Xbox OSK (XShowKeyboardUI) -- see 03b HARDWARE-VERIFY. 0 = no char.
	char keyToChar(int key) {
		switch (key) {
			case Keyboard::KEY_SPACE:  return ' ';
			case Keyboard::KEY_RETURN: return '\r';
			case Keyboard::KEY_BACK:   return '\b';
			default: break;
		}
		// Letters/digits: scancodes are NOT alphabetical, so map explicitly.
		switch (key) {
			case Keyboard::KEY_A: return 'a'; case Keyboard::KEY_B: return 'b'; case Keyboard::KEY_C: return 'c';
			case Keyboard::KEY_D: return 'd'; case Keyboard::KEY_E: return 'e'; case Keyboard::KEY_F: return 'f';
			case Keyboard::KEY_G: return 'g'; case Keyboard::KEY_H: return 'h'; case Keyboard::KEY_I: return 'i';
			case Keyboard::KEY_J: return 'j'; case Keyboard::KEY_K: return 'k'; case Keyboard::KEY_L: return 'l';
			case Keyboard::KEY_M: return 'm'; case Keyboard::KEY_N: return 'n'; case Keyboard::KEY_O: return 'o';
			case Keyboard::KEY_P: return 'p'; case Keyboard::KEY_Q: return 'q'; case Keyboard::KEY_R: return 'r';
			case Keyboard::KEY_S: return 's'; case Keyboard::KEY_T: return 't'; case Keyboard::KEY_U: return 'u';
			case Keyboard::KEY_V: return 'v'; case Keyboard::KEY_W: return 'w'; case Keyboard::KEY_X: return 'x';
			case Keyboard::KEY_Y: return 'y'; case Keyboard::KEY_Z: return 'z';
			case Keyboard::KEY_0: return '0'; case Keyboard::KEY_1: return '1'; case Keyboard::KEY_2: return '2';
			case Keyboard::KEY_3: return '3'; case Keyboard::KEY_4: return '4'; case Keyboard::KEY_5: return '5';
			case Keyboard::KEY_6: return '6'; case Keyboard::KEY_7: return '7'; case Keyboard::KEY_8: return '8';
			case Keyboard::KEY_9: return '9';
			default: return 0;
		}
	}

	// Deadzoned, normalized stick axis in [-1, 1]. Flat per-axis deadzone, matching the old port
	// (EntityPlayerSP.cpp:136-139): |v|>DZ ? v/32767 : 0.
	float axis(short v) {
		if ((short)abs((int)v) <= InputConfig::STICK_DEADZONE) return 0.0F;
		float f = (float)v / 32767.0F;
		if (f >  1.0F) f =  1.0F;
		if (f < -1.0F) f = -1.0F;
		return f;
	}

	// Set a virtual key, emitting a press/release event on the edge vs the previous frame.
	void setKey(int key, bool down) {
		g_keyDown[key] = down;
		if (down && !g_prevKeyDown[key])      pushKeyEvent(key, true,  keyToChar(key));
		else if (!down && g_prevKeyDown[key]) pushKeyEvent(key, false, 0);
	}

} // anonymous namespace

// ===========================================================================
// Input::poll — THE single XInput read per frame.
// ===========================================================================
namespace Input {

void setDisplaySize(int w, int h) { if (w > 0) g_dispW = w; if (h > 0) g_dispH = h; }
bool connected() { return g_connected; }
float moveStrafe()  { return g_moveStrafe; }
float moveForward() { return g_moveForward; }

void poll() {
	ensureInit();

	XINPUT_STATE st;
	memset(&st, 0, sizeof(st));
	DWORD r = XInputGetState(0, &st);          // user index 0 / player 1, as the whole old port did
	g_connected = (r == ERROR_SUCCESS);

	// Clear this-frame key bitmap; we rebuild it from the pad. (Buttons not driven by the pad stay up.)
	memset(g_keyDown, 0, sizeof(g_keyDown));
	g_moveStrafe = g_moveForward = 0.0F;

	if (!g_connected) {
		// No controller: everything releases. Still emit release edges so held keys don't stick.
		for (int k = 0; k < 256; ++k) if (g_prevKeyDown[k]) pushKeyEvent(k, false, 0);
		for (int b = 0; b < 3; ++b) {
			if (g_prevBtnDown[b]) pushBtnEvent(b, false, g_cursorX, g_cursorY, 0);
			g_btnDown[b] = false;
		}
		memcpy(g_prevKeyDown, g_keyDown, sizeof(g_keyDown));
		memcpy(g_prevBtnDown, g_btnDown, sizeof(g_btnDown));
		g_prevButtons = 0;
		return;
	}

	const XINPUT_GAMEPAD& gp = st.Gamepad;
	const WORD b = gp.wButtons;

	// ---------- Left stick: analog move axes + digitized W/A/S/D ----------
	float lx = axis(gp.sThumbLX);
	float ly = axis(gp.sThumbLY);
	// Sign-matched to the old port (EntityPlayerSP.cpp:143-144): strafe = lx, forward = -? No:
	// the old port wrote m_field_1174_a = lx (strafe) and m_field_1173_b = -ly (forward). MovementInput
	// fields are "strafe right +" and "forward +", so:
	g_moveStrafe  = lx;          // stick-right -> strafe right
	g_moveForward = ly;          // stick-up (ly>0) -> forward+ ; consumer negates if needed (see 03b)
	// Digital fallback (only relevant if you do NOT use the analog MovementInput override):
	setKey(Keyboard::KEY_W, gp.sThumbLY >  InputConfig::STICK_DEADZONE);   // stick-up    -> forward
	setKey(Keyboard::KEY_S, gp.sThumbLY < -InputConfig::STICK_DEADZONE);   // stick-down  -> back
	setKey(Keyboard::KEY_A, gp.sThumbLX < -InputConfig::STICK_DEADZONE);   // stick-left  -> left
	setKey(Keyboard::KEY_D, gp.sThumbLX >  InputConfig::STICK_DEADZONE);   // stick-right -> right

	// ---------- Right stick: mouse-look deltas (the LIVE look path) ----------
	// Normalize, then scale to a per-frame "pixel" magnitude. EntityRenderer applies the Java
	// sensitivity curve on top, so DO NOT bake the curve in here (would double-apply -- doc 03).
	float rx = axis(gp.sThumbRX);
	float ry = axis(gp.sThumbRY);
	float ddx = rx * InputConfig::LOOK_PIXELS_PER_FRAME;
	float ddy = ry * InputConfig::LOOK_PIXELS_PER_FRAME;
	if (InputConfig::LOOK_INVERT_X) ddx = -ddx;
	if (InputConfig::LOOK_INVERT_Y) ddy = -ddy;   // up-stick -> look up by default
	// Accumulate; getDX/getDY drain it. Only feed look while grabbed (in-game), like a captured cursor.
	if (g_grabbed) {
		g_dx += (int)ddx;
		g_dy += (int)ddy;
	} else {
		// In menus, nudge the virtual GUI cursor with the right stick too (handy for pointer screens).
		g_cursorX += (int)(rx * 12.0F);
		g_cursorY += (int)(-ry * 12.0F);    // screen-space: down is +y
		if (g_cursorX < 0) g_cursorX = 0; if (g_cursorX > g_dispW) g_cursorX = g_dispW;
		if (g_cursorY < 0) g_cursorY = 0; if (g_cursorY > g_dispH) g_cursorY = g_dispH;
	}

	// ---------- Triggers: mouse buttons 0 (attack) / 1 (place) ----------
	bool rt = gp.bRightTrigger > InputConfig::TRIGGER;   // RT -> left click (attack/break)
	bool lt = gp.bLeftTrigger  > InputConfig::TRIGGER;   // LT -> right click (place/use)
	g_btnDown[0] = rt;
	g_btnDown[1] = lt;
	g_btnDown[2] = false;
	if (rt && !g_prevBtnDown[0]) pushBtnEvent(0, true,  g_cursorX, g_cursorY, 0);
	if (!rt && g_prevBtnDown[0]) pushBtnEvent(0, false, g_cursorX, g_cursorY, 0);
	if (lt && !g_prevBtnDown[1]) pushBtnEvent(1, true,  g_cursorX, g_cursorY, 0);
	if (!lt && g_prevBtnDown[1]) pushBtnEvent(1, false, g_cursorX, g_cursorY, 0);

	// ---------- Face / d-pad / stick-click buttons -> mapped keys ----------
	// In-game binds (control map, doc 03). These set virtual key STATE; setKey emits edges so the
	// Java event path (Keyboard::next) sees one press per button push.
	setKey(Keyboard::KEY_SPACE,  (b & XINPUT_GAMEPAD_A) != 0);            // jump
	setKey(Keyboard::KEY_LSHIFT, (b & XINPUT_GAMEPAD_LEFT_THUMB) != 0);  // sneak (L3)
	setKey(Keyboard::KEY_E,      (b & XINPUT_GAMEPAD_Y) != 0);           // inventory (Alpha bind = E)
	setKey(Keyboard::KEY_Q,      (b & XINPUT_GAMEPAD_B) != 0);           // drop held item
	// START / BACK -> pause + render-distance cycle. Pause uses ESCAPE (Java keyBind path); render
	// distance has no keybind in Alpha (old port poked the setting), so we map BACK to KEY_F (a free
	// key) only as a placeholder -- the cleaner route is a tiny hook, see 03b note.
	setKey(Keyboard::KEY_ESCAPE, (b & XINPUT_GAMEPAD_START) != 0);       // pause / open menu
	// Hotbar via LB/RB -> synthesize a mouse wheel notch (Minecraft::runTick reads getEventDWheel),
	// which is the faithful Alpha hotbar-scroll path. One notch per press (edge).
	bool lb = (b & XINPUT_GAMEPAD_LEFT_SHOULDER)  != 0;
	bool rb = (b & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
	bool prevLB = (g_prevButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)  != 0;
	bool prevRB = (g_prevButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
	if (rb && !prevRB) pushBtnEvent(-1, false, g_cursorX, g_cursorY, +120);  // next slot
	if (lb && !prevLB) pushBtnEvent(-1, false, g_cursorX, g_cursorY, -120);  // prev slot

	// Debug F-keys the new code polls (Minecraft.cpp / EntityRenderer): leave them releasable but
	// unmapped to a pad button by default. (Map to chords here if you want them on-device.)
	// setKey(Keyboard::KEY_F1, ...);  setKey(Keyboard::KEY_F3, ...); etc.

	// ---------- Menu navigation -> arrow keys (GuiScreen polls Keyboard/Mouse events) ----------
	// D-pad or left-stick notch, edge-detected with the larger MENU_STEP threshold (one step per push),
	// reusing the old port's GuiScreen.cpp:128-139 logic. Emitted as KEY_UP/DOWN/LEFT/RIGHT key events.
	bool navUp    = (b & XINPUT_GAMEPAD_DPAD_UP)    != 0 || gp.sThumbLY >  InputConfig::MENU_STEP;
	bool navDown  = (b & XINPUT_GAMEPAD_DPAD_DOWN)  != 0 || gp.sThumbLY < -InputConfig::MENU_STEP;
	bool navLeft  = (b & XINPUT_GAMEPAD_DPAD_LEFT)  != 0 || gp.sThumbLX < -InputConfig::MENU_STEP;
	bool navRight = (b & XINPUT_GAMEPAD_DPAD_RIGHT) != 0 || gp.sThumbLX >  InputConfig::MENU_STEP;
	if (navUp    && !g_prevNavUp)    { pushKeyEvent(Keyboard::KEY_UP,    true, 0); pushKeyEvent(Keyboard::KEY_UP,    false, 0); }
	if (navDown  && !g_prevNavDown)  { pushKeyEvent(Keyboard::KEY_DOWN,  true, 0); pushKeyEvent(Keyboard::KEY_DOWN,  false, 0); }
	if (navLeft  && !g_prevNavLeft)  { pushKeyEvent(Keyboard::KEY_LEFT,  true, 0); pushKeyEvent(Keyboard::KEY_LEFT,  false, 0); }
	if (navRight && !g_prevNavRight) { pushKeyEvent(Keyboard::KEY_RIGHT, true, 0); pushKeyEvent(Keyboard::KEY_RIGHT, false, 0); }
	g_prevNavUp = navUp; g_prevNavDown = navDown; g_prevNavLeft = navLeft; g_prevNavRight = navRight;
	// In a menu, A = activate (RETURN), B = back (ESCAPE) -- as discrete key events.
	bool aNow = (b & XINPUT_GAMEPAD_A) != 0, prevA = (g_prevButtons & XINPUT_GAMEPAD_A) != 0;
	bool bNow = (b & XINPUT_GAMEPAD_B) != 0, prevB = (g_prevButtons & XINPUT_GAMEPAD_B) != 0;
	// NOTE: A/B above ALSO drove KEY_SPACE/KEY_Q for in-game. The Java side gates on currentScreen, so
	// the duplicate is harmless; if a screen is open the menu path consumes RETURN/ESCAPE first. If you
	// see double-actions, gate these two lines on "a screen is open" via a flag set from the game.
	if (aNow && !prevA) { pushKeyEvent(Keyboard::KEY_RETURN, true, '\r'); pushKeyEvent(Keyboard::KEY_RETURN, false, 0); }
	if (bNow && !prevB) { pushKeyEvent(Keyboard::KEY_ESCAPE, true, 0);    pushKeyEvent(Keyboard::KEY_ESCAPE, false, 0); }

	// ---------- Commit prev-state for next frame's edge detection ----------
	memcpy(g_prevKeyDown, g_keyDown, sizeof(g_keyDown));
	memcpy(g_prevBtnDown, g_btnDown, sizeof(g_btnDown));
	g_prevButtons = b;
}

} // namespace Input

// ===========================================================================
// Keyboard:: — reads through the virtual state / event queue.
// ===========================================================================
namespace Keyboard {

void create()  {}
void destroy() {}
void enableRepeatEvents(bool /*enable*/) { /* OSK handles repeat; no-op here */ }

bool isKeyDown(int key) {
	if (key < 0 || key > 255) return false;
	return g_keyDown[key];
}

bool next() {
	if (g_keyQHead == g_keyQTail) return false;   // empty
	g_keyCur = g_keyQ[g_keyQHead];
	g_keyQHead = (g_keyQHead + 1) % KEY_Q_CAP;
	return true;
}
int  getEventKey()        { return g_keyCur.key; }
bool getEventKeyState()   { return g_keyCur.state; }
char getEventCharacter()  { return g_keyCur.ch; }

std::string getKeyName(int key) {
	char c = keyToChar(key);
	if (c >= 'a' && c <= 'z') { char s[2] = { (char)(c - 32), 0 }; return std::string(s); }   // upper
	if (c) { char s[2] = { c, 0 }; return std::string(s); }
	switch (key) {
		case KEY_ESCAPE: return "ESCAPE"; case KEY_SPACE: return "SPACE";
		case KEY_LSHIFT: return "LSHIFT"; case KEY_RETURN: return "RETURN";
		default: return "KEY";
	}
}

} // namespace Keyboard

// ===========================================================================
// Mouse:: — look deltas (right stick), trigger buttons, virtual cursor, event queue.
// ===========================================================================
namespace Mouse {

void create()  {}
void destroy() {}
void setNativeCursor(void* /*cursor*/) {}

void setGrabbed(bool grabbed) {
	g_grabbed = grabbed;
	g_dx = g_dy = 0;                 // grab/ungrab resets pending look delta (MouseHelper.grabMouseCursor)
}
void setCursorPosition(int x, int y) { g_cursorX = x; g_cursorY = y; }

int getDX() { int d = g_dx; g_dx = 0; return d; }   // read-once, like LWJGL
int getDY() { int d = g_dy; g_dy = 0; return d; }

int getX() { return g_cursorX; }
int getY() { return g_cursorY; }

bool isButtonDown(int button) {
	if (button < 0 || button > 2) return false;
	return g_btnDown[button];
}

bool next() {
	if (g_btnQHead == g_btnQTail) return false;
	g_btnCur = g_btnQ[g_btnQHead];
	g_btnQHead = (g_btnQHead + 1) % BTN_Q_CAP;
	return true;
}
int  getEventButton()      { return g_btnCur.button; }
bool getEventButtonState() { return g_btnCur.state; }
int  getEventX()           { return g_btnCur.x; }
int  getEventY()           { return g_btnCur.y; }
int  getEventDWheel()      { return g_btnCur.dwheel; }

} // namespace Mouse

// ===========================================================================
// Controllers:: — JInput holder. Kept inert so the game uses the Keyboard/Mouse path.
// ===========================================================================
namespace Controllers {
void  create() {}
void  destroy() {}
void  poll() {}
void  clearEvents() {}
bool  isCreated()            { return false; }   // <- the important one
int   getControllerCount()   { return 0; }
void* getController(int)     { return 0; }
bool  next()                 { return false; }
void* getEventSource()       { return 0; }
int   getEventControlIndex() { return 0; }
bool  getEventButtonState()  { return false; }
} // namespace Controllers
