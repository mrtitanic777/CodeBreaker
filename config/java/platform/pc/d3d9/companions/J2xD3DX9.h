// J2xD3DX9.h -- the part of D3DX 9 the shared Direct3D 9 layer (GL11.cpp, GLU.cpp) uses, for the PC.
//
// Windows ships Direct3D 9 (d3d9.dll) but not D3DX: d3dx9_43.dll came only with the 2010 DirectX redistributable, and the
// Windows SDK that MSVC and clang-cl use has no d3dx9.h at all. So on the PC the layer gets the same names from here, written
// to the same definitions (row vectors, v' = v * M; the RH projections as D3DX documents them), and shader compiling goes to
// D3DCompile in d3dcompiler_47.dll, which every Windows since 8.1 carries. The Xbox 360 build uses the XDK's own d3dx9.h and
// never sees this file.
#ifndef J2X_D3DX9_H
#define J2X_D3DX9_H

#include <d3d9.h>
#include <d3dcompiler.h>
#include <math.h>
#include <string.h>

#define D3DX_PI          3.141592654f
#define D3DXToRadian(d)  ((d) * (D3DX_PI / 180.0f))
#define D3DXToDegree(r)  ((r) * (180.0f / D3DX_PI))

struct D3DXVECTOR3 {
	float x, y, z;
	D3DXVECTOR3() : x(0), y(0), z(0) {}
	D3DXVECTOR3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

struct D3DXVECTOR4 {
	float x, y, z, w;
	D3DXVECTOR4() : x(0), y(0), z(0), w(0) {}
	D3DXVECTOR4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
};

// A 4x4 matrix laid out as D3DMATRIX: m[row][col], row vectors (a point is a row on the left: p' = p * M).
struct D3DXMATRIX : public D3DMATRIX {
	D3DXMATRIX() {}
	explicit D3DXMATRIX(const float* f) { memcpy(m, f, sizeof(m)); }
	D3DXMATRIX(float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24,
	           float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44) {
		_11 = a11; _12 = a12; _13 = a13; _14 = a14; _21 = a21; _22 = a22; _23 = a23; _24 = a24;
		_31 = a31; _32 = a32; _33 = a33; _34 = a34; _41 = a41; _42 = a42; _43 = a43; _44 = a44;
	}
	operator float*() { return &m[0][0]; }
	operator const float*() const { return &m[0][0]; }
	float& operator()(unsigned r, unsigned c) { return m[r][c]; }
	float operator()(unsigned r, unsigned c) const { return m[r][c]; }
	D3DXMATRIX operator*(const D3DXMATRIX& b) const {
		D3DXMATRIX r;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				r.m[i][j] = m[i][0] * b.m[0][j] + m[i][1] * b.m[1][j] + m[i][2] * b.m[2][j] + m[i][3] * b.m[3][j];
		return r;
	}
	D3DXMATRIX& operator*=(const D3DXMATRIX& b) { *this = *this * b; return *this; }
	bool operator==(const D3DXMATRIX& b) const { return memcmp(m, b.m, sizeof(m)) == 0; }
	bool operator!=(const D3DXMATRIX& b) const { return !(*this == b); }
};

inline D3DXMATRIX* D3DXMatrixIdentity(D3DXMATRIX* o) {
	memset(o->m, 0, sizeof(o->m));
	o->_11 = o->_22 = o->_33 = o->_44 = 1.0f;
	return o;
}

inline D3DXMATRIX* D3DXMatrixMultiply(D3DXMATRIX* o, const D3DXMATRIX* a, const D3DXMATRIX* b) {
	*o = *a * *b;
	return o;
}

inline D3DXMATRIX* D3DXMatrixTranspose(D3DXMATRIX* o, const D3DXMATRIX* a) {
	D3DXMATRIX t;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) t.m[i][j] = a->m[j][i];
	*o = t;
	return o;
}

inline D3DXMATRIX* D3DXMatrixTranslation(D3DXMATRIX* o, float x, float y, float z) {
	D3DXMatrixIdentity(o);
	o->_41 = x; o->_42 = y; o->_43 = z;
	return o;
}

inline D3DXMATRIX* D3DXMatrixScaling(D3DXMATRIX* o, float x, float y, float z) {
	D3DXMatrixIdentity(o);
	o->_11 = x; o->_22 = y; o->_33 = z;
	return o;
}

// A rotation of `angle` radians about `axis` (normalized here), as D3DX builds it: for row vectors, the transpose of the
// column-vector form, so it turns the same way GL's glRotatef does once GL's matrices are read as rows.
inline D3DXMATRIX* D3DXMatrixRotationAxis(D3DXMATRIX* o, const D3DXVECTOR3* axis, float angle) {
	float x = axis->x, y = axis->y, z = axis->z;
	const float len = sqrtf(x * x + y * y + z * z);
	if (len > 0.0f) { x /= len; y /= len; z /= len; }
	const float c = cosf(angle), s = sinf(angle), t = 1.0f - c;
	D3DXMatrixIdentity(o);
	o->_11 = t * x * x + c;     o->_12 = t * x * y + s * z; o->_13 = t * x * z - s * y;
	o->_21 = t * x * y - s * z; o->_22 = t * y * y + c;     o->_23 = t * y * z + s * x;
	o->_31 = t * x * z + s * y; o->_32 = t * y * z - s * x; o->_33 = t * z * z + c;
	return o;
}

inline D3DXMATRIX* D3DXMatrixRotationX(D3DXMATRIX* o, float a) { D3DXVECTOR3 v(1, 0, 0); return D3DXMatrixRotationAxis(o, &v, a); }
inline D3DXMATRIX* D3DXMatrixRotationY(D3DXMATRIX* o, float a) { D3DXVECTOR3 v(0, 1, 0); return D3DXMatrixRotationAxis(o, &v, a); }
inline D3DXMATRIX* D3DXMatrixRotationZ(D3DXMATRIX* o, float a) { D3DXVECTOR3 v(0, 0, 1); return D3DXMatrixRotationAxis(o, &v, a); }

// The right-handed projections (eye looks down -Z, depth 0..1), exactly as the D3DX documentation gives them.
inline D3DXMATRIX* D3DXMatrixOrthoOffCenterRH(D3DXMATRIX* o, float l, float r, float b, float t, float zn, float zf) {
	D3DXMatrixIdentity(o);
	o->_11 = 2.0f / (r - l);
	o->_22 = 2.0f / (t - b);
	o->_33 = 1.0f / (zn - zf);
	o->_41 = (l + r) / (l - r);
	o->_42 = (t + b) / (b - t);
	o->_43 = zn / (zn - zf);
	return o;
}

inline D3DXMATRIX* D3DXMatrixPerspectiveOffCenterRH(D3DXMATRIX* o, float l, float r, float b, float t, float zn, float zf) {
	memset(o->m, 0, sizeof(o->m));
	o->_11 = 2.0f * zn / (r - l);
	o->_22 = 2.0f * zn / (t - b);
	o->_31 = (l + r) / (r - l);
	o->_32 = (t + b) / (t - b);
	o->_33 = zf / (zn - zf);
	o->_34 = -1.0f;
	o->_43 = zn * zf / (zn - zf);
	return o;
}

inline D3DXMATRIX* D3DXMatrixPerspectiveFovRH(D3DXMATRIX* o, float fovy, float aspect, float zn, float zf) {
	const float ys = 1.0f / tanf(fovy * 0.5f), xs = ys / aspect;
	memset(o->m, 0, sizeof(o->m));
	o->_11 = xs;
	o->_22 = ys;
	o->_33 = zf / (zn - zf);
	o->_34 = -1.0f;
	o->_43 = zn * zf / (zn - zf);
	return o;
}

// The general inverse (cofactors over the determinant); NULL, and `o` untouched, when `a` has none.
inline D3DXMATRIX* D3DXMatrixInverse(D3DXMATRIX* o, float* determinant, const D3DXMATRIX* a) {
	const float* s = &a->m[0][0];
	float inv[16];
	inv[0] = s[5] * s[10] * s[15] - s[5] * s[11] * s[14] - s[9] * s[6] * s[15] + s[9] * s[7] * s[14] + s[13] * s[6] * s[11] - s[13] * s[7] * s[10];
	inv[4] = -s[4] * s[10] * s[15] + s[4] * s[11] * s[14] + s[8] * s[6] * s[15] - s[8] * s[7] * s[14] - s[12] * s[6] * s[11] + s[12] * s[7] * s[10];
	inv[8] = s[4] * s[9] * s[15] - s[4] * s[11] * s[13] - s[8] * s[5] * s[15] + s[8] * s[7] * s[13] + s[12] * s[5] * s[11] - s[12] * s[7] * s[9];
	inv[12] = -s[4] * s[9] * s[14] + s[4] * s[10] * s[13] + s[8] * s[5] * s[14] - s[8] * s[6] * s[13] - s[12] * s[5] * s[10] + s[12] * s[6] * s[9];
	inv[1] = -s[1] * s[10] * s[15] + s[1] * s[11] * s[14] + s[9] * s[2] * s[15] - s[9] * s[3] * s[14] - s[13] * s[2] * s[11] + s[13] * s[3] * s[10];
	inv[5] = s[0] * s[10] * s[15] - s[0] * s[11] * s[14] - s[8] * s[2] * s[15] + s[8] * s[3] * s[14] + s[12] * s[2] * s[11] - s[12] * s[3] * s[10];
	inv[9] = -s[0] * s[9] * s[15] + s[0] * s[11] * s[13] + s[8] * s[1] * s[15] - s[8] * s[3] * s[13] - s[12] * s[1] * s[11] + s[12] * s[3] * s[9];
	inv[13] = s[0] * s[9] * s[14] - s[0] * s[10] * s[13] - s[8] * s[1] * s[14] + s[8] * s[2] * s[13] + s[12] * s[1] * s[10] - s[12] * s[2] * s[9];
	inv[2] = s[1] * s[6] * s[15] - s[1] * s[7] * s[14] - s[5] * s[2] * s[15] + s[5] * s[3] * s[14] + s[13] * s[2] * s[7] - s[13] * s[3] * s[6];
	inv[6] = -s[0] * s[6] * s[15] + s[0] * s[7] * s[14] + s[4] * s[2] * s[15] - s[4] * s[3] * s[14] - s[12] * s[2] * s[7] + s[12] * s[3] * s[6];
	inv[10] = s[0] * s[5] * s[15] - s[0] * s[7] * s[13] - s[4] * s[1] * s[15] + s[4] * s[3] * s[13] + s[12] * s[1] * s[7] - s[12] * s[3] * s[5];
	inv[14] = -s[0] * s[5] * s[14] + s[0] * s[6] * s[13] + s[4] * s[1] * s[14] - s[4] * s[2] * s[13] - s[12] * s[1] * s[6] + s[12] * s[2] * s[5];
	inv[3] = -s[1] * s[6] * s[11] + s[1] * s[7] * s[10] + s[5] * s[2] * s[11] - s[5] * s[3] * s[10] - s[9] * s[2] * s[7] + s[9] * s[3] * s[6];
	inv[7] = s[0] * s[6] * s[11] - s[0] * s[7] * s[10] - s[4] * s[2] * s[11] + s[4] * s[3] * s[10] + s[8] * s[2] * s[7] - s[8] * s[3] * s[6];
	inv[11] = -s[0] * s[5] * s[11] + s[0] * s[7] * s[9] + s[4] * s[1] * s[11] - s[4] * s[3] * s[9] - s[8] * s[1] * s[7] + s[8] * s[3] * s[5];
	inv[15] = s[0] * s[5] * s[10] - s[0] * s[6] * s[9] - s[4] * s[1] * s[10] + s[4] * s[2] * s[9] + s[8] * s[1] * s[6] - s[8] * s[2] * s[5];
	const float det = s[0] * inv[0] + s[1] * inv[4] + s[2] * inv[8] + s[3] * inv[12];
	if (determinant) *determinant = det;
	if (det == 0.0f) return 0;
	const float r = 1.0f / det;
	for (int i = 0; i < 16; ++i) (&o->m[0][0])[i] = inv[i] * r;
	return o;
}

// (x, y, z, 1) * M, divided through by w.
inline D3DXVECTOR3* D3DXVec3TransformCoord(D3DXVECTOR3* o, const D3DXVECTOR3* v, const D3DXMATRIX* m) {
	const float x = v->x * m->_11 + v->y * m->_21 + v->z * m->_31 + m->_41;
	const float y = v->x * m->_12 + v->y * m->_22 + v->z * m->_32 + m->_42;
	const float z = v->x * m->_13 + v->y * m->_23 + v->z * m->_33 + m->_43;
	const float w = v->x * m->_14 + v->y * m->_24 + v->z * m->_34 + m->_44;
	const float iw = w != 0.0f ? 1.0f / w : 1.0f;
	o->x = x * iw; o->y = y * iw; o->z = z * iw;
	return o;
}

// (x, y, z, 0) * M: a direction (a normal) through the matrix's 3x3, no translation.
inline D3DXVECTOR3* D3DXVec3TransformNormal(D3DXVECTOR3* o, const D3DXVECTOR3* v, const D3DXMATRIX* m) {
	const float x = v->x * m->_11 + v->y * m->_21 + v->z * m->_31;
	const float y = v->x * m->_12 + v->y * m->_22 + v->z * m->_32;
	const float z = v->x * m->_13 + v->y * m->_23 + v->z * m->_33;
	o->x = x; o->y = y; o->z = z;
	return o;
}

inline D3DXVECTOR3* D3DXVec3Normalize(D3DXVECTOR3* o, const D3DXVECTOR3* v) {
	const float len = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
	const float r = len > 0.0f ? 1.0f / len : 0.0f;
	o->x = v->x * r; o->y = v->y * r; o->z = v->z * r;
	return o;
}

// ------------------------------------------------------------------ shader compiling
// ID3DXBuffer is D3DCompile's ID3DBlob (both: GetBufferPointer, GetBufferSize, Release).
typedef ID3DBlob ID3DXBuffer;
typedef ID3DBlob* LPD3DXBUFFER;

// D3DXCompileShader's signature over D3DCompile, loaded from d3dcompiler_47.dll on first use (no import library, so any
// Windows compiler links it). The constant table (last argument) is not made: the layer sets its constants by register.
inline HRESULT D3DXCompileShader(const char* src, UINT len, const void* /*defines*/, void* /*include*/, const char* entry,
                                 const char* profile, DWORD flags, ID3DXBuffer** code, ID3DXBuffer** errors, void** constants) {
	static pD3DCompile compile = 0;
	static bool tried = false;
	if (!tried) {
		tried = true;
		HMODULE dll = LoadLibraryA("d3dcompiler_47.dll");
		if (!dll) dll = LoadLibraryA("d3dcompiler_43.dll");
		if (dll) compile = (pD3DCompile)GetProcAddress(dll, "D3DCompile");
	}
	if (code) *code = 0;
	if (errors) *errors = 0;
	if (constants) *constants = 0;
	if (!compile) return E_NOTIMPL;
	return compile(src, len, 0, 0, 0, entry, profile, flags, 0, code, errors);
}

#endif // J2X_D3DX9_H
