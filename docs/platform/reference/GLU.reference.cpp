#include "stdafx.h"
// GLU.cpp — org.lwjgl.util.glu.GLU -> D3D9, for the Xbox 360 j2ctool port.  [Step 2]
// REFERENCE (Xbox-side; not built by j2ctool). The game uses only gluPerspective (3D world projection)
// and gluErrorString. GL12 is just the GL_RESCALE_NORMAL enum token (glEnable no-op). Pair with
// GL11.reference.{h,cpp}. Provide the matching GLU.h (namespace GLU { void gluPerspective(float,
// float,float,float); JavaString gluErrorString(int); }).

#include "GL11.h"
#include "JavaRT.h"

namespace GLU {

// gluPerspective(fovy_degrees, aspect, zNear, zFar): build the 3D projection and load it as the
// current GL PROJECTION matrix (the game calls glMatrixMode(GL_PROJECTION); glLoadIdentity() first).
void gluPerspective(float fovy, float aspect, float zNear, float zFar) {
	D3DXMATRIX m;
	// RIGHT-handed to match GL (and our RH glOrtho). GL geometry is at NEGATIVE eye-z (the menu logo
	// sits at z=-13; the world is in front of the camera at -Z). An LH projection puts all of it
	// BEHIND the camera -> depth-clipped -> invisible. RH places it correctly in the frustum.
	D3DXMatrixPerspectiveFovRH(&m, D3DXToRadian(fovy), aspect, zNear, zFar);
	// HARDWARE-VERIFY: if the 3D world later renders mirrored/inside-out, the fix is cull winding
	// (GL11 reversed-winding / D3DCULL), NOT switching back to LH.
	GL11::glLoadProjection(m);
}

// gluPickMatrix: standard GL pick transform. Composes translate+scale onto the CURRENT matrix (the
// caller has glMatrixMode(GL_PROJECTION); glLoadIdentity() active) so the subsequent gluPerspective
// only "sees" a tiny region around (x,y) -- in GL_SELECT that's what limits which names get hit.
// viewport = {x, y, width, height} (LWJGL fills it via glGetInteger(GL_VIEWPORT)).
void gluPickMatrix(float x, float y, float deltaX, float deltaY, IntBuffer* viewport) {
	if (!viewport || deltaX <= 0.0f || deltaY <= 0.0f) return;
	float vx = (float)viewport->get(0), vy = (float)viewport->get(1);
	float vw = (float)viewport->get(2), vh = (float)viewport->get(3);
	GL11::glTranslatef((vw - 2.0f * (x - vx)) / deltaX, (vh - 2.0f * (y - vy)) / deltaY, 0.0f);
	GL11::glScalef(vw / deltaX, vh / deltaY, 1.0f);
}

JavaString gluErrorString(int) { return JavaString(""); }

} // namespace GLU
