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

// EXTERNAL linkage (NOT in the anon namespace below): set true by Minecraft::displayGuiScreen only when a
// container GUI (GuiContainer subclass: inventory/chest/furnace/crafting) is open. Gates the LEFT-stick free
// cursor + the Pointer.png icon to those screens; every other screen uses the discrete button nav.
bool g_guiCursorMode = false;

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
	float g_dx = 0, g_dy = 0;          // accumulated right-stick look delta since last getDX/getDY (float:
	                                   // carries sub-pixel remainder so slow stick + high FPS still turns)
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

// ---- Controller MENU navigation core (see Input.reference.h) -----------------------------------------
// Edition-agnostic 2D-spatial nav over the current screen's button rectangles. The GuiScreen adapter that
// j2ctool injects (controller_nav.py) fills the geometry arrays from controlList and calls this, so the
// console menu-nav can never again be a per-project overlay someone forgets. Ported from the proven Alpha
// hand-port's GuiScreen::handleControllerMenuNav (geometry-only, no game types).
int menuNavSelect(const int* bx, const int* by, const int* bw, const int* bh,
                  const unsigned char* enabled, int n, int cur) {
	bool up = navUp(), down = navDown(), left = navLeft(), right = navRight();   // consume the latched edges
	if (n <= 0) return -1;
	// Ensure a valid selection (first enabled button) -- first entry, or a selection that became disabled.
	if (cur < 0 || cur >= n || !enabled[cur]) {
		cur = -1;
		for (int i = 0; i < n; ++i) if (enabled[i]) { cur = i; break; }
	}
	if (cur < 0) return -1;                          // no navigable buttons on this screen
	int dir = up ? 1 : (down ? 2 : (left ? 3 : (right ? 4 : 0)));   // 1=up 2=down 3=left 4=right
	if (dir == 0) return cur;                        // no edge this frame -> keep the current selection
	int ccx = bx[cur] + bw[cur] / 2, ccy = by[cur] + bh[cur] / 2;
	int best = -1, bestCost = 0;
	for (int i = 0; i < n; ++i) {                    // pass 1: nearest enabled button strictly in the dir
		if (i == cur || !enabled[i]) continue;
		int dx = (bx[i] + bw[i] / 2) - ccx, dy = (by[i] + bh[i] / 2) - ccy;
		int primary, perp; bool inDir;
		if (dir == 1)      { inDir = dy < 0; primary = -dy; perp = (dx < 0 ? -dx : dx); }   // up
		else if (dir == 2) { inDir = dy > 0; primary =  dy; perp = (dx < 0 ? -dx : dx); }   // down
		else if (dir == 3) { inDir = dx < 0; primary = -dx; perp = (dy < 0 ? -dy : dy); }   // left
		else               { inDir = dx > 0; primary =  dx; perp = (dy < 0 ? -dy : dy); }   // right
		if (!inDir) continue;
		int cost = primary + perp * 2;               // prefer well-aligned (small perp), then nearest
		if (best < 0 || cost < bestCost) { bestCost = cost; best = i; }
	}
	if (best < 0 && (dir == 1 || dir == 2)) {        // pass 2 (up/down): wrap to the column's far end
		for (int i = 0; i < n; ++i) {
			if (i == cur || !enabled[i]) continue;
			int bcx = bx[i] + bw[i] / 2, bcy = by[i] + bh[i] / 2;
			int perp = (bcx < ccx ? ccx - bcx : bcx - ccx);
			int cost = (dir == 2 ? bcy : -bcy) * 4 + perp;   // down->topmost, up->bottommost; column-aware
			if (best < 0 || cost < bestCost) { bestCost = cost; best = i; }
		}
	}
	return best >= 0 ? best : cur;
}

// Snap the virtual cursor onto a button center so the game's own hover-highlight + A-as-click land on the
// d-pad selection. centerX/centerY are GUI-scaled; g_cursor* is DISPLAY pixels, top-down (inverse of the
// game's mouseX/Y mapping: guiX = getX*guiW/dispW, guiY = guiH - getY*guiH/dispH - 1).
void menuSnapCursor(int centerX, int centerY, int guiW, int guiH, int dispW, int dispH) {
	if (guiW <= 0 || guiH <= 0) return;
	int cx = centerX * dispW / guiW, cy = centerY * dispH / guiH;
	if (cx < 0) cx = 0; else if (cx >= dispW) cx = dispW - 1;
	if (cy < 0) cy = 0; else if (cy >= dispH) cy = dispH - 1;
	g_cursorX = cx; g_cursorY = cy;
}

// [J2X-BUGFIX fps-look-scale] Real seconds elapsed since the previous poll(). poll() runs once per render
// frame, so this is the frame time -- used to make right-stick look frame-rate independent. Uses the 64-bit
// performance counter (smooth at hundreds of fps, where GetTickCount's ~1ms granularity would jitter).
// Clamps to a sane window: the first call and any long stall (load, pause) must not fling the camera.
float _lookDtSeconds() {
	static LARGE_INTEGER s_freq = { 0 };
	static LARGE_INTEGER s_prev = { 0 };
	if (s_freq.QuadPart == 0) QueryPerformanceFrequency(&s_freq);
	LARGE_INTEGER now; QueryPerformanceCounter(&now);
	if (s_prev.QuadPart == 0 || s_freq.QuadPart == 0) { s_prev = now; return 1.0f / 60.0f; }  // first frame: assume 60fps
	double dt = double(now.QuadPart - s_prev.QuadPart) / double(s_freq.QuadPart);
	s_prev = now;
	if (dt < 0.0)   dt = 0.0;
	if (dt > 0.10)  dt = 0.10;   // clamp a hitch to <=100ms so a stutter can't spike the turn
	return (float)dt;
}

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
		// Right-stick-up = look up, right = look right, with NO inversion (invert-look is not a Beta
		// feature yet; the base signs are already correct so nothing to negate).
		// [J2X-BUGFIX fps-look-scale] Scale by REAL elapsed seconds so turn speed is frame-rate independent
		// (Release_LTCG runs uncapped at hundreds of fps). Accumulate as float -> sub-pixel remainder is kept.
		float lookDt = _lookDtSeconds();
		g_dx += rx * InputConfig::LOOK_PIXELS_PER_SECOND * lookDt;
		g_dy += ry * InputConfig::LOOK_PIXELS_PER_SECOND * lookDt;

		// Triggers -> mouse buttons. STANDARD Minecraft click semantics (Beta and later): mouse button 0 =
		// attack/break, button 1 = place/use. RT -> button 0, LT -> button 1 gives the console feel LT=place,
		// RT=destroy. NOTE: a game with INVERTED clicks (e.g. RubyDung, where button 0 places) needs LT/RT
		// swapped here to reach the same feel -- that is a per-game input choice, not a converter default.
		g_btnDown[0] = rt; g_btnDown[1] = lt; g_btnDown[2] = false;
		if (rt && !g_prevBtnDown[0]) pushBtnEvent(0, true,  g_cursorX, g_cursorY, 0);
		if (!rt && g_prevBtnDown[0]) pushBtnEvent(0, false, g_cursorX, g_cursorY, 0);
		if (lt && !g_prevBtnDown[1]) pushBtnEvent(1, true,  g_cursorX, g_cursorY, 0);
		if (!lt && g_prevBtnDown[1]) pushBtnEvent(1, false, g_cursorX, g_cursorY, 0);

		// Face / stick-click binds (CONTROL MAP -- see docs/platform/03-input-and-audio.md).
		setKey(Keyboard::KEY_SPACE,  vA != 0);     // A     -> jump
		setKey(Keyboard::KEY_LSHIFT, vR3 != 0);    // R3    -> sneak (right stick click; L3 now toggles the camera)
		setKey(Keyboard::KEY_E,      vY != 0);     // Y -> inventory. FIXED BUG y-inventory-wrong-key: was KEY_I (Alpha's inventory=23=I); Beta's keyBindInventory is keyCode 18 = E, so Y must send E to open the inventory. NOTE: X is INTENTIONALLY UNMAPPED -- Beta has no separate crafting GUI, so the 360-Layout-1 "Crafting" button does nothing (do NOT mirror Y).
		setKey(Keyboard::KEY_Q,      vB != 0);     // B     -> drop held item (keyBindDrop = 16 = Q)
		// START -> open the in-game pause menu. FIXED BUG start-no-pause-menu: Minecraft::runTick opens the pause
		// menu from a keyboard EVENT (getEventKey()==KEY_ESCAPE at Minecraft.cpp), NOT the key STATE -- so the old
		// level `setKey(KEY_ESCAPE, vStart)` flipped isKeyDown but queued no event, and the menu never came up. Push a
		// one-shot ESCAPE down+up on the RISING edge (edge-triggered so one press = open once), exactly like the L3->F5
		// toggle below and the GUI-path B/START->ESCAPE close above. (The GUI path handles START while a menu is open.)
		bool prevStartIG = (g_prevButtons & XINPUT_GAMEPAD_START) != 0;
		if (vStart && !prevStartIG) {
			pushKeyEvent(Keyboard::KEY_ESCAPE, true,  0);
			pushKeyEvent(Keyboard::KEY_ESCAPE, false, 0);
		}
		// BACK -> open TEXT CHAT (multiplayer). FEATURE console-chat: no PC keyboard, so map Back to the chat
		// keybind's key EVENT (same edge pattern as START->ESCAPE). Minecraft::runTick opens GuiChat when
		// getEventKey()==keyBindChat->keyCode (default KEY_T) AND the world is multiplayer; GuiChat then pops the
		// system OSK to type the message. Edge-triggered so one press = one open.
		bool prevBackIG = (g_prevButtons & XINPUT_GAMEPAD_BACK) != 0;
		if (vBack && !prevBackIG) {
			pushKeyEvent(Keyboard::KEY_T, true,  0);
			pushKeyEvent(Keyboard::KEY_T, false, 0);
		}
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
		// ===================== IN A GUI =====================
		// CONTAINER GUIs ONLY (inventory/chest/furnace/crafting -> g_guiCursorMode, set by
		// Minecraft::displayGuiScreen) use a FREE analog cursor on the LEFT stick + D-pad nudge, and
		// EntityRenderer draws the Pointer.png icon. EVERY OTHER screen (main menu / options / pause /
		// server list / ...) ALSO get the free cursor now (else branch, console-menu-nav) -- the highlight is the cue
		// the cursor ONLY in inventory/chest/furnace.
		if (g_guiCursorMode) {
			g_navUpFired = g_navDownFired = g_navLeftFired = g_navRightFired = false;
			static unsigned long s_lastMs = 0;
			static float s_fx = 0.0F, s_fy = 0.0F;
			unsigned long nowMs = GetTickCount();
			float dt = s_lastMs ? (float)(nowMs - s_lastMs) / 1000.0F : 0.0F;
			s_lastMs = nowMs;
			if (dt > 0.1F) dt = 0.1F;                                   // clamp first frame / hitches
			if ((int)s_fx != g_cursorX || (int)s_fy != g_cursorY) { s_fx = (float)g_cursorX; s_fy = (float)g_cursorY; }
			float spd = InputConfig::CURSOR_PIXELS_PER_SECOND * dt;
			s_fx += lx * spd;  s_fy += -ly * spd;                       // LEFT stick (analog), stick-up -> cursor up
			float step = (float)g_dispH / 14.0F;                        // D-pad nudge ~one inventory slot
			bool nU=(vUp!=0), nD=(vDn!=0), nL=(vLf!=0), nR=(vRt!=0);
			bool freshDpad = (nU&&!g_prevNavUp)||(nD&&!g_prevNavDown)||(nL&&!g_prevNavLeft)||(nR&&!g_prevNavRight);
			static int s_dpadRepeat = 0;
			bool dpadFire = false;
			if (nU||nD||nL||nR) { if (freshDpad) { dpadFire=true; s_dpadRepeat=8; } else if (--s_dpadRepeat<=0) { dpadFire=true; s_dpadRepeat=4; } }
			else s_dpadRepeat = 0;
			if (dpadFire) { if(nU) s_fy-=step; if(nD) s_fy+=step; if(nL) s_fx-=step; if(nR) s_fx+=step; }
			g_prevNavUp=nU; g_prevNavDown=nD; g_prevNavLeft=nL; g_prevNavRight=nR;
			if (s_fx < 0) s_fx = 0;  if (s_fx > (float)g_dispW) s_fx = (float)g_dispW;
			if (s_fy < 0) s_fy = 0;  if (s_fy > (float)g_dispH) s_fy = (float)g_dispH;
			g_cursorX = (int)s_fx;  g_cursorY = (int)s_fy;
		} else {
			// ===================== NON-CONTAINER GUI (menus) =====================
			// J2X-BUGFIX console-menu-nav: the console has NO mouse or keyboard, so the controller must drive
			// menus BY CONSTRUCTION. The LEFT STICK moves a free cursor here (exactly like the container path):
			// the game's drawScreen passes Mouse::getX/getY to each GuiButton, so the hovered button HIGHLIGHTS,
			// and A -> a click at the cursor -> GuiButton::mousePressed -> actionPerformed. That is the STANDARD
			// Mouse path EVERY converted GuiScreen already handles, so any menu (main menu / options / server
			// list / ...) is navigable with NO per-game wiring -- previously this branch only latched nav edges
			// that a bare converted GuiScreen never consumed, so a controller could not move the selection at
			// all (frozen cursor -> apparent lock-up). The D-PAD still fires the discrete nav edges for the rare
			// screen that DOES consume Input::navUp (e.g. the sign-line editor); harmless if unconsumed.
			// (g_guiCursorMode / the Pointer.png sprite stays container-only; in menus the highlight is the cue.)
			static unsigned long s_mLastMs = 0; static float s_mfx = 0.0F, s_mfy = 0.0F;
			unsigned long nowMs = GetTickCount();
			float dt = s_mLastMs ? (float)(nowMs - s_mLastMs) / 1000.0F : 0.0F;
			s_mLastMs = nowMs;
			if (dt > 0.1F) dt = 0.1F;                                    // clamp first frame / hitches
			if ((int)s_mfx != g_cursorX || (int)s_mfy != g_cursorY) { s_mfx = (float)g_cursorX; s_mfy = (float)g_cursorY; }
			float spd = InputConfig::CURSOR_PIXELS_PER_SECOND * dt;
			s_mfx += lx * spd;  s_mfy += -ly * spd;                      // left stick -> cursor (stick-up = cursor up)
			if (s_mfx < 0) s_mfx = 0;  if (s_mfx > (float)g_dispW) s_mfx = (float)g_dispW;
			if (s_mfy < 0) s_mfy = 0;  if (s_mfy > (float)g_dispH) s_mfy = (float)g_dispH;
			g_cursorX = (int)s_mfx;  g_cursorY = (int)s_mfy;
			bool navU = (vUp != 0), navD = (vDn != 0), navL = (vLf != 0), navR = (vRt != 0);   // D-PAD -> nav edges
			if (navU && !g_prevNavUp)    g_navUpFired    = true;
			if (navD && !g_prevNavDown)  g_navDownFired  = true;
			if (navL && !g_prevNavLeft)  g_navLeftFired  = true;
			if (navR && !g_prevNavRight) g_navRightFired = true;
			g_prevNavUp = navU; g_prevNavDown = navD; g_prevNavLeft = navL; g_prevNavRight = navR;
		}

		g_btnDown[0] = g_btnDown[1] = g_btnDown[2] = false;
		bool prevA     = (g_prevButtons & XINPUT_GAMEPAD_A)     != 0;
		bool prevX     = (g_prevButtons & XINPUT_GAMEPAD_X)     != 0;
		bool prevB     = (g_prevButtons & XINPUT_GAMEPAD_B)     != 0;
		bool prevStart = (g_prevButtons & XINPUT_GAMEPAD_START) != 0;
		if (vA && !prevA) { g_btnDown[0] = true; pushBtnEvent(0, true,  g_cursorX, g_cursorY, 0); }  // A press -> left click
		if (!vA && prevA) {                      pushBtnEvent(0, false, g_cursorX, g_cursorY, 0); }  // A release
		// J2X-BUGFIX gui-right-click: X -> RIGHT click (mouse button 1) INSIDE a container GUI, so the player
		// can grab HALF a stack / place ONE item at a time, matching PC right-click. This is distinct from the
		// IN-GAME map where X is intentionally unmapped -- here it only fires on container screens (this GUI
		// path). Without it a controller can only left-click whole stacks in inventory/chest/furnace/crafting.
		if (vX && !prevX) { g_btnDown[1] = true; pushBtnEvent(1, true,  g_cursorX, g_cursorY, 0); }  // X press -> right click
		if (!vX && prevX) {                      pushBtnEvent(1, false, g_cursorX, g_cursorY, 0); }  // X release
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
	if (grabbed && !g_grabbed) {
		// J2X-BUGFIX cross-screen-resync: transitioning from a GUI to in-game. A face button physically HELD
		// while the menu closes has a DIFFERENT in-game meaning than in the GUI (B = back/ESCAPE in a menu,
		// but B = Q/drop in-game). The in-game keys go through setKey(), which edge-detects against
		// g_prevKeyDown -- and those keys are never driven during the GUI period, so g_prevKeyDown is false for
		// them. Without a resync the first in-game frame sees a false RISING edge and fires the action: closing
		// a container with B held DROPS the held item; A held -> phantom jump; etc. Pre-seed g_prevKeyDown with
		// the IN-GAME mapping of any currently-held face button so setKey() sees no edge. Uses the SAME bindings
		// the in-game poll() path uses -- A->SPACE, B->Q, Y->E (Beta's keyBindInventory=18=E, NOT Alpha's I),
		// R3->LSHIFT. START/BACK/L3/LB/RB edge-detect against g_prevButtons (committed every frame), so they
		// already carry the held state across the grab and need no seeding. Matches the hand-port's setGrabbed.
		memset(g_prevKeyDown, 0, sizeof(g_prevKeyDown));
		BOOL curA = 0, curB = 0, curY = 0, curR3 = 0;
		XINPUT2CONTEXT ctx2 = 0; BOOL ch2 = FALSE;
		if (XInput2Sample(XInput2InputDevice(0), &ctx2, &ch2)) {
			XInput2GetBool(ctx2, XINPUTID_A, &curA);
			XInput2GetBool(ctx2, XINPUTID_B, &curB);
			XInput2GetBool(ctx2, XINPUTID_Y, &curY);
			XInput2GetBool(ctx2, XINPUTID_RIGHT_THUMB, &curR3);
		} else {
			XINPUT_STATE st2; memset(&st2, 0, sizeof(st2));
			if (XInputGetState(0, &st2) == ERROR_SUCCESS) {
				WORD gb2 = st2.Gamepad.wButtons;
				curA  = (gb2 & XINPUT_GAMEPAD_A) != 0;           curB  = (gb2 & XINPUT_GAMEPAD_B) != 0;
				curY  = (gb2 & XINPUT_GAMEPAD_Y) != 0;           curR3 = (gb2 & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
			}
		}
		if (curA)  g_prevKeyDown[Keyboard::KEY_SPACE]  = true;
		if (curB)  g_prevKeyDown[Keyboard::KEY_Q]      = true;
		if (curY)  g_prevKeyDown[Keyboard::KEY_E]      = true;
		if (curR3) g_prevKeyDown[Keyboard::KEY_LSHIFT] = true;
	}
	g_grabbed = grabbed;
	g_dx = g_dy = 0;                 // grab/ungrab resets pending look delta (MouseHelper.grabMouseCursor)
}
void setCursorPosition(int x, int y) { g_cursorX = x; g_cursorY = y; }

// read-once, like LWJGL. Return the whole-pixel part and CARRY the sub-pixel remainder so a slow stick at
// high FPS (small per-frame delta) still accumulates into turns instead of truncating to zero every frame.
int getDX() { int d = (int)g_dx; g_dx -= (float)d; return d; }
int getDY() { int d = (int)g_dy; g_dy -= (float)d; return d; }

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
