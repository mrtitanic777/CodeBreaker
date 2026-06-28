#include "stdafx.h"
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
#include "Common.h"  // DbgPrintF (one-time controller-source diagnostic)
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
	// Menu-nav edge state (d-pad / left-stick notch). g_prevNav* = last frame's active state (for edge
	// detection); g_nav*Fired = LATCHED edge waiting to be consumed by Input::navUp()/... (latched so an
	// edge isn't dropped on a frame that ran 0 ticks, i.e. didn't call GuiScreen::handleInput).
	bool  g_prevNavUp = false, g_prevNavDown = false, g_prevNavLeft = false, g_prevNavRight = false;
	bool  g_navUpFired = false, g_navDownFired = false, g_navLeftFired = false, g_navRightFired = false;

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

	// Deadzoned, normalized stick axis in [-1, 1]. XInput2 thumbsticks come back as normalized floats
	// (XInput2GetPoint), so the deadzone is applied in float space. Flat per-axis.
	float axisf(float v) {
		if (v > -InputConfig::STICK_DEADZONE_F && v < InputConfig::STICK_DEADZONE_F) return 0.0F;
		if (v >  1.0F) return  1.0F;
		if (v < -1.0F) return -1.0F;
		return v;
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

void setDisplaySize(int w, int h) {
	if (w > 0) g_dispW = w;
	if (h > 0) g_dispH = h;
	if (g_cursorX == 0 && g_cursorY == 0) { g_cursorX = g_dispW / 2; g_cursorY = g_dispH / 2; }  // start centered
}
bool connected() { return g_connected; }
float moveStrafe()  { return g_moveStrafe; }
float moveForward() { return g_moveForward; }

// Discrete menu-nav edges: consume the latched edge (read-once, so exactly one handleInput() acts on it).
bool navUp()    { bool f = g_navUpFired;    g_navUpFired    = false; return f; }
bool navDown()  { bool f = g_navDownFired;  g_navDownFired  = false; return f; }
bool navLeft()  { bool f = g_navLeftFired;  g_navLeftFired  = false; return f; }
bool navRight() { bool f = g_navRightFired; g_navRightFired = false; return f; }

void poll() {
	ensureInit();

	memset(g_keyDown, 0, sizeof(g_keyDown));   // rebuilt from the pad each frame
	g_moveStrafe = g_moveForward = 0.0F;

	// ---- Read the pad. PRIMARY = XInput2 (native 360). FALLBACK = classic XInputGetState (what some
	//      emulators expose). Both fill the same raw locals; the rest of poll() is source-agnostic. ----
	BOOL vA=0,vB=0,vX=0,vY=0, vUp=0,vDn=0,vLf=0,vRt=0, vStart=0,vBack=0, vL3=0,vR3=0, vLB=0,vRB=0;
	float fLT=0.0F, fRT=0.0F, sxl=0.0F, syl=0.0F, sxr=0.0F, syr=0.0F;   // triggers 0..1, sticks -1..1
	int src = 0;   // 0=none, 1=XInput2, 2=classic XInput

	XINPUT2CONTEXT ctx = 0; BOOL changed = FALSE;
	if (XInput2Sample(XInput2InputDevice(0), &ctx, &changed)) {
		src = 1;
		XInput2GetBool(ctx, XINPUTID_A, &vA);              XInput2GetBool(ctx, XINPUTID_B, &vB);
		XInput2GetBool(ctx, XINPUTID_X, &vX);              XInput2GetBool(ctx, XINPUTID_Y, &vY);
		XInput2GetBool(ctx, XINPUTID_DPAD_UP, &vUp);       XInput2GetBool(ctx, XINPUTID_DPAD_DOWN, &vDn);
		XInput2GetBool(ctx, XINPUTID_DPAD_LEFT, &vLf);     XInput2GetBool(ctx, XINPUTID_DPAD_RIGHT, &vRt);
		XInput2GetBool(ctx, XINPUTID_START, &vStart);      XInput2GetBool(ctx, XINPUTID_BACK, &vBack);
		XInput2GetBool(ctx, XINPUTID_LEFT_THUMB, &vL3);    XInput2GetBool(ctx, XINPUTID_RIGHT_THUMB, &vR3);
		XInput2GetBool(ctx, XINPUTID_LEFT_SHOULDER, &vLB); XInput2GetBool(ctx, XINPUTID_RIGHT_SHOULDER, &vRB);
		XInput2GetFloat(ctx, XINPUTID_LEFT_TRIGGER,  &fLT);
		XInput2GetFloat(ctx, XINPUTID_RIGHT_TRIGGER, &fRT);
		XINPUT2_POINT lp; lp.x=lp.y=0.0F;  XINPUT2_POINT rp; rp.x=rp.y=0.0F;
		XInput2GetPoint(ctx, XINPUTID_LEFT_THUMBSTICK,  &lp);
		XInput2GetPoint(ctx, XINPUTID_RIGHT_THUMBSTICK, &rp);
		sxl=lp.x; syl=lp.y; sxr=rp.x; syr=rp.y;
	} else {
		XINPUT_STATE st; memset(&st, 0, sizeof(st));
		if (XInputGetState(0, &st) == ERROR_SUCCESS) {
			src = 2;
			const XINPUT_GAMEPAD& g = st.Gamepad; WORD gb = g.wButtons;
			vA=(gb&XINPUT_GAMEPAD_A)!=0;            vB=(gb&XINPUT_GAMEPAD_B)!=0;
			vX=(gb&XINPUT_GAMEPAD_X)!=0;            vY=(gb&XINPUT_GAMEPAD_Y)!=0;
			vUp=(gb&XINPUT_GAMEPAD_DPAD_UP)!=0;     vDn=(gb&XINPUT_GAMEPAD_DPAD_DOWN)!=0;
			vLf=(gb&XINPUT_GAMEPAD_DPAD_LEFT)!=0;   vRt=(gb&XINPUT_GAMEPAD_DPAD_RIGHT)!=0;
			vStart=(gb&XINPUT_GAMEPAD_START)!=0;    vBack=(gb&XINPUT_GAMEPAD_BACK)!=0;
			vL3=(gb&XINPUT_GAMEPAD_LEFT_THUMB)!=0;  vR3=(gb&XINPUT_GAMEPAD_RIGHT_THUMB)!=0;
			vLB=(gb&XINPUT_GAMEPAD_LEFT_SHOULDER)!=0; vRB=(gb&XINPUT_GAMEPAD_RIGHT_SHOULDER)!=0;
			fLT=g.bLeftTrigger/255.0F;  fRT=g.bRightTrigger/255.0F;
			sxl=g.sThumbLX/32767.0F; syl=g.sThumbLY/32767.0F; sxr=g.sThumbRX/32767.0F; syr=g.sThumbRY/32767.0F;
		}
	}
	g_connected = (src != 0);

	// One-time diagnostic: which API answered + display/cursor mapping (paste from nexia.log if input dead).
	static int s_diag = 0;
	if (!s_diag) {
		s_diag = 1;
		DbgPrintF("[input] source=%d (1=XInput2 2=classic 0=none) disp=%dx%d cursor=%d,%d\n",
		          src, g_dispW, g_dispH, g_cursorX, g_cursorY);
	}

	if (!g_connected) {
		// No controller from either API: release everything (emit release edges so nothing sticks).
		for (int k = 0; k < 256; ++k) if (g_prevKeyDown[k]) pushKeyEvent(k, false, 0);
		for (int bb = 0; bb < 3; ++bb) {
			if (g_prevBtnDown[bb]) pushBtnEvent(bb, false, g_cursorX, g_cursorY, 0);
			g_btnDown[bb] = false;
		}
		memcpy(g_prevKeyDown, g_keyDown, sizeof(g_keyDown));
		memcpy(g_prevBtnDown, g_btnDown, sizeof(g_btnDown));
		g_prevButtons = 0;
		g_navUpFired = g_navDownFired = g_navLeftFired = g_navRightFired = false;
		g_prevNavUp = g_prevNavDown = g_prevNavLeft = g_prevNavRight = false;
		return;
	}

	float lx = axisf(sxl), ly = axisf(syl);   // [-1,1], y up-positive
	float rx = axisf(sxr), ry = axisf(syr);
	bool lt = fLT > InputConfig::TRIGGER_F;
	bool rt = fRT > InputConfig::TRIGGER_F;

	// Build a button-bit mask (reuse the XINPUT_GAMEPAD_* bits from xinputdefs.h) so edge detection
	// against g_prevButtons stays simple.
	WORD b = 0;
	if (vA) b |= XINPUT_GAMEPAD_A;            if (vB) b |= XINPUT_GAMEPAD_B;
	if (vX) b |= XINPUT_GAMEPAD_X;            if (vY) b |= XINPUT_GAMEPAD_Y;
	if (vUp) b |= XINPUT_GAMEPAD_DPAD_UP;     if (vDn) b |= XINPUT_GAMEPAD_DPAD_DOWN;
	if (vLf) b |= XINPUT_GAMEPAD_DPAD_LEFT;   if (vRt) b |= XINPUT_GAMEPAD_DPAD_RIGHT;
	if (vStart) b |= XINPUT_GAMEPAD_START;    if (vBack) b |= XINPUT_GAMEPAD_BACK;
	if (vL3) b |= XINPUT_GAMEPAD_LEFT_THUMB;  if (vR3) b |= XINPUT_GAMEPAD_RIGHT_THUMB;
	if (vLB) b |= XINPUT_GAMEPAD_LEFT_SHOULDER; if (vRB) b |= XINPUT_GAMEPAD_RIGHT_SHOULDER;

	// One unified controller, two modes -- decided by g_grabbed (set by MouseHelper grab/ungrab):
	//   grabbed   == IN-GAME  -> sticks = move + look, triggers = attack/use, face = jump/inventory/...
	//   !grabbed  == IN A GUI -> left stick + d-pad move a virtual cursor; A = click; B/START = back.
	if (g_grabbed) {
		// ===================== IN-GAME =====================
		// No menu open -> drop any latched menu-nav edge and keep its prev-state cleared so re-opening a
		// GUI starts clean (a held stick won't fire a nav the instant a menu appears).
		g_navUpFired = g_navDownFired = g_navLeftFired = g_navRightFired = false;
		g_prevNavUp = g_prevNavDown = g_prevNavLeft = g_prevNavRight = false;
		g_moveStrafe  = lx;   // stick-right -> strafe right
		g_moveForward = ly;   // stick-up    -> forward+ (consumer negates if needed)
		setKey(Keyboard::KEY_W, ly >  0.5F);   // digital fallback if the analog override isn't used
		setKey(Keyboard::KEY_S, ly < -0.5F);
		setKey(Keyboard::KEY_A, lx < -0.5F);
		setKey(Keyboard::KEY_D, lx >  0.5F);

		// Right stick -> look delta (EntityRenderer applies the Java sensitivity curve; don't bake it in).
		float ddx = rx * InputConfig::LOOK_PIXELS_PER_FRAME;
		float ddy = ry * InputConfig::LOOK_PIXELS_PER_FRAME;
		if (InputConfig::LOOK_INVERT_X) ddx = -ddx;
		if (InputConfig::LOOK_INVERT_Y) ddy = -ddy;
		g_dx += (int)ddx;
		g_dy += (int)ddy;

		// Triggers -> mouse buttons (RT = attack/break = 0, LT = place/use = 1).
		g_btnDown[0] = rt; g_btnDown[1] = lt; g_btnDown[2] = false;
		if (rt && !g_prevBtnDown[0]) pushBtnEvent(0, true,  g_cursorX, g_cursorY, 0);
		if (!rt && g_prevBtnDown[0]) pushBtnEvent(0, false, g_cursorX, g_cursorY, 0);
		if (lt && !g_prevBtnDown[1]) pushBtnEvent(1, true,  g_cursorX, g_cursorY, 0);
		if (!lt && g_prevBtnDown[1]) pushBtnEvent(1, false, g_cursorX, g_cursorY, 0);

		// Face / stick-click binds (CONTROL MAP -- see docs/platform/03-input-and-audio.md).
		setKey(Keyboard::KEY_SPACE,  vA != 0);     // A     -> jump
		setKey(Keyboard::KEY_LSHIFT, vR3 != 0);    // R3    -> sneak (right stick click; L3 now toggles the camera)
		setKey(Keyboard::KEY_I,      vY != 0);     // Y     -> inventory (Alpha keyBindInventory = 23 = I, NOT E)
		setKey(Keyboard::KEY_Q,      vB != 0);     // B     -> drop held item (keyBindDrop = 16 = Q)
		setKey(Keyboard::KEY_ESCAPE, vStart != 0); // START -> pause / open menu
		// L3 (click left stick) -> toggle 3rd-person camera, matching the 360 control. Synthesize an F5
		// key event so Minecraft's existing handler flips gameSettings->thirdPersonView; edge-triggered
		// (rising edge) so one click = one toggle.
		bool prevL3 = (g_prevButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
		if (vL3 && !prevL3) {
			pushKeyEvent(Keyboard::KEY_F5, true,  0);
			pushKeyEvent(Keyboard::KEY_F5, false, 0);
		}

		// LB/RB -> hotbar scroll (Minecraft::runTick reads getEventDWheel -> changeCurrentItem, where +1
		// moves the slot LEFT). RB must select the NEXT (right) slot, so RB -> -120, LB -> +120. (Swapped
		// from the inverted original where RB went left.)
		bool prevLB = (g_prevButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)  != 0;
		bool prevRB = (g_prevButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
		if (vRB && !prevRB) pushBtnEvent(-1, false, g_cursorX, g_cursorY, -120);   // RB -> next slot (right)
		if (vLB && !prevLB) pushBtnEvent(-1, false, g_cursorX, g_cursorY, +120);   // LB -> prev slot (left)
	} else {
		// ===================== IN A GUI (Indev-style DISCRETE selection) =====================
		// The d-pad / LEFT stick select buttons ONE step per press (edge-triggered + latched), instead of
		// drifting an analog cursor (which flew across the screen at the uncapped menu FPS). GuiScreen
		// consumes navUp()/navDown()/... to move the selected button and SNAPS this cursor onto it via
		// Mouse::setCursorPosition -- so the existing hover highlight lights up and A clicks the selection.
		bool navU = (vUp != 0) || ly >  0.5F;     // ly is up-positive
		bool navD = (vDn != 0) || ly < -0.5F;
		bool navL = (vLf != 0) || lx < -0.5F;
		bool navR = (vRt != 0) || lx >  0.5F;
		if (navU && !g_prevNavUp)    g_navUpFired    = true;   // latch the rising edge (consumed by navUp())
		if (navD && !g_prevNavDown)  g_navDownFired  = true;
		if (navL && !g_prevNavLeft)  g_navLeftFired  = true;
		if (navR && !g_prevNavRight) g_navRightFired = true;
		g_prevNavUp = navU; g_prevNavDown = navD; g_prevNavLeft = navL; g_prevNavRight = navR;

		// RIGHT stick = optional FREE analog pointer (frame-rate-independent) for non-button GUIs (slots).
		// Button menus snap the cursor onto the selected button, so this just lets a slot screen point.
		static unsigned long s_lastMs = 0;
		static float s_fx = 0.0F, s_fy = 0.0F;
		unsigned long nowMs = GetTickCount();
		float dt = s_lastMs ? (float)(nowMs - s_lastMs) / 1000.0F : 0.0F;
		s_lastMs = nowMs;
		if (dt > 0.1F) dt = 0.1F;                                   // clamp first frame / hitches
		if ((int)s_fx != g_cursorX || (int)s_fy != g_cursorY) { s_fx = (float)g_cursorX; s_fy = (float)g_cursorY; }
		float spd = InputConfig::CURSOR_PIXELS_PER_SECOND * dt;
		s_fx += rx * spd;  s_fy += -ry * spd;                       // RIGHT stick (analog), stick-up -> cursor up
		if (s_fx < 0) s_fx = 0;  if (s_fx > (float)g_dispW) s_fx = (float)g_dispW;
		if (s_fy < 0) s_fy = 0;  if (s_fy > (float)g_dispH) s_fy = (float)g_dispH;
		g_cursorX = (int)s_fx;  g_cursorY = (int)s_fy;

		g_btnDown[0] = g_btnDown[1] = g_btnDown[2] = false;
		bool prevA     = (g_prevButtons & XINPUT_GAMEPAD_A)     != 0;
		bool prevB     = (g_prevButtons & XINPUT_GAMEPAD_B)     != 0;
		bool prevStart = (g_prevButtons & XINPUT_GAMEPAD_START) != 0;
		if (vA && !prevA) { g_btnDown[0] = true; pushBtnEvent(0, true,  g_cursorX, g_cursorY, 0); }  // A press -> click
		if (!vA && prevA) {                      pushBtnEvent(0, false, g_cursorX, g_cursorY, 0); }  // A release
		if ((vB && !prevB) || (vStart && !prevStart)) {   // B / START -> ESCAPE (back/close screen)
			pushKeyEvent(Keyboard::KEY_ESCAPE, true, 0);
			pushKeyEvent(Keyboard::KEY_ESCAPE, false, 0);
		}
	}

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
int getY() { return g_dispH - 1 - g_cursorY; }   // cursor stored top-down; LWJGL Mouse Y is bottom-up

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
int  getEventY()           { return g_dispH - 1 - g_btnCur.y; }   // top-down store -> LWJGL bottom-up
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
