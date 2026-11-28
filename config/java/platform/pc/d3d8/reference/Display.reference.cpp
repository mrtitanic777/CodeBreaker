// Display on SDL2 + Direct3D 8 (see Display.reference.h). The window is SDL's (input, focus, the AWT mouse are the same code as
// the OpenGL and other Direct3D Displays); the drawing surface is a Direct3D 8 device made on the window's HWND and handed to the
// GL layer's Direct3D 8 backend (GpuD3D8.cpp). Windows has Direct3D 8 for 32-bit programs only (d3d8.dll in SysWOW64), so this
// build is a 32-bit program; the runtime is loaded when the window is made (Direct3D8.h: no import library, any compiler).
//
// The device is kept through the things that take a windowed Direct3D 8 device away: a resized window resets it to the new
// size, and a lost device (the lock screen, a UAC prompt, a display-mode change) waits until Windows hands it back, then resets
// it. Textures and display lists are in the managed pool, which a reset keeps. A windowed Direct3D 8 device waits for the
// screen by its swap effect (COPY_VSYNC), so turning vsync on or off is a reset too.
#define J2X_WITH_GDI   // (windows.h's GDI types, which stdafx.h keeps out of the game's files)
#include "stdafx.h"
#include "Display.h"
#include "Input.h"
#include "GL11.h"
#include "GpuD3D8.h"
#include "Direct3D8.h"
#include "Common.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <vector>
#include <cstring>
#include <cstdlib>

using namespace d3d8;

namespace {
	SDL_Window* g_win = 0;
	HWND g_hwnd = 0;
	IDirect3D8* g_d3d = 0;
	IDirect3DDevice8* g_dev = 0;
	D3DPRESENT_PARAMETERS g_pp;
	D3DFORMAT g_depth = D3DFMT_D24S8;
	bool g_needResize = false;    // the window changed size (or vsync was switched): reset the device before the next frame
	bool g_saidLost = false;
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

	// The present parameters for the window as it is now: its client size, a depth buffer, a back buffer the frame can be read
	// from (the scripted run's screenshots), vsync as asked.
	void fill_present_parameters(int w, int h) {
		std::memset(&g_pp, 0, sizeof(g_pp));
		g_pp.Windowed = TRUE;
		g_pp.SwapEffect = g_vsync ? D3DSWAPEFFECT_COPY_VSYNC : D3DSWAPEFFECT_DISCARD;
		g_pp.BackBufferFormat = D3DFMT_X8R8G8B8;   // (a windowed device draws in the desktop's format: 32-bit today)
		g_pp.BackBufferWidth = (UINT)(w > 0 ? w : 1);
		g_pp.BackBufferHeight = (UINT)(h > 0 ? h : 1);
		g_pp.BackBufferCount = 1;
		g_pp.hDeviceWindow = g_hwnd;
		g_pp.EnableAutoDepthStencil = TRUE;
		g_pp.AutoDepthStencilFormat = g_depth;
		g_pp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		g_pp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;   // (a windowed device must leave it 0)
	}

	// Reset the device to the current window size; true when it is usable again. The backend sets its state again after.
	bool reset_device() {
		if (!g_dev) return false;
		j2xgl::gpu::endSceneD3D8();
		int w = 0, h = 0;
		SDL_GetWindowSize(g_win, &w, &h);
		if (w < 1) w = 1;
		if (h < 1) h = 1;
		fill_present_parameters(w, h);
		const HRESULT hr = g_dev->Reset(&g_pp);
		if (FAILED(hr)) {
			DbgPrintF("[display] device Reset failed: 0x%08x\n", (unsigned)hr);
			return false;
		}
		j2xgl::gpu::resizedD3D8(w, h);
		Input::setDisplaySize(w, h);
		g_needResize = false;
		g_saidLost = false;
		return true;
	}

	// Before a frame: a lost device waits (Windows has it), a resized window resets.
	void device_ready() {
		if (!g_dev) return;
		const HRESULT coop = g_dev->TestCooperativeLevel();
		if (coop == D3DERR_DEVICELOST) { SDL_Delay(10); return; }
		if (coop == D3DERR_DEVICENOTRESET || g_needResize) reset_device();
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
	g_d3d = d3d8::create();
	if (!g_d3d) {
		std::fprintf(stderr, "[display] no Direct3D 8 on this system (d3d8.dll; Windows has it for 32-bit programs only)\n");
		std::exit(3);
	}
	// Hardware vertex processing where the card has it, else software (a VM's or a remote desktop's adapter); the first depth
	// format the card takes. FPU_PRESERVE keeps the game's double-precision maths double: without it Direct3D drops the FPU to
	// single precision for the whole process.
	const DWORD keep = D3DCREATE_FPU_PRESERVE | D3DCREATE_MULTITHREADED;
	static const D3DFORMAT depths[] = { D3DFMT_D24S8, D3DFMT_D24X8, D3DFMT_D16 };
	HRESULT hr = E_FAIL;
	for (int i = 0; i < 3 && !g_dev; ++i) {
		g_depth = depths[i];
		fill_present_parameters(w, h);
		hr = g_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | keep, &g_pp, &g_dev);
		if (FAILED(hr)) { g_dev = 0; hr = g_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | keep, &g_pp, &g_dev); }
		if (FAILED(hr)) g_dev = 0;
	}
	if (!g_dev) {
		std::fprintf(stderr, "[display] Direct3D 8 CreateDevice failed: 0x%08x\n", (unsigned)hr);
		std::exit(3);
	}
	if (!j2xgl::gpu::attachD3D8(g_dev, w, h)) {
		std::fprintf(stderr, "[display] the Direct3D 8 renderer could not start\n");
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
	j2xgl::gpu::detachD3D8();
	release(g_dev);
	release(g_d3d);
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

// Present the frame. A lost device skips the present; device_ready() brings it back.
void swapBuffers() {
	if (!g_dev) return;
	j2xgl::glFlush();   // the layer's pending batch and this frame's new display-list buffers
	j2xgl::gpu::endSceneD3D8();
	const HRESULT hr = g_dev->Present(0, 0, 0, 0);
	if (hr == D3DERR_DEVICELOST && !g_saidLost) { g_saidLost = true; DbgPrintF("[display] device lost; waiting for it\n"); }
	device_ready();
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

void setVSyncEnabled(bool on) {
	if (g_vsync == on) return;
	g_vsync = on;
	if (g_dev) g_needResize = true;   // (the swap effect is a present parameter: it takes a reset)
}

void sync(int fps) {
	static Uint32 last = 0;
	if (fps <= 0) return;
	Uint32 frame = 1000u / (Uint32)fps, now = SDL_GetTicks();
	if (last && now - last < frame) SDL_Delay(frame - (now - last));
	last = SDL_GetTicks();
}

} // namespace Display
