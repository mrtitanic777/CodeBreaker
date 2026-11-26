// org.lwjgl.util.glu.GLU on the GL layer (GL11.h): the same declarations as the PC's OpenGL GLU, carried out as GLU
// defines them -- each one multiplies the current matrix (gluPickMatrix narrows a projection gluPerspective then multiplies in).
#ifndef GLU_H
#define GLU_H
#include <string>
#include "JavaString.h"
#include "NIOBuffers.h"     // IntBuffer for the gluPickMatrix viewport

namespace j2xgl {
	void gluPerspective(float fovy, float aspect, float zNear, float zFar);
	void gluLookAt(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy, float uz);
	void gluPickMatrix(float x, float y, float deltaX, float deltaY, IntBuffer* viewport);
	void gluOrtho2D(float l, float r, float b, float t);
	JavaString gluErrorString(int err);
	// The image and the whole mip chain below it (box-filtered, as GLU does) into the bound texture.
	int gluBuild2DMipmaps(int target, int internalFormat, int w, int h, int format, int type, ByteBuffer* pixels);
	inline void gluProject(...) {}
	inline void gluUnProject(...) {}
}
namespace GLU = j2xgl;
#endif // GLU_H
