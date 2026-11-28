// GpuD3D7.cpp -- the GL layer's Direct3D 7 backend (Gpu.h), the PC built with --graphics d3d7. It carries out what GL11.cpp
// decided on a Direct3D 7 device the Display made, with Direct3D 7's fixed-function pipeline (hardware transform and lighting
// where the card has it): GL's transforms, lights, fog, alpha test and texture environments are set as device state
// (GpuFixed.h plans them, from the same constants the shader backends read).
//
// Textures are DirectDraw surfaces (a chain of them with mip levels), managed by Direct3D so they survive lost surfaces;
// immediate geometry goes up with each draw (DrawPrimitive from memory); display lists get vertex buffers. A draw call takes at
// most kChunk vertices (Direct3D 7's D3DMAXNUMVERTICES is 65535), so a big list is a few buffers. Direct3D 7 has no colour write
// mask: a draw that writes no colour (a depth-only pass) blends to keep what is there; a partial mask is not kept.
//
// The device's scene is the backend's: opened at the first draw or clear, closed before anything locks a surface or a buffer
// (Direct3D 7's drivers may not allow that inside a scene) and before the Display shows the frame.
#define J2X_WITH_GDI   // (ddraw.h needs the GDI types stdafx.h keeps out of the game's files)
#include "stdafx.h"
#include "GpuD3D7.h"
#include "GpuFixed.h"
#include "Common.h"     // DbgPrintF / J2X_UNHANDLED
#include <ddraw.h>
#include <d3d.h>
#include <vector>
#include <string.h>

namespace j2xgl {
namespace gpu {

using fixed::Mat;
using fixed::Plan;
using fixed::FVertex;

static const int kChunk = 65532;   // vertices a draw call takes at most (a multiple of 2 and 3, under D3DMAXNUMVERTICES)

struct TextureObject {   // (gpu::Texture)
	std::vector<IDirectDrawSurface7*> level;   // the top surface, then each mip level (each held)
	int w, h;
};
struct FlatValues { float nx, ny, nz; unsigned int color; };
struct BufferObject {    // (gpu::Buffer)
	std::vector<IDirect3DVertexBuffer7*> vb;     // kChunk vertices each, the last the rest
	int count;
	std::vector<FlatValues> flatValues;          // empty: flat shading draws the same vertices
	std::vector<IDirect3DVertexBuffer7*> flat;   // the flat-shading copy, made on the list's first flat draw
};

// =========================================================================== the device
static IDirectDraw7* g_dd = 0;
static IDirect3D7* g_d3d = 0;
static IDirect3DDevice7* g_dev = 0;
static IDirectDrawSurface7* g_target = 0;
static int g_w = 854, g_h = 480;
static bool g_systemMemory = false;
static bool g_inScene = false;
static bool g_pow2Only = false;
static DWORD g_maxTexture = 0;

template<class T> static void _release(T*& p) { if (p) { p->Release(); p = 0; } }
static DWORD _f(float f) { DWORD d; memcpy(&d, &f, 4); return d; }

static void _scene() {
	if (!g_inScene && g_dev && SUCCEEDED(g_dev->BeginScene())) g_inScene = true;
}
void endSceneD3D7() {
	if (g_inScene && g_dev) g_dev->EndScene();
	g_inScene = false;
}

// =========================================================================== what is on the device
static DWORD s_rs[256];
static bool s_rsKnown[256];
static DWORD s_tss[8][32];
static bool s_tssKnown[8][32];
static IDirectDrawSurface7* s_tex[8];
static bool s_texKnown[8];
static D3DMATRIX s_xf[3 + 8];   // world, view, projection, then each stage's texture matrix
static bool s_xfKnown[3 + 8];
static D3DVIEWPORT7 s_vp;
static bool s_vpKnown = false;
static D3DMATERIAL7 s_mat;
static bool s_matKnown = false;
static D3DLIGHT7 s_light[2];
static bool s_lightKnown[2], s_lightOn[2], s_lightOnKnown[2];

void forgetState() {
	memset(s_rsKnown, 0, sizeof(s_rsKnown));
	memset(s_tssKnown, 0, sizeof(s_tssKnown));
	memset(s_texKnown, 0, sizeof(s_texKnown));
	memset(s_xfKnown, 0, sizeof(s_xfKnown));
	memset(s_lightKnown, 0, sizeof(s_lightKnown));
	memset(s_lightOnKnown, 0, sizeof(s_lightOnKnown));
	s_vpKnown = s_matKnown = false;
}

static void _rs(D3DRENDERSTATETYPE state, DWORD value) {
	const unsigned i = (unsigned)state;
	if (i < 256 && s_rsKnown[i] && s_rs[i] == value) return;
	g_dev->SetRenderState(state, value);
	if (i < 256) { s_rs[i] = value; s_rsKnown[i] = true; }
}
static void _tss(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value) {
	const unsigned t = (unsigned)type;
	if (s_tssKnown[stage][t] && s_tss[stage][t] == value) return;
	g_dev->SetTextureStageState(stage, type, value);
	s_tss[stage][t] = value;
	s_tssKnown[stage][t] = true;
}
static void _texture(DWORD stage, IDirectDrawSurface7* t) {
	if (s_texKnown[stage] && s_tex[stage] == t) return;
	g_dev->SetTexture(stage, t);
	s_tex[stage] = t;
	s_texKnown[stage] = true;
}
static void _transform(int slot, D3DTRANSFORMSTATETYPE state, const Mat& m) {
	D3DMATRIX d;
	memcpy(&d, m.m, sizeof(d));
	if (s_xfKnown[slot] && memcmp(&s_xf[slot], &d, sizeof(d)) == 0) return;
	g_dev->SetTransform(state, &d);
	s_xf[slot] = d;
	s_xfKnown[slot] = true;
}
static void _viewport(int x, int y, int w, int h) {
	D3DVIEWPORT7 vp;
	vp.dwX = (DWORD)x; vp.dwY = (DWORD)y; vp.dwWidth = (DWORD)w; vp.dwHeight = (DWORD)h; vp.dvMinZ = 0.0f; vp.dvMaxZ = 1.0f;
	if (s_vpKnown && memcmp(&vp, &s_vp, sizeof(vp)) == 0) return;
	g_dev->SetViewport(&vp);
	s_vp = vp;
	s_vpKnown = true;
}
static void _color(D3DCOLORVALUE& to, const float* c) { memcpy(&to, c, sizeof(float) * 4); }   // (r, g, b, a)
static void _vector(D3DVECTOR& to, const float* v) { memcpy(&to, v, sizeof(float) * 3); }      // (x, y, z)

static bool s_saidMask = false;
static void _apply(const Plan& p, const RenderState& rs, const Bindings& b) {
	// what never changes
	_rs(D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID);
	_rs(D3DRENDERSTATE_SHADEMODE, D3DSHADE_GOURAUD);   // (flat shading is in the vertices: GpuFixed.h)
	_rs(D3DRENDERSTATE_CLIPPING, TRUE);
	_rs(D3DRENDERSTATE_DITHERENABLE, FALSE);
	_rs(D3DRENDERSTATE_STENCILENABLE, FALSE);
	_rs(D3DRENDERSTATE_SPECULARENABLE, FALSE);
	_rs(D3DRENDERSTATE_LOCALVIEWER, FALSE);
	_rs(D3DRENDERSTATE_RANGEFOGENABLE, FALSE);
	_rs(D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_NONE);

	// depth, blending, culling (GL: with the depth test off nothing is written to depth either)
	_rs(D3DRENDERSTATE_ZENABLE, rs.depthTest ? D3DZB_TRUE : D3DZB_FALSE);
	_rs(D3DRENDERSTATE_ZWRITEENABLE, rs.depthTest && rs.depthWrite ? TRUE : FALSE);
	_rs(D3DRENDERSTATE_ZFUNC, (DWORD)(D3DCMP_NEVER + rs.depthFunc));
	bool blend = rs.blend;
	int src = rs.srcBlend, dst = rs.dstBlend;
	if ((rs.colorWrite & 7u) == 0) { blend = true; src = kZero; dst = kOne; }   // (no colour written: the target's kept)
	else if ((rs.colorWrite & 7u) != 7u && !s_saidMask) { s_saidMask = true; J2X_UNHANDLED("a partial colour mask on Direct3D 7 (every channel is written)", 0); }
	_rs(D3DRENDERSTATE_ALPHABLENDENABLE, blend ? TRUE : FALSE);
	if (blend) {
		_rs(D3DRENDERSTATE_SRCBLEND, (DWORD)(D3DBLEND_ZERO + src));
		_rs(D3DRENDERSTATE_DESTBLEND, (DWORD)(D3DBLEND_ZERO + dst));
	}
	_rs(D3DRENDERSTATE_CULLMODE, rs.cull == kCullCW ? D3DCULL_CW : rs.cull == kCullCCW ? D3DCULL_CCW : D3DCULL_NONE);

	// transforms and viewport
	_transform(0, D3DTRANSFORMSTATE_WORLD, p.world);
	_transform(1, D3DTRANSFORMSTATE_VIEW, p.view);
	_transform(2, D3DTRANSFORMSTATE_PROJECTION, p.proj);
	_viewport(p.vpX, p.vpY, p.vpW, p.vpH);

	// lighting
	_rs(D3DRENDERSTATE_LIGHTING, p.lighting ? TRUE : FALSE);
	if (p.lighting) {
		_rs(D3DRENDERSTATE_AMBIENT, fixed::toArgb(p.ambient));
		_rs(D3DRENDERSTATE_NORMALIZENORMALS, p.normalize ? TRUE : FALSE);
		_rs(D3DRENDERSTATE_COLORVERTEX, TRUE);
		_rs(D3DRENDERSTATE_AMBIENTMATERIALSOURCE, p.ambientFromColor ? D3DMCS_COLOR1 : D3DMCS_MATERIAL);
		_rs(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, p.diffuseFromColor ? D3DMCS_COLOR1 : D3DMCS_MATERIAL);
		_rs(D3DRENDERSTATE_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
		_rs(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
		D3DMATERIAL7 m;
		memset(&m, 0, sizeof(m));
		_color(m.dcvDiffuse, p.matDiffuse);
		_color(m.dcvAmbient, p.matAmbient);
		if (!s_matKnown || memcmp(&m, &s_mat, sizeof(m)) != 0) { g_dev->SetMaterial(&m); s_mat = m; s_matKnown = true; }
		for (int i = 0; i < 2; ++i) {
			const fixed::Light& L = p.light[i];
			bool on = L.on;
			if (on) {
				D3DLIGHT7 l;
				memset(&l, 0, sizeof(l));
				l.dltType = L.directional ? D3DLIGHT_DIRECTIONAL : D3DLIGHT_POINT;
				_color(l.dcvDiffuse, L.diffuse);
				_color(l.dcvAmbient, L.ambient);
				_vector(l.dvPosition, L.pos);
				_vector(l.dvDirection, L.dir);
				l.dvRange = D3DLIGHT_RANGE_MAX;   // (GL's lights have no reach)
				l.dvAttenuation0 = 1.0f;          // (GL's default: no falloff)
				if (L.directional && L.dir[0] == 0 && L.dir[1] == 0 && L.dir[2] == 0) on = false;   // (no direction: no light)
				else if (!s_lightKnown[i] || memcmp(&l, &s_light[i], sizeof(l)) != 0) { g_dev->SetLight((DWORD)i, &l); s_light[i] = l; s_lightKnown[i] = true; }
			}
			if (!s_lightOnKnown[i] || s_lightOn[i] != on) { g_dev->LightEnable((DWORD)i, on ? TRUE : FALSE); s_lightOn[i] = on; s_lightOnKnown[i] = true; }
		}
	}

	// fog, at each vertex, from the camera-space distance
	_rs(D3DRENDERSTATE_FOGENABLE, p.fog ? TRUE : FALSE);
	if (p.fog) {
		_rs(D3DRENDERSTATE_FOGVERTEXMODE, p.fogMode == 1 ? D3DFOG_EXP : p.fogMode == 2 ? D3DFOG_EXP2 : D3DFOG_LINEAR);
		_rs(D3DRENDERSTATE_FOGCOLOR, p.fogColor);
		_rs(D3DRENDERSTATE_FOGSTART, _f(p.fogStart));
		_rs(D3DRENDERSTATE_FOGEND, _f(p.fogEnd));
		_rs(D3DRENDERSTATE_FOGDENSITY, _f(p.fogDensity));
	}

	// the alpha test
	_rs(D3DRENDERSTATE_ALPHATESTENABLE, p.alphaTest ? TRUE : FALSE);
	if (p.alphaTest) {
		_rs(D3DRENDERSTATE_ALPHAFUNC, (DWORD)(D3DCMP_NEVER + p.alphaFunc));
		_rs(D3DRENDERSTATE_ALPHAREF, (DWORD)p.alphaRef);
	}
	if (p.tinted) _rs(D3DRENDERSTATE_TEXTUREFACTOR, p.tint);

	// the texture stages
	for (int i = 0; i < p.stages; ++i) {
		const fixed::Stage& st = p.stage[i];
		_tss(i, D3DTSS_COLOROP, st.colorOp);
		_tss(i, D3DTSS_COLORARG1, st.colorArg1);
		_tss(i, D3DTSS_COLORARG2, st.colorArg2);
		_tss(i, D3DTSS_ALPHAOP, st.alphaOp);
		_tss(i, D3DTSS_ALPHAARG1, st.alphaArg1);
		_tss(i, D3DTSS_ALPHAARG2, st.alphaArg2);
		if (st.unit >= 0) {
			const Sampler& sm = b.samp[st.unit];
			_texture(i, b.tex[st.unit]->level[0]);
			_tss(i, D3DTSS_ADDRESSU, sm.wrapU ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
			_tss(i, D3DTSS_ADDRESSV, sm.wrapV ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
			_tss(i, D3DTSS_MAGFILTER, sm.mag == kLinear ? D3DTFG_LINEAR : D3DTFG_POINT);
			_tss(i, D3DTSS_MINFILTER, sm.min == kLinear ? D3DTFN_LINEAR : D3DTFN_POINT);
			_tss(i, D3DTSS_MIPFILTER, sm.mip < 0 ? D3DTFP_NONE : sm.mip == kLinear ? D3DTFP_LINEAR : D3DTFP_POINT);
			_tss(i, D3DTSS_TEXCOORDINDEX, st.coord == fixed::kCoordEyePosition ? D3DTSS_TCI_CAMERASPACEPOSITION : (DWORD)st.unit);
		} else {
			_texture(i, 0);
			_tss(i, D3DTSS_TEXCOORDINDEX, 0);
		}
		_tss(i, D3DTSS_TEXTURETRANSFORMFLAGS, st.transform ? D3DTTFF_COUNT4 | D3DTTFF_PROJECTED : D3DTTFF_DISABLE);
		if (st.transform) _transform(3 + i, (D3DTRANSFORMSTATETYPE)(D3DTRANSFORMSTATE_TEXTURE0 + i), st.matrix);
	}
	_tss(p.stages, D3DTSS_COLOROP, D3DTOP_DISABLE);
	_tss(p.stages, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	_texture(p.stages, 0);
}

static bool s_saidGen = false;
static bool _prepare(Plan& p, const RenderState& rs, const Constants& c, const Bindings& b) {
	fixed::plan(p, rs, c, b, g_w, g_h);
	if (p.skip) return false;
	if (p.partialGen && !s_saidGen) { s_saidGen = true; J2X_UNHANDLED("texture coordinate generation on s or t alone on Direct3D 7 (those get 0)", 0); }
	_scene();
	_apply(p, rs, b);
	return true;
}

bool attachD3D7(IDirectDraw7* dd, IDirect3D7* d3d, IDirect3DDevice7* device, IDirectDrawSurface7* target, int w, int h, bool systemMemory) {
	detachD3D7();
	if (!dd || !d3d || !device || !target) return false;
	g_dd = dd; g_d3d = d3d; g_dev = device; g_target = target;
	g_dd->AddRef(); g_d3d->AddRef(); g_dev->AddRef(); g_target->AddRef();
	if (w > 0) g_w = w;
	if (h > 0) g_h = h;
	g_systemMemory = systemMemory;
	D3DDEVICEDESC7 caps;
	memset(&caps, 0, sizeof(caps));
	if (SUCCEEDED(g_dev->GetCaps(&caps))) {
		g_pow2Only = (caps.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2) != 0 && (caps.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL) == 0;
		g_maxTexture = caps.dwMaxTextureWidth < caps.dwMaxTextureHeight ? caps.dwMaxTextureWidth : caps.dwMaxTextureHeight;
	}
	forgetState();
	DbgPrintF("[J2X] Direct3D 7: fixed-function pipeline ready (textures up to %u, %s)\n", (unsigned)g_maxTexture, g_pow2Only ? "powers of two only" : "any size");
	return true;
}
void setTargetD3D7(IDirectDrawSurface7* target, int w, int h) {
	endSceneD3D7();
	_release(g_target);
	g_target = target;
	if (g_target) g_target->AddRef();
	if (w > 0) g_w = w;
	if (h > 0) g_h = h;
	forgetState();
}
void detachD3D7() {
	endSceneD3D7();
	if (g_dev)
		for (int i = 0; i < 8; ++i) g_dev->SetTexture((DWORD)i, 0);
	_release(g_target);
	_release(g_dev);
	_release(g_d3d);
	_release(g_dd);
	forgetState();
}

bool ready() { return g_dev != 0; }
const char* name() { return "Direct3D 7"; }
int targetWidth() { return g_w; }
int targetHeight() { return g_h; }
bool halfPixelOffset() { return true; }   // (Direct3D 7 centres pixels on integers, as Direct3D 9 does)

// =========================================================================== drawing
static int _usable(Prim prim, int count) { return prim == kLines ? count - count % 2 : count - count % 3; }
static D3DPRIMITIVETYPE _type(Prim prim) { return prim == kLines ? D3DPT_LINELIST : D3DPT_TRIANGLELIST; }

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
		g_dev->DrawPrimitive(_type(prim), (DWORD)fixed::kFVF, &up[(size_t)at], (DWORD)n, 0);
	}
}

static IDirect3DVertexBuffer7* _vertexBuffer(const FVertex* v, int n) {
	D3DVERTEXBUFFERDESC d;
	memset(&d, 0, sizeof(d));
	d.dwSize = sizeof(d);
	d.dwCaps = g_systemMemory ? D3DVBCAPS_SYSTEMMEMORY : 0;   // (no WRITEONLY: the flat-shading copy is made from it)
	d.dwFVF = (DWORD)fixed::kFVF;
	d.dwNumVertices = (DWORD)n;
	IDirect3DVertexBuffer7* vb = 0;
	if (FAILED(g_d3d->CreateVertexBuffer(&d, &vb, 0)) || !vb) return 0;
	void* m = 0;
	DWORD size = 0;
	if (FAILED(vb->Lock(DDLOCK_WAIT | DDLOCK_WRITEONLY, &m, &size)) || !m) { vb->Release(); return 0; }
	memcpy(m, v, (size_t)n * sizeof(FVertex));
	vb->Unlock();
	return vb;
}

Buffer createBuffer(const Vertex* v, int count) {
	if (!g_dev || count <= 0) return 0;
	endSceneD3D7();   // (buffers are filled out of a scene)
	std::vector<FVertex> fv((size_t)count);
	fixed::toFixed(v, count, &fv[0], false);
	BufferObject* o = new BufferObject;
	o->count = count;
	for (int at = 0; at < count; at += kChunk) {
		const int n = count - at < kChunk ? count - at : kChunk;
		IDirect3DVertexBuffer7* vb = _vertexBuffer(&fv[(size_t)at], n);
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
	for (size_t i = 0; i < buf->vb.size(); ++i) buf->vb[i]->Release();
	for (size_t i = 0; i < buf->flat.size(); ++i) buf->flat[i]->Release();
	delete buf;
}
// The list's flat-shading copy: its vertices with the provoking normal and colour in place of their own.
static bool _makeFlat(BufferObject* o) {
	endSceneD3D7();
	for (size_t k = 0; k < o->vb.size(); ++k) {
		const int at = (int)k * kChunk;
		const int n = o->count - at < kChunk ? o->count - at : kChunk;
		void* m = 0;
		DWORD size = 0;
		if (FAILED(o->vb[k]->Lock(DDLOCK_WAIT | DDLOCK_READONLY, &m, &size)) || !m) return false;
		std::vector<FVertex> fv((const FVertex*)m, (const FVertex*)m + n);
		o->vb[k]->Unlock();
		for (int i = 0; i < n; ++i) {
			const FlatValues& f = o->flatValues[(size_t)(at + i)];
			fv[(size_t)i].nx = f.nx; fv[(size_t)i].ny = f.ny; fv[(size_t)i].nz = f.nz; fv[(size_t)i].color = f.color;
		}
		IDirect3DVertexBuffer7* vb = _vertexBuffer(&fv[0], n);
		if (!vb) return false;
		o->flat.push_back(vb);
	}
	return true;
}
void drawBuffer(Prim prim, Buffer buf, int count, const RenderState& rs, const Constants& c, const Bindings& b) {
	count = _usable(prim, count);
	if (!ready() || !buf || buf->vb.empty() || count <= 0) return;
	if (count > buf->count) count = _usable(prim, buf->count);
	std::vector<IDirect3DVertexBuffer7*>* set = &buf->vb;
	const bool flat = c.vs[27 * 4] > 0.5f;
	if (flat && !buf->flatValues.empty()) {
		if (buf->flat.empty() && !_makeFlat(buf)) {
			for (size_t i = 0; i < buf->flat.size(); ++i) buf->flat[i]->Release();
			buf->flat.clear();
		}
		if (!buf->flat.empty()) set = &buf->flat;
	}
	Plan p;
	if (!_prepare(p, rs, c, b)) return;
	for (size_t k = 0; k < set->size(); ++k) {
		const int at = (int)k * kChunk;
		if (at >= count) break;
		const int n = count - at < kChunk ? count - at : kChunk;
		g_dev->DrawPrimitiveVB(_type(prim), (*set)[k], 0, (DWORD)n, 0);
	}
}
void beginUploads() {}   // (each buffer closes the scene itself; the next draw opens it again)
void endUploads() {}

// GL clears the whole target where the masks let it (decided by the layer), inside the scissor box when that test is on.
// Direct3D 7's Clear keeps to the viewport, so the viewport is the box (or the whole target) for it.
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
	D3DRECT box;
	box.x1 = x0; box.y1 = y0; box.x2 = x1; box.y2 = y1;
	const float cc[4] = { r, g, b, a };
	g_dev->Clear(1, &box, flags, fixed::toArgb(cc), z, 0);
}

// =========================================================================== textures
// A 32-bit ARGB surface with `levels` levels, managed by Direct3D (a copy kept in system memory, put on the card as it is used,
// restored when surfaces are lost); the software device's in system memory.
Texture createTexture(int w, int h, int levels) {
	if (!g_dd || w <= 0 || h <= 0) return 0;
	if (g_pow2Only && ((w & (w - 1)) || (h & (h - 1)))) J2X_UNHANDLED("a texture whose size is not a power of two, on a Direct3D 7 card that needs one", 0);
	endSceneD3D7();
	DDSURFACEDESC2 sd;
	memset(&sd, 0, sizeof(sd));
	sd.dwSize = sizeof(sd);
	sd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | (levels > 1 ? DDSD_MIPMAPCOUNT : 0);
	sd.dwWidth = (DWORD)w; sd.dwHeight = (DWORD)h;
	sd.dwMipMapCount = (DWORD)levels;
	sd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | (levels > 1 ? DDSCAPS_MIPMAP | DDSCAPS_COMPLEX : 0) | (g_systemMemory ? DDSCAPS_SYSTEMMEMORY : 0);
	sd.ddsCaps.dwCaps2 = g_systemMemory ? 0 : DDSCAPS2_TEXTUREMANAGE;
	sd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	sd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
	sd.ddpfPixelFormat.dwRGBBitCount = 32;
	sd.ddpfPixelFormat.dwRBitMask = 0x00FF0000; sd.ddpfPixelFormat.dwGBitMask = 0x0000FF00; sd.ddpfPixelFormat.dwBBitMask = 0x000000FF;
	sd.ddpfPixelFormat.dwRGBAlphaBitMask = 0xFF000000;   // (the layer's ARGB words, as they lie in memory)
	IDirectDrawSurface7* top = 0;
	if (FAILED(g_dd->CreateSurface(&sd, &top, 0)) || !top) {
		DbgPrintF("[J2X] Direct3D 7: a %dx%d texture could not be made\n", w, h);
		return 0;
	}
	TextureObject* t = new TextureObject;
	t->w = w; t->h = h;
	t->level.push_back(top);
	for (int i = 1; i < levels; ++i) {   // (each mip level is the one before's attached surface)
		DDSCAPS2 caps;
		memset(&caps, 0, sizeof(caps));
		caps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
		IDirectDrawSurface7* next = 0;
		if (FAILED(t->level.back()->GetAttachedSurface(&caps, &next)) || !next) break;
		t->level.push_back(next);
	}
	return t;
}
void releaseTexture(Texture t) {
	if (!t) return;
	for (int i = 0; i < 8; ++i)
		if (s_texKnown[i] && !t->level.empty() && s_tex[i] == t->level[0] && g_dev) { g_dev->SetTexture((DWORD)i, 0); s_tex[i] = 0; }
	for (size_t i = t->level.size(); i-- > 0;) t->level[i]->Release();
	delete t;
}
bool writeTexture(Texture t, int level, int x, int y, int w, int h, const unsigned int* argb) {
	if (!t || level < 0 || level >= (int)t->level.size() || w <= 0 || h <= 0) return false;
	endSceneD3D7();
	RECT r;
	r.left = x; r.top = y; r.right = x + w; r.bottom = y + h;
	DDSURFACEDESC2 sd;
	memset(&sd, 0, sizeof(sd));
	sd.dwSize = sizeof(sd);
	IDirectDrawSurface7* s = t->level[(size_t)level];
	if (FAILED(s->Lock(&r, &sd, DDLOCK_WAIT | DDLOCK_WRITEONLY, 0)) || !sd.lpSurface) return false;
	for (int row = 0; row < h; ++row) memcpy((char*)sd.lpSurface + (size_t)row * sd.lPitch, argb + (size_t)row * w, (size_t)w * 4);
	s->Unlock(&r);
	return true;
}
bool readTexture(Texture t, int level, std::vector<unsigned int>& argb, int& w, int& h) {
	if (!t || level < 0 || level >= (int)t->level.size()) return false;
	endSceneD3D7();
	w = t->w >> level; if (w < 1) w = 1;
	h = t->h >> level; if (h < 1) h = 1;
	DDSURFACEDESC2 sd;
	memset(&sd, 0, sizeof(sd));
	sd.dwSize = sizeof(sd);
	IDirectDrawSurface7* s = t->level[(size_t)level];
	if (FAILED(s->Lock(0, &sd, DDLOCK_WAIT | DDLOCK_READONLY, 0)) || !sd.lpSurface) return false;
	argb.resize((size_t)w * h);
	for (int row = 0; row < h; ++row) memcpy(&argb[(size_t)row * w], (const char*)sd.lpSurface + (size_t)row * sd.lPitch, (size_t)w * 4);
	s->Unlock(0);
	return true;
}

// =========================================================================== the frame
// The render target, read in place (32-bit X8R8G8B8), the scene closed first.
bool readFrame(std::vector<unsigned int>& argb, int& w, int& h) {
	if (!g_target) return false;
	endSceneD3D7();
	DDSURFACEDESC2 sd;
	memset(&sd, 0, sizeof(sd));
	sd.dwSize = sizeof(sd);
	if (FAILED(g_target->Lock(0, &sd, DDLOCK_WAIT | DDLOCK_READONLY, 0)) || !sd.lpSurface) return false;
	w = (int)sd.dwWidth; h = (int)sd.dwHeight;
	argb.resize((size_t)w * h);
	for (int row = 0; row < h; ++row) {
		const unsigned int* src = (const unsigned int*)((const char*)sd.lpSurface + (size_t)row * sd.lPitch);
		for (int x = 0; x < w; ++x) argb[(size_t)row * w + x] = 0xFF000000u | src[x];   // (opaque, as GL's default framebuffer reads)
	}
	g_target->Unlock(0);
	return true;
}

}  // namespace gpu
}  // namespace j2xgl
