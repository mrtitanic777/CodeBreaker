// The OpenGL 1.x GRAPHICS LAYER -- namespace `j2xgl`, also named GL11 (what the game calls) and d3d9 (what the Xbox 360's platform
// files call). One layer for every graphics API: GL11.cpp is GL's whole state machine and knows no API; a backend (Gpu.h)
// draws with one -- GpuD3D9.cpp on Direct3D 9 (the Xbox 360, and the PC with --graphics d3d9), GpuD3D11.cpp on Direct3D 11 (the
// PC with --graphics d3d11). A converted game's LWJGL OpenGL 1.x calls land here unchanged; the backends share one HLSL
// description of GL's fixed-function pipeline (GpuShaders.h), so every API draws the same picture.
//
// It is the OpenGL 1.x state machine, written down: every call the PC's OpenGL layer (pc/reference/GL11.reference.h) declares is
// declared here with the same signature, so a game that builds against one builds against the other. What GL keeps, it keeps:
//   * matrices: MODELVIEW / PROJECTION / one TEXTURE stack per texture unit, held in GL's own convention (depth -1..1, what
//     glGetFloat(GL_PROJECTION_MATRIX) hands back is what GL would) and turned into the API's (depth 0..1, pixel centres) only
//     at draw time;
//   * fixed-function lighting: GL_LIGHTING with GL_LIGHT0/1 (directional or positional), the light model ambient, GL_COLOR_MATERIAL,
//     per-vertex normals (glNormal3f, glNormalPointer as floats or bytes), flat or smooth shading -- in the vertex stage, as GL
//     defines it;
//   * fog: GL_LINEAR / GL_EXP / GL_EXP2 on the eye-space distance, the colour clamped;
//   * two texture units (GL13 / ARB_multitexture): each its own binding, enable, texture environment (MODULATE, REPLACE, ADD,
//     DECAL) and texture matrix; the second unit's coordinates from glMultiTexCoord2f or a GL_SHORT/GL_FLOAT array;
//   * the alpha test with every comparison, blending, depth test/func/mask, colour mask, face culling (GL_BACK/FRONT), polygon
//     offset, the scissor box, the clear colour/depth;
//   * IMMEDIATE mode (glBegin..glEnd, every primitive incl. lines), VERTEX ARRAYS (glDrawArrays, glDrawElements,
//     glInterleavedArrays), DISPLAY LISTS (geometry + the texture bound inside them + the net matrix change, replayed with the
//     colour current at the call when the list set none), SELECTION (glRenderMode(GL_SELECT) with a real pick-volume test);
//   * textures: GL_RGBA/RGB/BGRA/BGR/ALPHA/LUMINANCE/LUMINANCE_ALPHA in bytes or packed ints, mip levels, sub-images, wrap and
//     filter state, glCopyTexSubImage2D and glReadPixels from the frame.
// A call it has no way to honour reports itself once (J2X_UNHANDLED) instead of doing nothing silently.
//
// The platform's Display makes the device and hands it to the backend (GpuD3D9.h: d3d9::glSetDevice; GpuD3D11.h:
// j2xgl::gpu::attachD3D11), then calls glInit; after a device reset it calls glOnDeviceReset.
//
// C++03: the Xbox 360's compiler builds this file.
#ifndef GL11_H
#define GL11_H

#include "stdafx.h"
#include "JavaRT.h"
#include "NIOBuffers.h"      // ByteBuffer / ShortBuffer / IntBuffer / FloatBuffer

namespace j2xgl {
	// ------------------------------------------------------------------ enum tokens (GL 1.1, and what the layer honours beyond it)
	const int GL_FALSE = 0, GL_TRUE = 1;
	const int GL_POINTS = 0x0000, GL_LINES = 0x0001, GL_LINE_LOOP = 0x0002, GL_LINE_STRIP = 0x0003;
	const int GL_TRIANGLES = 0x0004, GL_TRIANGLE_STRIP = 0x0005, GL_TRIANGLE_FAN = 0x0006, GL_QUADS = 0x0007;
	const int GL_QUAD_STRIP = 0x0008, GL_POLYGON = 0x0009;
	const int GL_BYTE = 0x1400, GL_UNSIGNED_BYTE = 0x1401, GL_SHORT = 0x1402, GL_UNSIGNED_SHORT = 0x1403;
	const int GL_INT = 0x1404, GL_UNSIGNED_INT = 0x1405, GL_FLOAT = 0x1406, GL_DOUBLE = 0x140A;
	const int GL_VERTEX_ARRAY = 0x8074, GL_NORMAL_ARRAY = 0x8075, GL_COLOR_ARRAY = 0x8076, GL_TEXTURE_COORD_ARRAY = 0x8078;
	const int GL_MODELVIEW = 0x1700, GL_PROJECTION = 0x1701, GL_TEXTURE = 0x1702;
	const int GL_MODELVIEW_MATRIX = 0x0BA6, GL_PROJECTION_MATRIX = 0x0BA7, GL_TEXTURE_MATRIX = 0x0BA8, GL_VIEWPORT = 0x0BA2;
	const int GL_MAX_TEXTURE_SIZE = 0x0D33, GL_TEXTURE_BINDING_2D = 0x8069, GL_MATRIX_MODE = 0x0BA0;
	const int GL_DEPTH_TEST = 0x0B71, GL_BLEND = 0x0BE2, GL_ALPHA_TEST = 0x0BC0, GL_CULL_FACE = 0x0B44,
	          GL_FOG = 0x0B60, GL_LIGHTING = 0x0B50, GL_TEXTURE_2D = 0x0DE1, GL_COLOR_MATERIAL = 0x0B57,
	          GL_DITHER = 0x0BD0, GL_NORMALIZE = 0x0BA1, GL_RESCALE_NORMAL = 0x803A, GL_SCISSOR_TEST = 0x0C11,
	          GL_LINE_SMOOTH = 0x0B20, GL_POINT_SMOOTH = 0x0B10, GL_POLYGON_SMOOTH = 0x0B41, GL_STENCIL_TEST = 0x0B90;
	const int GL_DEPTH_BUFFER_BIT = 0x0100, GL_COLOR_BUFFER_BIT = 0x4000, GL_ACCUM_BUFFER_BIT = 0x0200,
	          GL_STENCIL_BUFFER_BIT = 0x0400;
	const int GL_SRC_ALPHA = 0x0302, GL_ONE_MINUS_SRC_ALPHA = 0x0303, GL_ONE = 1, GL_ZERO = 0;
	const int GL_SRC_COLOR = 0x0300, GL_ONE_MINUS_SRC_COLOR = 0x0301, GL_DST_COLOR = 0x0306,
	          GL_ONE_MINUS_DST_COLOR = 0x0307, GL_DST_ALPHA = 0x0304, GL_ONE_MINUS_DST_ALPHA = 0x0305,
	          GL_SRC_ALPHA_SATURATE = 0x0308;
	const int GL_NEVER = 0x0200, GL_LESS = 0x0201, GL_EQUAL = 0x0202, GL_LEQUAL = 0x0203, GL_GREATER = 0x0204,
	          GL_NOTEQUAL = 0x0205, GL_GEQUAL = 0x0206, GL_ALWAYS = 0x0207;
	const int GL_BACK = 0x0405, GL_FRONT = 0x0404, GL_FRONT_AND_BACK = 0x0408, GL_CW = 0x0900, GL_CCW = 0x0901;
	const int GL_LIGHT0 = 0x4000, GL_LIGHT1 = 0x4001, GL_LIGHT_MODEL_AMBIENT = 0x0B53, GL_LIGHT_MODEL_LOCAL_VIEWER = 0x0B51,
	          GL_LIGHT_MODEL_TWO_SIDE = 0x0B52;
	const int GL_AMBIENT = 0x1200, GL_DIFFUSE = 0x1201, GL_SPECULAR = 0x1202, GL_POSITION = 0x1203,
	          GL_SPOT_DIRECTION = 0x1204, GL_SPOT_EXPONENT = 0x1205, GL_SPOT_CUTOFF = 0x1206, GL_CONSTANT_ATTENUATION = 0x1207,
	          GL_LINEAR_ATTENUATION = 0x1208, GL_QUADRATIC_ATTENUATION = 0x1209,
	          GL_EMISSION = 0x1600, GL_SHININESS = 0x1601, GL_AMBIENT_AND_DIFFUSE = 0x1602;
	const int GL_FOG_START = 0x0B63, GL_FOG_END = 0x0B64, GL_FOG_MODE = 0x0B65, GL_FOG_COLOR = 0x0B66,
	          GL_FOG_DENSITY = 0x0B62, GL_FOG_INDEX = 0x0B61, GL_EXP = 0x0800, GL_EXP2 = 0x0801;   // (GL_LINEAR fog is GL_LINEAR)
	const int GL_POLYGON_OFFSET_FILL = 0x8037, GL_COMPILE = 0x1300, GL_COMPILE_AND_EXECUTE = 0x1301;
	const int GL_CLAMP = 0x2900, GL_REPEAT = 0x2901, GL_TEXTURE_WRAP_S = 0x2802, GL_TEXTURE_WRAP_T = 0x2803;
	const int GL_RGB = 0x1907, GL_RGBA = 0x1908, GL_ALPHA = 0x1906, GL_LUMINANCE = 0x1909, GL_LUMINANCE_ALPHA = 0x190A,
	          GL_RGB8 = 0x8051, GL_RGBA8 = 0x8058;
	const int GL_PACK_ALIGNMENT = 0x0D05, GL_UNPACK_ALIGNMENT = 0x0CF5;
	const int GL_VENDOR = 0x1F00, GL_RENDERER = 0x1F01, GL_VERSION = 0x1F02, GL_EXTENSIONS = 0x1F03;
	const int GL_NEAREST = 0x2600, GL_LINEAR = 0x2601;
	const int GL_NEAREST_MIPMAP_NEAREST = 0x2700, GL_LINEAR_MIPMAP_NEAREST = 0x2701,
	          GL_NEAREST_MIPMAP_LINEAR = 0x2702, GL_LINEAR_MIPMAP_LINEAR = 0x2703;
	const int GL_TEXTURE_MIN_FILTER = 0x2800, GL_TEXTURE_MAG_FILTER = 0x2801;
	const int GL_SMOOTH = 0x1D01, GL_FLAT = 0x1D00;
	const int GL_RENDER = 0x1C00, GL_FEEDBACK = 0x1C01, GL_SELECT = 0x1C02;
	const int GL_PERSPECTIVE_CORRECTION_HINT = 0x0C50, GL_FASTEST = 0x1101, GL_NICEST = 0x1102, GL_DONT_CARE = 0x1100;
	const int GL_TEXTURE_ENV = 0x2300, GL_TEXTURE_ENV_MODE = 0x2200, GL_MODULATE = 0x2100, GL_REPLACE = 0x1E01,
	          GL_DECAL = 0x2101, GL_ADD = 0x0104;
	const int GL_NO_ERROR = 0;
	// glInterleavedArrays formats (early Indev's Tessellator draws from one interleaved FloatBuffer)
	const int GL_V2F = 0x2A20, GL_V3F = 0x2A21, GL_C4UB_V2F = 0x2A22, GL_C4UB_V3F = 0x2A23, GL_C3F_V3F = 0x2A24, GL_N3F_V3F = 0x2A25,
	          GL_C4F_N3F_V3F = 0x2A26, GL_T2F_V3F = 0x2A27, GL_T4F_V4F = 0x2A28, GL_T2F_C4UB_V3F = 0x2A29, GL_T2F_C3F_V3F = 0x2A2A,
	          GL_T2F_N3F_V3F = 0x2A2B, GL_T2F_C4F_N3F_V3F = 0x2A2C, GL_T4F_C4F_N3F_V4F = 0x2A2D;
	// GL 1.2 / 1.3 tokens the layer reads in its own calls (the game spells them GL12:: / GL13:: / ARBMultitexture::)
	const int GL_BGR = 0x80E0, GL_BGRA = 0x80E1, GL_UNSIGNED_INT_8_8_8_8 = 0x8035, GL_UNSIGNED_INT_8_8_8_8_REV = 0x8367,
	          GL_CLAMP_TO_EDGE = 0x812F, GL_TEXTURE_MAX_LEVEL = 0x813D, GL_TEXTURE_BASE_LEVEL = 0x813C,
	          GL_GENERATE_MIPMAP = 0x8191;
	const int GL_TEXTURE0 = 0x84C0, GL_TEXTURE1 = 0x84C1, GL_TEXTURE2 = 0x84C2, GL_TEXTURE3 = 0x84C3;
	const int GL_ACTIVE_TEXTURE = 0x84E0, GL_CLIENT_ACTIVE_TEXTURE = 0x84E1, GL_MAX_TEXTURE_UNITS = 0x84E2;

	// ------------------------------------------------------------------ lifecycle (the platform's Display calls these)
	void glInit();                             // once, once the backend has its device: GL's initial state
	void glOnDeviceReset();                    // after a device reset: the backend's caches dropped (every draw sends all state)

	// ------------------------------------------------------------------ matrix stacks
	void glMatrixMode(int mode);
	void glPushMatrix();
	void glPopMatrix();
	void glLoadIdentity();
	void glLoadMatrix(FloatBuffer* m);
	void glMultMatrix(FloatBuffer* m);
	void glLoadMatrixf(const float* m);        // the same from a float[16] in GL's (column-major) order
	void glMultMatrixf(const float* m);
	void glTranslatef(float x, float y, float z);
	inline void glTranslated(double x, double y, double z) { glTranslatef((float)x, (float)y, (float)z); }
	void glRotatef(float angle, float x, float y, float z);
	inline void glRotated(double a, double x, double y, double z) { glRotatef((float)a, (float)x, (float)y, (float)z); }
	void glScalef(float x, float y, float z);
	inline void glScaled(double x, double y, double z) { glScalef((float)x, (float)y, (float)z); }
	void glOrtho(double l, double r, double b, double t, double n, double f);
	void glFrustum(double l, double r, double b, double t, double n, double f);
	void glViewport(int x, int y, int w, int h);
	void glScissor(int x, int y, int w, int h);
	void glGetFloat(int pname, FloatBuffer* buf);
	void glGetInteger(int pname, IntBuffer* buf);
	int  glGetInteger(int pname);

	// ------------------------------------------------------------------ immediate mode
	void glBegin(int mode);
	void glEnd();
	void glVertex2f(float x, float y);
	void glVertex3f(float x, float y, float z);
	inline void glVertex2d(double x, double y) { glVertex2f((float)x, (float)y); }
	inline void glVertex3d(double x, double y, double z) { glVertex3f((float)x, (float)y, (float)z); }
	inline void glVertex2i(int x, int y) { glVertex2f((float)x, (float)y); }
	void glTexCoord2f(float u, float v);
	inline void glTexCoord2d(double u, double v) { glTexCoord2f((float)u, (float)v); }
	void glColor3f(float r, float g, float b);
	void glColor4f(float r, float g, float b, float a);
	inline void glColor3d(double r, double g, double b) { glColor3f((float)r, (float)g, (float)b); }
	inline void glColor4d(double r, double g, double b, double a) { glColor4f((float)r, (float)g, (float)b, (float)a); }
	void glColor3ub(unsigned char r, unsigned char g, unsigned char b);
	void glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void glNormal3f(float x, float y, float z);
	inline void glNormal3d(double x, double y, double z) { glNormal3f((float)x, (float)y, (float)z); }

	// ------------------------------------------------------------------ render state
	void glEnable(int cap);
	void glDisable(int cap);
	bool glIsEnabled(int cap);
	void glBlendFunc(int sfactor, int dfactor);
	void glDepthFunc(int func);
	void glDepthMask(bool flag);
	void glColorMask(bool r, bool g, bool b, bool a);
	void glCullFace(int mode);
	void glFrontFace(int mode);
	void glAlphaFunc(int func, float ref);
	void glShadeModel(int mode);
	void glClearColor(float r, float g, float b, float a);
	void glClearDepth(double depth);
	void glClear(int mask);
	void glHint(int target, int mode);
	void glPolygonOffset(float factor, float units);
	void glLineWidth(float w);
	void glPointSize(float s);
	void glColorMaterial(int face, int mode);

	// ------------------------------------------------------------------ fog and lighting
	void glFogf(int pname, float param);
	void glFogi(int pname, int param);
	void glFog(int pname, FloatBuffer* params);
	void glLight(int light, int pname, FloatBuffer* params);
	void glLightf(int light, int pname, float param);
	void glLightModel(int pname, FloatBuffer* params);
	void glLightModelf(int pname, float param);
	void glLightModeli(int pname, int param);
	void glTexEnvi(int target, int pname, int param);
	inline void glTexEnvf(int target, int pname, float param) { glTexEnvi(target, pname, (int)param); }

	// ------------------------------------------------------------------ textures
	int  glGenTextures();
	void glGenTextures(IntBuffer* buf);
	void glDeleteTextures(int id);
	void glDeleteTextures(IntBuffer* buf);
	void glBindTexture(int target, int id);
	bool glIsTexture(int id);
	void glTexParameteri(int target, int pname, int param);
	void glTexParameterf(int target, int pname, float param);
	void glTexImage2D(int target, int level, int internalFormat, int w, int h, int border, int format, int type,
	                  ByteBuffer* pixels);
	void glTexImage2D(int target, int level, int internalFormat, int w, int h, int border, int format, int type,
	                  IntBuffer* pixels);
	void glTexSubImage2D(int target, int level, int xoff, int yoff, int w, int h, int format, int type,
	                     ByteBuffer* pixels);
	void glTexSubImage2D(int target, int level, int xoff, int yoff, int w, int h, int format, int type,
	                     IntBuffer* pixels);
	void glCopyTexSubImage2D(int target, int level, int xoff, int yoff, int x, int y, int w, int h);   // (Beta 1.8's title panorama)
	void glPixelStorei(int pname, int param);
	void glReadPixels(int x, int y, int w, int h, int format, int type, ByteBuffer* pixels);
	void glReadPixels(int x, int y, int w, int h, int format, int type, IntBuffer* pixels);

	// ------------------------------------------------------------------ multitexture (GL13 / ARB_multitexture forward here)
	void glActiveTexture(int texture);
	void glClientActiveTexture(int texture);
	void glMultiTexCoord2f(int target, float s, float t);

	// ------------------------------------------------------------------ vertex arrays (LWJGL's NIO forms: size, stride in bytes, buffer)
	void glEnableClientState(int cap);
	void glDisableClientState(int cap);
	void glVertexPointer(int size, int stride, FloatBuffer* buf);
	void glColorPointer(int size, int stride, FloatBuffer* buf);
	void glColorPointer(int size, int stride, ByteBuffer* buf);
	void glColorPointer(int size, bool unsignedBytes, int stride, ByteBuffer* buf);
	void glTexCoordPointer(int size, int stride, FloatBuffer* buf);
	void glTexCoordPointer(int size, int stride, ShortBuffer* buf);   // GL_SHORT (Beta 1.8's lightmap coordinates)
	void glNormalPointer(int stride, ByteBuffer* buf);                // GL_BYTE, normalized
	void glNormalPointer(int stride, FloatBuffer* buf);
	// The bound-buffer offset forms: the layer reports no ARB_vertex_buffer_object, so a game's VBO branch never runs; these
	// exist so it compiles, and report themselves if it ever does.
	void glVertexPointer(int size, int type, int stride, long long offset);
	void glColorPointer(int size, int type, int stride, long long offset);
	void glTexCoordPointer(int size, int type, int stride, long long offset);
	void glNormalPointer(int type, int stride, long long offset);
	void glDrawArrays(int mode, int first, int count);
	void glDrawElements(int mode, IntBuffer* indices);   // LWJGL's: the indices from position to limit, unsigned ints
	void glDrawElements(int mode, ShortBuffer* indices);
	void glDrawElements(int mode, ByteBuffer* indices);
	void glInterleavedArrays(int format, int stride, Buffer* pointer);

	// ------------------------------------------------------------------ display lists
	int  glGenLists(int n);
	void glNewList(int list, int mode);
	void glEndList();
	void glCallList(int list);
	void glCallLists(IntBuffer* lists);
	void glDeleteLists(int list, int n);
	bool glIsList(int list);

	// ------------------------------------------------------------------ selection / picking
	void glInitNames();
	void glPushName(int name);
	void glPopName();
	void glLoadName(int name);
	void glSelectBuffer(IntBuffer* buf);
	int  glRenderMode(int mode);   // leaving GL_SELECT returns the hit count written to the select buffer

	// ------------------------------------------------------------------ queries / misc
	int  glGetError();
	JavaString glGetString(int name);
	void glFlush();    // draws the pending batch and makes this frame's new display-list buffers (Display calls it per frame)
	void glFinish();
	inline void glClipPlane(...) {}   // (no game in the corpus clips; a plane would be one more shader input)
	inline void glPolygonMode(int, int) {}
}

// The game calls GL11::glX; the Xbox 360's platform files call d3d9::glX. Both are this layer.
namespace GL11 = j2xgl;
namespace d3d9 = j2xgl;
#endif // GL11_H
