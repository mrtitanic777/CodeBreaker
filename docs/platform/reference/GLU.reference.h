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

namespace GLU {
	void gluPerspective(float fovy, float aspect, float zNear, float zFar);   // REAL -- GLU.reference.cpp
	JavaString gluErrorString(int err);                                       // REAL -- GLU.reference.cpp
	inline void gluLookAt(...) {}
	inline void gluProject(...) {}
	inline void gluUnProject(...) {}
	inline int gluBuild2DMipmaps(...) { return 0; }
}
#endif // GLU_H
