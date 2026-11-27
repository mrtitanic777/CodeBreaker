// GL11.cpp -- the OpenGL 1.x layer (see GL11.reference.h for what it covers): GL's whole state machine, written once, for every
// graphics API. It knows no API: every draw ends in one call to the backend (Gpu.h) with the vertices, the render state and the
// shader constants worked out here; GpuD3D9.cpp carries that out on Direct3D 9 (the Xbox 360 and the PC), GpuD3D11.cpp on
// Direct3D 11 (the PC). What GL decides is decided here, so every API draws the same picture.
//
// The shape: GL's state is mirrored in plain variables; a draw turns the part it needs into a gpu::RenderState, gpu::Constants
// and gpu::Bindings. Geometry from any source (glBegin..glEnd, glDrawArrays, glDrawElements, glInterleavedArrays) is gathered
// into one vertex format (gpu::Vertex), then drawn, recorded into the open display list, or (glBegin..glEnd only) coalesced
// into one batched draw.
//
// Lessons the Xbox 360 taught, kept for every target (each is marked where it lives):
//   * pixel bytes are read as bytes and packed colours as ints, never one as the other (tex-endian-byte-read,
//     color-endian-int-read): the 360 is big-endian, the PC little;
//   * the viewport's Y is flipped (GL's origin is bottom-left, the APIs' top-left), from the real target height;
//   * a vertex count past any real batch is a corrupted count, refused rather than allocated (runaway-vertex-count).
// (What the 360 taught about resources -- made out of a scene, never released while bound, untiled textures -- is GpuD3D9.cpp's.)
// C++03 throughout: the Xbox 360's compiler builds this file.

#include "stdafx.h"     // MUST be first: the Xbox 360 project uses /Yu precompiled headers
#include "GL11.h"
#include "Gpu.h"        // the backend: GpuD3D9.cpp or GpuD3D11.cpp
#include "Common.h"     // DbgPrintF / J2X_UNHANDLED / J2X_NOOP
#include <vector>
#include <string.h>
#include <math.h>

namespace j2xgl {

// =========================================================================== the vertex every draw goes through
// gpu::Vertex: position, normal, colour, two texture coordinates, and the flat-shading copies. Flat shading
// (glShadeModel(GL_FLAT)): a triangle takes the colour lit at its PROVOKING vertex, which GL names per primitive (the last vertex
// of a triangle, the fourth of a quad, the first of a polygon). Shaders have no flat interpolator GL's way round, so every
// vertex carries its primitive's provoking normal and colour, and the vertex stage lights those instead when the shade model is
// flat -- one result for all three corners, as GL gives.
typedef gpu::Vertex GLVert;
static const int kUnits = 2;              // texture units (GL13 / ARB_multitexture); GL_MAX_TEXTURE_UNITS
static const int kMaxStack = 64;          // matrix stack depth (GL's minimum is 32 for MODELVIEW)
static const int kMaxVerts = 1048576;     // a draw past this is a corrupted count, not geometry (runaway-vertex-count)

static unsigned int _argb(unsigned a, unsigned r, unsigned g, unsigned b) { return ((a & 255u) << 24) | ((r & 255u) << 16) | ((g & 255u) << 8) | (b & 255u); }
static unsigned _u8(float f) { int i = (int)(f * 255.0f + 0.5f); return (unsigned)(i < 0 ? 0 : i > 255 ? 255 : i); }
static unsigned int _colorValue(float r, float g, float b, float a) { return _argb(_u8(a), _u8(r), _u8(g), _u8(b)); }

// =========================================================================== GL state: matrices
// Every matrix is kept in GL's own convention (a GL column-major float[16] read as Direct3D rows is the same matrix for row
// vectors, so a GL `C = C * X` is `C = X * C` here). glGetFloat hands them back as GL has them.
// A 4x4 matrix as rows (m[row][col]); a point is a row on the left: p' = p * M.
struct Mat {
	float m[4][4];
	Mat operator*(const Mat& b) const {
		Mat r;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j) r.m[i][j] = m[i][0] * b.m[0][j] + m[i][1] * b.m[1][j] + m[i][2] * b.m[2][j] + m[i][3] * b.m[3][j];
		return r;
	}
};
static int g_matMode = GL_MODELVIEW;
static std::vector<Mat> g_mv, g_proj, g_texm[kUnits];
static int g_activeUnit = 0, g_clientUnit = 0;

static Mat _identity() { Mat m; memset(m.m, 0, sizeof(m.m)); m.m[0][0] = m.m[1][1] = m.m[2][2] = m.m[3][3] = 1.0f; return m; }
static Mat _fromGL(const float* f) { Mat m; memcpy(&m.m[0][0], f, sizeof(float) * 16); return m; }
static Mat _transpose(const Mat& a) { Mat t; for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) t.m[i][j] = a.m[j][i]; return t; }
static bool _isIdentity(const Mat& a) {
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			if (fabsf(a.m[r][c] - (r == c ? 1.0f : 0.0f)) > 1e-6f) return false;
	return true;
}
// The general inverse (cofactors over the determinant); false, and `o` untouched, when `a` has none.
static bool _inverse(Mat& o, const Mat& a) {
	const float* s = &a.m[0][0];
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
	if (det == 0.0f) return false;
	const float r = 1.0f / det;
	for (int i = 0; i < 16; ++i) (&o.m[0][0])[i] = inv[i] * r;
	return true;
}
// A vertex's position (x, y, z, 1) * M divided through by w, and its normals by M's 3x3: what moving it by M means.
static void _move(GLVert& v, const Mat& m) {
	const float x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	const float y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	const float z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
	const float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	const float iw = w != 0.0f ? 1.0f / w : 1.0f;
	v.x = x * iw; v.y = y * iw; v.z = z * iw;
	const float nx = v.nx * m.m[0][0] + v.ny * m.m[1][0] + v.nz * m.m[2][0];
	const float ny = v.nx * m.m[0][1] + v.ny * m.m[1][1] + v.nz * m.m[2][1];
	const float nz = v.nx * m.m[0][2] + v.ny * m.m[1][2] + v.nz * m.m[2][2];
	v.nx = nx; v.ny = ny; v.nz = nz;
	const float fx = v.fnx * m.m[0][0] + v.fny * m.m[1][0] + v.fnz * m.m[2][0];
	const float fy = v.fnx * m.m[0][1] + v.fny * m.m[1][1] + v.fnz * m.m[2][1];
	const float fz = v.fnx * m.m[0][2] + v.fny * m.m[1][2] + v.fnz * m.m[2][2];
	v.fnx = fx; v.fny = fy; v.fnz = fz;
}
static std::vector<Mat>& _stack(int mode) {
	if (mode == GL_PROJECTION) return g_proj;
	if (mode == GL_TEXTURE) return g_texm[g_activeUnit];
	return g_mv;
}
static std::vector<Mat>& _cur() { return _stack(g_matMode); }
static Mat& _top(std::vector<Mat>& s) { if (s.empty()) s.push_back(_identity()); return s.back(); }

// =========================================================================== GL state: everything else
static float g_cr = 1, g_cg = 1, g_cb = 1, g_ca = 1;          // the current colour
static float g_nx = 0, g_ny = 0, g_nz = 1;                    // the current normal (GL's initial (0,0,1))
static float g_tu[kUnits] = { 0, 0 }, g_tv[kUnits] = { 0, 0 }; // the current texture coordinates per unit

struct Light {
	bool on;
	float pos[4];        // eye space (transformed by the modelview current at glLight)
	float diffuse[4], ambient[4], specular[4];
};
static Light g_light[2];
static float g_sceneAmbient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
static float g_matAmbient[4] = { 0.2f, 0.2f, 0.2f, 1.0f }, g_matDiffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

struct Caps {
	bool tex[kUnits], blend, depth, cull, alphaTest, fog, lighting, colorMaterial, polyOffsetFill, normalize, rescaleNormal,
	     scissor;
};
static Caps g_caps;
static int g_texEnv[kUnits] = { GL_MODULATE, GL_MODULATE };
// Texture coordinate generation, per unit: for S, T, R, Q whether it is on, the mode, the object plane and the eye plane (the eye
// plane already through the inverse modelview of its glTexGen call).
struct TexGen {
	bool on[4];
	int mode[4];
	float object[4][4], eye[4][4];
};
static TexGen g_texGen[kUnits];
static void _resetTexGen(TexGen& g) {
	memset(&g, 0, sizeof(g));
	for (int i = 0; i < 4; ++i) g.mode[i] = GL_EYE_LINEAR;   // (GL's defaults: S on x, T on y, R and Q zero)
	g.object[0][0] = g.eye[0][0] = 1.0f;
	g.object[1][1] = g.eye[1][1] = 1.0f;
}
static bool _texGenObject() {   // unit 0 generates from object coordinates (its draws keep the modelview: no eye-space batching)
	for (int i = 0; i < 4; ++i)
		if (g_texGen[0].on[i] && g_texGen[0].mode[i] == GL_OBJECT_LINEAR) return true;
	return false;
}
static int g_blendSrc = GL_ONE, g_blendDst = GL_ZERO;
static int g_depthFunc = GL_LESS;
static bool g_depthMask = true;
static bool g_colorMask[4] = { true, true, true, true };
static int g_cullFace = GL_BACK, g_frontFace = GL_CCW;
static int g_alphaFunc = GL_ALWAYS;
static bool g_flat = false;                               // glShadeModel(GL_FLAT)
static int g_colorMaterialMode = GL_AMBIENT_AND_DIFFUSE;   // what glColorMaterial lets the colour stand for (GL's initial)
static float g_alphaRef = 0.0f;
static float g_clearR = 0, g_clearG = 0, g_clearB = 0, g_clearA = 0, g_clearDepth = 1.0f;
static int g_vpX = 0, g_vpY = 0, g_vpW = 640, g_vpH = 480;    // as GL was given it (bottom-left origin)
static int g_scX = 0, g_scY = 0, g_scW = 640, g_scH = 480;
static int g_fogMode = GL_EXP;
static float g_fogStart = 0, g_fogEnd = 1, g_fogDensity = 1, g_fogColor[4] = { 0, 0, 0, 0 };
static float g_polyFactor = 0, g_polyUnits = 0;
static int g_unpackAlign = 4, g_packAlign = 4;

// =========================================================================== textures
struct GLTex {
	gpu::Texture tex;
	int w, h, levels;
	int wrapS, wrapT, minF, magF;
	bool used;           // glGenTextures handed this name out (or glBindTexture made it)
	GLTex() : tex(0), w(0), h(0), levels(0), wrapS(GL_REPEAT), wrapT(GL_REPEAT), minF(GL_NEAREST_MIPMAP_LINEAR), magF(GL_LINEAR), used(false) {}
};
static std::vector<GLTex> g_textures;     // index = GL name; 0 is "no texture"
static std::vector<int> g_freeTexNames;   // deleted names, handed out again (as a GL driver does)
static int g_bound[kUnits] = { 0, 0 };

static GLTex* _tex(int id) { return (id > 0 && id < (int)g_textures.size()) ? &g_textures[id] : 0; }
static GLTex* _boundTex() { return _tex(g_bound[g_activeUnit]); }

// =========================================================================== vertex arrays
struct Array {
	bool on;
	const char* ptr;
	int size, type, stride;   // stride in bytes, already resolved (0 -> tightly packed)
	Array() : on(false), ptr(0), size(4), type(GL_FLOAT), stride(0) {}
};
static Array g_va, g_na, g_ca_, g_ta[kUnits];

static int _typeSize(int type) {
	switch (type) {
		case GL_BYTE: case GL_UNSIGNED_BYTE: return 1;
		case GL_SHORT: case GL_UNSIGNED_SHORT: return 2;
		case GL_DOUBLE: return 8;
		default: return 4;
	}
}
static void _setArray(Array& a, int size, int type, int stride, const char* ptr) {
	a.size = size; a.type = type; a.stride = stride ? stride : size * _typeSize(type); a.ptr = ptr;
}

// =========================================================================== display lists
struct GLBatch {
	gpu::Prim prim;                  // triangles or lines
	std::vector<GLVert> verts;       // CPU staging while recording; freed once `vb` holds it
	gpu::Buffer vb;
	int count;                       // vertices in vb
	int tex; bool texOn;             // unit 0's texture as bound inside the list
	GLBatch() : prim(gpu::kTriangles), vb(0), count(0), tex(0), texOn(false) {}
};
struct GLList {
	std::vector<GLBatch> batches;
	bool used;
	bool ownsTex;        // the list bound a texture while recording (terrain), else it takes the caller's (font glyphs)
	bool inheritColor;   // the list set no colour of its own: its vertices take the colour current at glCallList
	Mat xform; bool hasXform;   // the net modelview change made inside the list (FontRenderer's per-glyph advance)
	int endTex;                 // unit 0's binding when the list ended (a list that binds leaves it bound, as GL does)
	float endColor[4];          // the colour when the list ended (a list that sets one leaves it current)
	bool setsColor;
	GLList() : used(false), ownsTex(false), inheritColor(false), hasXform(false), endTex(0), setsColor(false) { xform = _identity(); endColor[0] = endColor[1] = endColor[2] = endColor[3] = 1.0f; }
};
static std::vector<GLList> g_lists;       // index = GL name; 0 reserved
static int g_recording = 0;               // the list being recorded, or 0
static bool g_recordExecute = false;      // GL_COMPILE_AND_EXECUTE
static bool g_recordSetColor = false;     // glColor* was called inside the list being recorded
static bool g_recordHadVB = false;        // the list being re-recorded was already on screen (upload now: no blank frame)
static Mat g_recordBaseMV, g_recordBaseInv;
// GL_COMPILE records commands without doing them; this layer does them while recording, so the state they touch is put back at
// glEndList: the binding, the texture enable, the colour, the normal, the texture coordinates (the modelview: see glEndList).
struct Saved { int bound0; bool tex0; float color[4], normal[3], tu[2], tv[2]; };
static Saved g_recordSaved;
static std::vector<int> g_pendingVB;      // lists whose buffers are made at the frame's end (one scene step for all)

// =========================================================================== selection
static int g_renderMode = GL_RENDER;
static std::vector<int> g_names;
static IntBuffer* g_selBuf = 0;
static int g_selWritten = 0, g_hitCount = 0;
static bool g_hit = false;
static float g_hitMinZ = 1.0f, g_hitMaxZ = 0.0f;

// =========================================================================== immediate mode
static int g_beginMode = -1;
static std::vector<GLVert> g_imm;
// glBegin..glEnd runs are coalesced into one draw (Indev's and the menus' immediate geometry: hundreds of tiny draws a frame):
// each run is baked into eye space (the modelview folded into its positions and normals) and drawn together with a modelview of
// identity. Any state change draws what is pending first, so the batch is always drawn under the state it was gathered in.
static std::vector<GLVert> g_batch;
static gpu::Prim g_batchPrim = gpu::kTriangles;
static void _flushBatch();
static void _stateChange() { _flushBatch(); }

// =========================================================================== a draw: GL's state in the backend's terms
static gpu::Blend _blendArg(int f) {
	switch (f) {
		case GL_ZERO: return gpu::kZero;
		case GL_ONE: return gpu::kOne;
		case GL_SRC_COLOR: return gpu::kSrcColor;
		case GL_ONE_MINUS_SRC_COLOR: return gpu::kInvSrcColor;
		case GL_SRC_ALPHA: return gpu::kSrcAlpha;
		case GL_ONE_MINUS_SRC_ALPHA: return gpu::kInvSrcAlpha;
		case GL_DST_ALPHA: return gpu::kDstAlpha;
		case GL_ONE_MINUS_DST_ALPHA: return gpu::kInvDstAlpha;
		case GL_DST_COLOR: return gpu::kDstColor;
		case GL_ONE_MINUS_DST_COLOR: return gpu::kInvDstColor;
		case GL_SRC_ALPHA_SATURATE: return gpu::kSrcAlphaSat;
		default: J2X_UNHANDLED("glBlendFunc", f); return gpu::kOne;
	}
}
static gpu::Compare _cmp(int f) {
	if (f < GL_NEVER || f > GL_ALWAYS) { J2X_UNHANDLED("glDepthFunc", f); return gpu::kLess; }
	return (gpu::Compare)(f - GL_NEVER);   // (GL_NEVER..GL_ALWAYS is the backend's order)
}

// GL's viewport and scissor box (bottom-left origin) in the target's top-left pixels, clipped to it (never a wrapped Y).
static void _rect(int gx, int gy, int gw, int gh, int& x, int& y, int& w, int& h) {
	const int fw = gpu::targetWidth(), fh = gpu::targetHeight();
	x = gx; w = gw; h = gh;
	y = fh - gy - gh;                                    // GL's bottom-left origin -> the API's top-left
	if (x < 0) { w += x; x = 0; }
	if (y < 0) { h += y; y = 0; }
	if (x + w > fw) w = fw - x;
	if (y + h > fh) h = fh - y;
	if (w < 1) w = 1;
	if (h < 1) h = 1;
}

// Every fixed-function state a draw runs under, from the GL mirror.
static gpu::RenderState _renderState() {
	gpu::RenderState rs;
	rs.blend = g_caps.blend; rs.srcBlend = _blendArg(g_blendSrc); rs.dstBlend = _blendArg(g_blendDst);
	rs.depthTest = g_caps.depth; rs.depthFunc = _cmp(g_depthFunc); rs.depthWrite = g_depthMask;
	rs.colorWrite = (g_colorMask[0] ? 1u : 0u) | (g_colorMask[1] ? 2u : 0u) | (g_colorMask[2] ? 4u : 0u) | (g_colorMask[3] ? 8u : 0u);
	// Face culling: GL's front faces are counter-clockwise in its bottom-up window; the viewport flip mirrors them, so culling GL's
	// BACK faces culls the clockwise ones on screen (the Xbox 360 port proved it). Culling FRONT faces, or calling clockwise
	// front, flips it.
	rs.cull = gpu::kCullNone;
	if (g_caps.cull && g_cullFace != GL_FRONT_AND_BACK) {
		bool cw = (g_cullFace == GL_BACK);
		if (g_frontFace == GL_CW) cw = !cw;
		rs.cull = cw ? gpu::kCullCW : gpu::kCullCCW;
	}
	// glPolygonOffset: GL units are depth-buffer steps (D24: 1/2^24 each); the factor is the slope scale.
	rs.slopeBias = g_caps.polyOffsetFill ? g_polyFactor : 0.0f;
	rs.depthBias = g_caps.polyOffsetFill ? g_polyUnits * (1.0f / 16777216.0f) : 0.0f;
	rs.scissor = g_caps.scissor;
	_rect(g_scX, g_scY, g_scW, g_scH, rs.scX, rs.scY, rs.scW, rs.scH);
	_rect(g_vpX, g_vpY, g_vpW, g_vpH, rs.vpX, rs.vpY, rs.vpW, rs.vpH);
	return rs;
}

static void _putMat(float* out, const Mat& m) {   // transposed for HLSL's mul(row vector, matrix) from its column-major constants
	const Mat t = _transpose(m);
	memcpy(out, &t.m[0][0], 64);
}
static void _putEyeLight(float* out, const Light& L, bool on) {
	if (!on) { memset(out, 0, 48); return; }
	memcpy(out, L.pos, 16);
	memcpy(out + 4, L.diffuse, 16);
	memcpy(out + 8, L.ambient, 16);
}
// GL's clip space (depth -1..1) -> the APIs' (depth 0..1): z' = (z + w) / 2. Direct3D 9 on the PC also puts pixel centres on
// integers, so its geometry moves half a pixel left and up there, landing a GL pixel-exact draw (the GUI's text) on the same
// pixels; Direct3D 10 and later, and the Xbox 360 (its Display turns its half-pixel offset on), have GL's centres already.
static Mat _glToApi() {
	Mat f = _identity();
	f.m[2][2] = 0.5f; f.m[3][2] = 0.5f;
	if (gpu::halfPixelOffset()) {
		f.m[3][0] = -1.0f / (float)(g_vpW > 0 ? g_vpW : 1);
		f.m[3][1] = 1.0f / (float)(g_vpH > 0 ? g_vpH : 1);
	}
	return f;
}
static int _envCode(int env) {
	switch (env) {
		case GL_MODULATE: return 0;
		case GL_REPLACE: return 1;
		case GL_ADD: return 2;
		case GL_DECAL: return 3;
		default: return 0;
	}
}
static float _colorMaterialCode() {
	if (!g_caps.colorMaterial) return 0.0f;
	switch (g_colorMaterialMode) {
		case GL_AMBIENT: return 1.0f;
		case GL_DIFFUSE: return 2.0f;
		case GL_AMBIENT_AND_DIFFUSE: return 3.0f;
		default: return 0.0f;
	}
}

// How a texture unit samples its texture, from the texture's GL parameters.
static gpu::Sampler _sampler(const GLTex& t) {
	gpu::Sampler s;
	s.wrapU = (t.wrapS == GL_REPEAT);
	s.wrapV = (t.wrapT == GL_REPEAT);
	s.mag = (t.magF == GL_LINEAR) ? gpu::kLinear : gpu::kPoint;
	const bool minLinear = t.minF == GL_LINEAR || t.minF == GL_LINEAR_MIPMAP_NEAREST || t.minF == GL_LINEAR_MIPMAP_LINEAR;
	s.min = minLinear ? gpu::kLinear : gpu::kPoint;
	s.mip = -1;   // (a mip filter on a texture with one level samples that level, as a lenient driver does)
	if (t.levels > 1) {
		if (t.minF == GL_NEAREST_MIPMAP_NEAREST || t.minF == GL_LINEAR_MIPMAP_NEAREST) s.mip = gpu::kPoint;
		else if (t.minF == GL_NEAREST_MIPMAP_LINEAR || t.minF == GL_LINEAR_MIPMAP_LINEAR) s.mip = gpu::kLinear;
	}
	return s;
}

// Everything a draw needs from GL's state: the constants and the textures. `bakedEye`: the vertices are already in eye space
// (the coalesced batch), so the modelview is identity. `tint`: the colour a colourless display list takes at glCallList.
// `useListTex`: a list that bound its own texture draws with it (`listTex`, `listTexOn`), not the caller's.
static void _drawInputs(bool bakedEye, const float* tint, int listTex, bool listTexOn, bool useListTex, gpu::Constants& c, gpu::Bindings& b) {
	const Mat mv = bakedEye ? _identity() : _top(g_mv);
	const Mat wvp = mv * _top(g_proj) * _glToApi();
	float* vc = c.vs;
	_putMat(vc + 0, wvp);
	_putMat(vc + 16, mv);
	_putMat(vc + 32, _top(g_texm[0]));
	_putMat(vc + 48, _top(g_texm[1]));
	const float lit[4] = { g_caps.lighting ? 1.0f : 0.0f, g_light[0].on ? 1.0f : 0.0f, g_light[1].on ? 1.0f : 0.0f, _colorMaterialCode() };
	memcpy(vc + 64, lit, 16);
	memcpy(vc + 68, g_sceneAmbient, 12);
	vc[71] = (g_caps.normalize || g_caps.rescaleNormal) ? 1.0f : 0.0f;
	_putEyeLight(vc + 72, g_light[0], g_light[0].on);
	_putEyeLight(vc + 84, g_light[1], g_light[1].on);
	memcpy(vc + 96, g_matAmbient, 16);
	memcpy(vc + 100, g_matDiffuse, 16);
	static const float white[4] = { 1, 1, 1, 1 };
	memcpy(vc + 104, tint ? tint : white, 16);
	vc[108] = g_flat ? 1.0f : 0.0f; vc[109] = vc[110] = vc[111] = 0.0f;
	// c28 gGen (S T R Q: 0 off, 1 object linear, 2 eye linear), c29-32 the object planes, c33-36 the eye planes -- unit 0's
	const TexGen& g = g_texGen[0];
	for (int i = 0; i < 4; ++i) vc[112 + i] = !g.on[i] ? 0.0f : g.mode[i] == GL_OBJECT_LINEAR ? 1.0f : 2.0f;
	memcpy(vc + 116, g.object, 64);
	memcpy(vc + 132, g.eye, 64);
	if (g_texGen[1].on[0] || g_texGen[1].on[1] || g_texGen[1].on[2] || g_texGen[1].on[3])
		J2X_UNHANDLED("texture coordinate generation on unit 1 (unit 0 only)", 1);

	const int texId[kUnits] = { useListTex ? listTex : g_bound[0], g_bound[1] };
	const bool texOn[kUnits] = { useListTex ? listTexOn : g_caps.tex[0], g_caps.tex[1] };
	bool on[kUnits];
	for (int u = 0; u < kUnits; ++u) {
		GLTex* t = _tex(texId[u]);
		on[u] = texOn[u] && t && t->tex;   // (GL: a unit with no image does not texture)
		b.tex[u] = on[u] ? t->tex : 0;
		if (on[u]) b.samp[u] = _sampler(*t);
		else { b.samp[u].wrapU = b.samp[u].wrapV = true; b.samp[u].mag = b.samp[u].min = gpu::kPoint; b.samp[u].mip = -1; }
	}
	float* pc = c.ps;
	pc[0] = on[0] ? 1.0f : 0.0f; pc[1] = on[1] ? 1.0f : 0.0f; pc[2] = (float)_envCode(g_texEnv[0]); pc[3] = (float)_envCode(g_texEnv[1]);
	pc[4] = g_caps.alphaTest ? 1.0f : 0.0f; pc[5] = (float)(g_alphaFunc - GL_NEVER); pc[6] = g_alphaRef; pc[7] = 0;
	pc[8] = g_caps.fog ? 1.0f : 0.0f; pc[9] = g_fogMode == GL_LINEAR ? 0.0f : g_fogMode == GL_EXP2 ? 2.0f : 1.0f; pc[10] = g_fogStart; pc[11] = g_fogEnd;
	pc[12] = g_fogColor[0]; pc[13] = g_fogColor[1]; pc[14] = g_fogColor[2]; pc[15] = g_fogDensity;
}

static void _drawUP(gpu::Prim prim, const GLVert* v, int verts, bool bakedEye) {
	if (!gpu::ready() || verts <= 0) return;
	gpu::Constants c;
	gpu::Bindings b;
	_drawInputs(bakedEye, 0, 0, false, false, c, b);
	gpu::draw(prim, v, verts, _renderState(), c, b);
}

// =========================================================================== gathering: GL primitives -> lists of triangles or lines
// Quads, strips, fans and polygons become triangle lists; line strips and loops become line lists. Points draw nothing (no game
// in the corpus uses them; one would be a quad pair here).
static void _put(std::vector<GLVert>& out, const GLVert& v, const GLVert& provoking) {
	GLVert o = v;
	o.fnx = provoking.nx; o.fny = provoking.ny; o.fnz = provoking.nz;
	o.fcolor = provoking.color;
	out.push_back(o);
}
static void _tri(std::vector<GLVert>& out, const GLVert& a, const GLVert& b, const GLVert& c, const GLVert& p) {
	_put(out, a, p); _put(out, b, p); _put(out, c, p);
}
static void _gather(int mode, const GLVert* v, int n, std::vector<GLVert>& tris, std::vector<GLVert>& lines) {
	int i;
	switch (mode) {   // the provoking vertex (the last argument) is GL's for each primitive
		case GL_TRIANGLES:
			for (i = 0; i + 2 < n; i += 3) _tri(tris, v[i], v[i + 1], v[i + 2], v[i + 2]);
			break;
		case GL_QUADS:   // both halves of a quad take its fourth vertex
			for (i = 0; i + 3 < n; i += 4) {
				_tri(tris, v[i], v[i + 1], v[i + 2], v[i + 3]);
				_tri(tris, v[i], v[i + 2], v[i + 3], v[i + 3]);
			}
			break;
		case GL_TRIANGLE_STRIP:
			for (i = 0; i + 2 < n; ++i) {
				if (i & 1) _tri(tris, v[i + 1], v[i], v[i + 2], v[i + 2]);
				else _tri(tris, v[i], v[i + 1], v[i + 2], v[i + 2]);
			}
			break;
		case GL_QUAD_STRIP:   // quad k takes vertex 2k + 3
			for (i = 0; i + 3 < n; i += 2) {
				_tri(tris, v[i], v[i + 1], v[i + 3], v[i + 3]);
				_tri(tris, v[i], v[i + 3], v[i + 2], v[i + 3]);
			}
			break;
		case GL_TRIANGLE_FAN:
			for (i = 1; i + 1 < n; ++i) _tri(tris, v[0], v[i], v[i + 1], v[i + 1]);
			break;
		case GL_POLYGON:      // a polygon takes its first vertex
			for (i = 1; i + 1 < n; ++i) _tri(tris, v[0], v[i], v[i + 1], v[0]);
			break;
		case GL_LINES:
			for (i = 0; i + 1 < n; i += 2) { _put(lines, v[i], v[i + 1]); _put(lines, v[i + 1], v[i + 1]); }
			break;
		case GL_LINE_STRIP:
		case GL_LINE_LOOP:
			for (i = 0; i + 1 < n; ++i) { _put(lines, v[i], v[i + 1]); _put(lines, v[i + 1], v[i + 1]); }
			if (mode == GL_LINE_LOOP && n > 2) { _put(lines, v[n - 1], v[0]); _put(lines, v[0], v[0]); }
			break;
		case GL_POINTS:
			J2X_NOOP("points: no game in the corpus draws them");
			break;
		default:
			J2X_UNHANDLED("glBegin/glDrawArrays mode", mode);
			break;
	}
}

// --------------------------------------------------------------------------- selection: a real pick-volume test
// In GL_SELECT nothing is drawn; a primitive that reaches the view volume (the pick matrix narrows it to the cursor) is a hit for
// the name stack. Tested conservatively: a primitive is out only when all its vertices are outside the same clip plane.
static void _selectTest(const std::vector<GLVert>& prims, int per) {
	const Mat m = _top(g_mv) * _top(g_proj);
	for (size_t i = 0; i + per <= prims.size(); i += per) {
		int outMask = 0x3F;
		float zmin = 1, zmax = 0;
		for (int k = 0; k < per; ++k) {
			const GLVert& v = prims[i + k];
			const float x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
			const float y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
			const float z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
			const float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
			int o = 0;
			if (x < -w) o |= 1; if (x > w) o |= 2; if (y < -w) o |= 4; if (y > w) o |= 8; if (z < -w) o |= 16; if (z > w) o |= 32;
			outMask &= o;
			if (w > 0) { float d = (z / w) * 0.5f + 0.5f; if (d < 0) d = 0; if (d > 1) d = 1; if (d < zmin) zmin = d; if (d > zmax) zmax = d; }
		}
		if (outMask == 0) {
			if (!g_hit || zmin < g_hitMinZ) g_hitMinZ = zmin;
			if (!g_hit || zmax > g_hitMaxZ) g_hitMaxZ = zmax;
			g_hit = true;
		}
	}
}

// --------------------------------------------------------------------------- the coalesced immediate batch
static void _flushBatch() {
	if (g_batch.empty()) return;
	if (gpu::ready()) _drawUP(g_batchPrim, &g_batch[0], (int)g_batch.size(), true);
	g_batch.clear();
}
static void _bakeEye(std::vector<GLVert>& v) {
	const Mat& mv = _top(g_mv);
	for (size_t k = 0; k < v.size(); ++k) _move(v[k], mv);
}

// --------------------------------------------------------------------------- recording into a display list
static void _record(gpu::Prim prim, std::vector<GLVert>& verts, bool fromColorArray) {
	GLList& L = g_lists[g_recording];
	if (fromColorArray) L.inheritColor = false;   // (its colours are its own)
	// A matrix op made inside the list before this geometry moves it, as GL replays it: the change since glNewList is baked in
	// (replay applies the caller's modelview on top).
	const Mat delta = _top(g_mv) * g_recordBaseInv;
	if (!_isIdentity(delta))
		for (size_t k = 0; k < verts.size(); ++k) _move(verts[k], delta);
	GLBatch b;
	b.prim = prim;
	b.verts.swap(verts);
	b.tex = g_bound[0];
	b.texOn = g_caps.tex[0];
	L.batches.push_back(b);
}

// One primitive run from any source. `batchable`: glBegin..glEnd (coalesced); `colorArray`: its colours came from an array.
static void _submit(int mode, const GLVert* v, int n, bool batchable, bool colorArray) {
	if (n <= 0) return;
	std::vector<GLVert> tris, lines;
	_gather(mode, v, n, tris, lines);
	if (tris.empty() && lines.empty()) return;
	if (g_renderMode == GL_SELECT) {
		_selectTest(tris, 3);
		_selectTest(lines, 2);
		return;
	}
	if (g_recording) {
		std::vector<GLVert> t2, l2;
		if (g_recordExecute) { t2 = tris; l2 = lines; }
		if (!tris.empty()) _record(gpu::kTriangles, tris, colorArray);
		if (!lines.empty()) _record(gpu::kLines, lines, colorArray);
		if (!g_recordExecute) return;
		tris.swap(t2);
		lines.swap(l2);
	}
	if (batchable && g_renderMode == GL_RENDER && !_texGenObject()) {
		if (!tris.empty()) {
			if (!g_batch.empty() && g_batchPrim != gpu::kTriangles) _flushBatch();
			_bakeEye(tris);
			g_batchPrim = gpu::kTriangles;
			g_batch.insert(g_batch.end(), tris.begin(), tris.end());
		}
		if (!lines.empty()) {
			if (!g_batch.empty() && g_batchPrim != gpu::kLines) _flushBatch();
			_bakeEye(lines);
			g_batchPrim = gpu::kLines;
			g_batch.insert(g_batch.end(), lines.begin(), lines.end());
		}
		return;
	}
	_flushBatch();
	if (!tris.empty()) _drawUP(gpu::kTriangles, &tris[0], (int)tris.size(), false);
	if (!lines.empty()) _drawUP(gpu::kLines, &lines[0], (int)lines.size(), false);
}

// =========================================================================== lifecycle
static void _resetGLState() {
	g_mv.clear(); g_mv.push_back(_identity());
	g_proj.clear(); g_proj.push_back(_identity());
	for (int u = 0; u < kUnits; ++u) { g_texm[u].clear(); g_texm[u].push_back(_identity()); _resetTexGen(g_texGen[u]); }
	memset(&g_caps, 0, sizeof(g_caps));
	g_caps.normalize = false;
	for (int i = 0; i < 2; ++i) {
		Light& L = g_light[i];
		L.on = false;
		L.pos[0] = 0; L.pos[1] = 0; L.pos[2] = 1; L.pos[3] = 0;
		for (int k = 0; k < 4; ++k) { L.ambient[k] = (k == 3) ? 1.0f : 0.0f; L.specular[k] = (i == 0 || k == 3) ? 1.0f : 0.0f; }
		for (int k = 0; k < 4; ++k) L.diffuse[k] = (i == 0 || k == 3) ? 1.0f : 0.0f;   // GL: light 0 diffuse white, the others black
	}
}
void glInit() {
	_resetGLState();
	g_textures.clear(); g_textures.push_back(GLTex());
	g_freeTexNames.clear();
	g_lists.clear(); g_lists.push_back(GLList());
	g_recording = 0;
	g_pendingVB.clear();
	g_batch.clear();
	gpu::forgetState();
}
void glOnDeviceReset() { gpu::forgetState(); }   // (every draw sends the whole state; the backend has only its caches to drop)

// =========================================================================== matrix ops
void glMatrixMode(int mode) {
	if (mode != GL_MODELVIEW && mode != GL_PROJECTION && mode != GL_TEXTURE) { J2X_UNHANDLED("glMatrixMode", mode); return; }
	if (mode != g_matMode) _stateChange();
	g_matMode = mode;
}
static void _matrixChanged() { if (g_matMode != GL_MODELVIEW) _stateChange(); }   // (modelview changes are baked per run)
void glPushMatrix() {
	std::vector<Mat>& s = _cur();
	if ((int)s.size() >= kMaxStack) { J2X_UNHANDLED("glPushMatrix: stack overflow", g_matMode); return; }
	s.push_back(_top(s));
}
void glPopMatrix() {
	std::vector<Mat>& s = _cur();
	if (s.size() <= 1) { J2X_UNHANDLED("glPopMatrix: stack underflow", g_matMode); return; }
	_matrixChanged();
	s.pop_back();
}
void glLoadIdentity() { _matrixChanged(); _top(_cur()) = _identity(); }
void glLoadMatrixf(const float* m) { if (!m) return; _matrixChanged(); _top(_cur()) = _fromGL(m); }
void glMultMatrixf(const float* m) { if (!m) return; _matrixChanged(); Mat& t = _top(_cur()); t = _fromGL(m) * t; }
static void _readMatrix(FloatBuffer* b, float* out) {
	for (int i = 0; i < 16; ++i) out[i] = b ? b->get(b->position() + i) : ((i % 5) == 0 ? 1.0f : 0.0f);
}
void glLoadMatrix(FloatBuffer* m) { float f[16]; _readMatrix(m, f); glLoadMatrixf(f); }
void glMultMatrix(FloatBuffer* m) { float f[16]; _readMatrix(m, f); glMultMatrixf(f); }
void glTranslatef(float x, float y, float z) {
	const float m[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1 };
	glMultMatrixf(m);
}
void glScalef(float x, float y, float z) {
	const float m[16] = { x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1 };
	glMultMatrixf(m);
}
void glRotatef(float angle, float x, float y, float z) {
	const float len = sqrtf(x * x + y * y + z * z);
	if (len <= 0.0f) return;
	x /= len; y /= len; z /= len;
	const float a = angle * (3.14159265358979f / 180.0f), c = cosf(a), s = sinf(a), t = 1.0f - c;
	const float m[16] = { x * x * t + c,     y * x * t + z * s, x * z * t - y * s, 0,
	                      x * y * t - z * s, y * y * t + c,     y * z * t + x * s, 0,
	                      x * z * t + y * s, y * z * t - x * s, z * z * t + c,     0,
	                      0, 0, 0, 1 };
	glMultMatrixf(m);
}
void glOrtho(double l, double r, double b, double t, double n, double f) {
	if (l == r || b == t || n == f) { J2X_UNHANDLED("glOrtho: empty volume", 0); return; }
	const float m[16] = { (float)(2 / (r - l)), 0, 0, 0,
	                      0, (float)(2 / (t - b)), 0, 0,
	                      0, 0, (float)(-2 / (f - n)), 0,
	                      (float)(-(r + l) / (r - l)), (float)(-(t + b) / (t - b)), (float)(-(f + n) / (f - n)), 1 };
	glMultMatrixf(m);
}
void glFrustum(double l, double r, double b, double t, double n, double f) {
	if (l == r || b == t || n == f || n <= 0 || f <= 0) { J2X_UNHANDLED("glFrustum: invalid volume", 0); return; }
	const float m[16] = { (float)(2 * n / (r - l)), 0, 0, 0,
	                      0, (float)(2 * n / (t - b)), 0, 0,
	                      (float)((r + l) / (r - l)), (float)((t + b) / (t - b)), (float)(-(f + n) / (f - n)), -1,
	                      0, 0, (float)(-2 * f * n / (f - n)), 0 };
	glMultMatrixf(m);
}
void glViewport(int x, int y, int w, int h) {
	_stateChange();
	g_vpX = x; g_vpY = y; g_vpW = w; g_vpH = h;   // (in the target's pixels at the next draw: _renderState)
}
void glScissor(int x, int y, int w, int h) {
	_stateChange();
	g_scX = x; g_scY = y; g_scW = w; g_scH = h;
}
void glGetFloat(int pname, FloatBuffer* buf) {
	if (!buf) return;
	const Mat* src = 0;
	if (pname == GL_MODELVIEW_MATRIX) src = &_top(g_mv);
	else if (pname == GL_PROJECTION_MATRIX) src = &_top(g_proj);
	else if (pname == GL_TEXTURE_MATRIX) src = &_top(g_texm[g_activeUnit]);
	if (src) {
		const float* d = &src->m[0][0];
		for (int i = 0; i < 16; ++i) buf->put(buf->position() + i, d[i]);
		return;
	}
	if (pname == GL_FOG_COLOR) { for (int i = 0; i < 4; ++i) buf->put(buf->position() + i, g_fogColor[i]); return; }
	J2X_UNHANDLED("glGetFloat", pname);
}
int glGetInteger(int pname) {
	switch (pname) {
		case GL_MAX_TEXTURE_SIZE: return 4096;
		case GL_MAX_TEXTURE_UNITS: return kUnits;
		case GL_TEXTURE_BINDING_2D: return g_bound[g_activeUnit];
		case GL_MATRIX_MODE: return g_matMode;
		case GL_ACTIVE_TEXTURE: return GL_TEXTURE0 + g_activeUnit;
		case GL_CLIENT_ACTIVE_TEXTURE: return GL_TEXTURE0 + g_clientUnit;
		case GL_UNPACK_ALIGNMENT: return g_unpackAlign;
		case GL_PACK_ALIGNMENT: return g_packAlign;
		case GL_VIEWPORT: return g_vpX;
		default: J2X_UNHANDLED("glGetInteger", pname); return 0;
	}
}
void glGetInteger(int pname, IntBuffer* buf) {
	if (!buf) return;
	const int p = buf->position();
	if (pname == GL_VIEWPORT) { buf->put(p, g_vpX); buf->put(p + 1, g_vpY); buf->put(p + 2, g_vpW); buf->put(p + 3, g_vpH); return; }
	buf->put(p, glGetInteger(pname));
}

// =========================================================================== immediate mode
static GLVert _current(float x, float y, float z) {
	GLVert v;
	v.x = x; v.y = y; v.z = z;
	v.nx = g_nx; v.ny = g_ny; v.nz = g_nz;
	v.color = _colorValue(g_cr, g_cg, g_cb, g_ca);
	v.u = g_tu[0]; v.v = g_tv[0]; v.u2 = g_tu[1]; v.v2 = g_tv[1];
	v.fnx = v.nx; v.fny = v.ny; v.fnz = v.nz; v.fcolor = v.color;   // (set per primitive by _gather)
	return v;
}
void glBegin(int mode) {
	if (g_beginMode != -1) J2X_UNHANDLED("glBegin inside glBegin", mode);
	g_beginMode = mode;
	g_imm.clear();
}
void glEnd() {
	if (g_beginMode == -1) { J2X_UNHANDLED("glEnd without glBegin", 0); return; }
	if (!g_imm.empty()) _submit(g_beginMode, &g_imm[0], (int)g_imm.size(), true, false);
	g_beginMode = -1;
	g_imm.clear();
}
void glVertex3f(float x, float y, float z) {
	if (g_beginMode == -1) return;   // (GL: a vertex outside glBegin is undefined; drawn nowhere)
	if ((int)g_imm.size() >= kMaxVerts) return;
	g_imm.push_back(_current(x, y, z));
}
void glVertex2f(float x, float y) { glVertex3f(x, y, 0.0f); }
void glTexCoord2f(float u, float v) { g_tu[0] = u; g_tv[0] = v; }
static float _clamp01(float x) { return x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x); }   // GL clamps the current colour
static void _colorSet() { if (g_recording) g_recordSetColor = true; }
void glColor3f(float r, float g, float b) { g_cr = _clamp01(r); g_cg = _clamp01(g); g_cb = _clamp01(b); g_ca = 1.0f; _colorSet(); }
void glColor4f(float r, float g, float b, float a) { g_cr = _clamp01(r); g_cg = _clamp01(g); g_cb = _clamp01(b); g_ca = _clamp01(a); _colorSet(); }
void glColor3ub(unsigned char r, unsigned char g, unsigned char b) { glColor4ub(r, g, b, 255); }
void glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	g_cr = r / 255.0f; g_cg = g / 255.0f; g_cb = b / 255.0f; g_ca = a / 255.0f;
	_colorSet();
}
void glNormal3f(float x, float y, float z) { g_nx = x; g_ny = y; g_nz = z; }

// =========================================================================== render state
static bool* _capFlag(int cap) {
	switch (cap) {
		case GL_TEXTURE_2D: return &g_caps.tex[g_activeUnit];
		case GL_BLEND: return &g_caps.blend;
		case GL_DEPTH_TEST: return &g_caps.depth;
		case GL_CULL_FACE: return &g_caps.cull;
		case GL_ALPHA_TEST: return &g_caps.alphaTest;
		case GL_FOG: return &g_caps.fog;
		case GL_LIGHTING: return &g_caps.lighting;
		case GL_COLOR_MATERIAL: return &g_caps.colorMaterial;
		case GL_POLYGON_OFFSET_FILL: return &g_caps.polyOffsetFill;
		case GL_NORMALIZE: return &g_caps.normalize;
		case GL_RESCALE_NORMAL: return &g_caps.rescaleNormal;
		case GL_SCISSOR_TEST: return &g_caps.scissor;
		case GL_LIGHT0: return &g_light[0].on;
		case GL_LIGHT1: return &g_light[1].on;
		case GL_TEXTURE_GEN_S: return &g_texGen[g_activeUnit].on[0];
		case GL_TEXTURE_GEN_T: return &g_texGen[g_activeUnit].on[1];
		case GL_TEXTURE_GEN_R: return &g_texGen[g_activeUnit].on[2];
		case GL_TEXTURE_GEN_Q: return &g_texGen[g_activeUnit].on[3];
		default: return 0;
	}
}
static void _setCap(int cap, bool on) {
	bool* flag = _capFlag(cap);
	if (!flag) {
		if (cap == GL_DITHER || cap == GL_LINE_SMOOTH || cap == GL_POINT_SMOOTH || cap == GL_POLYGON_SMOOTH || cap == GL_STENCIL_TEST)
			J2X_NOOP("dither / smoothing / stencil: no visible effect the corpus relies on");
		else J2X_UNHANDLED(on ? "glEnable" : "glDisable", cap);
		return;
	}
	if (*flag == on) return;
	_stateChange();
	*flag = on;   // (read at the next draw: _renderState / _drawInputs)
}
void glEnable(int cap) { _setCap(cap, true); }
void glDisable(int cap) { _setCap(cap, false); }
bool glIsEnabled(int cap) {
	bool* flag = _capFlag(cap);
	if (flag) return *flag;
	if (cap == GL_VERTEX_ARRAY) return g_va.on;
	if (cap == GL_NORMAL_ARRAY) return g_na.on;
	if (cap == GL_COLOR_ARRAY) return g_ca_.on;
	if (cap == GL_TEXTURE_COORD_ARRAY) return g_ta[g_clientUnit].on;
	return false;
}
void glBlendFunc(int s, int d) {
	if (s == g_blendSrc && d == g_blendDst) return;
	_stateChange();
	g_blendSrc = s; g_blendDst = d;
}
void glDepthFunc(int f) { if (f == g_depthFunc) return; _stateChange(); g_depthFunc = f; }
void glDepthMask(bool flag) { if (flag == g_depthMask) return; _stateChange(); g_depthMask = flag; }
void glColorMask(bool r, bool g, bool b, bool a) {
	_stateChange();
	g_colorMask[0] = r; g_colorMask[1] = g; g_colorMask[2] = b; g_colorMask[3] = a;
}
void glCullFace(int mode) { _stateChange(); g_cullFace = mode; }
void glFrontFace(int mode) { _stateChange(); g_frontFace = mode; }
void glAlphaFunc(int func, float ref) {
	if (func < GL_NEVER || func > GL_ALWAYS) { J2X_UNHANDLED("glAlphaFunc", func); return; }
	_stateChange();
	g_alphaFunc = func;
	g_alphaRef = _clamp01(ref);
}
void glShadeModel(int mode) {
	if (mode != GL_FLAT && mode != GL_SMOOTH) { J2X_UNHANDLED("glShadeModel", mode); return; }
	const bool flat = (mode == GL_FLAT);
	if (flat == g_flat) return;
	_stateChange();
	g_flat = flat;
}
void glClearColor(float r, float g, float b, float a) { g_clearR = _clamp01(r); g_clearG = _clamp01(g); g_clearB = _clamp01(b); g_clearA = _clamp01(a); }
void glClearDepth(double d) { g_clearDepth = _clamp01((float)d); }
// GL clears the whole target (not just the viewport) where the colour and depth masks let it, inside the scissor box when that
// test is on.
void glClear(int mask) {
	_stateChange();
	if (!gpu::ready()) return;
	bool color = (mask & GL_COLOR_BUFFER_BIT) != 0, depth = (mask & GL_DEPTH_BUFFER_BIT) != 0;
	const bool stencil = (mask & GL_STENCIL_BUFFER_BIT) != 0;
	if (!g_depthMask) depth = false;
	if (!g_colorMask[0] && !g_colorMask[1] && !g_colorMask[2] && !g_colorMask[3]) color = false;
	else if (color && !(g_colorMask[0] && g_colorMask[1] && g_colorMask[2] && g_colorMask[3])) J2X_UNHANDLED("glClear under a partial colour mask (clears every channel)", 0);
	if (!color && !depth && !stencil) return;
	gpu::clear(color, depth, stencil, g_clearR, g_clearG, g_clearB, g_clearA, g_clearDepth, _renderState());
}
void glHint(int, int) { J2X_NOOP("hints are hints"); }
void glPolygonOffset(float factor, float units) { _stateChange(); g_polyFactor = factor; g_polyUnits = units; }
void glLineWidth(float) { J2X_NOOP("Direct3D draws one-pixel lines"); }
void glPointSize(float) { J2X_NOOP("points are not drawn"); }
void glColorMaterial(int face, int mode) {
	(void)face;   // (one material for both faces: two-sided lighting is off)
	if (mode == GL_AMBIENT || mode == GL_DIFFUSE || mode == GL_AMBIENT_AND_DIFFUSE) { _stateChange(); g_colorMaterialMode = mode; }
	else if (mode == GL_EMISSION || mode == GL_SPECULAR) J2X_UNHANDLED("glColorMaterial: emission / specular", mode);
	else J2X_UNHANDLED("glColorMaterial", mode);
}

// =========================================================================== fog and lighting
void glFogf(int pname, float param) {
	_stateChange();
	switch (pname) {
		case GL_FOG_START: g_fogStart = param; break;
		case GL_FOG_END: g_fogEnd = param; break;
		case GL_FOG_DENSITY: g_fogDensity = param < 0 ? 0 : param; break;
		case GL_FOG_MODE: glFogi(pname, (int)param); break;
		case GL_FOG_INDEX: J2X_NOOP("colour-index fog: RGBA mode only"); break;
		default: J2X_UNHANDLED("glFogf", pname); break;
	}
}
void glFogi(int pname, int param) {
	if (pname != GL_FOG_MODE) { glFogf(pname, (float)param); return; }
	_stateChange();
	if (param == GL_LINEAR || param == GL_EXP || param == GL_EXP2) g_fogMode = param;
	else J2X_UNHANDLED("glFogi(GL_FOG_MODE)", param);
}
void glFog(int pname, FloatBuffer* p) {
	if (!p) return;
	_stateChange();
	const int at = p->position();
	if (pname == GL_FOG_COLOR) { for (int i = 0; i < 4; ++i) g_fogColor[i] = _clamp01(p->get(at + i)); return; }
	glFogf(pname, p->get(at));
}
static Light* _light(int light) { return (light == GL_LIGHT0) ? &g_light[0] : (light == GL_LIGHT1) ? &g_light[1] : 0; }
void glLight(int light, int pname, FloatBuffer* p) {
	Light* L = _light(light);
	if (!L || !p) { J2X_UNHANDLED("glLight: light", light); return; }
	_stateChange();
	const int at = p->position();
	float v[4];
	for (int i = 0; i < 4; ++i) v[i] = p->get(at + i);
	switch (pname) {
		case GL_POSITION: {   // into eye space by the modelview current now (GL's rule)
			const Mat& mv = _top(g_mv);
			for (int c = 0; c < 4; ++c) L->pos[c] = v[0] * mv.m[0][c] + v[1] * mv.m[1][c] + v[2] * mv.m[2][c] + v[3] * mv.m[3][c];
			if (L->pos[3] == 0.0f) {   // a direction: kept unit length (the shader takes it as is)
				const float len = sqrtf(L->pos[0] * L->pos[0] + L->pos[1] * L->pos[1] + L->pos[2] * L->pos[2]);
				if (len > 0) { L->pos[0] /= len; L->pos[1] /= len; L->pos[2] /= len; }
			} else if (L->pos[3] != 1.0f) {
				L->pos[0] /= L->pos[3]; L->pos[1] /= L->pos[3]; L->pos[2] /= L->pos[3]; L->pos[3] = 1.0f;
			}
			break;
		}
		case GL_DIFFUSE: memcpy(L->diffuse, v, 16); break;
		case GL_AMBIENT: memcpy(L->ambient, v, 16); break;
		case GL_SPECULAR: memcpy(L->specular, v, 16); J2X_NOOP("specular: no game in the corpus lights with it"); break;
		default: J2X_UNHANDLED("glLight", pname); break;
	}
}
void glLightf(int light, int pname, float param) {
	if (!_light(light)) { J2X_UNHANDLED("glLightf: light", light); return; }
	if (pname == GL_CONSTANT_ATTENUATION || pname == GL_LINEAR_ATTENUATION || pname == GL_QUADRATIC_ATTENUATION ||
	    pname == GL_SPOT_EXPONENT || pname == GL_SPOT_CUTOFF) {
		if (!(pname == GL_CONSTANT_ATTENUATION && param == 1.0f) && !(pname == GL_SPOT_CUTOFF && param == 180.0f) &&
		    !((pname == GL_LINEAR_ATTENUATION || pname == GL_QUADRATIC_ATTENUATION || pname == GL_SPOT_EXPONENT) && param == 0.0f))
			J2X_UNHANDLED("glLightf: attenuation / spot (GL's defaults only)", pname);
		return;
	}
	J2X_UNHANDLED("glLightf", pname);
}
void glLightModel(int pname, FloatBuffer* p) {
	if (!p) return;
	if (pname == GL_LIGHT_MODEL_AMBIENT) {
		_stateChange();
		const int at = p->position();
		for (int i = 0; i < 4; ++i) g_sceneAmbient[i] = p->get(at + i);
		return;
	}
	glLightModelf(pname, p->get(p->position()));
}
void glLightModelf(int pname, float param) {
	// The local viewer moves only the specular highlight, and the material's specular is GL's default black (no glMaterial here):
	// it changes no pixel, either way. Indev turns it on.
	if (pname == GL_LIGHT_MODEL_LOCAL_VIEWER) return;
	if (pname == GL_LIGHT_MODEL_TWO_SIDE) {
		if (param != 0.0f) J2X_UNHANDLED("glLightModel: two-sided lighting", pname);
		return;
	}
	J2X_UNHANDLED("glLightModelf", pname);
}
void glLightModeli(int pname, int param) { glLightModelf(pname, (float)param); }
// =========================================================================== texture coordinate generation
static int _texGenCoord(int coord) { return coord == GL_S ? 0 : coord == GL_T ? 1 : coord == GL_R ? 2 : coord == GL_Q ? 3 : -1; }
void glTexGeni(int coord, int pname, int param) {
	const int i = _texGenCoord(coord);
	if (i < 0 || pname != GL_TEXTURE_GEN_MODE) { J2X_UNHANDLED("glTexGeni", pname); return; }
	if (param != GL_OBJECT_LINEAR && param != GL_EYE_LINEAR) { J2X_UNHANDLED("glTexGeni mode (sphere map)", param); return; }
	_stateChange();
	g_texGen[g_activeUnit].mode[i] = param;
}
void glTexGen(int coord, int pname, FloatBuffer* params) {
	const int i = _texGenCoord(coord);
	if (i < 0 || !params || (pname != GL_OBJECT_PLANE && pname != GL_EYE_PLANE)) { J2X_UNHANDLED("glTexGen", pname); return; }
	_stateChange();
	const int at = params->position();
	float p[4];
	for (int k = 0; k < 4; ++k) p[k] = params->get(at + k);
	TexGen& g = g_texGen[g_activeUnit];
	if (pname == GL_OBJECT_PLANE) { memcpy(g.object[i], p, 16); return; }
	// GL: the eye plane is (p1 p2 p3 p4) times the inverse of the modelview now -- in this file's row convention, the inverse
	// times the plane as a column -- so a point's eye coordinates dotted with it give the plane of its object coordinates.
	Mat inv;
	if (!_inverse(inv, _top(g_mv))) inv = _identity();
	for (int r = 0; r < 4; ++r) g.eye[i][r] = inv.m[r][0] * p[0] + inv.m[r][1] * p[1] + inv.m[r][2] * p[2] + inv.m[r][3] * p[3];
}

void glTexEnvi(int target, int pname, int param) {
	if (target != GL_TEXTURE_ENV || pname != GL_TEXTURE_ENV_MODE) { J2X_UNHANDLED("glTexEnvi", pname); return; }
	if (param != GL_MODULATE && param != GL_REPLACE && param != GL_ADD && param != GL_DECAL) { J2X_UNHANDLED("glTexEnvi mode", param); return; }
	_stateChange();
	g_texEnv[g_activeUnit] = param;
}

// =========================================================================== textures
int glGenTextures() {
	if (!g_freeTexNames.empty()) { const int id = g_freeTexNames.back(); g_freeTexNames.pop_back(); g_textures[id] = GLTex(); g_textures[id].used = true; return id; }
	g_textures.push_back(GLTex());
	g_textures.back().used = true;
	return (int)g_textures.size() - 1;
}
void glGenTextures(IntBuffer* buf) {
	if (!buf) return;
	for (int i = buf->position(); i < buf->limit(); ++i) buf->put(i, glGenTextures());
}
static void _releaseTex(GLTex& t) {
	if (!t.tex) return;
	gpu::releaseTexture(t.tex);   // (the backend unbinds it first: never released while bound)
	t.tex = 0;
}
void glDeleteTextures(int id) {
	GLTex* t = _tex(id);
	if (!t || !t->used) return;
	_stateChange();
	_releaseTex(*t);
	*t = GLTex();
	for (int u = 0; u < kUnits; ++u) if (g_bound[u] == id) g_bound[u] = 0;   // (GL: deleting a bound texture binds 0)
	g_freeTexNames.push_back(id);
}
void glDeleteTextures(IntBuffer* buf) {
	if (!buf) return;
	for (int i = buf->position(); i < buf->limit(); ++i) glDeleteTextures(buf->get(i));
}
void glBindTexture(int, int id) {
	if (id < 0) return;
	if (id != g_bound[g_activeUnit]) _stateChange();
	if (id >= (int)g_textures.size()) g_textures.resize(id + 1);   // (GL: binding an unused name makes it)
	if (id > 0) g_textures[id].used = true;
	g_bound[g_activeUnit] = id;
	if (g_recording && g_activeUnit == 0) g_lists[g_recording].ownsTex = true;
}
bool glIsTexture(int id) { GLTex* t = _tex(id); return t && t->used; }
void glTexParameteri(int, int pname, int param) {
	GLTex* t = _boundTex();
	if (!t) return;
	_stateChange();
	switch (pname) {
		case GL_TEXTURE_WRAP_S: t->wrapS = param; break;
		case GL_TEXTURE_WRAP_T: t->wrapT = param; break;
		case GL_TEXTURE_MIN_FILTER: t->minF = param; break;
		case GL_TEXTURE_MAG_FILTER: t->magF = param; break;
		case GL_TEXTURE_MAX_LEVEL: case GL_TEXTURE_BASE_LEVEL: case GL_GENERATE_MIPMAP:
			J2X_NOOP("the levels uploaded are the levels sampled"); break;
		default: J2X_UNHANDLED("glTexParameteri", pname); break;
	}
}
void glTexParameterf(int target, int pname, float param) { glTexParameteri(target, pname, (int)param); }
void glPixelStorei(int pname, int param) {
	if (pname == GL_UNPACK_ALIGNMENT) g_unpackAlign = (param == 1 || param == 2 || param == 4 || param == 8) ? param : 4;
	else if (pname == GL_PACK_ALIGNMENT) g_packAlign = (param == 1 || param == 2 || param == 4 || param == 8) ? param : 4;
	else J2X_UNHANDLED("glPixelStorei", pname);
}

// --------------------------------------------------------------------------- pixel conversion: GL formats -> ARGB
// Bytes are read as bytes and packed ints as ints (tex-endian-byte-read): the same source pixels give the same colours on the
// big-endian 360 and the little-endian PC.
static int _bytesPerPixel(int format, int type) {
	if (type == GL_UNSIGNED_INT_8_8_8_8 || type == GL_UNSIGNED_INT_8_8_8_8_REV || type == GL_INT || type == GL_UNSIGNED_INT) return 4;
	switch (format) {
		case GL_RGBA: case GL_BGRA: return 4;
		case GL_RGB: case GL_BGR: return 3;
		case GL_LUMINANCE_ALPHA: return 2;
		case GL_ALPHA: case GL_LUMINANCE: return 1;
		default: return 0;
	}
}
static unsigned int _pixel(const unsigned char* p, int format, int type) {
	if (type == GL_UNSIGNED_INT_8_8_8_8_REV || type == GL_UNSIGNED_INT_8_8_8_8) {
		unsigned int v; memcpy(&v, p, 4);   // a packed int, in this machine's order (as the game's IntBuffer put it)
		unsigned int c0, c1, c2, c3;        // the components in GL's order (first = R for RGBA, B for BGRA)
		if (type == GL_UNSIGNED_INT_8_8_8_8_REV) { c0 = v & 255; c1 = (v >> 8) & 255; c2 = (v >> 16) & 255; c3 = v >> 24; }
		else { c0 = v >> 24; c1 = (v >> 16) & 255; c2 = (v >> 8) & 255; c3 = v & 255; }
		if (format == GL_BGRA) return _argb(c3, c2, c1, c0);
		return _argb(c3, c0, c1, c2);
	}
	switch (format) {
		case GL_RGBA: return _argb(p[3], p[0], p[1], p[2]);
		case GL_BGRA: return _argb(p[3], p[2], p[1], p[0]);
		case GL_RGB: return _argb(255, p[0], p[1], p[2]);
		case GL_BGR: return _argb(255, p[2], p[1], p[0]);
		case GL_LUMINANCE: return _argb(255, p[0], p[0], p[0]);
		case GL_LUMINANCE_ALPHA: return _argb(p[1], p[0], p[0], p[0]);
		case GL_ALPHA: return _argb(p[0], 255, 255, 255);   // (GL: an alpha texture leaves the fragment's colour as it is)
		default: return 0;
	}
}
static int _rowBytes(int w, int bpp, int align) { const int raw = w * bpp; return (raw + align - 1) / align * align; }

// Make (or remake) a texture of w x h with `levels` levels, keeping the levels it already had when only adding levels.
static bool _ensureTexture(GLTex* t, int w, int h, int levels) {
	if (!gpu::ready() || w <= 0 || h <= 0) return false;
	if (t->tex && t->w == w && t->h == h && t->levels >= levels) return true;
	gpu::Texture made = gpu::createTexture(w, h, levels);
	if (!made) { DbgPrintF("[J2X] %s: a %dx%d texture could not be made\n", gpu::name(), w, h); return t->tex != 0; }
	if (t->tex && t->w == w && t->h == h) {   // carry the levels already uploaded over to the deeper texture
		for (int l = 0; l < t->levels && l < levels; ++l) {
			std::vector<unsigned int> px;
			int lw = 0, lh = 0;
			if (gpu::readTexture(t->tex, l, px, lw, lh) && !px.empty()) gpu::writeTexture(made, l, 0, 0, lw, lh, &px[0]);
		}
	}
	_releaseTex(*t);
	t->tex = made;
	t->w = w; t->h = h; t->levels = levels;
	return true;
}
static int _levelSize(int base, int level) { int s = base >> level; return s > 0 ? s : 1; }

// Write w x h pixels at (xoff, yoff) of `level` from GL-format source bytes (`avail` of them).
static void _writePixels(GLTex* t, int level, int xoff, int yoff, int w, int h, int format, int type, const unsigned char* src, int avail) {
	if (!t || !t->tex || w <= 0 || h <= 0) return;
	const int bpp = _bytesPerPixel(format, type);
	if (!bpp) { J2X_UNHANDLED("glTexImage2D format", format); return; }
	const int stride = _rowBytes(w, bpp, g_unpackAlign);
	if (src && avail < stride * (h - 1) + w * bpp) { J2X_UNHANDLED("glTexImage2D: the buffer holds less than the image", avail); return; }
	std::vector<unsigned int> px((size_t)w * h, 0u);   // (GL: a null image allocates, contents undefined)
	if (src)
		for (int y = 0; y < h; ++y) {
			const unsigned char* row = src + y * stride;
			for (int x = 0; x < w; ++x) px[(size_t)y * w + x] = _pixel(row + x * bpp, format, type);
		}
	gpu::writeTexture(t->tex, level, xoff, yoff, w, h, &px[0]);
}
static void _texImage(int level, int w, int h, int format, int type, const unsigned char* src, int avail) {
	GLTex* t = _boundTex();
	if (!t || w <= 0 || h <= 0) return;
	_stateChange();
	if (level == 0) {
		if (!t->tex || t->w != w || t->h != h) { _releaseTex(*t); t->levels = 0; }
		if (!_ensureTexture(t, w, h, t->levels > 1 ? t->levels : 1)) return;
	} else {
		if (!t->tex) { J2X_UNHANDLED("glTexImage2D: a mip level before level 0", level); return; }
		if (_levelSize(t->w, level) != w || _levelSize(t->h, level) != h) { J2X_UNHANDLED("glTexImage2D: level size", level); return; }
		if (!_ensureTexture(t, t->w, t->h, level + 1)) return;
	}
	_writePixels(t, level, 0, 0, w, h, format, type, src, avail);
}
static void _texSubImage(int level, int xoff, int yoff, int w, int h, int format, int type, const unsigned char* src, int avail) {
	GLTex* t = _boundTex();
	if (!t || !t->tex || !src) return;
	if (level >= t->levels || xoff < 0 || yoff < 0 || xoff + w > _levelSize(t->w, level) || yoff + h > _levelSize(t->h, level)) {
		J2X_UNHANDLED("glTexSubImage2D: outside the texture", level);
		return;
	}
	_stateChange();
	_writePixels(t, level, xoff, yoff, w, h, format, type, src, avail);
}
static const unsigned char* _bytes(Buffer* b) { return b ? (const unsigned char*)b->base() : 0; }
void glTexImage2D(int, int level, int, int w, int h, int, int format, int type, ByteBuffer* p) {
	_texImage(level, w, h, format, type, _bytes(p), p ? p->remaining() : 0);
}
void glTexImage2D(int, int level, int, int w, int h, int, int format, int type, IntBuffer* p) {
	_texImage(level, w, h, format, type, _bytes(p), p ? p->remaining() * 4 : 0);
}
void glTexSubImage2D(int, int level, int x, int y, int w, int h, int format, int type, ByteBuffer* p) {
	_texSubImage(level, x, y, w, h, format, type, _bytes(p), p ? p->remaining() : 0);
}
void glTexSubImage2D(int, int level, int x, int y, int w, int h, int format, int type, IntBuffer* p) {
	_texSubImage(level, x, y, w, h, format, type, _bytes(p), p ? p->remaining() * 4 : 0);
}

// --------------------------------------------------------------------------- reading the frame
// The frame as drawn so far, ARGB rows top to bottom (GL's (x, y) is from the bottom-left).
static bool _readFrame(std::vector<unsigned int>& out, int& fw, int& fh) {
	if (!gpu::ready()) return false;
	_flushBatch();
	return gpu::readFrame(out, fw, fh);
}
void glCopyTexSubImage2D(int, int level, int xoff, int yoff, int x, int y, int w, int h) {
	GLTex* t = _boundTex();
	if (!t || !t->tex || w <= 0 || h <= 0) return;
	std::vector<unsigned int> frame;
	int fw = 0, fh = 0;
	if (!_readFrame(frame, fw, fh)) return;
	if (level >= t->levels || xoff < 0 || yoff < 0 || xoff + w > _levelSize(t->w, level) || yoff + h > _levelSize(t->h, level)) {
		J2X_UNHANDLED("glCopyTexSubImage2D: outside the texture", level);
		return;
	}
	std::vector<unsigned int> px((size_t)w * h);
	for (int r = 0; r < h; ++r) {      // texture row yoff+r is the frame's GL row y+r (counted from the bottom)
		const int fy = fh - 1 - (y + r);
		for (int c = 0; c < w; ++c) {
			const int fx = x + c;
			px[(size_t)r * w + c] = (fx >= 0 && fx < fw && fy >= 0 && fy < fh) ? (frame[(size_t)fy * fw + fx] | 0xFF000000u) : 0xFF000000u;
		}
	}
	_stateChange();
	gpu::writeTexture(t->tex, level, xoff, yoff, w, h, &px[0]);
}
static void _readPixels(int x, int y, int w, int h, int format, int type, unsigned char* dst, int avail) {
	if (!dst || w <= 0 || h <= 0) return;
	const int bpp = _bytesPerPixel(format, type);
	if (!bpp || (type != GL_UNSIGNED_BYTE && type != GL_UNSIGNED_INT_8_8_8_8_REV && type != GL_UNSIGNED_INT_8_8_8_8)) {
		J2X_UNHANDLED("glReadPixels format/type", format);
		return;
	}
	const int stride = _rowBytes(w, bpp, g_packAlign);
	if (avail < stride * (h - 1) + w * bpp) { J2X_UNHANDLED("glReadPixels: the buffer is smaller than the image", avail); return; }
	std::vector<unsigned int> frame;
	int fw = 0, fh = 0;
	if (!_readFrame(frame, fw, fh)) return;
	for (int r = 0; r < h; ++r) {      // GL rows run bottom-up
		unsigned char* row = dst + r * stride;
		const int fy = fh - 1 - (y + r);
		for (int c = 0; c < w; ++c) {
			const int fx = x + c;
			const unsigned int p = (fx >= 0 && fx < fw && fy >= 0 && fy < fh) ? frame[(size_t)fy * fw + fx] : 0;
			const unsigned char R = (unsigned char)(p >> 16), G = (unsigned char)(p >> 8), B = (unsigned char)p, A = 255;
			unsigned char* o = row + c * bpp;
			if (type != GL_UNSIGNED_BYTE) {
				unsigned int v;
				const unsigned int c0 = format == GL_BGRA ? B : R, c2 = format == GL_BGRA ? R : B;
				if (type == GL_UNSIGNED_INT_8_8_8_8_REV) v = c0 | (G << 8) | (c2 << 16) | ((unsigned int)A << 24);
				else v = (c0 << 24) | (G << 16) | (c2 << 8) | A;
				memcpy(o, &v, 4);
				continue;
			}
			switch (format) {
				case GL_RGBA: o[0] = R; o[1] = G; o[2] = B; o[3] = A; break;
				case GL_BGRA: o[0] = B; o[1] = G; o[2] = R; o[3] = A; break;
				case GL_RGB: o[0] = R; o[1] = G; o[2] = B; break;
				case GL_BGR: o[0] = B; o[1] = G; o[2] = R; break;
				case GL_LUMINANCE: o[0] = (unsigned char)((R * 77 + G * 150 + B * 29) >> 8); break;
				case GL_LUMINANCE_ALPHA: o[0] = (unsigned char)((R * 77 + G * 150 + B * 29) >> 8); o[1] = A; break;
				case GL_ALPHA: o[0] = A; break;
				default: break;
			}
		}
	}
}
void glReadPixels(int x, int y, int w, int h, int format, int type, ByteBuffer* p) {
	if (p) _readPixels(x, y, w, h, format, type, (unsigned char*)p->base(), p->remaining());
}
void glReadPixels(int x, int y, int w, int h, int format, int type, IntBuffer* p) {
	if (p) _readPixels(x, y, w, h, format, type, (unsigned char*)p->base(), p->remaining() * 4);
}

// =========================================================================== multitexture
static int _unit(int texture) {
	const int u = texture - GL_TEXTURE0;
	if (u < 0 || u >= kUnits) { J2X_UNHANDLED("texture unit", texture); return -1; }
	return u;
}
void glActiveTexture(int texture) { const int u = _unit(texture); if (u >= 0 && u != g_activeUnit) { _stateChange(); g_activeUnit = u; } }
void glClientActiveTexture(int texture) { const int u = _unit(texture); if (u >= 0) g_clientUnit = u; }
void glMultiTexCoord2f(int target, float s, float t) { const int u = _unit(target); if (u >= 0) { g_tu[u] = s; g_tv[u] = t; } }

// =========================================================================== vertex arrays
static Array* _clientArray(int cap) {
	switch (cap) {
		case GL_VERTEX_ARRAY: return &g_va;
		case GL_NORMAL_ARRAY: return &g_na;
		case GL_COLOR_ARRAY: return &g_ca_;
		case GL_TEXTURE_COORD_ARRAY: return &g_ta[g_clientUnit];
		default: return 0;
	}
}
void glEnableClientState(int cap) { Array* a = _clientArray(cap); if (a) a->on = true; else J2X_UNHANDLED("glEnableClientState", cap); }
void glDisableClientState(int cap) { Array* a = _clientArray(cap); if (a) a->on = false; else J2X_UNHANDLED("glDisableClientState", cap); }
void glVertexPointer(int size, int stride, FloatBuffer* buf) { _setArray(g_va, size, GL_FLOAT, stride, buf ? buf->base() : 0); }
void glColorPointer(int size, int stride, FloatBuffer* buf) { _setArray(g_ca_, size, GL_FLOAT, stride, buf ? buf->base() : 0); }
void glColorPointer(int size, int stride, ByteBuffer* buf) { _setArray(g_ca_, size, GL_UNSIGNED_BYTE, stride, buf ? buf->base() : 0); }
void glColorPointer(int size, bool unsignedBytes, int stride, ByteBuffer* buf) {
	_setArray(g_ca_, size, unsignedBytes ? GL_UNSIGNED_BYTE : GL_BYTE, stride, buf ? buf->base() : 0);
}
void glTexCoordPointer(int size, int stride, FloatBuffer* buf) { _setArray(g_ta[g_clientUnit], size, GL_FLOAT, stride, buf ? buf->base() : 0); }
void glTexCoordPointer(int size, int stride, ShortBuffer* buf) { _setArray(g_ta[g_clientUnit], size, GL_SHORT, stride, buf ? buf->base() : 0); }
void glNormalPointer(int stride, ByteBuffer* buf) { _setArray(g_na, 3, GL_BYTE, stride, buf ? buf->base() : 0); }
void glNormalPointer(int stride, FloatBuffer* buf) { _setArray(g_na, 3, GL_FLOAT, stride, buf ? buf->base() : 0); }
// The VBO-offset forms: the layer reports no vertex buffer objects, so these are reached only if a game ignores that; treating an
// offset as a pointer would read wild memory, so they say so instead.
void glVertexPointer(int, int, int, long long)   { J2X_UNHANDLED("glVertexPointer(VBO offset)", 0); }
void glColorPointer(int, int, int, long long)    { J2X_UNHANDLED("glColorPointer(VBO offset)", 0); }
void glTexCoordPointer(int, int, int, long long) { J2X_UNHANDLED("glTexCoordPointer(VBO offset)", 0); }
void glNormalPointer(int, int, long long)        { J2X_UNHANDLED("glNormalPointer(VBO offset)", 0); }

static float _comp(const char* p, int type, int i) {
	switch (type) {
		case GL_FLOAT: { float f; memcpy(&f, p + i * 4, 4); return f; }
		case GL_DOUBLE: { double d; memcpy(&d, p + i * 8, 8); return (float)d; }
		case GL_SHORT: { short s; memcpy(&s, p + i * 2, 2); return (float)s; }
		case GL_UNSIGNED_SHORT: { unsigned short s; memcpy(&s, p + i * 2, 2); return (float)s; }
		case GL_INT: { int v; memcpy(&v, p + i * 4, 4); return (float)v; }
		case GL_BYTE: return (float)(signed char)p[i];
		case GL_UNSIGNED_BYTE: return (float)(unsigned char)p[i];
		default: return 0.0f;
	}
}

// One vertex from the enabled arrays (what is not enabled comes from the current values, as in GL).
static GLVert _fetch(int idx, bool& colorArray) {
	GLVert v = _current(0, 0, 0);
	if (g_va.on && g_va.ptr) {
		const char* p = g_va.ptr + (size_t)idx * g_va.stride;
		v.x = _comp(p, g_va.type, 0); v.y = _comp(p, g_va.type, 1);
		v.z = g_va.size >= 3 ? _comp(p, g_va.type, 2) : 0.0f;
		if (g_va.size >= 4) { const float w = _comp(p, g_va.type, 3); if (w != 0.0f && w != 1.0f) { v.x /= w; v.y /= w; v.z /= w; } }
	}
	if (g_na.on && g_na.ptr) {
		const char* p = g_na.ptr + (size_t)idx * g_na.stride;
		// Signed integers become floats by GL 1.x's rule, (2c + 1) / (2^b - 1): a zero byte is 1/255, not 0. That matters: the Beta
		// Tessellator's quad-to-triangle copies never write their normal bytes, and GL lights them as the small vector those
		// zeros make once normalized; read as 0 they lit ambient-only (a dark wedge across every face of the first-person arm).
		if (g_na.type == GL_BYTE || g_na.type == GL_SHORT) {
			const float range = g_na.type == GL_BYTE ? 255.0f : 65535.0f;
			v.nx = (2.0f * _comp(p, g_na.type, 0) + 1.0f) / range;
			v.ny = (2.0f * _comp(p, g_na.type, 1) + 1.0f) / range;
			v.nz = (2.0f * _comp(p, g_na.type, 2) + 1.0f) / range;
		} else {
			v.nx = _comp(p, g_na.type, 0); v.ny = _comp(p, g_na.type, 1); v.nz = _comp(p, g_na.type, 2);
		}
	}
	for (int u = 0; u < kUnits; ++u) {
		const Array& a = g_ta[u];
		if (!a.on || !a.ptr) continue;
		const char* p = a.ptr + (size_t)idx * a.stride;
		const float s = _comp(p, a.type, 0), t = a.size >= 2 ? _comp(p, a.type, 1) : 0.0f;
		if (u == 0) { v.u = s; v.v = t; } else { v.u2 = s; v.v2 = t; }
	}
	if (g_ca_.on && g_ca_.ptr) {
		colorArray = true;
		const char* p = g_ca_.ptr + (size_t)idx * g_ca_.stride;
		if (g_ca_.type == GL_UNSIGNED_BYTE && g_ca_.size >= 4) {
			// A packed int in this machine's order (the Tessellator's 0xAABBGGRR through IntBuffer::put): read as an int and taken
			// apart by shifts, so the 360 and the PC agree (color-endian-int-read; on the PC it is GL's byte order R, G, B, A).
			unsigned int packed; memcpy(&packed, p, 4);
			v.color = _argb((unsigned char)(packed >> 24), (unsigned char)packed, (unsigned char)(packed >> 8), (unsigned char)(packed >> 16));
		} else if (g_ca_.type == GL_UNSIGNED_BYTE) {
			const unsigned char* b = (const unsigned char*)p;
			v.color = _argb(255, b[0], b[1], b[2]);
		} else {
			float c[4];
			for (int i = 0; i < 4; ++i) {
				c[i] = (i < g_ca_.size) ? _comp(p, g_ca_.type, i) : 1.0f;
				if (i < g_ca_.size && g_ca_.type == GL_BYTE) c[i] = (2.0f * c[i] + 1.0f) / 255.0f;   // (GL 1.x's signed rule)
			}
			v.color = _argb(_u8(c[3]), _u8(c[0]), _u8(c[1]), _u8(c[2]));
		}
	}
	return v;
}
static bool _arraysUsable(int count) {
	if (count <= 0) return false;
	if (count > kMaxVerts) { DbgPrintF("[J2X] draw of %d vertices refused (a corrupted count)\n", count); return false; }
	if (!g_va.on || !g_va.ptr) { J2X_UNHANDLED("draw with no vertex array", 0); return false; }
	return true;
}
void glDrawArrays(int mode, int first, int count) {
	if (first < 0 || !_arraysUsable(count)) return;
	std::vector<GLVert> v;
	v.reserve(count);
	bool colorArray = false;
	for (int i = 0; i < count; ++i) v.push_back(_fetch(first + i, colorArray));
	_submit(mode, &v[0], count, false, colorArray);
}
static void _drawIndexed(int mode, const char* idx, int type, int count) {
	if (!idx || !_arraysUsable(count)) return;
	std::vector<GLVert> v;
	v.reserve(count);
	bool colorArray = false;
	for (int i = 0; i < count; ++i) {
		const int k = type == GL_UNSIGNED_BYTE ? (int)(unsigned char)idx[i] : type == GL_UNSIGNED_SHORT ? (int)((const unsigned short*)idx)[i] : ((const int*)idx)[i];
		if (k < 0) continue;
		v.push_back(_fetch(k, colorArray));
	}
	if (!v.empty()) _submit(mode, &v[0], (int)v.size(), false, colorArray);
}
void glDrawElements(int mode, IntBuffer* indices) { if (indices) _drawIndexed(mode, indices->base(), GL_UNSIGNED_INT, indices->remaining()); }
void glDrawElements(int mode, ShortBuffer* indices) { if (indices) _drawIndexed(mode, indices->base(), GL_UNSIGNED_SHORT, indices->remaining()); }
void glDrawElements(int mode, ByteBuffer* indices) { if (indices) _drawIndexed(mode, indices->base(), GL_UNSIGNED_BYTE, indices->remaining()); }
// glInterleavedArrays: one buffer, the arrays it names enabled at their offsets, the others disabled (GL's rule). Colours as
// C4UB are four bytes, everything else floats.
void glInterleavedArrays(int format, int stride, Buffer* pointer) {
	const char* base = pointer ? pointer->base() : 0;
	int tc = 0, cc = 0, nc = 0, vc = 0, ctype = GL_FLOAT;
	switch (format) {
		case GL_V2F: vc = 2; break;
		case GL_V3F: vc = 3; break;
		case GL_C4UB_V2F: cc = 4; ctype = GL_UNSIGNED_BYTE; vc = 2; break;
		case GL_C4UB_V3F: cc = 4; ctype = GL_UNSIGNED_BYTE; vc = 3; break;
		case GL_C3F_V3F: cc = 3; vc = 3; break;
		case GL_N3F_V3F: nc = 3; vc = 3; break;
		case GL_C4F_N3F_V3F: cc = 4; nc = 3; vc = 3; break;
		case GL_T2F_V3F: tc = 2; vc = 3; break;
		case GL_T4F_V4F: tc = 4; vc = 4; break;
		case GL_T2F_C4UB_V3F: tc = 2; cc = 4; ctype = GL_UNSIGNED_BYTE; vc = 3; break;
		case GL_T2F_C3F_V3F: tc = 2; cc = 3; vc = 3; break;
		case GL_T2F_N3F_V3F: tc = 2; nc = 3; vc = 3; break;
		case GL_T2F_C4F_N3F_V3F: tc = 2; cc = 4; nc = 3; vc = 3; break;
		case GL_T4F_C4F_N3F_V4F: tc = 4; cc = 4; nc = 3; vc = 4; break;
		default: J2X_UNHANDLED("glInterleavedArrays", format); return;
	}
	const int cbytes = cc ? (ctype == GL_UNSIGNED_BYTE ? 4 : cc * 4) : 0;
	const int total = tc * 4 + cbytes + nc * 4 + vc * 4;
	const int s = stride ? stride : total;
	int off = 0;
	Array& ta = g_ta[g_clientUnit];
	ta.on = tc > 0; if (tc) _setArray(ta, tc, GL_FLOAT, s, base + off); off += tc * 4;
	g_ca_.on = cc > 0; if (cc) _setArray(g_ca_, cc, ctype, s, base + off); off += cbytes;
	g_na.on = nc > 0; if (nc) _setArray(g_na, 3, GL_FLOAT, s, base + off); off += nc * 4;
	g_va.on = true; _setArray(g_va, vc, GL_FLOAT, s, base + off);
}

// =========================================================================== display lists
int glGenLists(int n) {
	if (n <= 0) return 0;
	if (g_lists.empty()) g_lists.push_back(GLList());
	const int base = (int)g_lists.size();
	for (int i = 0; i < n; ++i) g_lists.push_back(GLList());
	return base;
}
static void _freeListVBs(GLList& L) {
	for (size_t i = 0; i < L.batches.size(); ++i)
		if (L.batches[i].vb) { gpu::releaseBuffer(L.batches[i].vb); L.batches[i].vb = 0; }   // (the backend unbinds first)
	L.batches.clear();
}
void glNewList(int list, int mode) {
	if (list <= 0) return;
	if (g_recording) { J2X_UNHANDLED("glNewList inside glNewList", list); return; }
	_stateChange();
	while ((int)g_lists.size() <= list) g_lists.push_back(GLList());
	GLList& L = g_lists[list];
	g_recordHadVB = false;
	for (size_t i = 0; i < L.batches.size(); ++i) if (L.batches[i].vb) { g_recordHadVB = true; break; }
	_freeListVBs(L);
	L.used = true;
	L.ownsTex = false;
	L.inheritColor = true;
	L.hasXform = false;
	g_recording = list;
	g_recordExecute = (mode == GL_COMPILE_AND_EXECUTE);
	g_recordSetColor = false;
	g_recordBaseMV = _top(g_mv);
	if (!_inverse(g_recordBaseInv, g_recordBaseMV)) g_recordBaseInv = _identity();
	Saved& s = g_recordSaved;
	s.bound0 = g_bound[0]; s.tex0 = g_caps.tex[0];
	s.color[0] = g_cr; s.color[1] = g_cg; s.color[2] = g_cb; s.color[3] = g_ca;
	s.normal[0] = g_nx; s.normal[1] = g_ny; s.normal[2] = g_nz;
	for (int u = 0; u < kUnits; ++u) { s.tu[u] = g_tu[u]; s.tv[u] = g_tv[u]; }
}
// Upload a list's recorded geometry into the backend's buffers (inside gpu::beginUploads / endUploads).
static void _createListVBs(GLList& L) {
	for (size_t i = 0; i < L.batches.size(); ++i) {
		GLBatch& b = L.batches[i];
		if (b.vb || b.verts.empty()) continue;
		b.vb = gpu::createBuffer(&b.verts[0], (int)b.verts.size());
		if (b.vb) b.count = (int)b.verts.size();
		else DbgPrintF("[J2X] %s: a buffer of %d vertices could not be made\n", gpu::name(), (int)b.verts.size());
		std::vector<GLVert>().swap(b.verts);   // the GPU holds it now
	}
}
// A first build (the world loading) is made at the frame's end with every other list built that frame: one scene step for all.
// A re-build of a list already on screen (a block edit) is made now, so it never blanks for a frame.
void glEndList() {
	if (!g_recording) { J2X_UNHANDLED("glEndList without glNewList", 0); return; }
	GLList& L = g_lists[g_recording];
	if (g_recordSetColor) L.inheritColor = false;
	L.setsColor = g_recordSetColor;
	// A list that set no colour takes the one current at glCallList (the shader's tint): its vertices are made white, or the
	// colour current while it was recorded would be multiplied in too (the first-person arm came out dark brown).
	if (L.inheritColor)
		for (size_t i = 0; i < L.batches.size(); ++i)
			for (size_t k = 0; k < L.batches[i].verts.size(); ++k) L.batches[i].verts[k].color = L.batches[i].verts[k].fcolor = _argb(255, 255, 255, 255);
	bool geometry = false;
	for (size_t i = 0; i < L.batches.size(); ++i) if (!L.batches[i].verts.empty()) { geometry = true; break; }
	if (geometry && gpu::ready()) {
		if (g_recordHadVB) { gpu::beginUploads(); _createListVBs(L); gpu::endUploads(); }
		else g_pendingVB.push_back(g_recording);
	}
	// The net modelview change made inside the list (end = delta * base): FontRenderer's per-glyph advance, replayed at glCallList.
	const Mat delta = _top(g_mv) * g_recordBaseInv;
	L.xform = delta;
	L.hasXform = !_isIdentity(delta);
	L.endTex = g_bound[0];
	L.endColor[0] = g_cr; L.endColor[1] = g_cg; L.endColor[2] = g_cb; L.endColor[3] = g_ca;
	if (!g_recordExecute) {   // GL_COMPILE: what the list's commands changed is put back (they run again at glCallList)
		const Saved& s = g_recordSaved;
		_top(g_mv) = g_recordBaseMV;
		g_bound[0] = s.bound0; g_caps.tex[0] = s.tex0;
		g_cr = s.color[0]; g_cg = s.color[1]; g_cb = s.color[2]; g_ca = s.color[3];
		g_nx = s.normal[0]; g_ny = s.normal[1]; g_nz = s.normal[2];
		for (int u = 0; u < kUnits; ++u) { g_tu[u] = s.tu[u]; g_tv[u] = s.tv[u]; }
	}
	g_recording = 0;
}
static void _pumpPendingVBs() {
	if (g_pendingVB.empty()) return;
	if (!gpu::ready()) { g_pendingVB.clear(); return; }
	gpu::beginUploads();
	for (size_t k = 0; k < g_pendingVB.size(); ++k) {
		const int id = g_pendingVB[k];
		if (id > 0 && id < (int)g_lists.size()) _createListVBs(g_lists[id]);
	}
	gpu::endUploads();
	g_pendingVB.clear();
}
void glCallList(int list) {
	if (list <= 0 || list >= (int)g_lists.size() || !g_lists[list].used) return;
	if (g_recording) { J2X_UNHANDLED("glCallList inside glNewList (nested lists)", list); return; }
	_stateChange();
	GLList& L = g_lists[list];
	if (g_renderMode == GL_RENDER && gpu::ready()) {
		// A list built this frame is drawn this frame: its buffers are made now if they are still waiting.
		bool pending = false;
		for (size_t i = 0; i < L.batches.size(); ++i) if (!L.batches[i].vb && !L.batches[i].verts.empty()) { pending = true; break; }
		if (pending) { gpu::beginUploads(); _createListVBs(L); gpu::endUploads(); }
		float tint[4] = { g_cr, g_cg, g_cb, g_ca };
		const gpu::RenderState rs = _renderState();
		for (size_t i = 0; i < L.batches.size(); ++i) {
			GLBatch& b = L.batches[i];
			if (!b.vb || b.count <= 0) continue;
			gpu::Constants c;
			gpu::Bindings bind;
			_drawInputs(false, L.inheritColor ? tint : 0, b.tex, b.texOn, L.ownsTex, c, bind);
			gpu::drawBuffer(b.prim, b.vb, b.count, rs, c, bind);
		}
	}
	// What the list's own commands leave behind, as GL's replay of them would: the matrix change, the binding, the colour.
	if (L.hasXform) { Mat& m = _top(g_mv); m = L.xform * m; }
	if (L.ownsTex) g_bound[0] = L.endTex;
	if (L.setsColor) { g_cr = L.endColor[0]; g_cg = L.endColor[1]; g_cb = L.endColor[2]; g_ca = L.endColor[3]; }
}
void glCallLists(IntBuffer* lists) {
	if (!lists) return;
	const int n = lists->remaining();
	const int at = lists->position();
	for (int i = 0; i < n; ++i) glCallList(lists->get(at + i));
}
void glDeleteLists(int list, int n) {
	for (int i = 0; i < n; ++i) {
		const int id = list + i;
		if (id > 0 && id < (int)g_lists.size()) { _freeListVBs(g_lists[id]); g_lists[id] = GLList(); }
	}
}
bool glIsList(int list) { return list > 0 && list < (int)g_lists.size() && g_lists[list].used; }

// =========================================================================== selection
// A hit record (GL's): the name count, the minimum and maximum depth scaled to 0..2^32-1, then the names, written when the name
// stack changes or selection ends.
static void _writeHit() {
	if (!g_hit) return;
	g_hit = false;
	if (!g_selBuf) return;
	const int cap = g_selBuf->capacity();
	const int need = 3 + (int)g_names.size();
	if (g_selWritten + need > cap) { J2X_UNHANDLED("glSelectBuffer overflow", cap); return; }
	const unsigned int zmin = (unsigned int)(g_hitMinZ * 4294967295.0), zmax = (unsigned int)(g_hitMaxZ * 4294967295.0);
	g_selBuf->put(g_selWritten + 0, (int)g_names.size());
	g_selBuf->put(g_selWritten + 1, (int)zmin);
	g_selBuf->put(g_selWritten + 2, (int)zmax);
	for (size_t i = 0; i < g_names.size(); ++i) g_selBuf->put(g_selWritten + 3 + (int)i, g_names[i]);
	g_selWritten += need;
	++g_hitCount;
	g_hitMinZ = 1.0f; g_hitMaxZ = 0.0f;
}
void glInitNames() { if (g_renderMode == GL_SELECT) _writeHit(); g_names.clear(); }
void glPushName(int name) { if (g_renderMode == GL_SELECT) _writeHit(); g_names.push_back(name); }
void glPopName() { if (g_renderMode == GL_SELECT) _writeHit(); if (!g_names.empty()) g_names.pop_back(); }
void glLoadName(int name) {
	if (g_renderMode == GL_SELECT) _writeHit();
	if (g_names.empty()) { J2X_UNHANDLED("glLoadName on an empty name stack", name); return; }
	g_names.back() = name;
}
void glSelectBuffer(IntBuffer* buf) { g_selBuf = buf; }
int glRenderMode(int mode) {
	if (mode != GL_RENDER && mode != GL_SELECT) { J2X_UNHANDLED("glRenderMode", mode); return 0; }
	_stateChange();
	int result = 0;
	if (g_renderMode == GL_SELECT) { _writeHit(); result = g_hitCount; }
	g_renderMode = mode;
	if (mode == GL_SELECT) { g_hitCount = 0; g_selWritten = 0; g_hit = false; g_names.clear(); g_hitMinZ = 1.0f; g_hitMaxZ = 0.0f; }
	return result;
}

// =========================================================================== queries
int glGetError() { return GL_NO_ERROR; }
JavaString glGetString(int name) {
	switch (name) {
		case GL_VENDOR: return JavaString("CodeBreaker");
		case GL_RENDERER: return JavaString(gpu::name());
		case GL_VERSION: return JavaString(std::string("1.3.0 CodeBreaker ") + gpu::name());
		case GL_EXTENSIONS: return JavaString("GL_ARB_multitexture");
		default: J2X_UNHANDLED("glGetString", name); return JavaString("");
	}
}
void glFlush() { _flushBatch(); _pumpPendingVBs(); }
void glFinish() { glFlush(); }

} // namespace j2xgl
