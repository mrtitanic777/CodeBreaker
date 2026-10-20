// Display on SDL2 + OpenGL (see Display.reference.h).
#include "stdafx.h"
#include "Display.h"
#include "Input.h"
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <vector>
#include <cstring>
#include <cstdlib>

namespace {
	SDL_Window* g_win = 0;
	SDL_GLContext g_ctx = 0;
	DisplayMode g_mode(854, 480);
	std::string g_title = "Minecraft";
	bool g_close = false;
	bool g_active = true;
	bool g_full = false;
}

int j2xDisplayWidth() { return Display::getWidth(); }    // AWT components report the window (stdafx.h)
int j2xDisplayHeight() { return Display::getHeight(); }

namespace Display {

void setDisplayMode(DisplayMode* mode) {
	if (mode) setDisplayMode(*mode);
}

void setDisplayMode(const DisplayMode& mode) {
	g_mode = mode;
	if (g_win && !g_full && mode.getWidth() > 0 && mode.getHeight() > 0)
		SDL_SetWindowSize(g_win, mode.getWidth(), mode.getHeight());
}

void create() {
	if (g_win) return;
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) != 0) {
		std::fprintf(stderr, "[display] SDL_Init failed: %s\n", SDL_GetError());
		return;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);        // fixed function, compatibility profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	int w = g_mode.getWidth() > 0 ? g_mode.getWidth() : 854;
	int h = g_mode.getHeight() > 0 ? g_mode.getHeight() : 480;
	g_win = SDL_CreateWindow(g_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
	                         SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!g_win) {
		std::fprintf(stderr, "[display] SDL_CreateWindow failed: %s\n", SDL_GetError());
		return;
	}
	g_ctx = SDL_GL_CreateContext(g_win);
	if (!g_ctx) std::fprintf(stderr, "[display] SDL_GL_CreateContext failed: %s\n", SDL_GetError());
	SDL_GL_SetSwapInterval(1);
	Input::setDisplaySize(w, h);
}

void destroy() {
	if (g_ctx) SDL_GL_DeleteContext(g_ctx);
	if (g_win) SDL_DestroyWindow(g_win);
	g_ctx = 0;
	g_win = 0;
}

void processMessages() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			g_close = true;
		} else if (e.type == SDL_WINDOWEVENT) {
			if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) g_active = true;
			else if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) g_active = false;
			else if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				Input::setDisplaySize(e.window.data1, e.window.data2);
		}
		Input::onEvent(&e);
	}
}

void swapBuffers() {
	if (g_win) SDL_GL_SwapWindow(g_win);
}

// The automation hook -- a look at what the port does without a person at the screen (tests, CI).
//   J2X_SHOT_FRAME=N        save frame N as j2x_shot.bmp in the working folder
//   J2X_SCRIPT="F:cmd;..."  at frame F run cmd: `click X Y` (left button, window pixels from the top
//                           left), `key NAME` (an SDL key name: Escape, Return, W...), `hold NAME` /
//                           `release NAME`, `move DX DY` (relative mouse), `shot FILE.bmp`, `quit`
static void save_frame(const char* file) {
	int w = getWidth(), h = getHeight();
	std::vector<unsigned char> px((size_t)w * h * 3);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, &px[0]);
	SDL_Surface* s = SDL_CreateRGBSurfaceWithFormat(0, w, h, 24, SDL_PIXELFORMAT_BGR24);
	if (!s) return;
	for (int y = 0; y < h; ++y)                          // GL rows run bottom-up
		std::memcpy((char*)s->pixels + (size_t)y * s->pitch, &px[(size_t)(h - 1 - y) * w * 3], (size_t)w * 3);
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
		SDL_WarpMouseInWindow(g_win, x, y);
		SDL_Event e;
		std::memset(&e, 0, sizeof(e));
		e.type = SDL_MOUSEMOTION; e.motion.x = x; e.motion.y = y;
		SDL_PushEvent(&e);
		std::memset(&e, 0, sizeof(e));
		e.type = SDL_MOUSEBUTTONDOWN; e.button.button = SDL_BUTTON_LEFT; e.button.state = SDL_PRESSED; e.button.x = x; e.button.y = y;
		SDL_PushEvent(&e);
		e.type = SDL_MOUSEBUTTONUP; e.button.state = SDL_RELEASED;
		SDL_PushEvent(&e);
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
	if (frame == shot_at) save_frame("j2x_shot.bmp");
	for (size_t i = 0; i < script.size(); ++i)
		if (script[i].first == frame) run_command(script[i].second);
}

void update() {
	automate();
	swapBuffers();
	processMessages();
}

bool isCloseRequested() { return g_close; }
bool isActive() { return g_active; }
bool isCreated() { return g_win != 0; }

int getWidth() {
	int w = 0, h = 0;
	if (g_win) SDL_GL_GetDrawableSize(g_win, &w, &h);
	return g_win ? w : g_mode.getWidth();
}

int getHeight() {
	int w = 0, h = 0;
	if (g_win) SDL_GL_GetDrawableSize(g_win, &w, &h);
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

void setFullscreen(bool full) {
	g_full = full;
	if (g_win) SDL_SetWindowFullscreen(g_win, full ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void setVSyncEnabled(bool on) {
	SDL_GL_SetSwapInterval(on ? 1 : 0);
}

void sync(int fps) {
	static Uint32 last = 0;
	if (fps <= 0) return;
	Uint32 frame = 1000u / (Uint32)fps, now = SDL_GetTicks();
	if (last && now - last < frame) SDL_Delay(frame - (now - last));
	last = SDL_GetTicks();
}

} // namespace Display
