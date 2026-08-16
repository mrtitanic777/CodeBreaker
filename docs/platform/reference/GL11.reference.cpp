// GL11.cpp -- game-agnostic OpenGL 1.x -> Direct3D 9 emulator (see GL11.reference.h for the design).
//
// Self-contained: owns its device, matrix stacks, texture table, immediate-mode accumulator, and the
// single built-in shader every draw goes through (Xenos has no fixed-function pipeline). No game class
// is referenced -- this backs any converted renderer's GL11::glX calls.
//
// HARDWARE-VERIFY notes are inline. The biggest ones: (1) the 360 XDK may not expose runtime
// D3DXCompileShader -- if so, replace _buildShader() with pre-compiled shader blobs (xsl/fxc offline);
// (2) GL right-handed vs D3D left-handed => projection Z + cull winding may need flipping;
// (3) the Display shim must call GL11::glSetDevice(dev) once the D3D device exists.

#include "stdafx.h"     // MUST be first: the Xbox 360 project uses /Yu precompiled headers
#include "GL11.h"
#include "Common.h"     // DbgPrintF -- boot trace sink that surfaces on the 360/emulator
#include <vector>
#include <string.h>

namespace GL11 {

// --------------------------------------------------------------------------- vertex format
// One interleaved vertex the whole emulator funnels through (immediate + array + selection gather).
struct GLVert {
	float x, y, z;
	D3DCOLOR color;     // ARGB
	float u, v;
};
static const DWORD GLVERT_FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

// --------------------------------------------------------------------------- device + shader
static IDirect3DDevice9* g_dev = 0;
static IDirect3DVertexShader9* g_vs = 0;
static IDirect3DPixelShader9*  g_ps = 0;

void glSetDevice(IDirect3DDevice9* dev) { g_dev = dev; }
IDirect3DDevice9* device() { return g_dev; }

// --------------------------------------------------------------------------- matrix stacks
static int g_matMode = GL_MODELVIEW;
static std::vector<D3DXMATRIX> g_mv;      // modelview stack (top = back())
static std::vector<D3DXMATRIX> g_proj;    // projection stack
static std::vector<D3DXMATRIX> g_tex;     // texture-matrix stack (rarely used)

static std::vector<D3DXMATRIX>& curStack() {
	if (g_matMode == GL_PROJECTION) return g_proj;
	if (g_matMode == GL_TEXTURE)    return g_tex;
	return g_mv;
}
static D3DXMATRIX& top(std::vector<D3DXMATRIX>& s) {
	if (s.empty()) { D3DXMATRIX i; D3DXMatrixIdentity(&i); s.push_back(i); }
	return s.back();
}
static D3DXMATRIX toXM(FloatBuffer* m) {
	// GL column-major 16 floats -> D3DXMATRIX (row-major). D3DX multiplies row-vector*matrix, GL uses
	// column-vector; feeding GL data as-is + our combined = mv*proj (below) keeps the algebra consistent.
	D3DXMATRIX r; float* d = (float*)r.m;
	for (int i = 0; i < 16; ++i) d[i] = m ? m->get(i) : (i % 5 == 0 ? 1.0f : 0.0f);
	return r;
}

// --------------------------------------------------------------------------- current attributes
static float g_cr = 1, g_cg = 1, g_cb = 1, g_ca = 1;     // current color (0..1)
static float g_tu = 0, g_tv = 0;                          // current texcoord
static D3DCOLOR curColor() {
	int a = (int)(g_ca * 255.0f + 0.5f), r = (int)(g_cr * 255.0f + 0.5f),
	    g = (int)(g_cg * 255.0f + 0.5f), b = (int)(g_cb * 255.0f + 0.5f);
	return D3DCOLOR_ARGB(a & 255, r & 255, g & 255, b & 255);
}

// --------------------------------------------------------------------------- render-state mirror
static bool g_texEnabled = false, g_blend = false, g_depth = true, g_cull = false, g_alphaTest = false, g_fog = false;
static float g_alphaRef = 0.0f;
static float g_clearR = 0, g_clearG = 0, g_clearB = 0, g_clearA = 1; static float g_clearDepth = 1.0f;
static D3DVIEWPORT9 g_vp = { 0, 0, 640, 480, 0.0f, 1.0f };
static float g_fogStart = 0, g_fogEnd = 1; static D3DCOLOR g_fogColor = 0;

// --------------------------------------------------------------------------- textures (self table)
struct GLTex {
	IDirect3DTexture9* tex;
	int w, h; int wrapS, wrapT, minF, magF;
	GLTex() : tex(0), w(0), h(0), wrapS(GL_REPEAT), wrapT(GL_REPEAT), minF(GL_NEAREST), magF(GL_NEAREST) {}
};
static std::vector<GLTex> g_textures;      // index 0 reserved as "no texture" (GL id 0)
static int g_boundTex = 0;

static GLTex* boundTex() {
	if (g_boundTex > 0 && g_boundTex < (int)g_textures.size()) return &g_textures[g_boundTex];
	return 0;
}

// --------------------------------------------------------------------------- immediate accumulator
static int g_beginMode = -1;
static std::vector<GLVert> g_imm;

// --------------------------------------------------------------------------- display lists
struct GLList { std::vector<GLVert> tris; bool used; GLList() : used(false) {} };
static std::vector<GLList> g_lists;      // index by list id (0 reserved)
static int g_recordingList = 0;          // != 0 while inside glNewList/glEndList

// --------------------------------------------------------------------------- vertex-array pointers
static bool g_vaVertex = false, g_vaColor = false, g_vaTex = false;
static const char* g_vpPtr = 0; static int g_vpSize = 3, g_vpStride = 0;
static const char* g_cpPtr = 0; static int g_cpSize = 4, g_cpStride = 0; static bool g_cpFloat = false;
static const char* g_tpPtr = 0; static int g_tpSize = 2, g_tpStride = 0;

// --------------------------------------------------------------------------- selection state
static int g_renderMode = GL_RENDER;
static std::vector<int> g_nameStack;
static IntBuffer* g_selBuf = 0; static int g_selWritten = 0; static int g_hitCount = 0;
static bool g_hitThisName = false; static float g_hitMinZ = 0, g_hitMaxZ = 0;

// =========================================================================== shader build
// SEPARATE VS/PS sources, each entry "main" -- this mirrors the Beta port's proven D3DXCompileShader
// usage on this exact platform (a single combined source with "VS"/"PS" entry points failed to
// compile). VS constants and PS constants are INDEPENDENT register files, so both start at c0.
static const char* kVS =
	"float4x4 gWVP : register(c0);\n"
	"void main(float3 p:POSITION, float4 c:COLOR0, float2 t:TEXCOORD0,\n"
	"          out float4 op:POSITION, out float4 oc:COLOR0, out float2 ot:TEXCOORD0, out float ofz:TEXCOORD1){\n"
	"  op = mul(float4(p,1), gWVP); oc = c; ot = t; ofz = op.w;\n"
	"}\n";
static const char* kPS =
	"float4 gCtl : register(c0);\n"   // x=useTex, y=alphaTest, z=alphaRef, w=fogEnable
	"float4 gFog : register(c1);\n"   // x=start, y=end
	"float4 gFogC: register(c2);\n"
	"sampler2D s0 : register(s0);\n"
	"float4 main(float4 c:COLOR0, float2 t:TEXCOORD0, float fz:TEXCOORD1):COLOR{\n"
	"  float4 o = c;\n"
	"  if (gCtl.x > 0.5) o *= tex2D(s0, t);\n"
	"  if (gCtl.y > 0.5) clip(o.a - gCtl.z);\n"   // alpha test: discard where o.a < ref
	"  if (gCtl.w > 0.5){ float f = saturate((gFog.y - fz)/(gFog.y-gFog.x)); o.rgb = lerp(gFogC.rgb, o.rgb, f); }\n"
	"  return o;\n"
	"}\n";

static IDirect3DVertexDeclaration9* g_decl = 0;   // SM3 needs an explicit decl (SetFVF is not enough on Xenos)

static void _buildShader() {
	if (!g_dev || g_vs) return;
	ID3DXBuffer* code = 0; ID3DXBuffer* err = 0;
	HRESULT hr = D3DXCompileShader(kVS, (UINT)strlen(kVS), 0, 0, "main", "vs_3_0", 0, &code, &err, 0);
	if (SUCCEEDED(hr) && code) g_dev->CreateVertexShader((const DWORD*)code->GetBufferPointer(), &g_vs);
	else if (err) DbgPrintF("[J2X] VS compile err: %s\n", (const char*)err->GetBufferPointer());
	else DbgPrintF("[J2X] VS D3DXCompileShader hr=0x%08x\n", (unsigned)hr);
	if (code) { code->Release(); code = 0; } if (err) { err->Release(); err = 0; }

	hr = D3DXCompileShader(kPS, (UINT)strlen(kPS), 0, 0, "main", "ps_3_0", 0, &code, &err, 0);
	if (SUCCEEDED(hr) && code) g_dev->CreatePixelShader((const DWORD*)code->GetBufferPointer(), &g_ps);
	else if (err) DbgPrintF("[J2X] PS compile err: %s\n", (const char*)err->GetBufferPointer());
	else DbgPrintF("[J2X] PS D3DXCompileShader hr=0x%08x\n", (unsigned)hr);
	if (code) code->Release(); if (err) err->Release();

	// Explicit vertex declaration for GLVert: POSITION float3 @0, COLOR D3DCOLOR @12, TEXCOORD float2 @16.
	static const D3DVERTEXELEMENT9 decl[] = {
		{ 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
		{ 0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	g_dev->CreateVertexDeclaration(decl, &g_decl);

	DbgPrintF((g_vs && g_ps && g_decl) ? "[J2X] built-in shader + decl OK\n"
	                                   : "[J2X] built-in shader FAILED (see VS/PS err above)\n");
}

// combined modelview*projection, transposed for HLSL row-major c-register upload.
static void _uploadMatrices() {
	if (!g_dev) return;
	D3DXMATRIX mv = g_mv.empty()   ? *(new D3DXMATRIX) : g_mv.back();
	D3DXMATRIX pr = g_proj.empty() ? *(new D3DXMATRIX) : g_proj.back();
	if (g_mv.empty())   D3DXMatrixIdentity(&mv);
	if (g_proj.empty()) D3DXMatrixIdentity(&pr);
	D3DXMATRIX wvp = mv * pr;                 // row-vector convention: v * MV * P
	D3DXMATRIX t;  D3DXMatrixTranspose(&t, &wvp);
	g_dev->SetVertexShaderConstantF(0, (const float*)t.m, 4);
}

static void _uploadControl() {
	if (!g_dev) return;
	float ctl[4] = { g_texEnabled && boundTex() ? 1.0f : 0.0f, g_alphaTest ? 1.0f : 0.0f, g_alphaRef, g_fog ? 1.0f : 0.0f };
	float fog[4] = { g_fogStart, g_fogEnd, 0, 0 };
	float fogc[4] = { ((g_fogColor >> 16) & 255) / 255.0f, ((g_fogColor >> 8) & 255) / 255.0f,
	                  (g_fogColor & 255) / 255.0f, 1.0f };
	g_dev->SetPixelShaderConstantF(0, ctl, 1);
	g_dev->SetPixelShaderConstantF(1, fog, 1);
	g_dev->SetPixelShaderConstantF(2, fogc, 1);
}

// =========================================================================== lifecycle
void glInit() {
	D3DXMATRIX id; D3DXMatrixIdentity(&id);
	g_mv.clear();   g_mv.push_back(id);
	g_proj.clear(); g_proj.push_back(id);
	g_tex.clear();  g_tex.push_back(id);
	g_textures.clear(); g_textures.push_back(GLTex());   // reserve id 0
	g_lists.clear(); g_recordingList = 0;
	_buildShader();
	if (g_dev) {
		// NOTE: no D3DRS_LIGHTING -- Xenos (360) has no fixed-function pipeline, so that render state
		// does not exist in the XDK. Lighting is off by construction (the shader does color*texture).
		g_dev->SetRenderState(D3DRS_ZENABLE, TRUE);
		g_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
}

// =========================================================================== matrix ops
void glMatrixMode(int mode) { g_matMode = mode; }
void glPushMatrix() { std::vector<D3DXMATRIX>& s = curStack(); s.push_back(top(s)); }
void glPopMatrix()  { std::vector<D3DXMATRIX>& s = curStack(); if (s.size() > 1) s.pop_back(); }
void glLoadIdentity() { D3DXMatrixIdentity(&top(curStack())); }
void glLoadMatrix(FloatBuffer* m) { top(curStack()) = toXM(m); }
void glMultMatrix(FloatBuffer* m) { D3DXMATRIX& t = top(curStack()); D3DXMATRIX in = toXM(m); t = in * t; }
void glTranslatef(float x, float y, float z) {
	D3DXMATRIX m; D3DXMatrixTranslation(&m, x, y, z); D3DXMATRIX& t = top(curStack()); t = m * t;
}
void glRotatef(float angle, float x, float y, float z) {
	D3DXVECTOR3 ax(x, y, z); D3DXMATRIX m; D3DXMatrixRotationAxis(&m, &ax, D3DXToRadian(angle));
	D3DXMATRIX& t = top(curStack()); t = m * t;
}
void glScalef(float x, float y, float z) {
	D3DXMATRIX m; D3DXMatrixScaling(&m, x, y, z); D3DXMATRIX& t = top(curStack()); t = m * t;
}
void glOrtho(double l, double r, double b, double t, double n, double f) {
	// GL glOrtho is right-handed; D3DXMatrixOrthoOffCenterRH matches. HARDWARE-VERIFY Z range (GL -1..1 vs D3D 0..1).
	D3DXMATRIX m; D3DXMatrixOrthoOffCenterRH(&m, (float)l, (float)r, (float)b, (float)t, (float)n, (float)f);
	D3DXMATRIX& tp = top(curStack()); tp = m * tp;
}
void glFrustum(double l, double r, double b, double t, double n, double f) {
	D3DXMATRIX m; D3DXMatrixPerspectiveOffCenterRH(&m, (float)l, (float)r, (float)b, (float)t, (float)n, (float)f);
	D3DXMATRIX& tp = top(curStack()); tp = m * tp;
}
void glViewport(int x, int y, int w, int h) {
	g_vp.X = x; g_vp.Y = y; g_vp.Width = w; g_vp.Height = h; g_vp.MinZ = 0.0f; g_vp.MaxZ = 1.0f;
	if (g_dev) g_dev->SetViewport(&g_vp);
}
void glLoadProjection(const D3DXMATRIX& m) { g_matMode = GL_PROJECTION; top(g_proj) = m; }

void glGetFloat(int pname, FloatBuffer* buf) {
	if (!buf) return;
	const D3DXMATRIX* src = 0;
	if (pname == GL_MODELVIEW_MATRIX)  src = &top(g_mv);
	else if (pname == GL_PROJECTION_MATRIX) src = &top(g_proj);
	if (src) { const float* d = (const float*)src->m; for (int i = 0; i < 16; ++i) buf->put(i, d[i]); }
}
void glGetInteger(int pname, IntBuffer* buf) {
	if (!buf) return;
	if (pname == GL_VIEWPORT) { buf->put(0, g_vp.X); buf->put(1, g_vp.Y); buf->put(2, g_vp.Width); buf->put(3, g_vp.Height); }
}

// =========================================================================== immediate mode
void glBegin(int mode) { g_beginMode = mode; g_imm.clear(); }
void glVertex3f(float x, float y, float z) {
	GLVert v; v.x = x; v.y = y; v.z = z; v.color = curColor(); v.u = g_tu; v.v = g_tv; g_imm.push_back(v);
}
void glVertex2f(float x, float y) { glVertex3f(x, y, 0.0f); }
void glTexCoord2f(float u, float v) { g_tu = u; g_tv = v; }
void glColor3f(float r, float g, float b) { g_cr = r; g_cg = g; g_cb = b; g_ca = 1.0f; }
void glColor4f(float r, float g, float b, float a) { g_cr = r; g_cg = g; g_cb = b; g_ca = a; }
void glColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	g_cr = r / 255.0f; g_cg = g / 255.0f; g_cb = b / 255.0f; g_ca = a / 255.0f;
}
void glNormal3f(float, float, float) { /* light baked into vertex color */ }

// Expand a run of GL primitives (quad/strip/fan -> triangles) into a triangle list.
static void _gatherTris(int mode, const GLVert* v, int n, std::vector<GLVert>& tri) {
	if (mode == GL_QUADS) {
		for (int i = 0; i + 3 < n; i += 4) {
			tri.push_back(v[i]); tri.push_back(v[i+1]); tri.push_back(v[i+2]);
			tri.push_back(v[i]); tri.push_back(v[i+2]); tri.push_back(v[i+3]);
		}
	} else if (mode == GL_TRIANGLES) {
		for (int i = 0; i + 2 < n; i += 3) { tri.push_back(v[i]); tri.push_back(v[i+1]); tri.push_back(v[i+2]); }
	} else if (mode == GL_TRIANGLE_STRIP) {
		for (int i = 0; i + 2 < n; ++i) {
			if (i & 1) { tri.push_back(v[i+1]); tri.push_back(v[i]); tri.push_back(v[i+2]); }
			else       { tri.push_back(v[i]); tri.push_back(v[i+1]); tri.push_back(v[i+2]); }
		}
	} else if (mode == GL_TRIANGLE_FAN || mode == GL_POLYGON) {
		for (int i = 1; i + 1 < n; ++i) { tri.push_back(v[0]); tri.push_back(v[i]); tri.push_back(v[i+1]); }
	}   // points/lines: not needed by the target renderers (extend if required)
}

// Draw a ready triangle list through the shared shader + current matrix/texture state.
static void _drawTris(const std::vector<GLVert>& tri) {
	if (!g_dev || !g_vs || tri.empty()) return;
	_uploadMatrices(); _uploadControl();
	g_dev->SetVertexShader(g_vs); g_dev->SetPixelShader(g_ps);
	g_dev->SetVertexDeclaration(g_decl);   // SM3 vertex decl (not SetFVF)
	GLTex* bt = boundTex();
	if (g_texEnabled && bt && bt->tex) {
		g_dev->SetTexture(0, bt->tex);
		g_dev->SetSamplerState(0, D3DSAMP_ADDRESSU, bt->wrapS == GL_CLAMP ? D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP);
		g_dev->SetSamplerState(0, D3DSAMP_ADDRESSV, bt->wrapT == GL_CLAMP ? D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP);
		D3DTEXTUREFILTERTYPE mag = (bt->magF == GL_LINEAR) ? D3DTEXF_LINEAR : D3DTEXF_POINT;
		g_dev->SetSamplerState(0, D3DSAMP_MAGFILTER, mag);
		g_dev->SetSamplerState(0, D3DSAMP_MINFILTER, mag);
	} else {
		g_dev->SetTexture(0, 0);
	}
	g_dev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, (UINT)(tri.size() / 3), &tri[0], sizeof(GLVert));
}

// Gather a primitive run, then either RECORD it into the open display list or draw it now.
static void _drawGather(int mode, const GLVert* v, int n) {
	if (n <= 0) return;
	std::vector<GLVert> tri;
	_gatherTris(mode, v, n, tri);
	if (tri.empty()) return;
	if (g_recordingList > 0 && g_recordingList < (int)g_lists.size()) {
		std::vector<GLVert>& L = g_lists[g_recordingList].tris;
		L.insert(L.end(), tri.begin(), tri.end());   // capture untransformed; matrix applied at glCallList
		return;
	}
	_drawTris(tri);
}

void glEnd() {
	if (g_renderMode == GL_SELECT) {
		// In selection mode any geometry inside the (pick) frustum registers a hit on the current name.
		if (!g_imm.empty()) g_hitThisName = true;
	} else {
		_drawGather(g_beginMode, g_imm.empty() ? 0 : &g_imm[0], (int)g_imm.size());
	}
	g_beginMode = -1; g_imm.clear();
}

// =========================================================================== render state
static void _rs(DWORD s, DWORD v) { if (g_dev) g_dev->SetRenderState((D3DRENDERSTATETYPE)s, v); }
static DWORD _blendArg(int f) {
	switch (f) {
		case GL_ZERO: return D3DBLEND_ZERO; case GL_ONE: return D3DBLEND_ONE;
		case GL_SRC_ALPHA: return D3DBLEND_SRCALPHA; case GL_ONE_MINUS_SRC_ALPHA: return D3DBLEND_INVSRCALPHA;
		case GL_SRC_COLOR: return D3DBLEND_SRCCOLOR; case GL_ONE_MINUS_SRC_COLOR: return D3DBLEND_INVSRCCOLOR;
		case GL_DST_COLOR: return D3DBLEND_DESTCOLOR; case GL_ONE_MINUS_DST_COLOR: return D3DBLEND_INVDESTCOLOR;
		case GL_DST_ALPHA: return D3DBLEND_DESTALPHA; default: return D3DBLEND_ONE;
	}
}
void glEnable(int cap) {
	switch (cap) {
		case GL_TEXTURE_2D: g_texEnabled = true; break;
		case GL_BLEND: g_blend = true; _rs(D3DRS_ALPHABLENDENABLE, TRUE); break;
		case GL_DEPTH_TEST: g_depth = true; _rs(D3DRS_ZENABLE, TRUE); break;
		case GL_CULL_FACE: g_cull = true; _rs(D3DRS_CULLMODE, D3DCULL_CCW); break;   // HARDWARE-VERIFY winding
		case GL_ALPHA_TEST: g_alphaTest = true; break;
		case GL_FOG: g_fog = true; break;
		default: break;
	}
}
void glDisable(int cap) {
	switch (cap) {
		case GL_TEXTURE_2D: g_texEnabled = false; break;
		case GL_BLEND: g_blend = false; _rs(D3DRS_ALPHABLENDENABLE, FALSE); break;
		case GL_DEPTH_TEST: g_depth = false; _rs(D3DRS_ZENABLE, FALSE); break;
		case GL_CULL_FACE: g_cull = false; _rs(D3DRS_CULLMODE, D3DCULL_NONE); break;
		case GL_ALPHA_TEST: g_alphaTest = false; break;
		case GL_FOG: g_fog = false; break;
		default: break;
	}
}
void glBlendFunc(int s, int d) { _rs(D3DRS_SRCBLEND, _blendArg(s)); _rs(D3DRS_DESTBLEND, _blendArg(d)); }
void glDepthFunc(int f) {
	DWORD c = D3DCMP_LESSEQUAL;
	if (f == GL_LESS) c = D3DCMP_LESS; else if (f == GL_GREATER) c = D3DCMP_GREATER;
	else if (f == GL_GEQUAL) c = D3DCMP_GREATEREQUAL; else if (f == GL_EQUAL) c = D3DCMP_EQUAL;
	_rs(D3DRS_ZFUNC, c);
}
void glDepthMask(bool flag) { _rs(D3DRS_ZWRITEENABLE, flag ? TRUE : FALSE); }
void glColorMask(bool r, bool g, bool b, bool a) {
	DWORD m = (r ? D3DCOLORWRITEENABLE_RED : 0) | (g ? D3DCOLORWRITEENABLE_GREEN : 0)
	        | (b ? D3DCOLORWRITEENABLE_BLUE : 0) | (a ? D3DCOLORWRITEENABLE_ALPHA : 0);
	_rs(D3DRS_COLORWRITEENABLE, m);
}
void glCullFace(int) { /* winding handled by enable(GL_CULL_FACE); HARDWARE-VERIFY */ }
void glAlphaFunc(int, float ref) { g_alphaRef = ref; }
void glShadeModel(int) {}
void glClearColor(float r, float g, float b, float a) { g_clearR = r; g_clearG = g; g_clearB = b; g_clearA = a; }
void glClearDepth(double d) { g_clearDepth = (float)d; }
void glClear(int mask) {
	if (!g_dev) return;
	DWORD flags = 0;
	if (mask & GL_COLOR_BUFFER_BIT) flags |= D3DCLEAR_TARGET;
	if (mask & GL_DEPTH_BUFFER_BIT) flags |= D3DCLEAR_ZBUFFER;
	D3DCOLOR c = D3DCOLOR_COLORVALUE(g_clearR, g_clearG, g_clearB, g_clearA);
	g_dev->Clear(0, 0, flags, c, g_clearDepth, 0);
}
void glHint(int, int) {}
void glPolygonOffset(float, float) { /* HARDWARE-VERIFY: map to D3DRS_DEPTHBIAS if z-fighting appears */ }
void glLineWidth(float) {}
void glColorMaterial(int, int) {}

// =========================================================================== fog
void glFogf(int pname, float param) {
	if (pname == GL_FOG_START) g_fogStart = param; else if (pname == GL_FOG_END) g_fogEnd = param;
}
void glFogi(int pname, int param) { glFogf(pname, (float)param); }
void glFog(int pname, FloatBuffer* p) {
	if (pname == GL_FOG_COLOR && p) {
		int r = (int)(p->get(0) * 255), g = (int)(p->get(1) * 255), b = (int)(p->get(2) * 255);
		g_fogColor = D3DCOLOR_XRGB(r & 255, g & 255, b & 255);
	}
}

// =========================================================================== textures
int glGenTextures() {
	g_textures.push_back(GLTex());
	return (int)g_textures.size() - 1;
}
void glGenTextures(IntBuffer* buf) {
	if (!buf) return;
	for (int i = buf->position(); i < buf->limit(); ++i) buf->put(i, glGenTextures());
}
void glDeleteTextures(int id) {
	if (id > 0 && id < (int)g_textures.size() && g_textures[id].tex) { g_textures[id].tex->Release(); g_textures[id].tex = 0; }
}
void glDeleteTextures(IntBuffer* buf) {
	if (!buf) return;
	for (int i = buf->position(); i < buf->limit(); ++i) glDeleteTextures(buf->get(i));
}
void glBindTexture(int, int id) { g_boundTex = id; }
void glTexParameteri(int, int pname, int param) {
	GLTex* t = boundTex(); if (!t) return;
	if (pname == GL_TEXTURE_WRAP_S) t->wrapS = param; else if (pname == GL_TEXTURE_WRAP_T) t->wrapT = param;
	else if (pname == GL_TEXTURE_MIN_FILTER) t->minF = param; else if (pname == GL_TEXTURE_MAG_FILTER) t->magF = param;
}
void glTexParameterf(int target, int pname, float param) { glTexParameteri(target, pname, (int)param); }

// Upload RGBA bytes into a fresh managed texture bound to the current id. GL byte order is R,G,B,A;
// D3DFMT_A8R8G8B8 wants B,G,R,A in memory -> swizzle per pixel. HARDWARE-VERIFY the 360 wants tiled
// textures (XGTileTextureLevel) -- a LOCKED linear D3DFMT_A8R8G8B8 texture may need untiling.
static void _uploadRGBA(GLTex* t, int w, int h, const unsigned char* rgba) {
	if (!g_dev || !t || w <= 0 || h <= 0) return;
	if (t->tex) { t->tex->Release(); t->tex = 0; }
	if (FAILED(g_dev->CreateTexture(w, h, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &t->tex, 0))) return;
	t->w = w; t->h = h;
	D3DLOCKED_RECT lr;
	if (FAILED(t->tex->LockRect(0, &lr, 0, 0))) return;
	for (int y = 0; y < h; ++y) {
		unsigned char* dst = (unsigned char*)lr.pBits + y * lr.Pitch;
		const unsigned char* src = rgba + y * w * 4;
		for (int x = 0; x < w; ++x) {
			unsigned char r = src[0], g = src[1], b = src[2], a = src[3]; src += 4;
			// ENDIAN-SAFE: write the texel as a native 32-bit ARGB value. On the big-endian 360/
			// emulator this stores [A,R,G,B] (correct D3DFMT_A8R8G8B8); a byte-by-byte [B,G,R,A] write
			// (little-endian PC layout) scrambled every channel -- and put ALPHA in the blue slot, so
			// alpha-tested terrain got clip()'d away (the missing ground). [J2X-BUGFIX tex-endianness]
			*(unsigned int*)dst = ((unsigned int)a << 24) | ((unsigned int)r << 16)
			                    | ((unsigned int)g << 8) | (unsigned int)b;
			dst += 4;
		}
	}
	t->tex->UnlockRect(0);
}
void glTexImage2D(int, int, int, int w, int h, int, int, int, ByteBuffer* pixels) {
	GLTex* t = boundTex(); if (!t) return;
	_uploadRGBA(t, w, h, pixels ? (const unsigned char*)pixels->base() : 0);
}
void glTexImage2D(int, int, int, int w, int h, int, int, int, IntBuffer* pixels) {
	GLTex* t = boundTex(); if (!t) return;
	_uploadRGBA(t, w, h, pixels ? (const unsigned char*)pixels->base() : 0);
}
void glTexSubImage2D(int, int, int xoff, int yoff, int w, int h, int, int, ByteBuffer* pixels) {
	GLTex* t = boundTex(); if (!t || !t->tex || !pixels) return;
	D3DLOCKED_RECT lr; RECT rc; rc.left = xoff; rc.top = yoff; rc.right = xoff + w; rc.bottom = yoff + h;
	if (FAILED(t->tex->LockRect(0, &lr, &rc, 0))) return;
	const unsigned char* src = (const unsigned char*)pixels->base();
	for (int y = 0; y < h; ++y) {
		unsigned char* dst = (unsigned char*)lr.pBits + y * lr.Pitch;
		for (int x = 0; x < w; ++x) {
			unsigned char r = src[0], g = src[1], b = src[2], a = src[3]; src += 4;
			dst[0] = b; dst[1] = g; dst[2] = r; dst[3] = a; dst += 4;
		}
	}
	t->tex->UnlockRect(0);
}
void glPixelStorei(int, int) {}

// =========================================================================== vertex arrays
void glEnableClientState(int cap) {
	if (cap == GL_VERTEX_ARRAY) g_vaVertex = true; else if (cap == GL_COLOR_ARRAY) g_vaColor = true;
	else if (cap == GL_TEXTURE_COORD_ARRAY) g_vaTex = true;
}
void glDisableClientState(int cap) {
	if (cap == GL_VERTEX_ARRAY) g_vaVertex = false; else if (cap == GL_COLOR_ARRAY) g_vaColor = false;
	else if (cap == GL_TEXTURE_COORD_ARRAY) g_vaTex = false;
}
void glVertexPointer(int size, int stride, FloatBuffer* buf) {
	g_vpSize = size; g_vpStride = stride ? stride : size * 4; g_vpPtr = buf ? buf->base() : 0;
}
void glColorPointer(int size, int stride, FloatBuffer* buf) {
	g_cpSize = size; g_cpStride = stride ? stride : size * 4; g_cpPtr = buf ? buf->base() : 0; g_cpFloat = true;
}
void glColorPointer(int size, int stride, ByteBuffer* buf) {
	g_cpSize = size; g_cpStride = stride ? stride : size; g_cpPtr = buf ? buf->base() : 0; g_cpFloat = false;
}
void glTexCoordPointer(int size, int stride, FloatBuffer* buf) {
	g_tpSize = size; g_tpStride = stride ? stride : size * 4; g_tpPtr = buf ? buf->base() : 0;
}
static unsigned char _u8(float f) { int i = (int)(f * 255.0f + 0.5f); return (unsigned char)(i < 0 ? 0 : i > 255 ? 255 : i); }
void glDrawArrays(int mode, int first, int count) {
	if (count <= 0) return;
	std::vector<GLVert> g; g.reserve(count);
	for (int i = 0; i < count; ++i) {
		int idx = first + i; GLVert v; v.x = v.y = v.z = 0; v.u = v.v = 0; v.color = curColor();
		if (g_vaVertex && g_vpPtr) {
			const float* p = (const float*)(g_vpPtr + idx * g_vpStride);
			v.x = p[0]; v.y = p[1]; if (g_vpSize >= 3) v.z = p[2];
		}
		if (g_vaTex && g_tpPtr) {
			const float* p = (const float*)(g_tpPtr + idx * g_tpStride); v.u = p[0]; v.v = p[1];
		}
		if (g_vaColor && g_cpPtr) {
			if (g_cpFloat) {
				const float* c = (const float*)(g_cpPtr + idx * g_cpStride);
				unsigned char a = g_cpSize >= 4 ? _u8(c[3]) : 255;
				v.color = D3DCOLOR_ARGB(a, _u8(c[0]), _u8(c[1]), _u8(c[2]));
			} else {
				const unsigned char* c = (const unsigned char*)(g_cpPtr + idx * g_cpStride);
				unsigned char a = g_cpSize >= 4 ? c[3] : 255;
				v.color = D3DCOLOR_ARGB(a, c[0], c[1], c[2]);
			}
		}
		g.push_back(v);
	}
	_drawGather(mode, &g[0], (int)g.size());
}

// =========================================================================== display lists
int glGenLists(int n) {
	if (g_lists.empty()) g_lists.push_back(GLList());   // reserve id 0
	int base = (int)g_lists.size();
	for (int i = 0; i < n; ++i) g_lists.push_back(GLList());
	return base;
}
void glNewList(int list, int /*mode*/) {
	if (list <= 0) return;
	while ((int)g_lists.size() <= list) g_lists.push_back(GLList());
	g_recordingList = list;
	g_lists[list].tris.clear();
	g_lists[list].used = true;
}
void glEndList() { g_recordingList = 0; }
void glCallList(int list) {
	if (list > 0 && list < (int)g_lists.size() && g_lists[list].used)
		_drawTris(g_lists[list].tris);
}
void glDeleteLists(int list, int n) {
	for (int i = 0; i < n; ++i) {
		int id = list + i;
		if (id > 0 && id < (int)g_lists.size()) { g_lists[id].tris.clear(); g_lists[id].used = false; }
	}
}

// =========================================================================== selection / picking
// Software emulation of GL selection: glRenderMode(GL_SELECT) records a "hit" for the current name
// stack whenever geometry is drawn (approximated: any glEnd/glDrawArrays inside the pick frustum set
// by GLU::gluPickMatrix). On GL_RENDER it writes the standard hit records (nameDepth, minZ, maxZ,
// names...) into the select buffer and returns the hit count. HARDWARE-VERIFY the pick precision vs a
// real GL selection pass; for block picking (RubyDung) the "closest name under cursor" is what matters.
void glInitNames() { g_nameStack.clear(); g_hitThisName = false; }
void glPushName(int name) { g_nameStack.push_back(name); }
void glPopName() { if (!g_nameStack.empty()) g_nameStack.pop_back(); }
void glLoadName(int name) {
	// finalize the previous name's hit (if any) before switching -- matches GL semantics.
	if (g_hitThisName && g_selBuf) {
		int base = g_selWritten;
		g_selBuf->put(base + 0, (int)g_nameStack.size());
		g_selBuf->put(base + 1, 0);                // minZ (approx)
		g_selBuf->put(base + 2, 0);                // maxZ (approx)
		for (int i = 0; i < (int)g_nameStack.size(); ++i) g_selBuf->put(base + 3 + i, g_nameStack[i]);
		g_selWritten = base + 3 + (int)g_nameStack.size();
		++g_hitCount; g_hitThisName = false;
	}
	if (g_nameStack.empty()) g_nameStack.push_back(name); else g_nameStack.back() = name;
}
void glSelectBuffer(IntBuffer* buf) { g_selBuf = buf; }
int glRenderMode(int mode) {
	int result = 0;
	if (g_renderMode == GL_SELECT) {
		if (g_hitThisName && g_selBuf) {           // flush a trailing pending hit
			int base = g_selWritten;
			g_selBuf->put(base + 0, (int)g_nameStack.size());
			g_selBuf->put(base + 1, 0); g_selBuf->put(base + 2, 0);
			for (int i = 0; i < (int)g_nameStack.size(); ++i) g_selBuf->put(base + 3 + i, g_nameStack[i]);
			g_selWritten = base + 3 + (int)g_nameStack.size(); ++g_hitCount; g_hitThisName = false;
		}
		result = g_hitCount;
	}
	g_renderMode = mode;
	if (mode == GL_SELECT) { g_hitCount = 0; g_selWritten = 0; g_hitThisName = false; g_nameStack.clear(); }
	return result;
}

// =========================================================================== queries
int glGetError() { return 0; }
JavaString glGetString(int name) {
	if (name == GL_VENDOR)   return JavaString("j2ctool");
	if (name == GL_RENDERER) return JavaString("Direct3D9 (Xenos) GL1.x emulator");
	if (name == GL_VERSION)  return JavaString("1.1 (j2ctool)");
	return JavaString("");
}

} // namespace GL11
