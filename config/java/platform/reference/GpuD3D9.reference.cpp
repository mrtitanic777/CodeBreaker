// GpuD3D9.cpp -- the GL layer's Direct3D 9 backend (Gpu.h): the Xbox 360 (Xenos, the XDK's d3d9/d3dx9) and the PC (Windows'
// d3d9.dll; J2xD3DX9.h stands in for D3DX). It carries out what GL11.cpp decided: GL's fixed-function pipeline is the shared HLSL
// in GpuShaders.h (Xenos has no fixed-function pipeline, so the PC runs the same shaders), wrapped here in shader model 3.
//
// Lessons the Xbox 360 taught, kept for both targets:
//   * a resource is made only out of a scene (sceneBegin / sceneEnd are this file's, so it can step out), and never released
//     or locked while it is still bound (vb-released-while-bound: releasing a buffer still set in the device, or locking one,
//     corrupted the 360's runtime and collapsed the frame rate);
//   * the 360's plain A8R8G8B8 is tiled: textures are made LIN_A8R8G8B8 so a linear copy lands as sampled;
//   * every redundant device call costs (the 360's runtime validates each): the last state, constants and bindings are
//     shadowed, and only changes reach the device.
// C++03: the Xbox 360's compiler builds this file.

#ifndef _XBOX
#define J2X_WITH_GDI   // (the PC: d3d9.h needs the GDI types stdafx.h keeps out of the game's files)
#endif
#include "stdafx.h"     // MUST be first: the Xbox 360 project uses /Yu precompiled headers
#include "GpuD3D9.h"
#include "GpuShaders.h"
#include "Common.h"     // Direct3D 9 + D3DX (the XDK's, or J2xD3DX9.h on the PC); DbgPrintF / J2X_UNHANDLED
#include <string>
#include <vector>
#include <string.h>

#ifdef _XBOX
  #define J2X_TEXPOOL  D3DPOOL_MANAGED       // 360: the pool is ignored (unified memory); MANAGED is a benign token
  #define J2X_TEXFMT   D3DFMT_LIN_A8R8G8B8   // the UNTILED twin of A8R8G8B8: a linear copy lays out as the GPU samples
#else
  #define J2X_TEXPOOL  D3DPOOL_MANAGED       // PC: managed (a system-memory copy, so a device reset keeps it)
  #define J2X_TEXFMT   D3DFMT_A8R8G8B8
#endif

namespace j2xgl {

// =========================================================================== device and scene
static IDirect3DDevice9* g_dev = 0;
static int g_fbW = 1280, g_fbH = 720;     // the backbuffer
static bool s_inScene = false;

void sceneBegin() { if (g_dev && !s_inScene && SUCCEEDED(g_dev->BeginScene())) s_inScene = true; }
void sceneEnd()   { if (g_dev && s_inScene) { g_dev->EndScene(); s_inScene = false; } }
bool inScene()    { return s_inScene; }

// Steps out of the scene for its lifetime (the 360 makes resources only out of a scene; the PC doesn't mind either way).
struct OutOfScene {
	bool was;
	OutOfScene() : was(s_inScene) { if (was) sceneEnd(); }
	~OutOfScene() { if (was) sceneBegin(); }
};

namespace gpu {
struct TextureObject { IDirect3DTexture9* tex; int w, h, levels; };   // (gpu::Texture)
struct BufferObject { IDirect3DVertexBuffer9* vb; };                 // (gpu::Buffer)
static void _buildShaders();
}

void glSetDevice(IDirect3DDevice9* dev) {
	g_dev = dev;
	IDirect3DSurface9* bb = 0;
	if (dev && SUCCEEDED(dev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &bb)) && bb) {
		D3DSURFACE_DESC d;
		if (SUCCEEDED(bb->GetDesc(&d)) && d.Height > 0) { g_fbW = (int)d.Width; g_fbH = (int)d.Height; }
		bb->Release();
	}
	gpu::_buildShaders();
	gpu::forgetState();
}
IDirect3DDevice9* device() { return g_dev; }

namespace gpu {

// =========================================================================== the shaders
// GL's pipeline (GpuShaders.h) in shader model 3: the constants as c-registers in GL11.cpp's order, the entry points with
// Direct3D 9's semantics. SEPARATE sources, each entry "main" (one combined source with two entry points failed to compile on
// the 360).
static const char* const kVSHead =
	"float4x4 gWVP : register(c0);\n"
	"float4x4 gMV  : register(c4);\n"
	"float4x4 gT0  : register(c8);\n"
	"float4x4 gT1  : register(c12);\n"
	"float4 gLit   : register(c16);\n"
	"float4 gAmb   : register(c17);\n"
	"float4 gL0p   : register(c18);\n"
	"float4 gL0d   : register(c19);\n"
	"float4 gL0a   : register(c20);\n"
	"float4 gL1p   : register(c21);\n"
	"float4 gL1d   : register(c22);\n"
	"float4 gL1a   : register(c23);\n"
	"float4 gMa    : register(c24);\n"
	"float4 gMd    : register(c25);\n"
	"float4 gTint  : register(c26);\n"
	"float4 gShade : register(c27);\n";
static const char* const kVSMain =
	"void main(float3 p : POSITION, float3 vn : NORMAL, float4 vc : COLOR0, float2 t0 : TEXCOORD0, float2 t1 : TEXCOORD1,\n"
	"          float3 fn : TEXCOORD2, float4 fc : COLOR1,\n"
	"          out float4 op : POSITION, out float4 oc : COLOR0, out float2 ot0 : TEXCOORD0, out float2 ot1 : TEXCOORD1,\n"
	"          out float ofz : TEXCOORD2) {\n"
	"  glVertex(p, vn, vc, t0, t1, fn, fc, op, oc, ot0, ot1, ofz);\n"
	"}\n";
static const char* const kPSHead =
	"float4 gTex   : register(c0);\n"
	"float4 gAlpha : register(c1);\n"
	"float4 gFog   : register(c2);\n"
	"float4 gFogC  : register(c3);\n"
	"sampler2D s0 : register(s0);\n"
	"sampler2D s1 : register(s1);\n";
static const char* const kPSMain =
	"float4 main(float4 c : COLOR0, float2 t0 : TEXCOORD0, float2 t1 : TEXCOORD1, float fz : TEXCOORD2) : COLOR {\n"
	"  bool keep;\n"
	"  float4 o = glFragment(c, tex2D(s0, t0), tex2D(s1, t1), fz, keep);\n"
	"  clip(keep ? 1 : -1);\n"
	"  return o;\n"
	"}\n";

static IDirect3DVertexShader9* g_vs = 0;
static IDirect3DPixelShader9* g_ps = 0;
static IDirect3DVertexDeclaration9* g_decl = 0;

static void _buildShaders() {
	if (!g_dev || g_vs) return;
	const std::string vs = std::string(kVSHead) + kGlVertexStage + kVSMain;
	const std::string ps = std::string(kPSHead) + kGlFragmentStage + kPSMain;
	ID3DXBuffer* code = 0;
	ID3DXBuffer* err = 0;
	HRESULT hr = D3DXCompileShader(vs.c_str(), (UINT)vs.size(), 0, 0, "main", "vs_3_0", 0, &code, &err, 0);
	if (SUCCEEDED(hr) && code) g_dev->CreateVertexShader((const DWORD*)code->GetBufferPointer(), &g_vs);
	else if (err) DbgPrintF("[J2X] VS compile: %s\n", (const char*)err->GetBufferPointer());
	else DbgPrintF("[J2X] VS D3DXCompileShader hr=0x%08x\n", (unsigned)hr);
	if (code) { code->Release(); code = 0; }
	if (err) { err->Release(); err = 0; }
	hr = D3DXCompileShader(ps.c_str(), (UINT)ps.size(), 0, 0, "main", "ps_3_0", 0, &code, &err, 0);
	if (SUCCEEDED(hr) && code) g_dev->CreatePixelShader((const DWORD*)code->GetBufferPointer(), &g_ps);
	else if (err) DbgPrintF("[J2X] PS compile: %s\n", (const char*)err->GetBufferPointer());
	else DbgPrintF("[J2X] PS D3DXCompileShader hr=0x%08x\n", (unsigned)hr);
	if (code) code->Release();
	if (err) err->Release();
	static const D3DVERTEXELEMENT9 decl[] = {   // gpu::Vertex
		{ 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
		{ 0, 28, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 0, 44, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },   // the provoking normal
		{ 0, 56, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    1 },   // the provoking colour
		D3DDECL_END()
	};
	g_dev->CreateVertexDeclaration(decl, &g_decl);
	DbgPrintF((g_vs && g_ps && g_decl) ? "[J2X] Direct3D 9: shaders + declaration ready\n"
	                                   : "[J2X] Direct3D 9: shader build FAILED (see above)\n");
}

// =========================================================================== what is on the device
// Shadows of what was last given the device, so a draw only touches what changed. Cleared whenever the device's state is
// unknown (a new or reset device).
static float s_vsc[kVSConsts * 4], s_psc[kPSConsts * 4];
static bool s_constsKnown = false;
static IDirect3DBaseTexture9* s_tex[2];
static DWORD s_samp[2][5];               // ADDRESSU, ADDRESSV, MAGFILTER, MINFILTER, MIPFILTER
static IDirect3DVertexBuffer9* s_stream = 0;
static bool s_streamKnown = false;
static bool s_shadersSet = false;
static RenderState s_rs;
static bool s_rsKnown = false;

bool ready() { return g_dev && g_vs && g_ps && g_decl; }
const char* name() { return "Direct3D 9"; }
int targetWidth() { return g_fbW; }
int targetHeight() { return g_fbH; }
#ifdef _XBOX
bool halfPixelOffset() { return false; }   // (the 360's Display turns on D3DRS_HALFPIXELOFFSET: GL's pixel centres)
#else
bool halfPixelOffset() { return true; }    // (the PC's Direct3D 9 centres pixels on integers)
#endif

void forgetState() {
	s_constsKnown = false;
	for (int u = 0; u < 2; ++u) { s_tex[u] = (IDirect3DBaseTexture9*)~(size_t)0; for (int k = 0; k < 5; ++k) s_samp[u][k] = 0xFFFFFFFF; }
	s_streamKnown = false;
	s_shadersSet = false;
	s_rsKnown = false;
	if (g_dev) {
		// The alpha test, fog and lighting are the shaders' (Xenos has no fixed-function pipeline); the PC's fixed-function states
		// stay off so nothing doubles them.
#ifndef _XBOX
		g_dev->SetRenderState(D3DRS_LIGHTING, FALSE);
		g_dev->SetRenderState(D3DRS_FOGENABLE, FALSE);
		g_dev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
#endif
	}
}

static DWORD _f2dw(float f) { DWORD d; memcpy(&d, &f, 4); return d; }
static const D3DBLEND kBlend[] = { D3DBLEND_ZERO, D3DBLEND_ONE, D3DBLEND_SRCCOLOR, D3DBLEND_INVSRCCOLOR, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA,
                                   D3DBLEND_DESTALPHA, D3DBLEND_INVDESTALPHA, D3DBLEND_DESTCOLOR, D3DBLEND_INVDESTCOLOR, D3DBLEND_SRCALPHASAT };
static const D3DCMPFUNC kCompare[] = { D3DCMP_NEVER, D3DCMP_LESS, D3DCMP_EQUAL, D3DCMP_LESSEQUAL, D3DCMP_GREATER, D3DCMP_NOTEQUAL,
                                       D3DCMP_GREATEREQUAL, D3DCMP_ALWAYS };

// The render state, only what changed since the last draw (all of it after forgetState).
static void _applyState(const RenderState& rs) {
	const bool all = !s_rsKnown;
	const RenderState& o = s_rs;
	if (all || rs.blend != o.blend) g_dev->SetRenderState(D3DRS_ALPHABLENDENABLE, rs.blend ? TRUE : FALSE);
	if (all || rs.srcBlend != o.srcBlend) g_dev->SetRenderState(D3DRS_SRCBLEND, kBlend[rs.srcBlend]);
	if (all || rs.dstBlend != o.dstBlend) g_dev->SetRenderState(D3DRS_DESTBLEND, kBlend[rs.dstBlend]);
	if (all || rs.depthTest != o.depthTest) g_dev->SetRenderState(D3DRS_ZENABLE, rs.depthTest ? D3DZB_TRUE : D3DZB_FALSE);
	if (all || rs.depthFunc != o.depthFunc) g_dev->SetRenderState(D3DRS_ZFUNC, kCompare[rs.depthFunc]);
	if (all || rs.depthWrite != o.depthWrite) g_dev->SetRenderState(D3DRS_ZWRITEENABLE, rs.depthWrite ? TRUE : FALSE);
	if (all || rs.colorWrite != o.colorWrite)
		g_dev->SetRenderState(D3DRS_COLORWRITEENABLE, ((rs.colorWrite & 1) ? D3DCOLORWRITEENABLE_RED : 0) | ((rs.colorWrite & 2) ? D3DCOLORWRITEENABLE_GREEN : 0) |
		                                              ((rs.colorWrite & 4) ? D3DCOLORWRITEENABLE_BLUE : 0) | ((rs.colorWrite & 8) ? D3DCOLORWRITEENABLE_ALPHA : 0));
	if (all || rs.cull != o.cull) g_dev->SetRenderState(D3DRS_CULLMODE, rs.cull == kCullCW ? D3DCULL_CW : rs.cull == kCullCCW ? D3DCULL_CCW : D3DCULL_NONE);
	if (all || rs.slopeBias != o.slopeBias) g_dev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, _f2dw(rs.slopeBias));
	if (all || rs.depthBias != o.depthBias) g_dev->SetRenderState(D3DRS_DEPTHBIAS, _f2dw(rs.depthBias));
	if (all || rs.scissor != o.scissor) g_dev->SetRenderState(D3DRS_SCISSORTESTENABLE, rs.scissor ? TRUE : FALSE);
	if (rs.scissor && (all || rs.scX != o.scX || rs.scY != o.scY || rs.scW != o.scW || rs.scH != o.scH)) {
		RECT r; r.left = rs.scX; r.top = rs.scY; r.right = rs.scX + rs.scW; r.bottom = rs.scY + rs.scH;
		g_dev->SetScissorRect(&r);
	}
	if (all || rs.vpX != o.vpX || rs.vpY != o.vpY || rs.vpW != o.vpW || rs.vpH != o.vpH) {
		D3DVIEWPORT9 vp;
		vp.X = (DWORD)rs.vpX; vp.Y = (DWORD)rs.vpY; vp.Width = (DWORD)rs.vpW; vp.Height = (DWORD)rs.vpH; vp.MinZ = 0.0f; vp.MaxZ = 1.0f;
		g_dev->SetViewport(&vp);
	}
	s_rs = rs;
	s_rsKnown = true;
}

static void _setSampler(int u, D3DSAMPLERSTATETYPE kind, int slot, DWORD value) {
	if (s_samp[u][slot] == value) return;
	s_samp[u][slot] = value;
	g_dev->SetSamplerState(u, kind, value);
}

// Shaders, constants, textures and samplers: only what changed.
static void _applyInputs(const Constants& c, const Bindings& b) {
	if (!s_shadersSet) {
		g_dev->SetVertexShader(g_vs);
		g_dev->SetPixelShader(g_ps);
		g_dev->SetVertexDeclaration(g_decl);
		s_shadersSet = true;
	}
	if (!s_constsKnown || memcmp(s_vsc, c.vs, sizeof(s_vsc)) != 0) { memcpy(s_vsc, c.vs, sizeof(s_vsc)); g_dev->SetVertexShaderConstantF(0, c.vs, kVSConsts); }
	if (!s_constsKnown || memcmp(s_psc, c.ps, sizeof(s_psc)) != 0) { memcpy(s_psc, c.ps, sizeof(s_psc)); g_dev->SetPixelShaderConstantF(0, c.ps, kPSConsts); }
	s_constsKnown = true;
	for (int u = 0; u < 2; ++u) {
		IDirect3DBaseTexture9* want = b.tex[u] ? (IDirect3DBaseTexture9*)b.tex[u]->tex : 0;
		if (want != s_tex[u]) { g_dev->SetTexture(u, want); s_tex[u] = want; }
		if (!want) continue;
		const Sampler& s = b.samp[u];
		_setSampler(u, D3DSAMP_ADDRESSU, 0, s.wrapU ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
		_setSampler(u, D3DSAMP_ADDRESSV, 1, s.wrapV ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
		_setSampler(u, D3DSAMP_MAGFILTER, 2, s.mag == kLinear ? D3DTEXF_LINEAR : D3DTEXF_POINT);
		_setSampler(u, D3DSAMP_MINFILTER, 3, s.min == kLinear ? D3DTEXF_LINEAR : D3DTEXF_POINT);
		_setSampler(u, D3DSAMP_MIPFILTER, 4, s.mip < 0 ? D3DTEXF_NONE : s.mip == kLinear ? D3DTEXF_LINEAR : D3DTEXF_POINT);
	}
}

// Stream 0: bound only through here, and unbound before any buffer is released or locked (vb-released-while-bound).
static void _bindStream(IDirect3DVertexBuffer9* vb) {
	if (!g_dev || (s_streamKnown && vb == s_stream)) return;
	g_dev->SetStreamSource(0, vb, 0, vb ? sizeof(Vertex) : 0);
	s_stream = vb;
	s_streamKnown = true;
}

// =========================================================================== drawing
static int _primitives(Prim prim, int count) { return prim == kLines ? count / 2 : count / 3; }
static D3DPRIMITIVETYPE _type(Prim prim) { return prim == kLines ? D3DPT_LINELIST : D3DPT_TRIANGLELIST; }

void draw(Prim prim, const Vertex* v, int count, const RenderState& rs, const Constants& c, const Bindings& b) {
	const int n = _primitives(prim, count);
	if (!ready() || n <= 0) return;
	_applyState(rs);
	_applyInputs(c, b);
	s_streamKnown = false;   // DrawPrimitiveUP sets stream 0 itself (and leaves it unset)
	g_dev->DrawPrimitiveUP(_type(prim), (UINT)n, v, sizeof(Vertex));
}

Buffer createBuffer(const Vertex* v, int count) {
	if (!g_dev || count <= 0) return 0;
	const UINT bytes = (UINT)(count * sizeof(Vertex));
	IDirect3DVertexBuffer9* vb = 0;
	_bindStream(0);   // (no lock while a buffer may be the bound stream)
	if (FAILED(g_dev->CreateVertexBuffer(bytes, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &vb, 0)) || !vb) return 0;
	void* p = 0;
	if (FAILED(vb->Lock(0, bytes, &p, 0))) { vb->Release(); return 0; }
	memcpy(p, v, bytes);
	vb->Unlock();
	BufferObject* o = new BufferObject;
	o->vb = vb;
	return o;
}
void releaseBuffer(Buffer buf) {
	if (!buf) return;
	if (s_stream == buf->vb) _bindStream(0);   // never released while bound
	if (buf->vb) buf->vb->Release();
	delete buf;
}
void drawBuffer(Prim prim, Buffer buf, int count, const RenderState& rs, const Constants& c, const Bindings& b) {
	const int n = _primitives(prim, count);
	if (!ready() || !buf || !buf->vb || n <= 0) return;
	_applyState(rs);
	_applyInputs(c, b);
	_bindStream(buf->vb);
	g_dev->DrawPrimitive(_type(prim), 0, (UINT)n);
	_bindStream(0);   // leave stream 0 empty (a later release or lock of this buffer must not find it bound)
}

// A frame's new display lists: one step out of the scene for all of their buffers (one device stall on the 360, not one each).
static int s_uploadDepth = 0;
static bool s_uploadWasInScene = false;
void beginUploads() { if (s_uploadDepth++ == 0) { s_uploadWasInScene = s_inScene; if (s_inScene) sceneEnd(); } }
void endUploads() { if (--s_uploadDepth == 0 && s_uploadWasInScene) sceneBegin(); }

// GL clears the whole target where the masks let it (the masks are decided by the layer), inside the scissor box when that test
// is on. Direct3D 9's Clear takes the viewport, so the viewport is the whole target for it.
void clear(bool color, bool depth, bool stencil, float r, float g, float b, float a, float z, const RenderState& rs) {
	if (!g_dev) return;
	DWORD flags = 0;
	if (color) flags |= D3DCLEAR_TARGET;
	if (depth) flags |= D3DCLEAR_ZBUFFER;
	if (stencil) flags |= D3DCLEAR_STENCIL;
	if (!flags) return;
	RenderState whole = rs;
	whole.vpX = 0; whole.vpY = 0; whole.vpW = g_fbW; whole.vpH = g_fbH;
	_applyState(whole);
	D3DRECT box;
	box.x1 = rs.scX; box.y1 = rs.scY; box.x2 = rs.scX + rs.scW; box.y2 = rs.scY + rs.scH;
	const D3DCOLOR c = D3DCOLOR_COLORVALUE(r, g, b, a);
	if (rs.scissor) g_dev->Clear(1, &box, flags, c, z, 0);
	else g_dev->Clear(0, 0, flags, c, z, 0);
}

// =========================================================================== textures
Texture createTexture(int w, int h, int levels) {
	if (!g_dev || w <= 0 || h <= 0) return 0;
	IDirect3DTexture9* made = 0;
	HRESULT hr;
	{
		OutOfScene out;   // (the 360 makes resources only out of a scene)
		hr = g_dev->CreateTexture((UINT)w, (UINT)h, (UINT)levels, 0, J2X_TEXFMT, J2X_TEXPOOL, &made, 0);
	}
	if (FAILED(hr) || !made) { DbgPrintF("[J2X] CreateTexture %dx%d failed: 0x%08x\n", w, h, (unsigned)hr); return 0; }
	TextureObject* t = new TextureObject;
	t->tex = made; t->w = w; t->h = h; t->levels = levels;
	return t;
}
static void _unbindTexture(Texture t) {
	for (int u = 0; u < 2; ++u)
		if (g_dev && s_tex[u] == (IDirect3DBaseTexture9*)t->tex) { g_dev->SetTexture(u, 0); s_tex[u] = 0; }   // (the 360: no lock or release while bound)
}
void releaseTexture(Texture t) {
	if (!t) return;
	_unbindTexture(t);
	if (t->tex) t->tex->Release();
	delete t;
}
bool writeTexture(Texture t, int level, int x, int y, int w, int h, const unsigned int* argb) {
	if (!t || !t->tex || w <= 0 || h <= 0) return false;
	_unbindTexture(t);
	D3DLOCKED_RECT lr;
	RECT rc; rc.left = x; rc.top = y; rc.right = x + w; rc.bottom = y + h;
	if (FAILED(t->tex->LockRect(level, &lr, &rc, 0))) return false;
	for (int r = 0; r < h; ++r) memcpy((unsigned char*)lr.pBits + r * lr.Pitch, argb + (size_t)r * w, (size_t)w * 4);
	t->tex->UnlockRect(level);
	return true;
}
bool readTexture(Texture t, int level, std::vector<unsigned int>& argb, int& w, int& h) {
	if (!t || !t->tex) return false;
	D3DSURFACE_DESC d;
	if (FAILED(t->tex->GetLevelDesc(level, &d))) return false;
	_unbindTexture(t);
	D3DLOCKED_RECT lr;
	if (FAILED(t->tex->LockRect(level, &lr, 0, D3DLOCK_READONLY))) return false;
	w = (int)d.Width; h = (int)d.Height;
	argb.resize((size_t)w * h);
	for (int r = 0; r < h; ++r) memcpy(&argb[(size_t)r * w], (const unsigned char*)lr.pBits + r * lr.Pitch, (size_t)w * 4);
	t->tex->UnlockRect(level);
	return true;
}

// =========================================================================== the frame
// The backbuffer as drawn so far, in a system-memory copy. The Xbox 360 would resolve it (not written: no 360 game in the corpus
// reads the frame).
bool readFrame(std::vector<unsigned int>& out, int& fw, int& fh) {
#ifdef _XBOX
	(void)out; (void)fw; (void)fh;
	J2X_UNHANDLED("reading the frame (glReadPixels / glCopyTexSubImage2D) on the Xbox 360", 0);
	return false;
#else
	if (!g_dev) return false;
	IDirect3DSurface9* back = 0;
	IDirect3DSurface9* copy = 0;
	if (FAILED(g_dev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back)) || !back) return false;
	D3DSURFACE_DESC d;
	back->GetDesc(&d);
	bool ok = false;
	if (SUCCEEDED(g_dev->CreateOffscreenPlainSurface(d.Width, d.Height, d.Format, D3DPOOL_SYSTEMMEM, &copy, 0)) && copy &&
	    SUCCEEDED(g_dev->GetRenderTargetData(back, copy))) {
		D3DLOCKED_RECT lr;
		if (SUCCEEDED(copy->LockRect(&lr, 0, D3DLOCK_READONLY))) {
			fw = (int)d.Width; fh = (int)d.Height;
			out.resize((size_t)fw * fh);
			for (int y = 0; y < fh; ++y) memcpy(&out[(size_t)y * fw], (const char*)lr.pBits + (size_t)y * lr.Pitch, (size_t)fw * 4);
			copy->UnlockRect();
			ok = true;
		}
	}
	if (copy) copy->Release();
	back->Release();
	return ok;
#endif
}

}  // namespace gpu
}  // namespace j2xgl
