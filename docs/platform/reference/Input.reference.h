// Input.h — LWJGL Keyboard/Mouse/Controllers -> Xbox 360 XInput adapter for the j2ctool port.  [Step 4]
//
// REFERENCE IMPLEMENTATION for the Xbox-project integration. It REPLACES the converter's no-op
// Keyboard / Mouse stubs (j2ctool/runtime.py Keyboard.h / Mouse.h): the new game code is faithful to the
// Java and calls Keyboard::isKeyDown(KEY_*) / Keyboard::next()/getEventKey()/getEventKeyState() /
// Mouse::getDX()/getDY() / Mouse::isButtonDown(0|1) / Mouse::next()/getEventButton(), so a real
// backing makes input work. Lives in the Xbox project against the XDK's XInput (xinput.h via xtl.h);
// it is NOT compiled by the j2ctool build (which only syntax-checks the no-op stubs). Grounded in the OLD
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

#include "stdafx.h"          // xtl.h -> xbox.h/xinputdefs.h (button-bit constants, used for edge masks)
#include <xinput2.h>         // THE Xbox 360 controller API: XInput2Sample / XInput2Get{Bool,Float,Point} / XINPUTID_*
#include <string>

// ---------------------------------------------------------------------------
// Tunables (start = old-port constants; tune the look gain on hardware).
// ---------------------------------------------------------------------------
namespace InputConfig {
	// XInput2 thumbsticks/triggers are normalized floats (NOT the PC API's shorts/bytes), so the live
	// thresholds are these float ones. HARDWARE-VERIFY the exact range on-device (treated as [-1,1]).
	const float STICK_DEADZONE_F      = 0.24F;   // ~ old 8000/32767; below this an axis reads 0
	const float TRIGGER_F             = 0.12F;   // trigger counts as "pulled" above this
	// GUI cursor speed in pixels PER SECOND (frame-rate-independent). Per-FRAME movement flew across the
	// screen because the menu renders with no vsync (uncapped FPS). HARDWARE-VERIFY the feel.
	const float CURSOR_PIXELS_PER_SECOND = 650.0F;

	const short STICK_DEADZONE   = 8000;    // (legacy short constants kept for reference / any old callers)
	const short MENU_STEP        = 16384;
	const unsigned char TRIGGER  = 40;

	// Right-stick -> mouse look, in pixels PER SECOND (frame-rate-independent, like CURSOR_PIXELS_PER_SECOND).
	// [J2X-BUGFIX fps-look-scale] This was PER-FRAME (40 px/frame). Uncapped FPS (Release_LTCG runs many
	// hundreds of fps) then turned the camera ~10x too fast, exactly the bug the GUI cursor already fixed.
	// Poll now scales by real elapsed seconds: ddx = rx * LOOK_PIXELS_PER_SECOND * dt.
	// The felt speed is preserved at the 60fps reference: 40 px/frame * 60 = 2400 px/s, and the Java chain
	// (rx * px * sensCurve(=1.0 at default 0.5) * 0.15 in setAngles) still yields 6 deg/frame @60 == Indev.
	// HARDWARE-VERIFY: tune LOOK_PIXELS_PER_SECOND so full-deflection turn speed feels right on-device.
	// Tuning: turn speed at full deflection (deg/sec) = LOOK_PIXELS_PER_SECOND / 2400 * 360.
	//   600 -> 90 deg/s   750 -> ~112 deg/s   900 -> 135 deg/s   1200 -> 180 deg/s   2400 -> 360 deg/s.
	const float LOOK_PIXELS_PER_SECOND = 750.0F;

	// Right-stick Y sign. LWJGL getDY is screen-space; XInput sThumbRY is up-positive. EntityRenderer
	// applies invertMouse on top, so this is the BASE sign before the user's invert option.
	// HARDWARE-VERIFY: if pitch is inverted with invertMouse OFF, flip LOOK_INVERT_Y.
	// stick-up must = look UP. The chain (getDY -> setAngles does rotationPitch -= dy*0.15) means
	// NO negation here gives up-stick->look-up; negating inverted it (the reported bug). So: false.
	// NOTE: invert-look and Southpaw (stick-swap) are NOT Beta features and are intentionally
	// omitted -- the base look signs above are already correct (up-stick = look up), so there is
	// nothing to invert or swap. Do not re-add them as toggles unless the game gains the option.
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

	// Discrete menu navigation (Indev-style). Each returns true ONCE per d-pad / left-stick press
	// (edge-triggered + latched until read, so an edge isn't lost on a tickless frame). GuiScreen
	// consumes these to move the selected button and snaps the cursor onto it, so the existing
	// hover-highlight + A-to-click land on the selection. The RIGHT stick stays a free analog pointer
	// for non-button GUIs (slots). Replaces the old analog-cursor drift that scrolled "super fast".
	bool navUp();
	bool navDown();
	bool navLeft();
	bool navRight();
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
	// Meta/command keys (LWJGL scancodes). Defined even though no 360 controller key maps to them --
	// games reference them in bind tables (RubyDung's Player.KEY_LMETA). [J2X-BUGFIX key-meta-consts]
	const int KEY_LMETA = 219, KEY_RMETA = 221, KEY_LWIN = 219, KEY_RWIN = 221, KEY_APPS = 221;

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

// True only while a container GUI (inventory/chest/furnace/crafting) is open -- set by
// Minecraft::displayGuiScreen. Gates the LEFT-stick free cursor + Pointer.png icon to those screens.
extern bool g_guiCursorMode;

#endif // INPUT_H
