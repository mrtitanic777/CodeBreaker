// GpuD3D8.cpp -- the GL layer's Direct3D 8 backend (Gpu.h), the PC built with --graphics d3d8. It carries out what GL11.cpp
// decided on a Direct3D 8 device the Display made, with Direct3D 8's fixed-function pipeline: GL's transforms, lights, fog,
// alpha test and texture environments are set as device state (GpuFixed.h plans them, from the same constants the shader
// backends read), so any Direct3D 8 card -- and Windows' own Direct3D 8 runtime today -- draws them.
//
// Immediate geometry goes up with each draw (DrawPrimitiveUP); display lists get managed vertex buffers, which survive a device
// Reset. A draw call takes at most kChunk vertices (Direct3D 8's cards guarantee 65535 primitives, not more), so a big list is
// a few buffers. The device's state is cached here, so a draw sets only what changed since the one before.
#define J2X_WITH_GDI   // (windows.h's GDI types, which stdafx.h keeps out of the game's files)
#include "stdafx.h"
#include "GpuD3D8.h"
#include "GpuFixed.h"
#include "Direct3D8.h"
#include "Common.h"     // DbgPrintF / J2X_UNHANDLED
#include <vector>
#include <string.h>

namespace j2xgl {
namespace gpu {

using namespace d3d8;
using fixed::Mat;
using fixed::Plan;
using fixed::FVertex;

static const int kChunk = 65532;   // vertices a draw call takes at most (a multiple of 2 and 3)

struct TextureObject {   // (gpu::Texture)
	IDirect3DTexture8* tex;
	int w, h, levels;
};
// The flat-shading values of one vertex, kept for a list whose flat values differ from its smooth ones.
struct FlatValues { float nx, ny, nz; unsigned int color; };
struct BufferObject {    // (gpu::Buffer)
	std::vector<IDirect3DVertexBuffer8*> vb;     // kChunk vertices each, the last the rest
	int count;
	std::vector<FlatValues> flatValues;          // empty: flat shading draws the same vertices
	std::vector<IDirect3DVertexBuffer8*> flat;   // the flat-shading copy, made on the list's first flat draw
};

// =========================================================================== the device
static IDirect3DDevice8* g_dev = 0;
static int g_w = 854, g_h = 480;
static bool g_inScene = false;

template<class T> static void _release(T*& p) { if (p) { p->Release(); p = 0; } }
static DWORD _f(float f) { DWORD d; memcpy(&d, &f, 4); return d; }

static void _scene() {
	if (!g_inScene && g_dev && SUCCEEDED(g_dev->BeginScene())) g_inScene = true;
}
void endSceneD3D8() {
	if (g_inScene && g_dev) g_dev->EndScene();
	g_inScene = false;
}

// =========================================================================== what is on the device
static DWORD s_rs[256];
static bool s_rsKnown[256];
static DWORD s_tss[8][32];
static bool s_tssKnown[8][32];
static IDirect3DTexture8* s_tex[8];
static bool s_texKnown[8];
static D3DMATRIX s_xf[3 + 8];   // world, view, projection, then each stage's texture matrix
static bool s_xfKnown[3 + 8];
static D3DVIEWPORT8 s_vp;
static bool s_vpKnown = false;
static D3DMATERIAL8 s_mat;
static bool s_matKnown = false;
static D3DLIGHT8 s_light[2];
static bool s_lightKnown[2], s_lightOn[2], s_lightOnKnown[2];
static bool s_fvfKnown = false;
static IDirect3DVertexBuffer8* s_stream = 0;

void forgetState() {
	memset(s_rsKnown, 0, sizeof(s_rsKnown));
	memset(s_tssKnown, 0, sizeof(s_tssKnown));
	memset(s_texKnown, 0, sizeof(s_texKnown));
	memset(s_xfKnown, 0, sizeof(s_xfKnown));
	memset(s_lightKnown, 0, sizeof(s_lightKnown));
	memset(s_lightOnKnown, 0, sizeof(s_lightOnKnown));
	s_vpKnown = s_matKnown = s_fvfKnown = false;
	s_stream = 0;
}

static void _rs(DWORD state, DWORD value) {
	if (state < 256 && s_rsKnown[state] && s_rs[state] == value) return;
	g_dev->SetRenderState(state, value);
	if (state < 256) { s_rs[state] = value; s_rsKnown[state] = true; }
}
static void _tss(DWORD stage, DWORD type, DWORD value) {
	if (s_tssKnown[stage][type] && s_tss[stage][type] == value) return;
	g_dev->SetTextureStageState(stage, type, value);
	s_tss[stage][type] = value;
	s_tssKnown[stage][type] = true;
}
static void _texture(DWORD stage, IDirect3DTexture8* t) {
	if (s_texKnown[stage] && s_tex[stage] == t) return;
	g_dev->SetTexture(stage, t);
	s_tex[stage] = t;
	s_texKnown[stage] = true;
}
static void _transform(int slot, DWORD state, const Mat& m) {
	D3DMATRIX d;
	memcpy(d.m, m.m, sizeof(d.m));
	if (s_xfKnown[slot] && memcmp(&s_xf[slot], &d, sizeof(d)) == 0) return;
	g_dev->SetTransform(state, &d);
	s_xf[slot] = d;
	s_xfKnown[slot] = true;
}
static void _viewport(int x, int y, int w, int h) {
	D3DVIEWPORT8 vp;
	vp.X = (DWORD)x; vp.Y = (DWORD)y; vp.Width = (DWORD)w; vp.Height = (DWORD)h; vp.MinZ = 0.0f; vp.MaxZ = 1.0f;
	if (s_vpKnown && memcmp(&vp, &s_vp, sizeof(vp)) == 0) return;
	g_dev->SetViewport(&vp);
	s_vp = vp;
	s_vpKnown = true;
}
static void _fvf() {
	if (s_fvfKnown) return;
	g_dev->SetVertexShader((DWORD)fixed::kFVF);
	s_fvfKnown = true;
}
static void _stream(IDirect3DVertexBuffer8* vb) {
	if (vb == s_stream) return;
	g_dev->SetStreamSource(0, vb, sizeof(FVertex));
	s_stream = vb;
}
static D3DCOLORVALUE _cv(const float* c) { D3DCOLORVALUE v; v.r = c[0]; v.g = c[1]; v.b = c[2]; v.a = c[3]; return v; }

static void _apply(const Plan& p, const RenderState& rs, const Bindings& b) {
	// what never changes
	_rs(D3DRS_FILLMODE, D3DFILL_SOLID);
	_rs(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);   // (flat shading is in the vertices: GpuFixed.h)
	_rs(D3DRS_CLIPPING, TRUE);
	_rs(D3DRS_DITHERENABLE, FALSE);
	_rs(D3DRS_STENCILENABLE, FALSE);
	_rs(D3DRS_SPECULARENABLE, FALSE);
	_rs(D3DRS_LOCALVIEWER, FALSE);
	_rs(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	_rs(D3DRS_RANGEFOGENABLE, FALSE);
	_rs(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
	_fvf();

	// depth, blending, culling (GL: with the depth test off nothing is written to depth either)
	_rs(D3DRS_ZENABLE, rs.depthTest ? D3DZB_TRUE : D3DZB_FALSE);
	_rs(D3DRS_ZWRITEENABLE, rs.depthTest && rs.depthWrite ? TRUE : FALSE);
	_rs(D3DRS_ZFUNC, (DWORD)(D3DCMP_NEVER + rs.depthFunc));
	_rs(D3DRS_ALPHABLENDENABLE, rs.blend ? TRUE : FALSE);
	if (rs.blend) {
		_rs(D3DRS_SRCBLEND, (DWORD)(D3DBLEND_ZERO + rs.srcBlend));
		_rs(D3DRS_DESTBLEND, (DWORD)(D3DBLEND_ZERO + rs.dstBlend));
	}
	_rs(D3DRS_COLORWRITEENABLE, rs.colorWrite & 15u);   // (red 1, green 2, blue 4, alpha 8: Direct3D's bits too)
	_rs(D3DRS_CULLMODE, rs.cull == kCullCW ? D3DCULL_CW : rs.cull == kCullCCW ? D3DCULL_CCW : D3DCULL_NONE);

	// transforms and viewport
	_transform(0, D3DTS_WORLD, p.world);
	_transform(1, D3DTS_VIEW, p.view);
	_transform(2, D3DTS_PROJECTION, p.proj);
	_viewport(p.vpX, p.vpY, p.vpW, p.vpH);

	// lighting
	_rs(D3DRS_LIGHTING, p.lighting ? TRUE : FALSE);
	if (p.lighting) {
		_rs(D3DRS_AMBIENT, fixed::toArgb(p.ambient));
		_rs(D3DRS_NORMALIZENORMALS, p.normalize ? TRUE : FALSE);
		_rs(D3DRS_COLORVERTEX, TRUE);
		_rs(D3DRS_AMBIENTMATERIALSOURCE, p.ambientFromColor ? D3DMCS_COLOR1 : D3DMCS_MATERIAL);
		_rs(D3DRS_DIFFUSEMATERIALSOURCE, p.diffuseFromColor ? D3DMCS_COLOR1 : D3DMCS_MATERIAL);
		_rs(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
		_rs(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
		D3DMATERIAL8 m;
		memset(&m, 0, sizeof(m));
		m.Diffuse = _cv(p.matDiffuse);
		m.Ambient = _cv(p.matAmbient);
		if (!s_matKnown || memcmp(&m, &s_mat, sizeof(m)) != 0) { g_dev->SetMaterial(&m); s_mat = m; s_matKnown = true; }
		for (int i = 0; i < 2; ++i) {
			const fixed::Light& L = p.light[i];
			bool on = L.on;
			if (on) {
				D3DLIGHT8 l;
				memset(&l, 0, sizeof(l));
				l.Type = L.directional ? D3DLIGHT_DIRECTIONAL : D3DLIGHT_POINT;
				l.Diffuse = _cv(L.diffuse);
				l.Ambient = _cv(L.ambient);
				l.Position.x = L.pos[0]; l.Position.y = L.pos[1]; l.Position.z = L.pos[2];
				l.Direction.x = L.dir[0]; l.Direction.y = L.dir[1]; l.Direction.z = L.dir[2];
				l.Range = 1e18f;          // (no reach: GL's lights have none)
				l.Attenuation0 = 1.0f;    // (GL's default: no falloff)
				if (L.directional && L.dir[0] == 0 && L.dir[1] == 0 && L.dir[2] == 0) on = false;   // (no direction: no light)
				else if (!s_lightKnown[i] || memcmp(&l, &s_light[i], sizeof(l)) != 0) { g_dev->SetLight((DWORD)i, &l); s_light[i] = l; s_lightKnown[i] = true; }
			}
			if (!s_lightOnKnown[i] || s_lightOn[i] != on) { g_dev->LightEnable((DWORD)i, on ? TRUE : FALSE); s_lightOn[i] = on; s_lightOnKnown[i] = true; }
		}
	}

	// fog, at each vertex, from the camera-space distance
	_rs(D3DRS_FOGENABLE, p.fog ? TRUE : FALSE);
	if (p.fog) {
		_rs(D3DRS_FOGVERTEXMODE, p.fogMode == 1 ? D3DFOG_EXP : p.fogMode == 2 ? D3DFOG_EXP2 : D3DFOG_LINEAR);
		_rs(D3DRS_FOGCOLOR, p.fogColor);
		_rs(D3DRS_FOGSTART, _f(p.fogStart));
		_rs(D3DRS_FOGEND, _f(p.fogEnd));
		_rs(D3DRS_FOGDENSITY, _f(p.fogDensity));
	}

	// the alpha test
	_rs(D3DRS_ALPHATESTENABLE, p.alphaTest ? TRUE : FALSE);
	if (p.alphaTest) {
		_rs(D3DRS_ALPHAFUNC, (DWORD)(D3DCMP_NEVER + p.alphaFunc));
		_rs(D3DRS_ALPHAREF, (DWORD)p.alphaRef);
	}
	if (p.tinted) _rs(D3DRS_TEXTUREFACTOR, p.tint);

	// the texture stages
	for (int i = 0; i < p.stages; ++i) {
		const fixed::Stage& st = p.stage[i];
		_tss(i, D3DTSS_COLOROP, st.colorOp);
		_tss(i, D3DTSS_COLORARG1, st.colorArg1);
		_tss(i, D3DTSS_COLORARG2, st.colorArg2);
		_tss(i, D3DTSS_ALPHAOP, st.alphaOp);
		_tss(i, D3DTSS_ALPHAARG1, st.alphaArg1);
		_tss(i, D3DTSS_ALPHAARG2, st.alphaArg2);
		_tss(i, D3DTSS_RESULTARG, D3DTA_CURRENT);
		if (st.unit >= 0) {
			const Sampler& sm = b.samp[st.unit];
			_texture(i, b.tex[st.unit]->tex);
			_tss(i, D3DTSS_ADDRESSU, sm.wrapU ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
			_tss(i, D3DTSS_ADDRESSV, sm.wrapV ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
			_tss(i, D3DTSS_MAGFILTER, sm.mag == kLinear ? D3DTEXF_LINEAR : D3DTEXF_POINT);
			_tss(i, D3DTSS_MINFILTER, sm.min == kLinear ? D3DTEXF_LINEAR : D3DTEXF_POINT);
			_tss(i, D3DTSS_MIPFILTER, sm.mip < 0 ? D3DTEXF_NONE : sm.mip == kLinear ? D3DTEXF_LINEAR : D3DTEXF_POINT);
			_tss(i, D3DTSS_TEXCOORDINDEX, st.coord == fixed::kCoordEyePosition ? (DWORD)D3DTSS_TCI_CAMERASPACEPOSITION : (DWORD)st.unit);
		} else {
			_texture(i, 0);
			_tss(i, D3DTSS_TEXCOORDINDEX, 0);
		}
		_tss(i, D3DTSS_TEXTURETRANSFORMFLAGS, st.transform ? D3DTTFF_COUNT4 | D3DTTFF_PROJECTED : D3DTTFF_DISABLE);
		if (st.transform) _transform(3 + i, D3DTS_TEXTURE0 + i, st.matrix);
	}
	_tss(p.stages, D3DTSS_COLOROP, D3DTOP_DISABLE);
	_tss(p.stages, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	_texture(p.stages, 0);
}

static bool s_saidGen = false;
static bool _prepare(Plan& p, const RenderState& rs, const Constants& c, const Bindings& b) {
	fixed::plan(p, rs, c, b, g_w, g_h);
	if (p.skip) return false;
	if (p.partialGen && !s_saidGen) { s_saidGen = true; J2X_UNHANDLED("texture coordinate generation on s or t alone on Direct3D 8 (those get 0)", 0); }
	_scene();
	_apply(p, rs, b);
	return true;
}

bool attachD3D8(IDirect3DDevice8* device, int w, int h) {
	detachD3D8();
	g_dev = device;
	if (!g_dev) return false;
	g_dev->AddRef();
	if (w > 0) g_w = w;
	if (h > 0) g_h = h;
	forgetState();
	DbgPrintF("[J2X] Direct3D 8: fixed-function pipeline ready\n");
	return true;
}
void resizedD3D8(int w, int h) {
	if (w > 0) g_w = w;
	if (h > 0) g_h = h;
	g_inScene = false;   // (a Reset ends the scene)
	forgetState();
}
void detachD3D8() {
	endSceneD3D8();
	if (g_dev) {
		for (int i = 0; i < 8; ++i) g_dev->SetTexture((DWORD)i, 0);
		g_dev->SetStreamSource(0, 0, 0);
	}
	_release(g_dev);
	forgetState();
}

bool ready() { return g_dev != 0; }
const char* name() { return "Direct3D 8"; }
int targetWidth() { return g_w; }
int targetHeight() { return g_h; }
bool halfPixelOffset() { return true; }   // (Direct3D 8 centres pixels on integers, as Direct3D 9 does)

// =========================================================================== drawing
static int _usable(Prim prim, int count) { return prim == kLines ? count - count % 2 : count - count % 3; }
static UINT _prims(Prim prim, int vertices) { return (UINT)(prim == kLines ? vertices / 2 : vertices / 3); }
static DWORD _type(Prim prim) { return prim == kLines ? D3DPT_LINELIST : D3DPT_TRIANGLELIST; }

void draw(Prim prim, const Vertex* v, int count, const RenderState& rs, const Constants& c, const Bindings& b) {
	count = _usable(prim, count);
	if (!ready() || count <= 0) return;
	Plan p;
	if (!_prepare(p, rs, c, b)) return;
	static std::vector<FVertex> up;
	up.resize((size_t)count);
	fixed::toFixed(v, count, &up[0], p.flat);
	for (int at = 0; at < count; at += kChunk) {
		const int n = count - at < kChunk ? count - at : kChunk;
		g_dev->DrawPrimitiveUP(_type(prim), _prims(prim, n), &up[(size_t)at], sizeof(FVertex));
	}
	s_stream = 0;   // (DrawPrimitiveUP leaves stream 0 empty)
}

static IDirect3DVertexBuffer8* _vertexBuffer(const FVertex* v, int n) {
	IDirect3DVertexBuffer8* vb = 0;
	// managed: kept through a Reset; no WRITEONLY, so the flat-shading copy can be made from it
	if (FAILED(g_dev->CreateVertexBuffer((UINT)n * sizeof(FVertex), 0, (DWORD)fixed::kFVF, D3DPOOL_MANAGED, &vb)) || !vb) return 0;
	BYTE* m = 0;
	if (FAILED(vb->Lock(0, 0, &m, 0)) || !m) { vb->Release(); return 0; }
	memcpy(m, v, (size_t)n * sizeof(FVertex));
	vb->Unlock();
	return vb;
}

Buffer createBuffer(const Vertex* v, int count) {
	if (!g_dev || count <= 0) return 0;
	std::vector<FVertex> fv((size_t)count);
	fixed::toFixed(v, count, &fv[0], false);
	BufferObject* o = new BufferObject;
	o->count = count;
	for (int at = 0; at < count; at += kChunk) {
		const int n = count - at < kChunk ? count - at : kChunk;
		IDirect3DVertexBuffer8* vb = _vertexBuffer(&fv[(size_t)at], n);
		if (!vb) { releaseBuffer(o); return 0; }
		o->vb.push_back(vb);
	}
	bool differs = false;
	for (int i = 0; i < count && !differs; ++i)
		differs = v[i].fcolor != v[i].color || v[i].fnx != v[i].nx || v[i].fny != v[i].ny || v[i].fnz != v[i].nz;
	if (differs) {
		o->flatValues.resize((size_t)count);
		for (int i = 0; i < count; ++i) {
			FlatValues& f = o->flatValues[(size_t)i];
			f.nx = v[i].fnx; f.ny = v[i].fny; f.nz = v[i].fnz; f.color = v[i].fcolor;
		}
	}
	return o;
}
void releaseBuffer(Buffer buf) {
	if (!buf) return;
	for (size_t i = 0; i < buf->vb.size(); ++i) { if (s_stream == buf->vb[i] && g_dev) { g_dev->SetStreamSource(0, 0, 0); s_stream = 0; } buf->vb[i]->Release(); }
	for (size_t i = 0; i < buf->flat.size(); ++i) { if (s_stream == buf->flat[i] && g_dev) { g_dev->SetStreamSource(0, 0, 0); s_stream = 0; } buf->flat[i]->Release(); }
	delete buf;
}
// The list's flat-shading copy: its vertices with the provoking normal and colour in place of their own.
static bool _makeFlat(BufferObject* o) {
	for (size_t k = 0; k < o->vb.size(); ++k) {
		const int at = (int)k * kChunk;
		const int n = o->count - at < kChunk ? o->count - at : kChunk;
		BYTE* m = 0;
		if (FAILED(o->vb[k]->Lock(0, 0, &m, D3DLOCK_READONLY)) || !m) return false;
		std::vector<FVertex> fv((const FVertex*)m, (const FVertex*)m + n);
		o->vb[k]->Unlock();
		for (int i = 0; i < n; ++i) {
			const FlatValues& f = o->flatValues[(size_t)(at + i)];
			fv[(size_t)i].nx = f.nx; fv[(size_t)i].ny = f.ny; fv[(size_t)i].nz = f.nz; fv[(size_t)i].color = f.color;
		}
		IDirect3DVertexBuffer8* vb = _vertexBuffer(&fv[0], n);
		if (!vb) return false;
		o->flat.push_back(vb);
	}
	return true;
}
void drawBuffer(Prim prim, Buffer buf, int count, const RenderState& rs, const Constants& c, const Bindings& b) {
	count = _usable(prim, count);
	if (!ready() || !buf || buf->vb.empty() || count <= 0) return;
	if (count > buf->count) count = _usable(prim, buf->count);
	Plan p;
	if (!_prepare(p, rs, c, b)) return;
	std::vector<IDirect3DVertexBuffer8*>* set = &buf->vb;
	if (p.flat && !buf->flatValues.empty()) {
		if (buf->flat.empty() && !_makeFlat(buf)) {
			for (size_t i = 0; i < buf->flat.size(); ++i) buf->flat[i]->Release();
			buf->flat.clear();
		}
		if (!buf->flat.empty()) set = &buf->flat;
	}
	for (size_t k = 0; k < set->size(); ++k) {
		const int at = (int)k * kChunk;
		if (at >= count) break;
		const int n = count - at < kChunk ? count - at : kChunk;
		_stream((*set)[k]);
		g_dev->DrawPrimitive(_type(prim), 0, _prims(prim, n));
	}
}
void beginUploads() {}   // (Direct3D 8 makes and fills managed resources inside a scene)
void endUploads() {}

// GL clears the whole target where the masks let it (decided by the layer), inside the scissor box when that test is on.
// Direct3D 8's Clear keeps to the viewport, so the viewport is the box (or the whole target) for it.
void clear(bool color, bool depth, bool stencil, float r, float g, float b, float a, float z, const RenderState& rs) {
	(void)stencil;   // (the layer has no stencil test: the depth buffer's stencil bits, if any, are never read)
	if (!g_dev) return;
	DWORD flags = 0;
	if (color) flags |= D3DCLEAR_TARGET;
	if (depth) flags |= D3DCLEAR_ZBUFFER;
	if (!flags) return;
	int x0 = 0, y0 = 0, x1 = g_w, y1 = g_h;
	if (rs.scissor) {
		if (rs.scX > x0) x0 = rs.scX;
		if (rs.scY > y0) y0 = rs.scY;
		if (rs.scX + rs.scW < x1) x1 = rs.scX + rs.scW;
		if (rs.scY + rs.scH < y1) y1 = rs.scY + rs.scH;
		if (x1 <= x0 || y1 <= y0) return;
	}
	_scene();
	_viewport(x0, y0, x1 - x0, y1 - y0);
	const float cc[4] = { r, g, b, a };
	g_dev->Clear(0, 0, flags, fixed::toArgb(cc), z, 0);
}

// =========================================================================== textures
Texture createTexture(int w, int h, int levels) {
	if (!g_dev || w <= 0 || h <= 0) return 0;
	IDirect3DTexture8* tex = 0;
	if (FAILED(g_dev->CreateTexture((UINT)w, (UINT)h, (UINT)levels, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex)) || !tex) {
		DbgPrintF("[J2X] Direct3D 8: a %dx%d texture could not be made\n", w, h);
		return 0;
	}
	TextureObject* t = new TextureObject;
	t->tex = tex; t->w = w; t->h = h; t->levels = levels;
	return t;
}
void releaseTexture(Texture t) {
	if (!t) return;
	for (int i = 0; i < 8; ++i)
		if (s_texKnown[i] && s_tex[i] == t->tex && g_dev) { g_dev->SetTexture((DWORD)i, 0); s_tex[i] = 0; }
	_release(t->tex);
	delete t;
}
bool writeTexture(Texture t, int level, int x, int y, int w, int h, const unsigned int* argb) {
	if (!t || !t->tex || w <= 0 || h <= 0) return false;
	RECT r;
	r.left = x; r.top = y; r.right = x + w; r.bottom = y + h;
	D3DLOCKED_RECT lr;
	if (FAILED(t->tex->LockRect((UINT)level, &lr, &r, 0))) return false;
	for (int row = 0; row < h; ++row) memcpy((char*)lr.pBits + (size_t)row * lr.Pitch, argb + (size_t)row * w, (size_t)w * 4);   // (A8R8G8B8 is the layer's ARGB words)
	t->tex->UnlockRect((UINT)level);
	return true;
}
bool readTexture(Texture t, int level, std::vector<unsigned int>& argb, int& w, int& h) {
	if (!t || !t->tex) return false;
	w = t->w >> level; if (w < 1) w = 1;
	h = t->h >> level; if (h < 1) h = 1;
	D3DLOCKED_RECT lr;
	if (FAILED(t->tex->LockRect((UINT)level, &lr, 0, D3DLOCK_READONLY))) return false;
	argb.resize((size_t)w * h);
	for (int row = 0; row < h; ++row) memcpy(&argb[(size_t)row * w], (const char*)lr.pBits + (size_t)row * lr.Pitch, (size_t)w * 4);
	t->tex->UnlockRect((UINT)level);
	return true;
}

// =========================================================================== the frame
// The back buffer is lockable (the Display asks for one): read in place, the scene closed first.
bool readFrame(std::vector<unsigned int>& argb, int& w, int& h) {
	if (!g_dev) return false;
	endSceneD3D8();
	IDirect3DSurface8* back = 0;
	if (FAILED(g_dev->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &back)) || !back) return false;
	D3DSURFACE_DESC d;
	bool ok = false;
	D3DLOCKED_RECT lr;
	if (SUCCEEDED(back->GetDesc(&d)) && SUCCEEDED(back->LockRect(&lr, 0, D3DLOCK_READONLY))) {
		w = (int)d.Width; h = (int)d.Height;
		argb.resize((size_t)w * h);
		for (int row = 0; row < h; ++row) {
			const unsigned int* src = (const unsigned int*)((const char*)lr.pBits + (size_t)row * lr.Pitch);
			for (int x = 0; x < w; ++x) argb[(size_t)row * w + x] = 0xFF000000u | src[x];   // (X8R8G8B8: opaque, as GL's default framebuffer reads)
		}
		back->UnlockRect();
		ok = true;
	}
	back->Release();
	return ok;
}

}  // namespace gpu
}  // namespace j2xgl
