// GLU on the GL layer (see GLU.reference.h): the matrices exactly as GLU builds them (column-major, GL's depth -1..1;
// the layer turns them into Direct3D's at draw time), multiplied into the current matrix.
#include "stdafx.h"
#include "GL11.h"
#include "GLU.h"
#include "Common.h"
#include "JavaRT.h"
#include <vector>
#include <math.h>

namespace j2xgl {

void gluPerspective(float fovy, float aspect, float zNear, float zFar) {
	const float rad = fovy * (3.14159265358979f / 360.0f);   // half the field of view
	const float s = sinf(rad), dz = zFar - zNear;
	if (dz == 0.0f || s == 0.0f || aspect == 0.0f) return;   // (GLU returns without touching the matrix)
	const float f = cosf(rad) / s;
	const float m[16] = { f / aspect, 0, 0, 0,
	                      0, f, 0, 0,
	                      0, 0, -(zFar + zNear) / dz, -1,
	                      0, 0, -2.0f * zNear * zFar / dz, 0 };
	glMultMatrixf(m);
}

void gluLookAt(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy, float uz) {
	float fx = cx - ex, fy = cy - ey, fz = cz - ez;
	float l = sqrtf(fx * fx + fy * fy + fz * fz);
	if (l == 0.0f) return;
	fx /= l; fy /= l; fz /= l;
	float sx = fy * uz - fz * uy, sy = fz * ux - fx * uz, sz = fx * uy - fy * ux;   // side = forward x up
	l = sqrtf(sx * sx + sy * sy + sz * sz);
	if (l == 0.0f) return;
	sx /= l; sy /= l; sz /= l;
	const float vx = sy * fz - sz * fy, vy = sz * fx - sx * fz, vz = sx * fy - sy * fx;   // up' = side x forward
	const float m[16] = { sx, vx, -fx, 0,
	                      sy, vy, -fy, 0,
	                      sz, vz, -fz, 0,
	                      0, 0, 0, 1 };
	glMultMatrixf(m);
	glTranslatef(-ex, -ey, -ez);
}

// GLU's: translate and scale the region (x, y) +- delta/2 of the viewport onto the whole view volume.
void gluPickMatrix(float x, float y, float deltaX, float deltaY, IntBuffer* viewport) {
	if (!viewport || deltaX <= 0.0f || deltaY <= 0.0f) return;
	const int at = viewport->position();
	const float vx = (float)viewport->get(at), vy = (float)viewport->get(at + 1);
	const float vw = (float)viewport->get(at + 2), vh = (float)viewport->get(at + 3);
	glTranslatef((vw - 2.0f * (x - vx)) / deltaX, (vh - 2.0f * (y - vy)) / deltaY, 0.0f);
	glScalef(vw / deltaX, vh / deltaY, 1.0f);
}

void gluOrtho2D(float l, float r, float b, float t) { glOrtho(l, r, b, t, -1.0, 1.0); }

// The image as RGBA bytes, then each level half the size of the one above (a 2x2 box filter; an odd edge takes what it has),
// down to 1x1, each uploaded as its level.
int gluBuild2DMipmaps(int target, int internalFormat, int w, int h, int format, int type, ByteBuffer* pixels) {
	if (!pixels || w <= 0 || h <= 0) return 0;
	glTexImage2D(target, 0, internalFormat, w, h, 0, format, type, pixels);
	if (type != GL_UNSIGNED_BYTE || (format != GL_RGBA && format != GL_RGB)) return 0;   // (deeper levels: the RGBA/RGB byte forms)
	const int bpp = format == GL_RGBA ? 4 : 3;
	const unsigned char* src = (const unsigned char*)pixels->base();
	std::vector<unsigned char> cur((size_t)w * h * 4);
	for (int i = 0; i < w * h; ++i) {
		cur[i * 4] = src[i * bpp]; cur[i * 4 + 1] = src[i * bpp + 1]; cur[i * 4 + 2] = src[i * bpp + 2];
		cur[i * 4 + 3] = bpp == 4 ? src[i * bpp + 3] : 255;
	}
	int cw = w, ch = h, level = 0;
	while (cw > 1 || ch > 1) {
		const int nw = cw > 1 ? cw / 2 : 1, nh = ch > 1 ? ch / 2 : 1;
		std::vector<unsigned char> next((size_t)nw * nh * 4);
		for (int y = 0; y < nh; ++y)
			for (int x = 0; x < nw; ++x)
				for (int c = 0; c < 4; ++c) {
					int sum = 0, n = 0;
					for (int dy = 0; dy < 2; ++dy)
						for (int dx = 0; dx < 2; ++dx) {
							const int sx = x * 2 + dx, sy = y * 2 + dy;
							if (sx < cw && sy < ch) { sum += cur[((size_t)sy * cw + sx) * 4 + c]; ++n; }
						}
					next[((size_t)y * nw + x) * 4 + c] = (unsigned char)(n ? (sum + n / 2) / n : 0);
				}
		++level;
		ByteBuffer* b = ByteBuffer::allocateDirect((int)next.size());
		memcpy(b->base(), &next[0], next.size());
		glTexImage2D(target, level, internalFormat, nw, nh, 0, GL_RGBA, GL_UNSIGNED_BYTE, b);
		delete b;
		cur.swap(next);
		cw = nw; ch = nh;
	}
	return 0;   // GLU_NO_ERROR
}

JavaString gluErrorString(int err) {
	switch (err) {
		case 0: return JavaString("no error");
		case 0x0500: return JavaString("invalid enumerant");
		case 0x0501: return JavaString("invalid value");
		case 0x0502: return JavaString("invalid operation");
		case 0x0503: return JavaString("stack overflow");
		case 0x0504: return JavaString("stack underflow");
		case 0x0505: return JavaString("out of memory");
		default: return JavaString("unknown error");
	}
}

} // namespace j2xgl
