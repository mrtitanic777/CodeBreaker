// GL13 on the PC: each call loaded once from the driver (SDL_GL_GetProcAddress, after the window's context exists) and made
// natively; a driver without it (none in practice: every OpenGL since 2001 has 1.3) leaves the call a no-op.
#include "stdafx.h"
#include "GL13.h"
#include <SDL2/SDL.h>
#ifndef APIENTRY
#define APIENTRY
#endif

namespace {
typedef void (APIENTRY* ActiveTextureFn)(unsigned int);
typedef void (APIENTRY* MultiTexCoord2fFn)(unsigned int, float, float);

template<class F> F gl13_load(F& slot, bool& tried, const char* name) {
	if (!tried) { tried = true; slot = (F)SDL_GL_GetProcAddress(name); }
	return slot;
}
ActiveTextureFn s_active = 0, s_clientActive = 0;
MultiTexCoord2fFn s_multiTexCoord2f = 0;
bool s_activeTried = false, s_clientActiveTried = false, s_multiTried = false;
}  // namespace

namespace GL13 {
void glActiveTexture(int texture) {
	if (ActiveTextureFn f = gl13_load(s_active, s_activeTried, "glActiveTexture")) f((unsigned int)texture);
}
void glClientActiveTexture(int texture) {
	if (ActiveTextureFn f = gl13_load(s_clientActive, s_clientActiveTried, "glClientActiveTexture")) f((unsigned int)texture);
}
void glMultiTexCoord2f(int target, float s, float t) {
	if (MultiTexCoord2fFn f = gl13_load(s_multiTexCoord2f, s_multiTried, "glMultiTexCoord2f")) f((unsigned int)target, s, t);
}
}  // namespace GL13
