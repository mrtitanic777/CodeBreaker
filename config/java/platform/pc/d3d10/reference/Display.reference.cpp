// Display on SDL2 + Direct3D 10 (see Display.reference.h). The window is SDL's (input, focus, the AWT mouse are the same code as
// the OpenGL and Direct3D 9 Displays); the drawing surface is a DXGI swap chain on the window's HWND with a Direct3D 10 device,
// handed to the GL layer's Direct3D 10 backend (GpuD3D10.cpp). A frame is drawn, then update() presents it.
//
// The device is the card's, else WARP, Windows' own software rasterizer (a virtual machine or a remote desktop without a
// Direct3D 10 card still runs the game). J2X_D3D_DEBUG=1 makes it with the debug layer, whose errors and warnings the backend
// prints. A resized window resizes the swap chain's buffers and the
// depth buffer; a removed device (a driver update or crash) is reported, as Direct3D 10 cannot get it back without remaking
// every texture.
#include "stdafx.h"
#include "Display.h"
#include "Input.h"
#include "GL11.h"
#include "GpuD3D10.h"
#include "Common.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <d3d10.h>
#include <dxgi.h>
#include <vector>
#include <cstring>
#include <cstdlib>

namespace {
	SDL_Window* g_win = 0;
	HWND g_hwnd = 0;
	ID3D10Device* g_dev = 0;
	IDXGISwapChain* g_swap = 0;
	ID3D10Texture2D* g_back = 0;
	ID3D10RenderTargetView* g_rtv = 0;
	ID3D10Texture2D* g_depth = 0;
	ID3D10DepthStencilView* g_dsv = 0;
	int g_bw = 0, g_bh = 0;       // the swap chain's buffer size
	bool g_needResize = false;    // the window changed size: resize the buffers before the next frame
	bool g_removed = false;
	DisplayMode g_mode(854, 480);
	std::string g_title = "Minecraft";
	bool g_close = false;
	bool g_active = true;
	bool g_full = false;
	bool g_parented = false;
	bool g_vsync = true;

	template<class T> void release(T*& p) { if (p) { p->Release(); p = 0; } }

	// The device is released before the process ends, however it ends (the game's System.exit, the script's quit, the last thread
	// returning): the GPU's work finished and every object let go while the process can still run it. A process that exits with
	// a live device leaves that to the driver as Windows tears the process down, and Direct3D 12's has been seen to hang there.
	void release_at_exit() { Display::destroy(); }

	// The views on the swap chain's buffer and a depth buffer of its size, handed to the backend.
	bool make_targets(int w, int h) {
		if (FAILED(g_swap->GetBuffer(0, __uuidof(ID3D10Texture2D), (void**)&g_back)) || !g_back) return false;
		if (FAILED(g_dev->CreateRenderTargetView(g_back, 0, &g_rtv))) return false;
		D3D10_TEXTURE2D_DESC d;
		std::memset(&d, 0, sizeof(d));
		d.Width = (UINT)w; d.Height = (UINT)h; d.MipLevels = 1; d.ArraySize = 1;
		d.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; d.SampleDesc.Count = 1;
		d.Usage = D3D10_USAGE_DEFAULT; d.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		if (FAILED(g_dev->CreateTexture2D(&d, 0, &g_depth)) || FAILED(g_dev->CreateDepthStencilView(g_depth, 0, &g_dsv))) return false;
		g_bw = w; g_bh = h;
		j2xgl::gpu::setTargetsD3D10(g_rtv, g_dsv, g_back, w, h);
		return true;
	}
	void drop_targets() {
		j2xgl::gpu::setTargetsD3D10(0, 0, 0, 0, 0);
		release(g_rtv); release(g_back); release(g_dsv); release(g_depth);
	}
	// Resize the swap chain's buffers to the window (ResizeBuffers needs every view on them released first).
	void resize_targets() {
		if (!g_swap) return;
		int w = 0, h = 0;
		SDL_GetWindowSize(g_win, &w, &h);
		if (w < 1) w = 1;
		if (h < 1) h = 1;
		drop_targets();
		const HRESULT hr = g_swap->ResizeBuffers(0, (UINT)w, (UINT)h, DXGI_FORMAT_UNKNOWN, 0);
		if (FAILED(hr)) DbgPrintF("[display] ResizeBuffers failed: 0x%08x\n", (unsigned)hr);
		if (!make_targets(w, h)) DbgPrintF("[display] the resized targets could not be made\n");
		Input::setDisplaySize(w, h);
		g_needResize = false;
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
	DXGI_SWAP_CHAIN_DESC sd;
	std::memset(&sd, 0, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = (UINT)w; sd.BufferDesc.Height = (UINT)h;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // (Direct3D 10 has no B8G8R8A8: see GpuD3D10.cpp)
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hwnd;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	const UINT flags = std::getenv("J2X_D3D_DEBUG") ? D3D10_CREATE_DEVICE_DEBUG : 0;   // (Direct3D's validation, the Graphics Tools feature)
	// The card's device, else WARP (Windows' software rasterizer): the game runs where there is no Direct3D 10 card at all.
	HRESULT hr = D3D10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE, 0, flags, D3D10_SDK_VERSION, &sd, &g_swap, &g_dev);
	if (FAILED(hr)) {
		std::fprintf(stderr, "[display] no Direct3D 10 hardware device (0x%08x); using WARP\n", (unsigned)hr);
		hr = D3D10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_WARP, 0, flags, D3D10_SDK_VERSION, &sd, &g_swap, &g_dev);
	}
	if (FAILED(hr) || !g_dev || !g_swap) {
		std::fprintf(stderr, "[display] D3D10CreateDeviceAndSwapChain failed: 0x%08x\n", (unsigned)hr);
		std::exit(3);
	}
	IDXGIFactory* factory = 0;   // (Alt+Enter is the game's own F11, not DXGI's exclusive full screen)
	if (SUCCEEDED(g_swap->GetParent(__uuidof(IDXGIFactory), (void**)&factory)) && factory) {
		factory->MakeWindowAssociation(g_hwnd, DXGI_MWA_NO_ALT_ENTER);
		factory->Release();
	}
	if (!j2xgl::gpu::attachD3D10(g_dev) || !make_targets(w, h)) {
		std::fprintf(stderr, "[display] the Direct3D 10 renderer could not start\n");
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
	drop_targets();
	j2xgl::gpu::detachD3D10();
	release(g_swap); release(g_dev);
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

// Present the frame. A removed device is said once; the game goes on (drawing nothing) rather than dying.
void swapBuffers() {
	if (!g_swap) return;
	j2xgl::glFlush();   // the layer's pending batch and this frame's new display-list buffers
	const HRESULT hr = g_swap->Present(g_vsync ? 1 : 0, 0);
	j2xgl::gpu::frameDoneD3D10();
	if ((hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) && !g_removed) {
		g_removed = true;
		DbgPrintF("[display] the Direct3D 10 device was removed (0x%08x): reason 0x%08x\n", (unsigned)hr, (unsigned)g_dev->GetDeviceRemovedReason());
	}
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

// Full screen is the desktop-sized borderless window (the swap chain stays windowed: no exclusive mode to lose on alt-tab).
void setFullscreen(bool full) {
	g_full = full;
	if (g_win) {
		SDL_SetWindowFullscreen(g_win, full ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
		g_needResize = true;
	}
}

void setVSyncEnabled(bool on) { g_vsync = on; }   // (Present's sync interval, frame by frame)

void sync(int fps) {
	static Uint32 last = 0;
	if (fps <= 0) return;
	Uint32 frame = 1000u / (Uint32)fps, now = SDL_GetTicks();
	if (last && now - last < frame) SDL_Delay(frame - (now - last));
	last = SDL_GetTicks();
}

} // namespace Display
