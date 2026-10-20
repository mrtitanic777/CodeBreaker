// LWJGL org.lwjgl.opengl.GL11 on the PC: the real OpenGL 1.1 the game was written for. Each call is
// the native one; LWJGL's NIO-buffer forms pass the buffer's bytes from its position. This header does
// NOT include <GL/gl.h> (its GL_* macros would rewrite these constants) -- GL11.cpp does.
#ifndef GL11_H
#define GL11_H
#include "stdafx.h"
#include "JavaRT.h"
#include "NIOBuffers.h"

namespace GL11 {
	const int GL_FALSE = 0, GL_TRUE = 1;
	const int GL_POINTS = 0x0000, GL_LINES = 0x0001, GL_LINE_LOOP = 0x0002, GL_LINE_STRIP = 0x0003;
	const int GL_TRIANGLES = 0x0004, GL_TRIANGLE_STRIP = 0x0005, GL_TRIANGLE_FAN = 0x0006, GL_QUADS = 0x0007;
	const int GL_QUAD_STRIP = 0x0008, GL_POLYGON = 0x0009;
	const int GL_BYTE = 0x1400, GL_UNSIGNED_BYTE = 0x1401, GL_SHORT = 0x1402, GL_UNSIGNED_SHORT = 0x1403;
	const int GL_INT = 0x1404, GL_UNSIGNED_INT = 0x1405, GL_FLOAT = 0x1406;
	const int GL_VERTEX_ARRAY = 0x8074, GL_NORMAL_ARRAY = 0x8075, GL_COLOR_ARRAY = 0x8076,
	          GL_TEXTURE_COORD_ARRAY = 0x8078;
	const int GL_MODELVIEW = 0x1700, GL_PROJECTION = 0x1701, GL_TEXTURE = 0x1702;
	const int GL_MODELVIEW_MATRIX = 0x0BA6, GL_PROJECTION_MATRIX = 0x0BA7, GL_VIEWPORT = 0x0BA2;
	const int GL_DEPTH_TEST = 0x0B71, GL_BLEND = 0x0BE2, GL_ALPHA_TEST = 0x0BC0, GL_CULL_FACE = 0x0B44,
	          GL_FOG = 0x0B60, GL_LIGHTING = 0x0B50, GL_TEXTURE_2D = 0x0DE1, GL_COLOR_MATERIAL = 0x0B57,
	          GL_DITHER = 0x0BD0, GL_NORMALIZE = 0x0BA1, GL_RESCALE_NORMAL = 0x803A;
	const int GL_DEPTH_BUFFER_BIT = 0x0100, GL_COLOR_BUFFER_BIT = 0x4000, GL_ACCUM_BUFFER_BIT = 0x0200,
	          GL_STENCIL_BUFFER_BIT = 0x0400;
	const int GL_SRC_ALPHA = 0x0302, GL_ONE_MINUS_SRC_ALPHA = 0x0303, GL_ONE = 1, GL_ZERO = 0;
	const int GL_SRC_COLOR = 0x0300, GL_ONE_MINUS_SRC_COLOR = 0x0301, GL_DST_COLOR = 0x0306,
	          GL_ONE_MINUS_DST_COLOR = 0x0307, GL_DST_ALPHA = 0x0304, GL_ONE_MINUS_DST_ALPHA = 0x0305;
	const int GL_NEVER = 0x0200, GL_LESS = 0x0201, GL_EQUAL = 0x0202, GL_LEQUAL = 0x0203, GL_GREATER = 0x0204,
	          GL_NOTEQUAL = 0x0205, GL_GEQUAL = 0x0206, GL_ALWAYS = 0x0207;
	const int GL_BACK = 0x0405, GL_FRONT = 0x0404, GL_FRONT_AND_BACK = 0x0408;
	const int GL_LIGHT0 = 0x4000, GL_LIGHT1 = 0x4001, GL_LIGHT_MODEL_AMBIENT = 0x0B53;
	const int GL_AMBIENT = 0x1200, GL_DIFFUSE = 0x1201, GL_SPECULAR = 0x1202, GL_POSITION = 0x1203,
	          GL_AMBIENT_AND_DIFFUSE = 0x1602;
	const int GL_FOG_START = 0x0B63, GL_FOG_END = 0x0B64, GL_FOG_MODE = 0x0B65, GL_FOG_COLOR = 0x0B66,
	          GL_FOG_DENSITY = 0x0B62, GL_EXP = 0x0800, GL_EXP2 = 0x0801;
	const int GL_POLYGON_OFFSET_FILL = 0x8037, GL_COMPILE = 0x1300, GL_COMPILE_AND_EXECUTE = 0x1301;
	const int GL_CLAMP = 0x2900, GL_REPEAT = 0x2901, GL_TEXTURE_WRAP_S = 0x2802, GL_TEXTURE_WRAP_T = 0x2803;
	const int GL_RGB = 0x1907, GL_RGBA = 0x1908, GL_ALPHA = 0x1906, GL_LUMINANCE = 0x1909;
	const int GL_PACK_ALIGNMENT = 0x0D05, GL_UNPACK_ALIGNMENT = 0x0CF5;
	const int GL_VENDOR = 0x1F00, GL_RENDERER = 0x1F01, GL_VERSION = 0x1F02, GL_EXTENSIONS = 0x1F03;
	const int GL_NEAREST = 0x2600, GL_LINEAR = 0x2601;
	const int GL_NEAREST_MIPMAP_NEAREST = 0x2700, GL_LINEAR_MIPMAP_NEAREST = 0x2701,
	          GL_NEAREST_MIPMAP_LINEAR = 0x2702, GL_LINEAR_MIPMAP_LINEAR = 0x2703;
	const int GL_TEXTURE_MIN_FILTER = 0x2800, GL_TEXTURE_MAG_FILTER = 0x2801;
	const int GL_SMOOTH = 0x1D01, GL_FLAT = 0x1D00;
	const int GL_RENDER = 0x1C00, GL_FEEDBACK = 0x1C01, GL_SELECT = 0x1C02;
	const int GL_PERSPECTIVE_CORRECTION_HINT = 0x0C50, GL_FASTEST = 0x1101, GL_NICEST = 0x1102,
	          GL_DONT_CARE = 0x1100;
	const int GL_TEXTURE_ENV = 0x2300, GL_TEXTURE_ENV_MODE = 0x2200, GL_MODULATE = 0x2100, GL_REPLACE = 0x1E01;

	// matrix stack
	void glMatrixMode(int mode);
	void glPushMatrix();
	void glPopMatrix();
	void glLoadIdentity();
	void glLoadMatrix(FloatBuffer* m);
	void glMultMatrix(FloatBuffer* m);
	void glTranslatef(float x, float y, float z);
	void glTranslated(double x, double y, double z);
	void glRotatef(float angle, float x, float y, float z);
	void glScalef(float x, float y, float z);
	void glScaled(double x, double y, double z);
	void glOrtho(double l, double r, double b, double t, double n, double f);
	void glFrustum(double l, double r, double b, double t, double n, double f);
	void glViewport(int x, int y, int w, int h);
	void glGetFloat(int pname, FloatBuffer* buf);
	void glGetInteger(int pname, IntBuffer* buf);
	int  glGetInteger(int pname);
	// immediate mode
	void glBegin(int mode);
	void glEnd();
	void glVertex2f(float x, float y);
	void glVertex3f(float x, float y, float z);
	void glVertex3d(double x, double y, double z);
	void glTexCoord2f(float u, float v);
	void glTexCoord2d(double u, double v);
	void glColor3f(float r, float g, float b);
	void glColor4f(float r, float g, float b, float a);
	void glColor3ub(unsigned char r, unsigned char g, unsigned char b);
	void glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void glNormal3f(float x, float y, float z);
	// state
	void glEnable(int cap);
	void glDisable(int cap);
	bool glIsEnabled(int cap);
	void glBlendFunc(int sfactor, int dfactor);
	void glDepthFunc(int func);
	void glDepthMask(bool flag);
	void glColorMask(bool r, bool g, bool b, bool a);
	void glCullFace(int mode);
	void glAlphaFunc(int func, float ref);
	void glShadeModel(int mode);
	void glClearColor(float r, float g, float b, float a);
	void glClearDepth(double depth);
	void glClear(int mask);
	void glHint(int target, int mode);
	void glPolygonOffset(float factor, float units);
	void glLineWidth(float w);
	void glColorMaterial(int face, int mode);
	void glFogf(int pname, float param);
	void glFogi(int pname, int param);
	void glFog(int pname, FloatBuffer* params);
	void glLight(int light, int pname, FloatBuffer* params);
	void glLightf(int light, int pname, float param);
	void glLightModel(int pname, FloatBuffer* params);
	void glTexEnvi(int target, int pname, int param);
	// textures
	int  glGenTextures();
	void glGenTextures(IntBuffer* buf);
	void glDeleteTextures(int id);
	void glDeleteTextures(IntBuffer* buf);
	void glBindTexture(int target, int id);
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
	void glPixelStorei(int pname, int param);
	void glReadPixels(int x, int y, int w, int h, int format, int type, ByteBuffer* pixels);
	// vertex arrays (LWJGL's NIO forms: size, stride in bytes, buffer)
	void glEnableClientState(int cap);
	void glDisableClientState(int cap);
	void glVertexPointer(int size, int stride, FloatBuffer* buf);
	void glColorPointer(int size, int stride, FloatBuffer* buf);
	void glColorPointer(int size, int stride, ByteBuffer* buf);
	void glColorPointer(int size, bool unsignedBytes, int stride, ByteBuffer* buf);
	void glTexCoordPointer(int size, int stride, FloatBuffer* buf);
	void glNormalPointer(int stride, ByteBuffer* buf);
	void glNormalPointer(int stride, FloatBuffer* buf);
	void glVertexPointer(int size, int type, int stride, long long offset);       // bound-buffer offset forms
	void glColorPointer(int size, int type, int stride, long long offset);
	void glTexCoordPointer(int size, int type, int stride, long long offset);
	void glNormalPointer(int type, int stride, long long offset);
	void glDrawArrays(int mode, int first, int count);
	// display lists
	int  glGenLists(int n);
	void glNewList(int list, int mode);
	void glEndList();
	void glCallList(int list);
	void glCallLists(IntBuffer* lists);
	void glDeleteLists(int list, int n);
	// selection
	void glInitNames();
	void glPushName(int name);
	void glPopName();
	void glLoadName(int name);
	void glSelectBuffer(IntBuffer* buf);
	int  glRenderMode(int mode);
	// queries
	int  glGetError();
	JavaString glGetString(int name);
	void glFlush();
	void glFinish();
	inline void glClipPlane(...) {}
}
#endif // GL11_H
