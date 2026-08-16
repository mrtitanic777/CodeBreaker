#ifndef GLU_H
#define GLU_H
// PLATFORM REFERENCE header for GLU (pairs with GLU.reference.cpp). org.lwjgl.util.glu.GLU -> D3D9.
// gluPerspective is THE 3D world projection: it builds a D3D perspective matrix and loads it as the current
// GL PROJECTION (GL11::glLoadProjection). Without it the projection stays IDENTITY -> the whole 3D world is
// flattened into a thin 2D band (no perspective). gluErrorString is real (empty). The remaining GLU entry
// points are unused by Beta -> inline no-ops so the game links.
//
// This .h was MISSING, so emit_references (which keys off *.reference.h) never emitted the GLU reference and
// the game silently fell back to the no-op GLU stub -> flat world. Keep .h + .cpp paired for every reference.
#include <string>
#include "JavaString.h"
#include "NIOBuffers.h"     // IntBuffer for the gluPickMatrix viewport

namespace GLU {
	void gluPerspective(float fovy, float aspect, float zNear, float zFar);   // REAL -- GLU.reference.cpp
	// Picking: narrows the current PROJECTION to a small region around (x,y) before gluPerspective, in
	// GL_SELECT mode (RubyDung block picking). Composes onto the current matrix via GL11 translate/scale.
	void gluPickMatrix(float x, float y, float deltaX, float deltaY, IntBuffer* viewport);   // REAL
	JavaString gluErrorString(int err);                                       // REAL -- GLU.reference.cpp
	// Texture upload path: builds/uploads the 2D texture into the currently-bound GL texture. Real
	// (forwards to the GL emulator's glTexImage2D) so textures actually reach the GPU -- a no-op here
	// left every surface untextured (white). RubyDung's Textures.load calls this.
	int gluBuild2DMipmaps(int target, int internalFormat, int w, int h, int format, int type, ByteBuffer* pixels);
	inline void gluLookAt(...) {}
	inline void gluProject(...) {}
	inline void gluUnProject(...) {}
}
#endif // GLU_H
