// GpuFixed.h -- GL's pipeline on a fixed-function Direct3D: the part the Direct3D 7 and Direct3D 8 backends (GpuD3D7.cpp,
// GpuD3D8.cpp) share. Those APIs have no shaders the GL layer can use (Direct3D 7 none, Direct3D 8 only shader model 1), so
// the work GpuShaders.h does in HLSL is set up here as fixed-function state: transforms, lights, material, fog, the alpha test,
// texture stages and texture-coordinate transforms. A backend reads a Plan and makes its own API's calls.
//
// What the layer computed (gpu::Constants) is read back into fixed-function terms:
//   - Direct3D lights and fogs in camera space, so the modelview goes in WORLD with VIEW the identity, and PROJECTION is what is
//     left of the layer's model-view-projection: inverse(MV) x WVP. The camera space is GL's eye space mirrored in z (S), as
//     Direct3D's fog distance is the camera-space z, positive ahead, where GL's eye looks down -z: WORLD = MV x S,
//     PROJECTION = S x inverse(MV) x WVP, the product unchanged.
//   - GL's viewport and scissor box become one Direct3D viewport inside the target (neither API has a scissor test); the
//     projection is remapped so every pixel lands where GL puts it. glPolygonOffset becomes a depth offset in the projection
//     (neither API's depth bias is GL's).
//   - The colour at glCallList (a colourless display list's colour) is a texture factor stage before the texture units.
//   - Texture coordinate generation (Beta 1.9's end portal) is the camera-space position through a texture transform.
//
// C++03, like the rest of the GL layer.
#ifndef GPU_FIXED_H
#define GPU_FIXED_H

#include "Gpu.h"
#include <string.h>
#include <math.h>

namespace j2xgl {
namespace gpu {
namespace fixed {

// A 4x4 matrix the way Direct3D's fixed function takes one (D3DMATRIX): rows, with row vectors (v' = v x M).
struct Mat { float m[4][4]; };

inline Mat identity() {
	Mat r;
	memset(&r, 0, sizeof(r));
	r.m[0][0] = r.m[1][1] = r.m[2][2] = r.m[3][3] = 1.0f;
	return r;
}
inline bool isIdentity(const Mat& a) {
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (a.m[i][j] != (i == j ? 1.0f : 0.0f)) return false;
	return true;
}
// a, then b (v x a x b).
inline Mat mul(const Mat& a, const Mat& b) {
	Mat r;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			double s = 0;
			for (int k = 0; k < 4; ++k) s += (double)a.m[i][k] * b.m[k][j];
			r.m[i][j] = (float)s;
		}
	return r;
}
// The inverse, in doubles; false when the matrix has none (a glScalef(0) model, say).
inline bool invert(const Mat& in, Mat& out) {
	double a[4][8];
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 8; ++j) a[i][j] = j < 4 ? in.m[i][j] : (j - 4 == i ? 1.0 : 0.0);
	for (int c = 0; c < 4; ++c) {
		int p = c;
		for (int r = c + 1; r < 4; ++r)
			if (fabs(a[r][c]) > fabs(a[p][c])) p = r;
		if (fabs(a[p][c]) < 1e-12) return false;
		if (p != c)
			for (int j = 0; j < 8; ++j) { const double t = a[c][j]; a[c][j] = a[p][j]; a[p][j] = t; }
		const double d = a[c][c];
		for (int j = 0; j < 8; ++j) a[c][j] /= d;
		for (int r = 0; r < 4; ++r) {
			if (r == c || a[r][c] == 0) continue;
			const double f = a[r][c];
			for (int j = 0; j < 8; ++j) a[r][j] -= f * a[c][j];
		}
	}
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) out.m[i][j] = (float)a[i][j + 4];
	return true;
}
// A float4x4 of the vertex stage's constants (GpuShaders.h: four registers, column-major), as a row-vector matrix.
inline Mat fromRegisters(const float* vs, int reg) {
	Mat r;
	for (int row = 0; row < 4; ++row)
		for (int col = 0; col < 4; ++col) r.m[row][col] = vs[(reg + col) * 4 + row];
	return r;
}

// One vertex as the fixed-function pipeline reads it: position, normal, colour, two texture coordinates (44 bytes). The layer's
// flat-shading copies have no place in it: a flat draw carries the provoking normal and colour in the ordinary ones (every
// vertex of a primitive has the same, so the smooth interpolation Direct3D does is GL's flat shading).
struct FVertex {
	float x, y, z;
	float nx, ny, nz;
	unsigned int color;   // ARGB, Direct3D's D3DCOLOR
	float u, v, u2, v2;
};
const unsigned long kFVF = 0x002 | 0x010 | 0x040 | 0x200;   // D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2 (the same in 7 and 8)

inline void toFixed(const Vertex* v, int n, FVertex* out, bool flat) {
	for (int i = 0; i < n; ++i) {
		FVertex& o = out[i];
		o.x = v[i].x; o.y = v[i].y; o.z = v[i].z;
		if (flat) { o.nx = v[i].fnx; o.ny = v[i].fny; o.nz = v[i].fnz; o.color = v[i].fcolor; }
		else      { o.nx = v[i].nx;  o.ny = v[i].ny;  o.nz = v[i].nz;  o.color = v[i].color; }
		o.u = v[i].u; o.v = v[i].v; o.u2 = v[i].u2; o.v2 = v[i].v2;
	}
}

// A texture stage, in the values Direct3D 7 and 8 share (D3DTOP_*, D3DTA_*).
enum { kTopDisable = 1, kTopSelectArg1 = 2, kTopSelectArg2 = 3, kTopModulate = 4, kTopAdd = 7, kTopBlendTextureAlpha = 13 };
enum { kTaDiffuse = 0, kTaCurrent = 1, kTaTexture = 2, kTaFactor = 3 };
enum { kCoordUnit = 0, kCoordEyePosition = 1 };   // where a stage's texture coordinate comes from
struct Stage {
	int colorOp, colorArg1, colorArg2;
	int alphaOp, alphaArg1, alphaArg2;
	int unit;          // the texture unit whose texture and sampler it reads; -1 none
	int coord;         // kCoordUnit (the vertex's coordinate for `unit`) or kCoordEyePosition (generated)
	bool transform;    // `matrix` applies, with four components out and the divide by the fourth
	Mat matrix;
};

struct Light {
	bool on;
	bool directional;
	float pos[3];      // camera space (a point light)
	float dir[3];      // the way the light travels (a directional one)
	float diffuse[4], ambient[4];
};

// Everything a draw sets, decided once for both backends.
struct Plan {
	bool skip;                       // nothing of it can land on the target (viewport and scissor do not meet)
	Mat world, view, proj;
	int vpX, vpY, vpW, vpH;          // the device's viewport, inside the target
	bool lighting, normalize;
	bool ambientFromColor, diffuseFromColor;
	float ambient[4];                // the model ambient
	float matAmbient[4], matDiffuse[4];
	Light light[2];
	bool flat;
	bool tinted;                     // the factor stage is on, with `tint`
	unsigned int tint;
	int stages;
	Stage stage[3];
	bool alphaTest;
	int alphaFunc;                   // gpu::Compare
	int alphaRef;                    // 0..255, rounded so the 8-bit comparison passes the alphas GL's does
	bool fog;
	int fogMode;                     // 0 LINEAR, 1 EXP, 2 EXP2
	float fogStart, fogEnd, fogDensity;
	unsigned int fogColor;           // ARGB
	bool partialGen;                 // texture coordinate generation on some of s, t but not both (approximated: 0)
};

inline unsigned int toArgb(const float* c) {
	unsigned int r = 0;
	for (int i = 0; i < 4; ++i) {
		float f = c[i] < 0 ? 0 : c[i] > 1 ? 1 : c[i];
		const unsigned int b = (unsigned int)(f * 255.0f + 0.5f);
		r |= b << (i == 3 ? 24 : 16 - 8 * i);   // r g b a -> ARGB
	}
	return r;
}

// The alpha test's reference in 0..255 for comparing 8-bit alphas k/255 the way GL compares them with the float `ref`.
inline int alphaReference(int func, float ref) {
	if (ref < 0) ref = 0;
	if (ref > 1) ref = 1;
	const float x = ref * 255.0f;
	const int lo = (int)floor(x), hi = (int)ceil(x);
	switch (func) {
		case kGreater: case kLessEqual: return lo;    // k > x  <=>  k > floor(x);  k <= x  <=>  k <= floor(x)
		case kGreaterEqual: case kLess: return hi;    // k >= x <=>  k >= ceil(x);  k < x   <=>  k < ceil(x)
		default: return (int)floor(x + 0.5f);
	}
}

// GL's viewport and scissor box as one viewport inside the target, the projection moved so each pixel stays where GL puts it.
// False when nothing is left.
inline bool fitViewport(Mat& proj, const RenderState& rs, int tw, int th, int& x, int& y, int& w, int& h) {
	int x0 = rs.vpX, y0 = rs.vpY, x1 = rs.vpX + rs.vpW, y1 = rs.vpY + rs.vpH;
	if (x0 < 0) x0 = 0;
	if (y0 < 0) y0 = 0;
	if (x1 > tw) x1 = tw;
	if (y1 > th) y1 = th;
	if (rs.scissor) {
		if (x0 < rs.scX) x0 = rs.scX;
		if (y0 < rs.scY) y0 = rs.scY;
		if (x1 > rs.scX + rs.scW) x1 = rs.scX + rs.scW;
		if (y1 > rs.scY + rs.scH) y1 = rs.scY + rs.scH;
	}
	if (x1 <= x0 || y1 <= y0 || rs.vpW <= 0 || rs.vpH <= 0) return false;
	x = x0; y = y0; w = x1 - x0; h = y1 - y0;
	if (x == rs.vpX && y == rs.vpY && w == rs.vpW && h == rs.vpH) return true;
	// pixel x = vpX + (ndc.x + 1) vpW / 2, and the same pixel in the new viewport: ndc.x' = ndc.x sx + tx (y likewise, downwards).
	const double sx = (double)rs.vpW / w, tx = (2.0 * (rs.vpX - x) + rs.vpW) / w - 1.0;
	const double sy = (double)rs.vpH / h, ty = 1.0 - (2.0 * (rs.vpY - y) + rs.vpH) / h;
	for (int r = 0; r < 4; ++r) {
		const double cw = proj.m[r][3];
		proj.m[r][0] = (float)(proj.m[r][0] * sx + cw * tx);
		proj.m[r][1] = (float)(proj.m[r][1] * sy + cw * ty);
	}
	return true;
}

// glPolygonOffset's constant part is already depth-buffer units (Gpu.h); its slope part has no fixed-function equivalent and is
// taken at a fixed scale, enough to lift Beta's block-breaking cracks off the block they lie on.
const double kSlopeDepth = 1.0 / 65536.0;

// A texture matrix (GL: (s, t, 0, 1) x T, sampled at s/q, t/q) for Direct3D's 2D coordinates, which go in as (u, v, 1, 0): T's
// translation row moves to the third, and four components come out for the divide.
inline Mat texture2D(const Mat& t) {
	Mat r;
	for (int c = 0; c < 4; ++c) { r.m[0][c] = t.m[0][c]; r.m[1][c] = t.m[1][c]; r.m[2][c] = t.m[3][c]; r.m[3][c] = 0; }
	return r;
}

// Fill `p` for one draw.
inline void plan(Plan& p, const RenderState& rs, const Constants& c, const Bindings& b, int tw, int th) {
	memset(&p, 0, sizeof(p));
	const float* vs = c.vs;
	const float* ps = c.ps;
	const Mat wvp = fromRegisters(vs, 0);
	const Mat mv = fromRegisters(vs, 4);
	Mat s = identity();
	s.m[2][2] = -1.0f;
	Mat inv;
	const bool eye = invert(mv, inv);   // (no inverse: no eye space; lighting, fog and generation are left out)
	const float* lit = vs + 16 * 4;
	const float* gen = vs + 28 * 4;
	const float* fogp = ps + 2 * 4;
	const bool needEye = eye && (lit[0] > 0.5f || fogp[0] > 0.5f || gen[0] > 0.5f || gen[1] > 0.5f || gen[2] > 0.5f || gen[3] > 0.5f);
	p.view = identity();
	if (needEye) {
		p.world = mul(mv, s);
		p.proj = mul(mul(s, inv), wvp);
	} else {   // (nothing reads the eye space: the layer's matrix as it is, no rounding through the inverse)
		p.world = identity();
		p.proj = wvp;
	}
	if (!fitViewport(p.proj, rs, tw, th, p.vpX, p.vpY, p.vpW, p.vpH)) { p.skip = true; return; }
	const double bias = rs.depthBias + rs.slopeBias * kSlopeDepth;
	if (bias != 0)
		for (int r = 0; r < 4; ++r) p.proj.m[r][2] = (float)(p.proj.m[r][2] + bias * p.proj.m[r][3]);   // z' = z + bias w

	// lighting (GpuShaders.h's glVertex)
	const int colorMaterial = (int)(lit[3] + 0.5f);   // 0 off, 1 AMBIENT, 2 DIFFUSE, 3 both
	p.lighting = eye && lit[0] > 0.5f;
	if (p.lighting) {
		const float* amb = vs + 17 * 4;
		p.normalize = amb[3] > 0.5f;
		for (int i = 0; i < 4; ++i) p.ambient[i] = amb[i];
		p.ambient[3] = 1.0f;
		p.ambientFromColor = colorMaterial == 1 || colorMaterial == 3;
		p.diffuseFromColor = colorMaterial >= 2;
		memcpy(p.matAmbient, vs + 24 * 4, sizeof(p.matAmbient));
		memcpy(p.matDiffuse, vs + 25 * 4, sizeof(p.matDiffuse));
		for (int l = 0; l < 2; ++l) {
			Light& L = p.light[l];
			L.on = lit[1 + l] > 0.5f;
			const float* lp = vs + (18 + 3 * l) * 4;
			memcpy(L.diffuse, vs + (19 + 3 * l) * 4, sizeof(L.diffuse));
			memcpy(L.ambient, vs + (20 + 3 * l) * 4, sizeof(L.ambient));
			L.directional = lp[3] == 0.0f;
			L.pos[0] = lp[0]; L.pos[1] = lp[1]; L.pos[2] = -lp[2];               // eye -> camera space (z mirrored)
			L.dir[0] = -lp[0]; L.dir[1] = -lp[1]; L.dir[2] = lp[2];              // GL names the way to the light; Direct3D the way it goes
		}
	}
	p.flat = vs[27 * 4] > 0.5f;

	// the colour at glCallList: GL multiplies it into the vertex colour before lighting; a factor stage multiplies after, the same
	// when the lit colour is the colour scaled (both material colours from it) and when nothing is lit.
	const float* tint = vs + 26 * 4;
	const bool white = tint[0] == 1.0f && tint[1] == 1.0f && tint[2] == 1.0f && tint[3] == 1.0f;
	p.tinted = !white && !(p.lighting && colorMaterial == 0);
	p.tint = toArgb(tint);

	// texture stages (GpuShaders.h's glFragment: the units' environments in turn)
	if (p.tinted) {
		Stage& st = p.stage[p.stages++];
		st.colorOp = kTopModulate; st.colorArg1 = kTaDiffuse; st.colorArg2 = kTaFactor;
		st.alphaOp = kTopModulate; st.alphaArg1 = kTaDiffuse; st.alphaArg2 = kTaFactor;
		st.unit = -1;
	}
	const float* tex = ps;
	const bool anyGen = eye && (gen[0] > 0.5f || gen[1] > 0.5f || gen[2] > 0.5f || gen[3] > 0.5f);
	for (int u = 0; u < 2; ++u) {
		if (tex[u] < 0.5f || !b.tex[u]) continue;
		Stage& st = p.stage[p.stages++];
		const int env = (int)(tex[2 + u] + 0.5f);   // 0 MODULATE 1 REPLACE 2 ADD 3 DECAL
		st.unit = u;
		st.colorArg1 = st.alphaArg1 = kTaTexture;
		st.colorArg2 = st.alphaArg2 = kTaCurrent;
		if (env == 1)      { st.colorOp = kTopSelectArg1; st.alphaOp = kTopSelectArg1; }
		else if (env == 2) { st.colorOp = kTopAdd; st.alphaOp = kTopModulate; }
		else if (env == 3) { st.colorOp = kTopBlendTextureAlpha; st.alphaOp = kTopSelectArg2; }
		else               { st.colorOp = kTopModulate; st.alphaOp = kTopModulate; }
		const Mat t = fromRegisters(vs, u == 0 ? 8 : 12);
		if (u == 0 && anyGen) {
			// the camera-space position (x, y, z, 1) -> GL's generated (s, t, r, q): a plane per column, then the texture matrix
			Mat g;
			memset(&g, 0, sizeof(g));
			for (int i = 0; i < 4; ++i) {
				const int mode = (int)(gen[i] + 0.5f);   // 0 off, 1 object linear, 2 eye linear
				float col[4] = { 0, 0, 0, i == 3 ? 1.0f : 0.0f };
				if (mode == 2) {
					const float* e = vs + (33 + i) * 4;   // eye plane: dot(eye, E) = dot(camera, S E)
					col[0] = e[0]; col[1] = e[1]; col[2] = -e[2]; col[3] = e[3];
				} else if (mode == 1) {
					const float* o = vs + (29 + i) * 4;   // object plane: dot(eye x inverse(MV), O) = dot(camera, S inverse(MV) O)
					const Mat si = mul(s, inv);
					for (int r = 0; r < 4; ++r) col[r] = si.m[r][0] * o[0] + si.m[r][1] * o[1] + si.m[r][2] * o[2] + si.m[r][3] * o[3];
				} else if (i < 2) {
					p.partialGen = true;   // (GL keeps the vertex's own s or t: not reachable from the camera-space position)
				}
				for (int r = 0; r < 4; ++r) g.m[r][i] = col[r];
			}
			st.coord = kCoordEyePosition;
			st.transform = true;
			st.matrix = mul(g, t);
		} else {
			st.coord = kCoordUnit;
			st.transform = !isIdentity(t);
			if (st.transform) st.matrix = texture2D(t);
		}
	}
	if (p.stages == 0) {
		Stage& st = p.stage[p.stages++];
		st.colorOp = kTopSelectArg1; st.colorArg1 = kTaDiffuse; st.colorArg2 = kTaCurrent;
		st.alphaOp = kTopSelectArg1; st.alphaArg1 = kTaDiffuse; st.alphaArg2 = kTaCurrent;
		st.unit = -1;
	}

	// the alpha test
	const float* alpha = ps + 1 * 4;
	p.alphaTest = alpha[0] > 0.5f;
	p.alphaFunc = (int)(alpha[1] + 0.5f);
	p.alphaRef = alphaReference(p.alphaFunc, alpha[2]);

	// fog: the camera-space distance (|eye z|), at each vertex
	p.fog = eye && fogp[0] > 0.5f;
	p.fogMode = (int)(fogp[1] + 0.5f);
	p.fogStart = fogp[2];
	p.fogEnd = fogp[3];
	const float* fogc = ps + 3 * 4;
	p.fogDensity = fogc[3];
	const float fc[4] = { fogc[0], fogc[1], fogc[2], 1.0f };
	p.fogColor = toArgb(fc);
}

}  // namespace fixed
}  // namespace gpu
}  // namespace j2xgl

#endif // GPU_FIXED_H
