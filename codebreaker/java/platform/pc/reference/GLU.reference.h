// LWJGL org.lwjgl.util.glu.GLU on the PC: the system's GLU (glu32 / libGLU).
#ifndef GLU_H
#define GLU_H
#include "JavaString.h"
#include "NIOBuffers.h"

namespace GLU {
	void gluPerspective(float fovy, float aspect, float zNear, float zFar);
	void gluLookAt(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy, float uz);
	void gluPickMatrix(float x, float y, float deltaX, float deltaY, IntBuffer* viewport);
	void gluOrtho2D(float l, float r, float b, float t);
	JavaString gluErrorString(int err);
	int gluBuild2DMipmaps(int target, int internalFormat, int w, int h, int format, int type, ByteBuffer* pixels);
	inline void gluProject(...) {}
	inline void gluUnProject(...) {}
}
#endif // GLU_H
