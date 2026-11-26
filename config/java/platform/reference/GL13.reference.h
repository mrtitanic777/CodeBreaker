// org.lwjgl.opengl.GL13's multitexture calls on the GL layer (GL11.h): the layer keeps two texture units, each with its
// own binding, enable, environment and texture matrix (Beta 1.8's lightmap is unit 1).
#ifndef GL13_H
#define GL13_H
#include "stdafx.h"
#include "JavaRT.h"
#include "GL11.h"

namespace GL13 {
	const int GL_TEXTURE0 = 0x84C0, GL_TEXTURE1 = 0x84C1, GL_TEXTURE2 = 0x84C2, GL_TEXTURE3 = 0x84C3;
	const int GL_ACTIVE_TEXTURE = 0x84E0, GL_CLIENT_ACTIVE_TEXTURE = 0x84E1, GL_MAX_TEXTURE_UNITS = 0x84E2;
	inline void glActiveTexture(int texture) { j2xgl::glActiveTexture(texture); }
	inline void glClientActiveTexture(int texture) { j2xgl::glClientActiveTexture(texture); }
	inline void glMultiTexCoord2f(int target, float s, float t) { j2xgl::glMultiTexCoord2f(target, s, t); }
}
#endif // GL13_H
