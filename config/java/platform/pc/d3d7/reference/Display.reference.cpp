// Display on SDL2 + Direct3D 7 (see Display.reference.h). The window is SDL's (input, focus, the AWT mouse are the same code as
// the OpenGL and other Direct3D Displays); the drawing surface is DirectDraw 7's: an offscreen render target with a depth buffer
// attached, a Direct3D 7 device on it handed to the GL layer's Direct3D 7 backend (GpuD3D7.cpp), and the frame blitted to the
// window's part of the screen (the primary surface, clipped to the window). Windows has Direct3D 7 for 32-bit programs only
// (d3dim700.dll in SysWOW64), so this build is a 32-bit program.
//
// The device is the card's with hardware transform and lighting, else the card's without, else Direct3D's own software
// rasterizer (the surfaces in system memory then). A resized window gets new targets of its size under the same device; lost
// surfaces (the lock screen, a display-mode change) are restored when the blit reports them, and managed textures with them.
#define J2X_WITH_GDI   // (ddraw.h needs the GDI types stdafx.h keeps out of the game's files)
#include "stdafx.h"
#include "Display.h"
#include "Input.h"
#include "GL11.h"
#include "GpuD3D7.h"
#include "Common.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <ddraw.h>
#include <d3d.h>
#include <vector>
#include <cstring>
#include <cstdlib>

namespace {
	SDL_Window* g_win = 0;
	HWND g_hwnd = 0;
	IDirectDraw7* g_dd = 0;
	IDirectDrawSurface7* g_primary = 0;
	IDirectDrawClipper* g_clipper = 0;
	IDirect3D7* g_d3d = 0;
	IDirect3DDevice7* g_dev = 0;
	IDirectDrawSurface7* g_back = 0;   // the render target the device draws on
	IDirectDrawSurface7* g_depth = 0;  // attached to it
	DDPIXELFORMAT g_depthFormat;
	bool g_systemMemory = false;       // the software device: its surfaces in system memory
	int g_bw = 0, g_bh = 0;
	bool g_needResize = false;
	DisplayMode g_mode(854, 480);
	std::string g_title = "Minecraft";
	bool g_close = false;
	bool g_active = true;
	bool g_full = false;
	bool g_parented = false;
	bool g_vsync = true;

	template<class T> void release(T*& p) { if (p) { p->Release(); p = 0; } }

	// The device is released before the process ends, however it ends (the game's System.exit, the script's quit, the last thread
	// returning): every object let go while the process can still run it, not left to Windows as it tears the process down.
	void release_at_exit() { Display::destroy(); }

	// The device's depth formats: the deepest of 24, 32 and 16 bits.
	HRESULT CALLBACK pick_depth(DDPIXELFORMAT* pf, void* best) {
		DDPIXELFORMAT* b = (DDPIXELFORMAT*)best;
		const int rank[3] = { pf->dwZBufferBitDepth == 24 ? 3 : pf->dwZBufferBitDepth == 32 ? 2 : pf->dwZBufferBitDepth == 16 ? 1 : 0,
		                      b->dwZBufferBitDepth == 24 ? 3 : b->dwZBufferBitDepth == 32 ? 2 : b->dwZBufferBitDepth == 16 ? 1 : 0, 0 };
		if ((pf->dwFlags & DDPF_ZBUFFER) && rank[0] > rank[1]) *b = *pf;
		return D3DENUMRET_OK;
	}

	// A render target of w x h (32-bit, the frame's format) with a depth buffer attached.
	bool make_targets(int w, int h, IDirectDrawSurface7** back, IDirectDrawSurface7** depth) {
		DDSURFACEDESC2 sd;
		std::memset(&sd, 0, sizeof(sd));
		sd.dwSize = sizeof(sd);
		sd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
		sd.dwWidth = (DWORD)w; sd.dwHeight = (DWORD)h;
		sd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE | (g_systemMemory ? DDSCAPS_SYSTEMMEMORY : DDSCAPS_VIDEOMEMORY);
		sd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
		sd.ddpfPixelFormat.dwFlags = DDPF_RGB;
		sd.ddpfPixelFormat.dwRGBBitCount = 32;
		sd.ddpfPixelFormat.dwRBitMask = 0x00FF0000; sd.ddpfPixelFormat.dwGBitMask = 0x0000FF00; sd.ddpfPixelFormat.dwBBitMask = 0x000000FF;
		if (FAILED(g_dd->CreateSurface(&sd, back, 0)) || !*back) return false;
		std::memset(&sd, 0, sizeof(sd));
		sd.dwSize = sizeof(sd);
		sd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
		sd.dwWidth = (DWORD)w; sd.dwHeight = (DWORD)h;
		sd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | (g_systemMemory ? DDSCAPS_SYSTEMMEMORY : DDSCAPS_VIDEOMEMORY);
		sd.ddpfPixelFormat = g_depthFormat;
		if (FAILED(g_dd->CreateSurface(&sd, depth, 0)) || !*depth || FAILED((*back)->AddAttachedSurface(*depth))) {
			release(*depth);
			release(*back);
			return false;
		}
		return true;
	}

	// New targets the window's size, under the same device.
	void resize_targets() {
		int w = 0, h = 0;
		SDL_GetWindowSize(g_win, &w, &h);
		if (w < 1) w = 1;
		if (h < 1) h = 1;
		g_needResize = false;
		IDirectDrawSurface7* back = 0;
		IDirectDrawSurface7* depth = 0;
		j2xgl::gpu::endSceneD3D7();
		if (!make_targets(w, h, &back, &depth) || FAILED(g_dev->SetRenderTarget(back, 0))) {
			DbgPrintF("[display] the resized targets could not be made\n");
			release(depth);
			release(back);
			return;
		}
		release(g_depth);
		release(g_back);
		g_back = back; g_depth = depth; g_bw = w; g_bh = h;
		j2xgl::gpu::setTargetD3D7(g_back, w, h);
		Input::setDisplaySize(w, h);
	}
}

int j2xDisplayWidth() { return Display::getWidth(); }    // AWT components report the window (stdafx.h)
int j2xDisplayHeight() { return Display::getHeight(); }

// The AWT mouse (stdafx.h): the same as the OpenGL Display's -- the window's place on the screen, the pointer, Robot.mouseMove. A
// J2X_HIDDEN test run never reads or moves the person's mouse.
int j2xWindowScreenX() { int x = 0, y = 0; if (g_win) SDL_GetWindowPosition(g_win, &x, &y); return x; }
int j2xWindowScreenY() { int x = 0, y = 0; if (g_win) SDL_GetWindowPosition(g_win, &x, &y); return y; }
static int g_robot_x = 0, g_robot_y = 0;
static bool g_robot_set = false;
static bool j2xPointerVirtual() { return Input::awtLooking() || std::getenv("J2X_HIDDEN") != 0; }
static void j2xPointerAt(int* x, int* y) {
	if (j2xPointerVirtual()) {
		if (!g_robot_set) {
			g_robot_x = j2xWindowScreenX() + Display::getWidth() / 2;
			g_robot_y = j2xWindowScreenY() + Display::getHeight() / 2;
		}
		int dx = 0, dy = 0;
		Input::awtMotion(&dx, &dy);
		*x = g_robot_x + dx;
		*y = g_robot_y + dy;
	} else {
		SDL_GetGlobalMouseState(x, y);
	}
}
int j2xPointerX() { int x = 0, y = 0; j2xPointerAt(&x, &y); return x; }
int j2xPointerY() { int x = 0, y = 0; j2xPointerAt(&x, &y); return y; }
void j2xRobotMouseMove(int x, int y) {
	g_robot_x = x;
	g_robot_y = y;
	g_robot_set = true;
	Input::awtRecentred();
	if (!j2xPointerVirtual() && g_win && (SDL_GetWindowFlags(g_win) & SDL_WINDOW_INPUT_FOCUS)) SDL_WarpMouseGlobal(x, y);
}

namespace Display {

void setDisplayMode(DisplayMode* mode) {
	if (mode) setDisplayMode(*mode);
}

void setDisplayMode(const DisplayMode& mode) {
	g_mode = mode;
	if (g_win && !g_full && mode.getWidth() > 0 && mode.getHeight() > 0) {
		SDL_SetWindowSize(g_win, mode.getWidth(), mode.getHeight());
		g_needResize = true;
	}
}

// A window or device that cannot be made ends the program (exit code 3), after saying why: LWJGL's Display.create throws there,
// and the game has nothing to draw with.
void create() {
	if (g_win) return;
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) != 0) {
		std::fprintf(stderr, "[display] SDL_Init failed: %s\n", SDL_GetError());
		std::exit(3);
	}
	const int w = g_mode.getWidth() > 0 ? g_mode.getWidth() : 854;
	const int h = g_mode.getHeight() > 0 ? g_mode.getHeight() : 480;
	g_win = SDL_CreateWindow(g_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
	                         (std::getenv("J2X_HIDDEN") ? SDL_WINDOW_HIDDEN : SDL_WINDOW_SHOWN) | SDL_WINDOW_RESIZABLE);   // (J2X_HIDDEN: a test run off-screen)
	if (!g_win) {
		std::fprintf(stderr, "[display] SDL_CreateWindow failed: %s\n", SDL_GetError());
		std::exit(3);
	}
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	if (!SDL_GetWindowWMInfo(g_win, &info)) {
		std::fprintf(stderr, "[display] SDL_GetWindowWMInfo failed: %s\n", SDL_GetError());
		std::exit(3);
	}
	g_hwnd = info.info.win.window;
	// FPUPRESERVE keeps the game's double-precision maths double: without it Direct3D drops the FPU to single precision for the
	// whole process. NORMAL: a window's share of the screen, no exclusive mode.
	if (FAILED(DirectDrawCreateEx(0, (void**)&g_dd, IID_IDirectDraw7, 0)) || !g_dd ||
	    FAILED(g_dd->SetCooperativeLevel(g_hwnd, DDSCL_NORMAL | DDSCL_FPUPRESERVE | DDSCL_MULTITHREADED))) {
		std::fprintf(stderr, "[display] no DirectDraw 7 on this system (Windows has Direct3D 7 for 32-bit programs only)\n");
		std::exit(3);
	}
	DDSURFACEDESC2 sd;
	std::memset(&sd, 0, sizeof(sd));
	sd.dwSize = sizeof(sd);
	sd.dwFlags = DDSD_CAPS;
	sd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	if (FAILED(g_dd->CreateSurface(&sd, &g_primary, 0)) || FAILED(g_dd->CreateClipper(0, &g_clipper, 0)) ||
	    FAILED(g_clipper->SetHWnd(0, g_hwnd)) || FAILED(g_primary->SetClipper(g_clipper)) ||
	    FAILED(g_dd->QueryInterface(IID_IDirect3D7, (void**)&g_d3d))) {
		std::fprintf(stderr, "[display] the DirectDraw 7 screen or Direct3D 7 could not be had\n");
		std::exit(3);
	}
	// The card's device with hardware transform and lighting, the card's without, Direct3D's software rasterizer: the first that
	// takes the targets.
	const GUID* devices[3] = { &IID_IDirect3DTnLHalDevice, &IID_IDirect3DHALDevice, &IID_IDirect3DRGBDevice };
	const char* names[3] = { "hardware T&L", "hardware", "software (RGB)" };
	for (int i = 0; i < 3 && !g_dev; ++i) {
		g_systemMemory = i == 2;
		std::memset(&g_depthFormat, 0, sizeof(g_depthFormat));
		g_d3d->EnumZBufferFormats(*devices[i], pick_depth, &g_depthFormat);
		if (!(g_depthFormat.dwFlags & DDPF_ZBUFFER)) continue;
		if (!make_targets(w, h, &g_back, &g_depth)) continue;
		if (FAILED(g_d3d->CreateDevice(*devices[i], g_back, &g_dev)) || !g_dev) {
			g_dev = 0;
			release(g_depth);
			release(g_back);
			continue;
		}
		DbgPrintF("[display] Direct3D 7: the %s device, %u-bit depth\n", names[i], (unsigned)g_depthFormat.dwZBufferBitDepth);
	}
	if (!g_dev) {
		std::fprintf(stderr, "[display] no Direct3D 7 device would draw on a %dx%d target\n", w, h);
		std::exit(3);
	}
	g_bw = w; g_bh = h;
	if (!j2xgl::gpu::attachD3D7(g_dd, g_d3d, g_dev, g_back, w, h, g_systemMemory)) {
		std::fprintf(stderr, "[display] the Direct3D 7 renderer could not start\n");
		std::exit(3);
	}
	static bool registered = false;
	if (!registered) { registered = true; std::atexit(release_at_exit); }
	j2xgl::glInit();
	j2xgl::glViewport(0, 0, w, h);
	Input::setDisplaySize(w, h);
}

void setParent(const void* parent) { g_parented = parent != 0; }

void destroy() {
	j2xgl::gpu::detachD3D7();
	release(g_dev);
	release(g_depth);
	release(g_back);
	release(g_d3d);
	if (g_primary) g_primary->SetClipper(0);
	release(g_clipper);
	release(g_primary);
	release(g_dd);
	if (g_win) SDL_DestroyWindow(g_win);
	g_win = 0;
	g_hwnd = 0;
}

void processMessages() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			g_close = true;
			if (g_parented) std::exit(0);   // a game in a canvas never asks isCloseRequested: closing the window ends it (Display.cpp, OpenGL)
		} else if (e.type == SDL_WINDOWEVENT) {
			if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) g_active = true;
			else if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) g_active = false;
			else if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				Input::setDisplaySize(e.window.data1, e.window.data2);
				g_needResize = true;
			}
		}
		Input::onEvent(&e);
	}
}

// Present the frame: the render target blitted to the window's place on the screen (after the vertical blank, with vsync on).
// Surfaces the blit finds lost are restored.
void swapBuffers() {
	if (!g_dev) return;
	j2xgl::glFlush();   // the layer's pending batch and this frame's new display-list buffers
	j2xgl::gpu::endSceneD3D7();
	if (g_vsync) g_dd->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
	RECT dst;
	POINT at = { 0, 0 };
	if (IsWindowVisible(g_hwnd) && GetClientRect(g_hwnd, &dst) && ClientToScreen(g_hwnd, &at)) {
		OffsetRect(&dst, at.x, at.y);
		RECT src = { 0, 0, g_bw, g_bh };
		const HRESULT hr = g_primary->Blt(&dst, g_back, &src, DDBLT_WAIT, 0);
		if (hr == DDERR_SURFACELOST) g_dd->RestoreAllSurfaces();
	}
	if (g_back && g_back->IsLost() == DDERR_SURFACELOST) g_dd->RestoreAllSurfaces();
	if (g_needResize) resize_targets();
}

// The automation hook -- a look at what the port does without a person at the screen (tests, CI). The same commands as the
// OpenGL Display: J2X_SHOT_FRAME=N saves frame N as j2x_shot.bmp; J2X_SCRIPT="F:cmd;..." runs `click X Y`, `key NAME`, `hold NAME`,
// `release NAME`, `move DX DY`, `resize W H`, `shot FILE.bmp`,
// `quit` at frame F.
static void save_frame(const char* file) {
	std::vector<unsigned int> px;
	int w = 0, h = 0;
	if (!j2xgl::gpu::readFrame(px, w, h) || px.empty()) return;
	SDL_Surface* s = SDL_CreateRGBSurfaceWithFormat(0, w, h, 24, SDL_PIXELFORMAT_BGR24);
	if (!s) return;
	for (int y = 0; y < h; ++y) {       // ARGB words -> B, G, R bytes
		unsigned char* dst = (unsigned char*)s->pixels + (size_t)y * s->pitch;
		for (int x = 0; x < w; ++x) {
			const unsigned int p = px[(size_t)y * w + x];
			dst[x * 3] = (unsigned char)p; dst[x * 3 + 1] = (unsigned char)(p >> 8); dst[x * 3 + 2] = (unsigned char)(p >> 16);
		}
	}
	SDL_SaveBMP(s, file);
	SDL_FreeSurface(s);
	std::fprintf(stderr, "[display] frame saved to %s\n", file);
}

static void push_key(const char* name, bool down) {
	SDL_Event e;
	std::memset(&e, 0, sizeof(e));
	e.type = down ? SDL_KEYDOWN : SDL_KEYUP;
	e.key.state = down ? SDL_PRESSED : SDL_RELEASED;
	e.key.keysym.sym = SDL_GetKeyFromName(name);
	e.key.keysym.scancode = SDL_GetScancodeFromKey(e.key.keysym.sym);
	SDL_PushEvent(&e);
}

static void run_command(const std::string& cmd) {
	char a[64] = {0};
	int x = 0, y = 0;
	if (std::sscanf(cmd.c_str(), "click %d %d", &x, &y) == 2) {
		if (!std::getenv("J2X_HIDDEN")) SDL_WarpMouseInWindow(g_win, x, y);
		SDL_Event e;
		std::memset(&e, 0, sizeof(e));
		e.type = SDL_MOUSEMOTION; e.motion.x = x; e.motion.y = y;
		SDL_PushEvent(&e);
		std::memset(&e, 0, sizeof(e));
		e.type = SDL_MOUSEBUTTONDOWN; e.button.button = SDL_BUTTON_LEFT; e.button.state = SDL_PRESSED; e.button.x = x; e.button.y = y;
		SDL_PushEvent(&e);
		e.type = SDL_MOUSEBUTTONUP; e.button.state = SDL_RELEASED;
		SDL_PushEvent(&e);
	} else if (std::sscanf(cmd.c_str(), "resize %d %d", &x, &y) == 2 && x > 0 && y > 0) {   // (the window, as a person drags it)
		SDL_SetWindowSize(g_win, x, y);
		Input::setDisplaySize(x, y);
		g_needResize = true;
	} else if (std::sscanf(cmd.c_str(), "move %d %d", &x, &y) == 2) {
		SDL_Event e;
		std::memset(&e, 0, sizeof(e));
		e.type = SDL_MOUSEMOTION; e.motion.xrel = x; e.motion.yrel = y;
		SDL_PushEvent(&e);
	} else if (std::sscanf(cmd.c_str(), "key %63s", a) == 1) {
		push_key(a, true);
		push_key(a, false);
	} else if (std::sscanf(cmd.c_str(), "hold %63s", a) == 1) {
		push_key(a, true);
	} else if (std::sscanf(cmd.c_str(), "release %63s", a) == 1) {
		push_key(a, false);
	} else if (std::sscanf(cmd.c_str(), "shot %63s", a) == 1) {
		save_frame(a);
	} else if (cmd == "quit") {
		std::fprintf(stderr, "[display] script: quit\n");
		std::exit(0);
	}
}

static void automate() {
	static int frame = 0, shot_at = -2;
	static std::vector<std::pair<int, std::string> > script;
	if (shot_at == -2) {
		const char* s = std::getenv("J2X_SHOT_FRAME");
		shot_at = s ? std::atoi(s) : -1;
		const char* sc = std::getenv("J2X_SCRIPT");
		std::string all = sc ? sc : "";
		size_t pos = 0;
		while (pos < all.size()) {
			size_t end = all.find(';', pos);
			std::string item = all.substr(pos, end == std::string::npos ? std::string::npos : end - pos);
			size_t colon = item.find(':');
			if (colon != std::string::npos)
				script.push_back(std::make_pair(std::atoi(item.substr(0, colon).c_str()), item.substr(colon + 1)));
			if (end == std::string::npos) break;
			pos = end + 1;
		}
	}
	if (!g_win) return;
	++frame;
	const bool shoot = frame == shot_at;
	bool scripted_shot = false;
	for (size_t i = 0; i < script.size(); ++i)
		if (script[i].first == frame && script[i].second.compare(0, 5, "shot ") == 0) scripted_shot = true;
	if (shoot || scripted_shot) j2xgl::glFlush();   // (a shot is the frame as drawn so far, the layer's pending batch in it)
	if (shoot) save_frame("j2x_shot.bmp");
	for (size_t i = 0; i < script.size(); ++i)
		if (script[i].first == frame) run_command(script[i].second);
}

void update() {
	automate();
	swapBuffers();
	processMessages();
}

bool isCloseRequested() { return g_close; }
bool isActive() { return g_active || std::getenv("J2X_SCRIPT") != 0; }   // a scripted run never pauses
bool isCreated() { return g_win != 0; }

int getWidth() {
	int w = 0, h = 0;
	if (g_win) SDL_GetWindowSize(g_win, &w, &h);
	return g_win ? w : g_mode.getWidth();
}

int getHeight() {
	int w = 0, h = 0;
	if (g_win) SDL_GetWindowSize(g_win, &w, &h);
	return g_win ? h : g_mode.getHeight();
}

DisplayMode getDisplayMode() {
	return g_win ? DisplayMode(getWidth(), getHeight()) : g_mode;
}

DisplayMode getDesktopDisplayMode() {
	SDL_DisplayMode m;
	if (SDL_WasInit(SDL_INIT_VIDEO) && SDL_GetDesktopDisplayMode(0, &m) == 0)
		return DisplayMode(m.w, m.h, SDL_BITSPERPIXEL(m.format), m.refresh_rate);
	return DisplayMode(1280, 720);
}

void setTitle(const std::string& title) {
	g_title = title;
	if (g_win) SDL_SetWindowTitle(g_win, title.c_str());
}

// Full screen is the desktop-sized borderless window (Direct3D stays windowed: no exclusive mode to lose on alt-tab).
void setFullscreen(bool full) {
	g_full = full;
	if (g_win) {
		SDL_SetWindowFullscreen(g_win, full ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
		g_needResize = true;
	}
}

void setVSyncEnabled(bool on) { g_vsync = on; }   // (the wait for the vertical blank, frame by frame)

void sync(int fps) {
	static Uint32 last = 0;
	if (fps <= 0) return;
	Uint32 frame = 1000u / (Uint32)fps, now = SDL_GetTicks();
	if (last && now - last < frame) SDL_Delay(frame - (now - last));
	last = SDL_GetTicks();
}

} // namespace Display
