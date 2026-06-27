// GL11.h — OpenGL (LWJGL) -> Direct3D 9 emulator for the Xbox 360 j2x port.  [Step 2]
//
// REFERENCE IMPLEMENTATION for the Xbox-project integration. It REPLACES the converter's no-op
// GL11 stub (j2x/runtime.py GL11_H): the new game code is faithful to the Java and calls GL11::glX,
// so a real GL->D3D9 layer makes it render. Lives in the Xbox project against the XDK's d3d9 + the
// platform layer (Common.h / Shaders.h / Tesselator.h / Textures.h) -- it is NOT compiled by the
// j2x build (which only syntax-checks the no-op stub). Grounded in the old port's Shaders.cpp +
// Tesselator.cpp (the proven GL->D3D9 mechanism) and docs/platform/02-rendering-gl-to-d3d9.md.
//
// Design: a classic immediate/array-mode emulator over D3D9 fixed-shader draws.
//   * Matrix mode MODELVIEW -> Shaders world matrix; PROJECTION -> Shaders projection (view=I).
//     glPushMatrix/glPop keep per-mode D3DXMATRIX stacks; Translate/Rotate/Scale/Mult pre-multiply.
//   * The hot path is the vertex-ARRAY path (glVertexPointer/glColorPointer/glTexCoordPointer +
//     glEnableClientState + glDrawArrays) -- that's what the new Tessellator uses (32B stride:
//     pos@0, uv@12, color@20 as RGBA bytes, normal@24). glDrawArrays gathers into TesselatorVertex
//     and draws via the SAME quad->tri reversed-winding DrawPrimitiveUP path as Tesselator::flush.
//   * Color: GL packs RGBA bytes (R,G,B,A order); D3DCOLOR is ARGB -> swizzle on gather.
//   * Alpha test (cutout: leaves/glass/items) -> Shaders::setAlphaRef (Xenos ignores
//     D3DRS_ALPHATESTENABLE). Fog -> Shaders::setFog*. Lighting/normals are no-ops (MC bakes light
//     into vertex color).
// HARDWARE-VERIFY (can't be checked off-device): projection handedness/winding (GL is RH, D3D LH --
// see old port's _11 negate + cull flip), display-list replay, and the exact set of LWJGL overloads.

#ifndef GL11_H
#define GL11_H

#include "stdafx.h"          // xtl.h / d3d9.h / d3dx9.h
#include "JavaRT.h"          // FloatBuffer / IntBuffer / ByteBuffer (NIO buffer overloads)

namespace GL11 {
	// --- enum tokens (only the ones the game uses; extend as needed) ---
	const int GL_FALSE = 0, GL_TRUE = 1;
	const int GL_POINTS = 0x0000, GL_LINES = 0x0001, GL_LINE_STRIP = 0x0003;
	const int GL_TRIANGLES = 0x0004, GL_TRIANGLE_STRIP = 0x0005, GL_QUADS = 0x0007;
	const int GL_BYTE = 0x1400, GL_UNSIGNED_BYTE = 0x1401, GL_SHORT = 0x1402, GL_FLOAT = 0x1406;
	const int GL_VERTEX_ARRAY = 0x8074, GL_NORMAL_ARRAY = 0x8075,
	          GL_COLOR_ARRAY = 0x8076, GL_TEXTURE_COORD_ARRAY = 0x8078;
	const int GL_MODELVIEW = 0x1700, GL_PROJECTION = 0x1701, GL_TEXTURE = 0x1702;
	const int GL_DEPTH_TEST = 0x0B71, GL_BLEND = 0x0BE2, GL_ALPHA_TEST = 0x0BC0,
	          GL_CULL_FACE = 0x0B44, GL_FOG = 0x0B60, GL_LIGHTING = 0x0B50,
	          GL_TEXTURE_2D = 0x0DE1, GL_COLOR_MATERIAL = 0x0B57, GL_DITHER = 0x0BD0;
	const int GL_DEPTH_BUFFER_BIT = 0x0100, GL_COLOR_BUFFER_BIT = 0x4000;
	const int GL_SRC_ALPHA = 0x0302, GL_ONE_MINUS_SRC_ALPHA = 0x0303, GL_ONE = 1, GL_ZERO = 0;
	const int GL_LEQUAL = 0x0203, GL_LESS = 0x0201, GL_GREATER = 0x0204, GL_GEQUAL = 0x0206;
	const int GL_BACK = 0x0405, GL_FRONT = 0x0404;
	const int GL_NEAREST = 0x2600, GL_LINEAR = 0x2601;
	const int GL_TEXTURE_MIN_FILTER = 0x2800, GL_TEXTURE_MAG_FILTER = 0x2801;
	const int GL_SMOOTH = 0x1D01, GL_FLAT = 0x1D00;
	const int GL_RGBA = 0x1908, GL_FOG_COLOR = 0x0B66, GL_FOG_DENSITY = 0x0B62, GL_EXP = 0x0800;

	// --- lifecycle (called from the platform layer, not the game) ---
	void glInit();                                   // bind decl + default state (after D3D device up)

	// --- matrix stack ---
	void glMatrixMode(int mode);
	void glPushMatrix();
	void glPopMatrix();
	void glLoadIdentity();
	void glTranslatef(float x, float y, float z);
	void glRotatef(float angle, float x, float y, float z);
	void glScalef(float x, float y, float z);
	void glMultMatrix(FloatBuffer* m);
	void glOrtho(double l, double r, double b, double t, double n, double f);
	void glViewport(int x, int y, int w, int h);
	void glLoadProjection(const D3DXMATRIX& m);      // set current PROJECTION top (used by GLU)

	// --- immediate color / normal (the "current" attribute when no array is bound) ---
	void glColor3f(float r, float g, float b);
	void glColor4f(float r, float g, float b, float a);
	void glNormal3f(float x, float y, float z);      // no-op (light baked into color)

	// --- render state ---
	void glEnable(int cap);
	void glDisable(int cap);
	void glBlendFunc(int sfactor, int dfactor);
	void glDepthFunc(int func);
	void glDepthMask(bool flag);
	void glColorMask(bool r, bool g, bool b, bool a);
	void glCullFace(int mode);
	void glAlphaFunc(int func, float ref);           // -> Shaders::setAlphaRef
	void glShadeModel(int mode);                     // no-op
	void glClearColor(float r, float g, float b, float a);
	void glClearDepth(double depth);
	void glClear(int mask);

	// --- fog ---
	void glFogf(int pname, float param);
	void glFogi(int pname, int param);
	void glFog(int pname, FloatBuffer* params);

	// --- textures ---
	int  glGenTextures();                            // -> a Textures registry id
	void glDeleteTextures(int id);
	void glBindTexture(int target, int id);
	void glTexParameteri(int target, int pname, int param);
	void glTexImage2D(int target, int level, int internalFormat, int w, int h,
	                  int border, int format, int type, IntBuffer* pixels);
	void glTexSubImage2D(int target, int level, int xoff, int yoff, int w, int h,
	                     int format, int type, IntBuffer* pixels);
	void glPixelStorei(int pname, int param);

	// --- vertex arrays (the hot path) ---
	void glEnableClientState(int cap);
	void glDisableClientState(int cap);
	// VBO/offset overloads (offset into the bound buffer, in bytes):
	void glVertexPointer(int size, int type, int stride, long long offset);
	void glColorPointer(int size, int type, int stride, long long offset);
	void glTexCoordPointer(int size, int type, int stride, long long offset);
	// NIO-buffer overloads (LWJGL): the buffer's current position is the byte offset.
	void glVertexPointer(int size, int stride, FloatBuffer* buf);
	void glColorPointer(int size, bool unsigned_, int stride, ByteBuffer* buf);
	void glTexCoordPointer(int size, int stride, FloatBuffer* buf);
	void glDrawArrays(int mode, int first, int count);

	// --- display lists (chunk geometry: record draw batches, replay on call) ---
	int  glGenLists(int n);
	void glNewList(int list, int mode);
	void glEndList();
	void glCallList(int list);
	void glDeleteLists(int list, int n);

	// --- queries / misc (sane defaults; mostly no-ops on console) ---
	int  glGetError();
	JavaString glGetString(int name);
	void glPolygonOffset(float factor, float units);
	void glLineWidth(float w);
	void glColorMaterial(int face, int mode);        // no-op
	void glLightModel(int pname, FloatBuffer* params);   // no-op
}

#endif // GL11_H
