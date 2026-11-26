// org.lwjgl.opengl.ARBMultitexture on the GL layer (GL11.h): the ARB spellings of GL13's multitexture calls, the same
// two texture units (a game that asks for the extension rather than GL 1.3 gets the same lightmap).
#ifndef ARBMULTITEXTURE_H
#define ARBMULTITEXTURE_H
#include "stdafx.h"
#include "JavaRT.h"
#include "GL11.h"

namespace ARBMultitexture {
	const int GL_TEXTURE0_ARB = 0x84C0, GL_TEXTURE1_ARB = 0x84C1, GL_TEXTURE2_ARB = 0x84C2, GL_TEXTURE3_ARB = 0x84C3;
	const int GL_ACTIVE_TEXTURE_ARB = 0x84E0, GL_CLIENT_ACTIVE_TEXTURE_ARB = 0x84E1, GL_MAX_TEXTURE_UNITS_ARB = 0x84E2;
	inline void glActiveTextureARB(int texture) { j2xgl::glActiveTexture(texture); }
	inline void glClientActiveTextureARB(int texture) { j2xgl::glClientActiveTexture(texture); }
	inline void glMultiTexCoord2fARB(int target, float s, float t) { j2xgl::glMultiTexCoord2f(target, s, t); }
}
#endif // ARBMULTITEXTURE_H
