// Input.h — LWJGL Keyboard/Mouse/Controllers -> Xbox 360 XInput adapter for the j2x port.  [Step 4]
//
// REFERENCE IMPLEMENTATION for the Xbox-project integration. It REPLACES the converter's no-op
// Keyboard / Mouse stubs (j2x/runtime.py Keyboard.h / Mouse.h): the new game code is faithful to the
// Java and calls Keyboard::isKeyDown(KEY_*) / Keyboard::next()/getEventKey()/getEventKeyState() /
// Mouse::getDX()/getDY() / Mouse::isButtonDown(0|1) / Mouse::next()/getEventButton(), so a real
// backing makes input work. Lives in the Xbox project against the XDK's XInput (xinput.h via xtl.h);
// it is NOT compiled by the j2x build (which only syntax-checks the no-op stubs). Grounded in the OLD
// port's proven inline XInput mapping (Minecraft.cpp:847-884, EntityPlayerSP.cpp:132-152,
// GuiScreen.cpp:114-172) and docs/platform/03-input-and-audio.md.
//
// Design: ONE XInput poll per render frame. The platform layer calls Input::poll() once at the top of
// the game loop body (Minecraft::run, before runTick()). poll() reads XINPUT_STATE(user 0) once, then
// derives the whole LWJGL key/mouse model from it:
//   * Right stick   -> Mouse::getDX()/getDY()  (the LIVE look path: EntityRenderer reads
//                      mouseHelper->updateMouseDelta() -> getDX/getDY every frame, applies the Java
//                      sensitivity curve itself -- so getDX/getDY return RAW pixel-like deltas, NOT
//                      pre-gained, or the curve double-applies). This is the path the OLD port left
//                      DEAD (MouseHelper was a no-op) and re-implemented by mutating rotationYaw/Pitch;
//                      the NEW build does it the faithful way.
//   * Triggers      -> Mouse::isButtonDown(0) (RT = attack/break) and isButtonDown(1) (LT = place/use),
//                      plus synthesized button events on the rising/falling edge for Mouse::next().
//   * Face/d-pad/stick-click buttons -> Keyboard::isKeyDown(KEY_*) state for the mapped binds (jump,
//                      sneak, inventory, drop, pause, hotbar, F-keys, menu nav) + a key event queue for
//                      Keyboard::next()/getEventKey()/getEventKeyState()/getEventCharacter().
//   * Left stick    -> ANALOG movement (recommended in doc 03): poll() exposes the normalized axes via
//                      Input::moveStrafe()/moveForward() for a thin MovementInput override to consume.
//                      It ALSO digitizes the left stick into KEY_W/A/S/D so the pure-keyboard Java path
//                      still moves if you skip the analog override. Pick one (see 03b integration).
// Constants reused verbatim from the old port: stick deadzone 8000, menu step 16384, trigger 40.
// HARDWARE-VERIFY (can't be checked off-device): right-stick sensitivity magnitude + Y-invert sign,
// deadzone feel, and whether an OSK (XShowKeyboardUI) is wired for chat/sign text. See 03b doc.

#ifndef INPUT_H
#define INPUT_H

#include "stdafx.h"          // xtl.h pulls in xinput.h (XInputGetState / XINPUT_STATE / XINPUT_GAMEPAD_*)
#include <string>

// ---------------------------------------------------------------------------
// Tunables (start = old-port constants; tune the look gain on hardware).
// ---------------------------------------------------------------------------
namespace InputConfig {
	const short STICK_DEADZONE   = 8000;    // old port _DZ (EntityPlayerSP.cpp:135) -- flat per-axis
	const short MENU_STEP        = 16384;   // old port one-notch menu threshold (GuiScreen.cpp:129-131)
	const unsigned char TRIGGER  = 40;      // old port TRIG (Minecraft.cpp:856) -- pulled when value > 40

	// Right-stick -> mouse look. The look gain here is the "pixels of mouse movement per frame at full
	// deflection" BEFORE the Java sensitivity curve ((sens*0.6+0.2)^3 * 8) in EntityRenderer:411-414.
	// Old port mutated rotation directly with normalized * 6.0F, so 6 deg/frame at full tilt was the
	// felt speed; an LWJGL "pixel" maps ~1 px -> the curve, so ~8-12 px/frame is a sane start.
	// HARDWARE-VERIFY: tune LOOK_PIXELS_PER_FRAME so full-deflection turn speed feels right.
	const float LOOK_PIXELS_PER_FRAME = 10.0F;

	// Right-stick Y sign. LWJGL getDY is screen-space; XInput sThumbRY is up-positive. EntityRenderer
	// applies invertMouse on top, so this is the BASE sign before the user's invert option.
	// HARDWARE-VERIFY: if pitch is inverted with invertMouse OFF, flip LOOK_INVERT_Y.
	const bool  LOOK_INVERT_Y    = true;    // negate getDY by default (up-stick -> look up)
	const bool  LOOK_INVERT_X    = false;   // right-stick-right -> look right
}

// ---------------------------------------------------------------------------
// Input — the single per-frame XInput poller + the virtual K/M state it fills.
// The platform layer owns this; the game never calls it directly (it calls
// Keyboard::/Mouse:: which read through it).
// ---------------------------------------------------------------------------
namespace Input {
	// Called ONCE per render frame from the platform layer (Minecraft::run, before runTick()).
	// Reads XInput(0) once and rebuilds the entire virtual keyboard/mouse state + event queues.
	void poll();

	// True if a controller answered the last poll() (XInputGetState == ERROR_SUCCESS).
	bool connected();

	// Analog left-stick axes for an optional MovementInput override (see 03b). Normalized [-1,1],
	// deadzoned. strafe>0 = right, forward>0 = forward (already sign-matched to the old port).
	float moveStrafe();
	float moveForward();

	// Set the on-screen display size so Mouse::getX()/getY() (GUI cursor) scale correctly.
	void setDisplaySize(int w, int h);
}

// ---------------------------------------------------------------------------
// org.lwjgl.input.Keyboard — REPLACES runtime.py Keyboard.h.
// KEY_* codes are the LWJGL DirectInput scancodes, IDENTICAL to the converter stub
// (runtime.py:801-824) so game code that compares against KEY_* / keyBind->keyCode still matches.
// ---------------------------------------------------------------------------
namespace Keyboard {
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
	const int KEY_RSHIFT = 54, KEY_MULTIPLY = 55, KEY_LMENU = 56, KEY_SPACE = 57;
	const int KEY_CAPITAL = 58;
	const int KEY_F1 = 59, KEY_F2 = 60, KEY_F3 = 61, KEY_F4 = 62, KEY_F5 = 63;
	const int KEY_F6 = 64, KEY_F7 = 65, KEY_F8 = 66, KEY_F9 = 67, KEY_F10 = 68;
	const int KEY_NUMLOCK = 69, KEY_SCROLL = 70, KEY_F11 = 87, KEY_F12 = 88;
	const int KEY_NUMPAD0 = 82, KEY_NUMPAD1 = 79, KEY_NUMPAD2 = 80, KEY_NUMPAD3 = 81;
	const int KEY_NUMPAD4 = 75, KEY_NUMPAD5 = 76, KEY_NUMPAD6 = 77, KEY_NUMPAD7 = 71;
	const int KEY_NUMPAD8 = 72, KEY_NUMPAD9 = 73, KEY_ADD = 78, KEY_SUBTRACT = 74;
	const int KEY_DECIMAL = 83, KEY_RCONTROL = 157, KEY_RMENU = 184, KEY_HOME = 199;
	const int KEY_UP = 200, KEY_PRIOR = 201, KEY_LEFT = 203, KEY_RIGHT = 205;
	const int KEY_END = 207, KEY_DOWN = 208, KEY_NEXT = 209, KEY_INSERT = 210;
	const int KEY_DELETE = 211, CHAR_NONE = 0, KEY_NONE = 0;

	// lifecycle (called by Minecraft::startGame / shutdown). No-ops on console: poll() owns the device.
	void create();
	void destroy();
	void enableRepeatEvents(bool enable);

	// state query — backed by the current virtual key bitmap (filled by Input::poll()).
	bool isKeyDown(int key);

	// event queue (LWJGL): next() advances to the next queued key event; the getEvent* read it.
	bool next();
	int  getEventKey();
	bool getEventKeyState();   // true = pressed (down), false = released
	char getEventCharacter();  // mapped char for the event key (for text screens); 0 if none
	std::string getKeyName(int key);
}

// ---------------------------------------------------------------------------
// org.lwjgl.input.Mouse — REPLACES runtime.py Mouse.h.
// getDX/getDY are the LIVE look deltas (right stick). isButtonDown(0|1) are the triggers.
// getX/getY are a virtual GUI cursor (left stick / d-pad nudges it in menus). next()/getEvent* are
// the synthesized button-event queue used by GuiScreen::handleMouseInput and Minecraft::runTick.
// ---------------------------------------------------------------------------
namespace Mouse {
	// lifecycle
	void create();
	void destroy();
	void setNativeCursor(void* cursor);            // no-op on console
	void setGrabbed(bool grabbed);                 // MouseHelper grab/ungrab -> reset deltas
	void setCursorPosition(int x, int y);          // recenter virtual cursor

	// look deltas — consumed once per frame by EntityRenderer via MouseHelper::updateMouseDelta().
	// Returns the right-stick delta for THIS frame and clears it (LWJGL getDX/getDY are read-once).
	int getDX();
	int getDY();

	// virtual cursor position (GUI). y is top-down here; callers flip to MC's bottom-up coords.
	int getX();
	int getY();

	// button state — triggers. 0 = left (RT, attack/break), 1 = right (LT, place/use), 2 = middle.
	bool isButtonDown(int button);

	// event queue (synthesized on trigger / cursor edges).
	bool next();
	int  getEventButton();        // 0/1/2, or -1 for a move-only event
	bool getEventButtonState();   // true = pressed
	int  getEventX();
	int  getEventY();
	int  getEventDWheel();        // hotbar scroll surrogate (LB/RB) -> +/-120 notches
}

// ---------------------------------------------------------------------------
// org.lwjgl.input.Controllers — REPLACES runtime.py Controllers.h.
// The OLD port never used JInput Controllers; the NEW Java code only touches it via isCreated().
// Keep isCreated() == false so the game stays on the Keyboard/Mouse path we back with XInput.
// (If the new code is ever found to REQUIRE a controller present, flip isCreated() to true and
//  forward the rest to Input::; left as the simplest correct surface for now.)
// ---------------------------------------------------------------------------
namespace Controllers {
	void create();
	void destroy();
	void poll();
	void clearEvents();
	bool isCreated();             // returns false -> Java falls back to Keyboard/Mouse (what we back)
	int  getControllerCount();
	void* getController(int i);
	bool next();
	void* getEventSource();
	int  getEventControlIndex();
	bool getEventButtonState();
}

#endif // INPUT_H
