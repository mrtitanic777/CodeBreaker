// GLU.cpp — org.lwjgl.util.glu.GLU -> D3D9, for the Xbox 360 j2x port.  [Step 2]
// REFERENCE (Xbox-side; not built by j2x). The game uses only gluPerspective (3D world projection)
// and gluErrorString. GL12 is just the GL_RESCALE_NORMAL enum token (glEnable no-op). Pair with
// GL11.reference.{h,cpp}. Provide the matching GLU.h (namespace GLU { void gluPerspective(float,
// float,float,float); JavaString gluErrorString(int); }).

#include "GL11.h"
#include "Shaders.h"
#include "JavaRT.h"

namespace GLU {

// gluPerspective(fovy_degrees, aspect, zNear, zFar): build the 3D projection and load it as the
// current GL PROJECTION matrix (the game calls glMatrixMode(GL_PROJECTION); glLoadIdentity() first).
void gluPerspective(float fovy, float aspect, float zNear, float zFar) {
	D3DXMATRIX m;
	D3DXMatrixPerspectiveFovLH(&m, D3DXToRadian(fovy), aspect, zNear, zFar);
	// HARDWARE-VERIFY: GL is right-handed, D3D left-handed. The old port mirrored X (negate m._11)
	// and flipped cull winding to compensate. The GL11 emulator already uses reversed triangle
	// winding (CW cull) -- if the world renders mirrored, negate m._11 here (and re-check cull).
	GL11::glLoadProjection(m);
}

JavaString gluErrorString(int) { return JavaString(""); }

} // namespace GLU
