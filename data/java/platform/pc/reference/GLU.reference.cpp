// GLU on the system's GLU (see GLU.reference.h).
#include "stdafx.h"
#include "GLU.h"
#include <GL/gl.h>
#include <GL/glu.h>

namespace GLU {

void gluPerspective(float fovy, float aspect, float zNear, float zFar) { ::gluPerspective(fovy, aspect, zNear, zFar); }

void gluLookAt(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy, float uz) {
	::gluLookAt(ex, ey, ez, cx, cy, cz, ux, uy, uz);
}

void gluPickMatrix(float x, float y, float dx, float dy, IntBuffer* viewport) {
	::gluPickMatrix(x, y, dx, dy, viewport ? (GLint*)viewport->base() : 0);
}

void gluOrtho2D(float l, float r, float b, float t) { ::gluOrtho2D(l, r, b, t); }

JavaString gluErrorString(int err) {
	const GLubyte* s = ::gluErrorString((GLenum)err);
	return JavaString(s ? (const char*)s : "");
}

int gluBuild2DMipmaps(int target, int ifmt, int w, int h, int fmt, int type, ByteBuffer* px) {
	return ::gluBuild2DMipmaps(target, ifmt, w, h, fmt, type, px ? (const void*)px->base() : 0);
}

} // namespace GLU
