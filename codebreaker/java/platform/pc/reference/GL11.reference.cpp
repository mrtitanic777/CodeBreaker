// GL11 on real OpenGL (see GL11.reference.h). The game's constants are GL's own values, so each int is
// passed straight through.
#include "stdafx.h"
#include "GL11.h"
#include <GL/gl.h>

namespace {
// the bytes of an NIO buffer from its position (LWJGL passes buffer contents from position() on)
inline void* at(Buffer* b) { return b ? (void*)b->base() : 0; }          // base() includes the position
inline int remainingInts(IntBuffer* b) { return b ? b->remaining() : 0; } // elements, java.nio's way
}

namespace GL11 {

void glMatrixMode(int mode) { ::glMatrixMode(mode); }
void glPushMatrix() { ::glPushMatrix(); }
void glPopMatrix() { ::glPopMatrix(); }
void glLoadIdentity() { ::glLoadIdentity(); }
void glLoadMatrix(FloatBuffer* m) { ::glLoadMatrixf((const GLfloat*)at(m)); }
void glMultMatrix(FloatBuffer* m) { ::glMultMatrixf((const GLfloat*)at(m)); }
void glTranslatef(float x, float y, float z) { ::glTranslatef(x, y, z); }
void glTranslated(double x, double y, double z) { ::glTranslated(x, y, z); }
void glRotatef(float a, float x, float y, float z) { ::glRotatef(a, x, y, z); }
void glScalef(float x, float y, float z) { ::glScalef(x, y, z); }
void glScaled(double x, double y, double z) { ::glScaled(x, y, z); }
void glOrtho(double l, double r, double b, double t, double n, double f) { ::glOrtho(l, r, b, t, n, f); }
void glFrustum(double l, double r, double b, double t, double n, double f) { ::glFrustum(l, r, b, t, n, f); }
void glViewport(int x, int y, int w, int h) { ::glViewport(x, y, w, h); }
void glGetFloat(int pname, FloatBuffer* buf) { ::glGetFloatv(pname, (GLfloat*)at(buf)); }
void glGetInteger(int pname, IntBuffer* buf) { ::glGetIntegerv(pname, (GLint*)at(buf)); }
int glGetInteger(int pname) { GLint v = 0; ::glGetIntegerv(pname, &v); return v; }

void glBegin(int mode) { ::glBegin(mode); }
void glEnd() { ::glEnd(); }
void glVertex2f(float x, float y) { ::glVertex2f(x, y); }
void glVertex3f(float x, float y, float z) { ::glVertex3f(x, y, z); }
void glVertex3d(double x, double y, double z) { ::glVertex3d(x, y, z); }
void glTexCoord2f(float u, float v) { ::glTexCoord2f(u, v); }
void glTexCoord2d(double u, double v) { ::glTexCoord2d(u, v); }
void glColor3f(float r, float g, float b) { ::glColor3f(r, g, b); }
void glColor4f(float r, float g, float b, float a) { ::glColor4f(r, g, b, a); }
void glColor3ub(unsigned char r, unsigned char g, unsigned char b) { ::glColor3ub(r, g, b); }
void glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { ::glColor4ub(r, g, b, a); }
void glNormal3f(float x, float y, float z) { ::glNormal3f(x, y, z); }

void glEnable(int cap) { ::glEnable(cap); }
void glDisable(int cap) { ::glDisable(cap); }
bool glIsEnabled(int cap) { return ::glIsEnabled(cap) != 0; }
void glBlendFunc(int s, int d) { ::glBlendFunc(s, d); }
void glDepthFunc(int f) { ::glDepthFunc(f); }
void glDepthMask(bool flag) { ::glDepthMask(flag ? GL_TRUE : GL_FALSE); }
void glColorMask(bool r, bool g, bool b, bool a) { ::glColorMask(r, g, b, a); }
void glCullFace(int mode) { ::glCullFace(mode); }
void glAlphaFunc(int f, float ref) { ::glAlphaFunc(f, ref); }
void glShadeModel(int mode) { ::glShadeModel(mode); }
void glClearColor(float r, float g, float b, float a) { ::glClearColor(r, g, b, a); }
void glClearDepth(double d) { ::glClearDepth(d); }
void glClear(int mask) { ::glClear(mask); }
void glHint(int target, int mode) { ::glHint(target, mode); }
void glPolygonOffset(float f, float u) { ::glPolygonOffset(f, u); }
void glLineWidth(float w) { ::glLineWidth(w); }
void glColorMaterial(int face, int mode) { ::glColorMaterial(face, mode); }
void glFogf(int p, float v) { ::glFogf(p, v); }
void glFogi(int p, int v) { ::glFogi(p, v); }
void glFog(int p, FloatBuffer* v) { ::glFogfv(p, (const GLfloat*)at(v)); }
void glLight(int light, int p, FloatBuffer* v) { ::glLightfv(light, p, (const GLfloat*)at(v)); }
void glLightf(int light, int p, float v) { ::glLightf(light, p, v); }
void glLightModel(int p, FloatBuffer* v) { ::glLightModelfv(p, (const GLfloat*)at(v)); }
void glTexEnvi(int t, int p, int v) { ::glTexEnvi(t, p, v); }

int glGenTextures() { GLuint id = 0; ::glGenTextures(1, &id); return (int)id; }
void glGenTextures(IntBuffer* buf) { ::glGenTextures(remainingInts(buf), (GLuint*)at(buf)); }
void glDeleteTextures(int id) { GLuint i = (GLuint)id; ::glDeleteTextures(1, &i); }
void glDeleteTextures(IntBuffer* buf) { ::glDeleteTextures(remainingInts(buf), (const GLuint*)at(buf)); }
void glBindTexture(int target, int id) { ::glBindTexture(target, (GLuint)id); }
void glTexParameteri(int t, int p, int v) { ::glTexParameteri(t, p, v); }
void glTexParameterf(int t, int p, float v) { ::glTexParameterf(t, p, v); }
void glTexImage2D(int t, int l, int ifmt, int w, int h, int b, int fmt, int type, ByteBuffer* px) {
	::glTexImage2D(t, l, ifmt, w, h, b, fmt, type, at(px));
}
void glTexImage2D(int t, int l, int ifmt, int w, int h, int b, int fmt, int type, IntBuffer* px) {
	::glTexImage2D(t, l, ifmt, w, h, b, fmt, type, at(px));
}
void glTexSubImage2D(int t, int l, int x, int y, int w, int h, int fmt, int type, ByteBuffer* px) {
	::glTexSubImage2D(t, l, x, y, w, h, fmt, type, at(px));
}
void glTexSubImage2D(int t, int l, int x, int y, int w, int h, int fmt, int type, IntBuffer* px) {
	::glTexSubImage2D(t, l, x, y, w, h, fmt, type, at(px));
}
void glPixelStorei(int p, int v) { ::glPixelStorei(p, v); }
void glReadPixels(int x, int y, int w, int h, int fmt, int type, ByteBuffer* px) {
	::glReadPixels(x, y, w, h, fmt, type, at(px));
}

void glEnableClientState(int cap) { ::glEnableClientState(cap); }
void glDisableClientState(int cap) { ::glDisableClientState(cap); }
void glVertexPointer(int size, int stride, FloatBuffer* b) { ::glVertexPointer(size, GL_FLOAT, stride, at(b)); }
void glColorPointer(int size, int stride, FloatBuffer* b) { ::glColorPointer(size, GL_FLOAT, stride, at(b)); }
void glColorPointer(int size, int stride, ByteBuffer* b) { ::glColorPointer(size, GL_UNSIGNED_BYTE, stride, at(b)); }
void glColorPointer(int size, bool unsignedBytes, int stride, ByteBuffer* b) {
	::glColorPointer(size, unsignedBytes ? GL_UNSIGNED_BYTE : GL_BYTE, stride, at(b));
}
void glTexCoordPointer(int size, int stride, FloatBuffer* b) { ::glTexCoordPointer(size, GL_FLOAT, stride, at(b)); }
void glNormalPointer(int stride, ByteBuffer* b) { ::glNormalPointer(GL_BYTE, stride, at(b)); }
void glNormalPointer(int stride, FloatBuffer* b) { ::glNormalPointer(GL_FLOAT, stride, at(b)); }
void glVertexPointer(int size, int type, int stride, long long off) { ::glVertexPointer(size, type, stride, (const void*)(size_t)off); }
void glColorPointer(int size, int type, int stride, long long off) { ::glColorPointer(size, type, stride, (const void*)(size_t)off); }
void glTexCoordPointer(int size, int type, int stride, long long off) { ::glTexCoordPointer(size, type, stride, (const void*)(size_t)off); }
void glNormalPointer(int type, int stride, long long off) { ::glNormalPointer(type, stride, (const void*)(size_t)off); }
void glDrawArrays(int mode, int first, int count) { ::glDrawArrays(mode, first, count); }

int glGenLists(int n) { return (int)::glGenLists(n); }
void glNewList(int list, int mode) { ::glNewList((GLuint)list, mode); }
void glEndList() { ::glEndList(); }
void glCallList(int list) { ::glCallList((GLuint)list); }
void glCallLists(IntBuffer* lists) { ::glCallLists(remainingInts(lists), GL_INT, at(lists)); }
void glDeleteLists(int list, int n) { ::glDeleteLists((GLuint)list, n); }

void glInitNames() { ::glInitNames(); }
void glPushName(int n) { ::glPushName((GLuint)n); }
void glPopName() { ::glPopName(); }
void glLoadName(int n) { ::glLoadName((GLuint)n); }
void glSelectBuffer(IntBuffer* buf) { ::glSelectBuffer(remainingInts(buf), (GLuint*)at(buf)); }
int glRenderMode(int mode) { return ::glRenderMode(mode); }

int glGetError() { return (int)::glGetError(); }
JavaString glGetString(int name) {
	const GLubyte* s = ::glGetString(name);
	return JavaString(s ? (const char*)s : "");
}
void glFlush() { ::glFlush(); }
void glFinish() { ::glFinish(); }

} // namespace GL11
