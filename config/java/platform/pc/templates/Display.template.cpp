//@hole intro
//
//@hole device_notes
#include "stdafx.h"
#include "Display.h"
#include "Input.h"
#include "GL11.h"
//@hole gpu_include
#include "Common.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
//@hole api_includes
#include <vector>
#include <cstring>
#include <cstdlib>

//@hole api_globals
namespace {
	SDL_Window* g_win = 0;
	HWND g_hwnd = 0;
//@hole device_state
	DisplayMode g_mode(854, 480);
	std::string g_title = "Minecraft";
	bool g_close = false;
	bool g_active = true;
	bool g_full = false;
	bool g_parented = false;
	bool g_vsync = true;

//@hole device_helpers
		int w = 0, h = 0;
		SDL_GetWindowSize(g_win, &w, &h);
//@hole resize_device
		Input::setDisplaySize(w, h);
//@hole resize_done
	}
//@hole device_ready_fn
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
//@hole resize_flag
	}
}

//@hole fatal_comment
void create() {
	if (g_win) return;
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) != 0) {
		std::fprintf(stderr, "[display] SDL_Init failed: %s\n", SDL_GetError());
//@hole fail
	}
	const int w = g_mode.getWidth() > 0 ? g_mode.getWidth() : 854;
	const int h = g_mode.getHeight() > 0 ? g_mode.getHeight() : 480;
	g_win = SDL_CreateWindow(g_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
	                         (std::getenv("J2X_HIDDEN") ? SDL_WINDOW_HIDDEN : SDL_WINDOW_SHOWN) | SDL_WINDOW_RESIZABLE);   // (J2X_HIDDEN: a test run off-screen)
	if (!g_win) {
		std::fprintf(stderr, "[display] SDL_CreateWindow failed: %s\n", SDL_GetError());
//@hole fail
	}
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	if (!SDL_GetWindowWMInfo(g_win, &info)) {
		std::fprintf(stderr, "[display] SDL_GetWindowWMInfo failed: %s\n", SDL_GetError());
//@hole fail
	}
	g_hwnd = info.info.win.window;
//@hole create_device_1
	}
//@hole create_device_2
	}
//@hole create_device_3
	Input::setDisplaySize(w, h);
//@hole first_frame
}

void setParent(const void* parent) { g_parented = parent != 0; }

void destroy() {
//@hole destroy_device
	if (g_win) SDL_DestroyWindow(g_win);
//@hole destroy_cleared
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
//@hole resize_flag_deep
			}
		}
		Input::onEvent(&e);
	}
}

//@hole present_comment
void swapBuffers() {
//@hole present_body
}

// The automation hook -- a look at what the port does without a person at the screen (tests, CI). The same commands as the
// OpenGL Display: J2X_SHOT_FRAME=N saves frame N as j2x_shot.bmp; J2X_SCRIPT="F:cmd;..." runs `click X Y`, `key NAME`, `hold NAME`,
// `release NAME`, `move DX DY`, `resize W H`, `shot FILE.bmp`,
// `quit` at frame F.
static void save_frame(const char* file) {
//@hole save_frame_body
		}
	}
//@hole save_frame_end
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
//@hole resize_flag
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
//@hole flush_for_shot
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

//@hole fullscreen_comment
void setFullscreen(bool full) {
	g_full = full;
	if (g_win) {
		SDL_SetWindowFullscreen(g_win, full ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
//@hole resize_flag
	}
}

//@hole vsync_fn

void sync(int fps) {
	static Uint32 last = 0;
	if (fps <= 0) return;
	Uint32 frame = 1000u / (Uint32)fps, now = SDL_GetTicks();
	if (last && now - last < frame) SDL_Delay(frame - (now - last));
	last = SDL_GetTicks();
}

} // namespace Display
