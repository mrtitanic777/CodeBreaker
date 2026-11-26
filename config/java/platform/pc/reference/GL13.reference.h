// LWJGL org.lwjgl.opengl.GL13 on the PC: the multitexture calls (Beta 1.8 lights the world through a second texture unit,
// the lightmap). They are OpenGL 1.3, which Windows' opengl32 does not export: GL13.cpp loads them from the driver. This header
// does NOT include <GL/gl.h> (its GL_* macros would rewrite these constants) -- GL13.cpp does.
#ifndef GL13_H
#define GL13_H
#include "stdafx.h"
#include "JavaRT.h"

namespace GL13 {
	const int GL_TEXTURE0 = 0x84C0, GL_TEXTURE1 = 0x84C1, GL_TEXTURE2 = 0x84C2, GL_TEXTURE3 = 0x84C3;
	const int GL_ACTIVE_TEXTURE = 0x84E0, GL_CLIENT_ACTIVE_TEXTURE = 0x84E1, GL_MAX_TEXTURE_UNITS = 0x84E2;
	void glActiveTexture(int texture);
	void glClientActiveTexture(int texture);
	void glMultiTexCoord2f(int target, float s, float t);
}
#endif // GL13_H
