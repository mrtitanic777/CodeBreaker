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

// TARGET-AWARE D3D FORM FACTOR. The Xbox 360's Direct3D is a DIFFERENT form of D3D9 than the PC's:
// unified memory (no managed pool), no fixed-function pipeline, D3DX9-centric. The 360 build configs
// define _XBOX (see the .vcxproj), exactly as Microsoft's own code keys off it. Select the form here so
// ONE platform layer serves both a Windows/PC-D3D9 target and an Xbox 360/D3DX9-360 target.
#ifdef _XBOX
  #define J2X_TEXPOOL  D3DPOOL_MANAGED       // 360: pool is ignored (unified memory); MANAGED is a benign token
  // CRITICAL: the 360's plain D3DFMT_A8R8G8B8 is TILED (swizzled). A LockRect + linear pixel copy into a
  // tiled texture scrambles the image into block/stripe garbage when sampled. D3DFMT_LIN_A8R8G8B8 is the
  // UNTILED twin, so a linear copy lays out exactly as the GPU samples. (Proven in the Beta port.)
  #define J2X_TEXFMT   D3DFMT_LIN_A8R8G8B8
#else
  #define J2X_TEXPOOL  D3DPOOL_MANAGED       // PC D3D9: managed pool (auto system-memory backing)
  #define J2X_TEXFMT   D3DFMT_A8R8G8B8       // PC has no tiling; plain A8R8G8B8
#endif

namespace d3d9 {

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

static int g_fbH = 720;   // backbuffer height, for the GL(bottom-origin)->D3D(top-origin) viewport Y-flip
// Read the REAL backbuffer height from the device (once, at hand-off). It used to be captured from any
// glViewport(0,0,w,h), so a render-to-texture/sub-viewport like (0,0,256,256) shrank it and a later viewport
// with y+h > 256 produced a negative Y that wrapped in the unsigned D3DVIEWPORT9::Y.
void glSetDevice(IDirect3DDevice9* dev) {
	g_dev = dev;
	IDirect3DSurface9* bb = 0;
	if (dev && SUCCEEDED(dev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &bb)) && bb) {
		D3DSURFACE_DESC d;
		if (SUCCEEDED(bb->GetDesc(&d)) && d.Height > 0) g_fbH = (int)d.Height;
		bb->Release();
	}
}
IDirect3DDevice9* device() { return g_dev; }

// --------------------------------------------------------------------------- stream-source binding
// FIXED BUG vb-released-while-bound: SetStreamSource was called once per display-list batch and NEVER
// unset, so stream 0 stayed bound to the last list's vertex buffer indefinitely. Then:
//   * glNewList re-recording a list -> _freeBatchVBs Release()s a VB that is STILL the bound stream
//     ("You can't do a final 'Release' on a resource that is still set in a D3D device", followed by
//     "Was the VertexBuffer structure memory at 0x... already freed?" and the INTREFCOUNT assertion);
//   * a later Lock/validate sees a slot pointing at freed memory ("Invalid 'Common' field (resource
//     probably already freed)", "The specified vertex buffer is being modified while still currently
//     set in a D3D device ... un-set it first");
//   * DrawPrimitiveUP (immediate/entity path) swaps in its own internal resource, so the stale binding
//     also produced "the resource's type doesn't match the type expected by the API. Expected type
//     VertexBuffer, got type ."
// On the 360 the GPU runs asynchronously, so touching a resource it may still be reading forces the
// driver to serialize -- which is why this showed up as a framerate collapse and unbuilt (black) chunks
// rather than a clean crash. Bind through here (cached, so the state-cache work is preserved) and ALWAYS
// unbind before releasing or locking a buffer.
static IDirect3DVertexBuffer9* g_lastStreamVB = (IDirect3DVertexBuffer9*)~(size_t)0;   // sentinel

static void _bindStream(IDirect3DVertexBuffer9* vb) {
	if (!g_dev || vb == g_lastStreamVB) return;
	g_dev->SetStreamSource(0, vb, 0, vb ? sizeof(GLVert) : 0);
	g_lastStreamVB = vb;
}
static void _unbindStream() { _bindStream(0); }

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
static float g_modR = 1, g_modG = 1, g_modB = 1, g_modA = 1;  // PS gCol: white except while replaying a colourless (font) display list
static float g_tu = 0, g_tv = 0;                          // current texcoord
static int   _clamp255(float x) { int i = (int)(x * 255.0f + 0.5f); return i < 0 ? 0 : (i > 255 ? 255 : i); }
static float _clampf(float x)   { return x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x); }
static D3DCOLOR curColor() {
	// J2X-BUGFIX color-clamp: CLAMP each channel into [0,255]; do NOT `& 255` (which WRAPS -- an alpha of
	// 1.001 -> 256 -> &255 = 0 = fully transparent, an over-bright channel wraps to near-black). Matches the
	// hand-port's per-channel clamp in glColor4f.
	return D3DCOLOR_ARGB(_clamp255(g_ca), _clamp255(g_cr), _clamp255(g_cg), _clamp255(g_cb));
}

// --------------------------------------------------------------------------- render-state mirror
static bool g_texEnabled = false, g_blend = false, g_depth = true, g_cull = false, g_alphaTest = false, g_fog = false;
// J2X-BUGFIX alpha-test-ref: the port's default cutout threshold is 0.5 (the hand-port's value), so a renderer
// that enables GL_ALPHA_TEST WITHOUT a glAlphaFunc gets a clean cutout (leaves/glass/saplings/sprites/item icons)
// instead of clip(a-0) discarding nothing. Like GL, the reference is state of its OWN: glEnable/glDisable never
// touch it, so a glAlphaFunc(GL_GREATER, 0.1F) survives later toggles (resetting it eroded particles/sprites).
static float g_alphaRef = 0.5f;
static float g_clearR = 0, g_clearG = 0, g_clearB = 0, g_clearA = 1; static float g_clearDepth = 1.0f;
static D3DVIEWPORT9 g_vp = { 0, 0, 640, 480, 0.0f, 1.0f };
static float g_fogStart = 0, g_fogEnd = 1, g_fogDensity = 1.0f; static int g_fogMode = GL_EXP; static D3DCOLOR g_fogColor = 0;

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
// [J2X-PERF entity-batch] Coalesced immediate-mode draws. Each glBegin/glEnd entity run is baked to VIEW
// space (modelview folded into the verts) and appended here, then drawn in ONE call with a projection-only
// matrix -- collapsing the ~600 per-frame entity-cube DrawPrimitiveUP calls. See _drawGather / _flushEntityBatch.
static std::vector<GLVert> g_eBatch;
static bool g_eBatchOpen = false;
static void _flushEntityBatch();   // fwd decl: called from matrix/state ops defined above its body

// --------------------------------------------------------------------------- display lists
// A display list records DRAW BATCHES, each capturing the texture state (bound texture + enable) that
// was current when the geometry was recorded -- real GL records glBindTexture INTO the list, so glCallList
// must replay with the list's texture, not whatever is bound at call time. (This was the white ground:
// terrain glBindTexture happened inside glNewList, but replay drew untextured -> vertex color = white.)
// A batch is baked into a STATIC vertex buffer at glEndList and drawn with DrawPrimitive (fast) on every
// glCallList -- NOT re-uploaded per frame via DrawPrimitiveUP (that was the terrible FPS). `tris` is only
// the transient CPU staging during recording; it's freed once `vb` is built.
struct GLBatch {
	std::vector<GLVert> tris;
	IDirect3DVertexBuffer9* vb;
	int triCount;
	int tex; bool texEnabled;
	bool inheritColor;   // recorded with NO explicit vertex-colour array (font glyphs) -> tint by current glColor at glCallList; verts baked white
	GLBatch() : vb(0), triCount(0), tex(0), texEnabled(false), inheritColor(false) {}
};
// `xform` = the NET modelview delta applied by matrix ops (glTranslatef/glRotatef/glScalef) recorded
// INSIDE this list. Real GL records matrix ops into the list and replays them; this layer records
// geometry-only, so it captures the net delta at glEndList and re-applies it at glCallList. The one
// caller that needs it is FontRenderer: each glyph list ends with glTranslatef(charWidth,0,0) as the
// pen advance, and it draws whole strings via glCallLists -- without replaying that translate every
// glyph stacks at x=0 (one white blob per string). Identity for geometry-only lists (terrain chunks),
// so hasXform stays false there and glCallList is unchanged.
struct GLList {
	std::vector<GLBatch> batches; bool used;
	D3DXMATRIX xform; bool hasXform;
	bool ownsTex;   // [font-fix] did this list call glBindTexture DURING recording? If not it's texture-agnostic
	GLList() : used(false), hasXform(false), ownsTex(false) { D3DXMatrixIdentity(&xform); }
};

static void _freeBatchVBs(GLList& L) {
	// Un-bind stream 0 FIRST: one of these buffers is very likely the currently-set stream (the list was
	// just replayed), and releasing a bound resource is the vb-released-while-bound bug above.
	_unbindStream();
	for (size_t i = 0; i < L.batches.size(); ++i)
		if (L.batches[i].vb) { L.batches[i].vb->Release(); L.batches[i].vb = 0; }
	L.batches.clear();
}
static std::vector<GLList> g_lists;      // index by list id (0 reserved)
static int g_recordingList = 0;          // != 0 while inside glNewList/glEndList
static D3DXMATRIX g_recordBaseMV;        // modelview snapshot at glNewList; glEndList records the net delta since
// [J2X-PERF terrain-load] Display lists whose batches still need a GPU vertex buffer. glEndList only STAGES
// the geometry (CPU-side); the actual CreateVertexBuffer happens once per frame in _pumpPendingVBs, inside a
// SINGLE EndScene/BeginScene window. The 360 backend only requires creates to be out-of-scene, not one scene
// toggle PER buffer -- so building 8 chunks/frame costs ONE device stall, not sixteen. Keeps load-time FPS up.
static std::vector<int> g_pendingVB;
static bool g_recordHadVB = false;   // set in glNewList: did the list being (re)recorded already have live VBs?
static void _pumpPendingVBs();   // fwd decl: defined near glEndList, called from glFlush (per frame)

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
	"float4 gCol : register(c3);\n"   // glColor modulation: white(1,1,1,1) for coloured geometry; the string colour for FontRenderer's COLOURLESS glyph lists (baked white -> tinted here), reproducing GL's "colourless vertex inherits current glColor"
	"sampler2D s0 : register(s0);\n"
	"float4 main(float4 c:COLOR0, float2 t:TEXCOORD0, float fz:TEXCOORD1):COLOR{\n"
	"  float4 o = c * gCol;\n"
	"  if (gCtl.x > 0.5) o *= tex2D(s0, t);\n"
	"  if (gCtl.y > 0.5) clip(o.a - gCtl.z);\n"   // alpha test: discard where o.a < ref
	// J2X-BUGFIX fog-textured-only: fog applies ONLY to TEXTURED geometry (gCtl.x). The hand-port routes
	// textured/untextured through SEPARATE pixel shaders and the untextured one has NO fog term, so untextured
	// colour geometry (sky dome, sunset/void colours, selection/debug lines, some particles) is never fogged.
	// Gating on useTex here reproduces that -- without it those elements get tinted toward the fog colour.
	"  if (gCtl.w > 0.5 && gCtl.x > 0.5){ float f = (gFog.w > 0.5) ? saturate(exp(-gFog.z * fz)) : saturate((gFog.y - fz)/(gFog.y-gFog.x)); o.rgb = lerp(gFogC.rgb, o.rgb, f); }\n"
	"  return o;\n"
	"}\n";

static IDirect3DVertexDeclaration9* g_decl = 0;   // SM3 needs an explicit decl (SetFVF is not enough on Xenos)
// [J2X-PERF state-cache] Redundant-state filtering. The emulator validates EVERY D3D call, so re-setting
// the (unchanging) shader/decl and the (mostly-shared) texture/matrix on every one of the hundreds of
// chunk batches + ~600 entity draws per frame was the bulk of the frame cost. Cache the last-set values
// and skip no-op device calls. Invalidated in glInit (device (re)bound).
static IDirect3DVertexShader9*      g_lastVS   = 0;
static IDirect3DPixelShader9*       g_lastPS   = 0;
static IDirect3DVertexDeclaration9* g_lastDecl = 0;
static IDirect3DBaseTexture9*       g_lastTex  = (IDirect3DBaseTexture9*)~(size_t)0;  // sentinel: 1st bind always applies
static int  g_lastWrapS = -1, g_lastWrapT = -1, g_lastMag = -1;
static D3DXMATRIX g_lastWVP; static bool g_lastWVPValid = false;

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
	D3DXMATRIX mv, pr;
	if (g_mv.empty())   D3DXMatrixIdentity(&mv);   else mv = g_mv.back();     // (was *(new D3DXMATRIX) -- a leak)
	if (g_proj.empty()) D3DXMatrixIdentity(&pr);   else pr = g_proj.back();
	D3DXMATRIX wvp = mv * pr;                 // row-vector convention: v * MV * P
	// [J2X-PERF state-cache] Skip the shader-constant upload when the combined matrix is unchanged. Every
	// terrain chunk is baked in world space and shares the camera matrix -> uploaded ONCE per frame instead
	// of once per chunk. A 64-byte memcmp is far cheaper than a SetVertexShaderConstantF the emulator must
	// validate. (Entities DO change it per body part, so those still upload -- correctly.)
	if (g_lastWVPValid && memcmp(&wvp, &g_lastWVP, sizeof(D3DXMATRIX)) == 0) return;
	g_lastWVP = wvp; g_lastWVPValid = true;
	D3DXMATRIX t;  D3DXMatrixTranspose(&t, &wvp);
	g_dev->SetVertexShaderConstantF(0, (const float*)t.m, 4);
}

static void _uploadControl() {
	if (!g_dev) return;
	float ctl[4] = { g_texEnabled && boundTex() ? 1.0f : 0.0f, g_alphaTest ? 1.0f : 0.0f, g_alphaRef, g_fog ? 1.0f : 0.0f };
	float fog[4] = { g_fogStart, g_fogEnd, g_fogDensity, (g_fogMode == GL_EXP || g_fogMode == 0x0801) ? 1.0f : 0.0f };
	float fogc[4] = { ((g_fogColor >> 16) & 255) / 255.0f, ((g_fogColor >> 8) & 255) / 255.0f,
	                  (g_fogColor & 255) / 255.0f, 1.0f };
	float col[4] = { g_modR, g_modG, g_modB, g_modA };
	g_dev->SetPixelShaderConstantF(0, ctl, 1);
	g_dev->SetPixelShaderConstantF(1, fog, 1);
	g_dev->SetPixelShaderConstantF(2, fogc, 1);
	g_dev->SetPixelShaderConstantF(3, col, 1);   // gCol modulation (white for normal geometry)
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
		// GL starts with blending DISABLED (g_blend=false). D3D9's device default for ALPHABLENDENABLE is also
		// FALSE, but sync it explicitly so a device reset/re-init that left a stale enable can't leak blending
		// onto the first opaque draws. Matches the hand-port's glInit. [J2X parity]
		g_dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
	// [J2X-PERF state-cache] invalidate the redundant-state cache: the device is (re)bound, so the next
	// draw must re-apply shader/decl/texture/matrix once before the filtering kicks in.
	g_lastVS = 0; g_lastPS = 0; g_lastDecl = 0;
	g_lastTex = (IDirect3DBaseTexture9*)~(size_t)0;
	g_lastWrapS = g_lastWrapT = g_lastMag = -1;
	g_lastWVPValid = false;
	g_lastStreamVB = (IDirect3DVertexBuffer9*)~(size_t)0;   // sentinel: nothing known-bound on a fresh device
}

// =========================================================================== matrix ops
// [J2X-PERF] The entity batch bakes MODELVIEW into its verts and draws with PROJECTION-only, so a projection
// change would invalidate a pending batch. Projection is only ever touched while g_matMode==GL_PROJECTION, and
// entities are always batched under GL_MODELVIEW, so flushing on the mode switch closes the batch under the
// correct (still-current) projection. Not in the per-entity hot path (mode stays MODELVIEW across entities).
void glMatrixMode(int mode) { if (mode != g_matMode) _flushEntityBatch(); g_matMode = mode; }
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
	// J2X-BUGFIX viewport-y-flip: GL viewport origin is BOTTOM-left, D3D is TOP-left -> flip Y. Without this a
	// GL top-strip viewport (y = H-h, e.g. GuiMainMenu's 3D block logo drawn into the top strip) lands in the
	// BOTTOM/middle strip. Full-screen (0,0,W,H) is unaffected (Y stays 0). Matches the hand-port's glViewport.
	int fy = g_fbH - y - h;
	if (fy < 0) fy = 0;   // never hand D3D a wrapped (huge unsigned) Y
	g_vp.X = x; g_vp.Y = fy; g_vp.Width = w; g_vp.Height = h; g_vp.MinZ = 0.0f; g_vp.MaxZ = 1.0f;
	if (g_dev) g_dev->SetViewport(&g_vp);
}
void glLoadProjection(const D3DXMATRIX& m) { _flushEntityBatch(); g_matMode = GL_PROJECTION; top(g_proj) = m; }

// [J2X-PERF] End-of-frame drain (called from Display::swapBuffers before Present): flush the coalesced entity
// batch, then upload this frame's freshly-built chunk vertex buffers in one out-of-scene window.
void glFlush() { _flushEntityBatch(); _pumpPendingVBs(); }

void glGetFloat(int pname, FloatBuffer* buf) {
	if (!buf) return;
	const D3DXMATRIX* src = 0;
	if (pname == GL_MODELVIEW_MATRIX)  src = &top(g_mv);
	else if (pname == GL_PROJECTION_MATRIX) src = &top(g_proj);
	else J2X_UNHANDLED("glGetFloat", pname);   // fail-loud: unknown matrix pname would silently return garbage
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
// J2X-BUGFIX color-clamp: clamp each channel into [0,1] at set time, exactly like the hand-port's glColor4f.
// These floats feed BOTH the baked vertex colour (curColor) AND the gCol pixel-shader tint, so an out-of-range
// value must not wrap or over-brighten either path.
void glColor3f(float r, float g, float b) { g_cr = _clampf(r); g_cg = _clampf(g); g_cb = _clampf(b); g_ca = 1.0f; }
void glColor4f(float r, float g, float b, float a) { g_cr = _clampf(r); g_cg = _clampf(g); g_cb = _clampf(b); g_ca = _clampf(a); }
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
		// [defensive] a trailing 3-vertex remainder (vertexCount % 4 == 3) is one lone triangle -- emit it,
		// matching the hand-port's Tesselator. MC's Tessellator normally emits multiples of 4, so this is rare.
		if ((n & 3) == 3) { int i = n - 3; tri.push_back(v[i]); tri.push_back(v[i+1]); tri.push_back(v[i+2]); }
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

// Set the shared shader + current matrix/texture/sampler state for a draw. Shared by the immediate
// DrawPrimitiveUP path and the display-list vertex-buffer DrawPrimitive path.
static void _applyDrawState() {
	_uploadMatrices(); _uploadControl();
	// [J2X-PERF state-cache] shader + vertex decl NEVER change after glInit -> set once, not per draw.
	if (g_vs   != g_lastVS)   { g_dev->SetVertexShader(g_vs);        g_lastVS = g_vs; }
	if (g_ps   != g_lastPS)   { g_dev->SetPixelShader(g_ps);         g_lastPS = g_ps; }
	if (g_decl != g_lastDecl) { g_dev->SetVertexDeclaration(g_decl); g_lastDecl = g_decl; }   // SM3 decl (not SetFVF)
	GLTex* bt = boundTex();
	IDirect3DBaseTexture9* want = (g_texEnabled && bt && bt->tex) ? (IDirect3DBaseTexture9*)bt->tex : 0;
	if (want != g_lastTex) { g_dev->SetTexture(0, want); g_lastTex = want; }   // terrain + all zombies share one texture
	if (want) {                                                               // sampler params: touch the device only on change
		int ws = bt->wrapS, wt = bt->wrapT, mg = bt->magF;
		if (ws != g_lastWrapS) { g_dev->SetSamplerState(0, D3DSAMP_ADDRESSU, ws == GL_CLAMP ? D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP); g_lastWrapS = ws; }
		if (wt != g_lastWrapT) { g_dev->SetSamplerState(0, D3DSAMP_ADDRESSV, wt == GL_CLAMP ? D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP); g_lastWrapT = wt; }
		if (mg != g_lastMag) {
			D3DTEXTUREFILTERTYPE mag = (mg == GL_LINEAR) ? D3DTEXF_LINEAR : D3DTEXF_POINT;
			g_dev->SetSamplerState(0, D3DSAMP_MAGFILTER, mag);
			g_dev->SetSamplerState(0, D3DSAMP_MINFILTER, mag);
			// MIPFILTER=NONE because this memory-upload path builds 1-level textures (see _uploadRGBA). NOTE:
			// this does NOT match the hand-port, which DOES ship a 3-level terrain mip chain + MIPFILTER=LINEAR.
			// The hand-port identifies the terrain atlas by FILENAME ("terrain.png") and either loads pre-baked
			// per-tile mip PNGs or runs an atlas-AWARE (per-16px-tile) downsample -- both avoid the cross-tile
			// colour bleed a naive whole-atlas mip causes. glTexImage2D here has no filename/atlas hint, and
			// terrain/gui/items are all 256x256, so a safe port needs an atlas tag threaded in first (see the
			// _uploadRGBA NOTE). Until then, no mips (distant terrain shimmers slightly; no regression).
			g_dev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			g_lastMag = mg;
		}
	}
}

// Draw a ready triangle list in ONE-SHOT (immediate mode): uploads the verts inline. Used for the
// small dynamic geometry (entities in glBegin/glEnd) -- NOT for terrain (that goes through cached VBs).
static void _drawTris(const std::vector<GLVert>& tri) {
	if (!g_dev || !g_vs || tri.empty()) return;
	_applyDrawState();
	g_dev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, (UINT)(tri.size() / 3), &tri[0], sizeof(GLVert));
}

// J2X-BUGFIX line-primitives: draw GL line runs directly as D3D lines. GL lines CANNOT go through the
// triangle gather -- _gatherTris expands them to ZERO triangles, so the block-selection wireframe (the
// black outline around the targeted block, drawn by RenderGlobal::drawSelectionBox in ALL three games via
// GL_LINE_STRIP + GL_LINES) rendered nothing. Emit the CLEAN primitive: a real D3DPT_LINESTRIP for a strip
// (NOT the pairwise-LINELIST approximation the Beta hand-port used), a closed strip for a loop, and a
// LINELIST for disjoint pairs. Immediate only -- the sole caller draws per frame, never into a display
// list or the entity batch. Verts already carry their baked vertex colour (the selection box has no colour
// array), and g_mod stays white here (this runs before the colour-array tint path), so colour is correct.
static void _drawLines(int mode, const GLVert* v, int n) {
	if (!g_dev || !g_vs || n < 2) return;
	_applyDrawState();
	if (mode == GL_LINE_STRIP) {
		g_dev->DrawPrimitiveUP(D3DPT_LINESTRIP, (UINT)(n - 1), v, sizeof(GLVert));
	} else if (mode == GL_LINE_LOOP) {
		std::vector<GLVert> loop(v, v + n); loop.push_back(v[0]);   // close the ring back to vertex 0
		g_dev->DrawPrimitiveUP(D3DPT_LINESTRIP, (UINT)n, &loop[0], sizeof(GLVert));
	} else {   // GL_LINES: disjoint segment pairs
		g_dev->DrawPrimitiveUP(D3DPT_LINELIST, (UINT)(n / 2), v, sizeof(GLVert));
	}
}

// [J2X-PERF entity-batch] Draw the accumulated (view-space) immediate geometry in ONE call. Called before
// any state change / list draw / present, so the CURRENT g_* state IS the state the geometry was gathered
// under -- _applyDrawState uses it correctly. Verts are pre-baked to VIEW space, so upload PROJECTION-ONLY.
static void _flushEntityBatch() {
	if (!g_eBatchOpen) return;
	g_eBatchOpen = false;
	if (g_eBatch.empty() || !g_dev || !g_vs) { g_eBatch.clear(); return; }
	_applyDrawState();
	D3DXMATRIX pr; if (g_proj.empty()) D3DXMatrixIdentity(&pr); else pr = g_proj.back();
	D3DXMATRIX t;  D3DXMatrixTranspose(&t, &pr);
	g_dev->SetVertexShaderConstantF(0, (const float*)t.m, 4);   // proj only (modelview already baked in)
	g_lastWVPValid = false;                                     // we clobbered c0 -> next draw re-uploads its WVP
	g_dev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, (UINT)(g_eBatch.size() / 3), &g_eBatch[0], sizeof(GLVert));
	g_eBatch.clear();
}

// Gather a primitive run, then either RECORD it into the open display list, BATCH it for a coalesced draw
// (entities), or draw it immediately. allowBatch is TRUE only for the glBegin/glEnd path -- that is where the
// ~600 per-frame entity cube draws come from. The glDrawArrays path (Tesselator: the block highlight, and
// terrain that is recorded into lists) passes FALSE so it keeps its proven full-WVP direct draw untouched.
static void _drawGather(int mode, const GLVert* v, int n, bool allowBatch) {
	if (n <= 0) return;
	// [line-fix] line primitives can't triangulate -- draw them directly as D3D lines and return. Immediate
	// only: the block-selection wireframe is drawn per frame, never recorded into a list or entity-batched.
	if (mode == GL_LINES || mode == GL_LINE_STRIP || mode == GL_LINE_LOOP) {
		_flushEntityBatch();   // keep draw order: flush any pending entity batch before the overlay lines
		_drawLines(mode, v, n);
		return;
	}
	std::vector<GLVert> tri;
	_gatherTris(mode, v, n, tri);
	if (tri.empty()) return;
	if (g_recordingList > 0 && g_recordingList < (int)g_lists.size()) {
		GLBatch b;                                   // capture geometry AND the current texture binding
		b.tris.swap(tri);
		b.tex = g_boundTex; b.texEnabled = g_texEnabled;
		// A list drawn with NO explicit vertex-colour array (FontRenderer's glyphs: Tessellator quads with
		// no setColor) must inherit the glColor set at glCallList time -- real GL applies the current colour
		// to a colourless vertex. Baking record-time curColor() instead is why every glyph came out white
		// (glColor ignored -> shadow==main == the "doubled" text, and yellow splash lost). Flag it, bake the
		// verts WHITE, and tint via the gCol PS constant at replay (glCallList). Only the direct/Tessellator
		// record path (allowBatch==false) qualifies; immediate glBegin verts keep their baked colour.
		b.inheritColor = !allowBatch && !g_vaColor;
		if (b.inheritColor)
			for (size_t k = 0; k < b.tris.size(); ++k) b.tris[k].color = D3DCOLOR_ARGB(255, 255, 255, 255);
		g_lists[g_recordingList].batches.push_back(b);   // matrix applied at glCallList; texture replayed too
		return;
	}
	if (!allowBatch) {
		// J2X-BUGFIX item-world-brightness: a direct draw carrying a per-vertex COLOUR ARRAY (g_vaColor:
		// dropped items, held items, entity meshes) drops glColor's block-light multiplier unless glColor is
		// fed in as a GLOBAL tint (final = texel * vertexColour * glColor) -- otherwise they render FULL-BRIGHT
		// and never darken at night / in caves / in shadow. No-colour-array draws already bake curColor() into
		// the verts, so the tint stays white there. Matches the hand-port's issueDraw (stampColor==false).
		if (g_vaColor) { g_modR = g_cr; g_modG = g_cg; g_modB = g_cb; g_modA = g_ca; }
		_drawTris(tri);   // [J2X-PERF] non-entity immediate geometry: draw as before
		if (g_vaColor) { g_modR = g_modG = g_modB = g_modA = 1.0f; }
		return;
	}
	// Entity immediate mode: bake the current modelview into the verts (-> VIEW space) so this run can share ONE
	// batched draw with the other entity cubes (which each have a different per-body-part matrix). The batch
	// is flushed with a projection-only matrix; matrix changes between glEnds are absorbed by this bake.
	D3DXMATRIX mv; if (g_mv.empty()) D3DXMatrixIdentity(&mv); else mv = g_mv.back();
	for (size_t k = 0; k < tri.size(); ++k) {
		D3DXVECTOR3 in(tri[k].x, tri[k].y, tri[k].z), out;
		D3DXVec3TransformCoord(&out, &in, &mv);      // out = in * mv (row-vector), affine -> exact view pos
		tri[k].x = out.x; tri[k].y = out.y; tri[k].z = out.z;
	}
	g_eBatch.insert(g_eBatch.end(), tri.begin(), tri.end());
	g_eBatchOpen = true;
}

void glEnd() {
	if (g_renderMode == GL_SELECT) {
		// In selection mode any geometry inside the (pick) frustum registers a hit on the current name.
		if (!g_imm.empty()) g_hitThisName = true;
	} else {
		_drawGather(g_beginMode, g_imm.empty() ? 0 : &g_imm[0], (int)g_imm.size(), true);   // entities: batchable
	}
	g_beginMode = -1; g_imm.clear();
}

// =========================================================================== render state
static void _rs(DWORD s, DWORD v) { if (g_dev) g_dev->SetRenderState((D3DRENDERSTATETYPE)s, v); }

// Polygon offset -- block-break crack overlay depth bias. glPolygonOffset was a no-op stub, so the mining crack
// rendered coplanar with the block face and z-fought (flicker). Map GL polygon offset to the 360 D3D9 depth-bias
// render states: SLOPESCALEDEPTHBIAS = GL factor, DEPTHBIAS = GL units in depth-buffer LSBs (D24 -> 1/2^24 each).
static bool  s_polyOffFill   = false;
static float s_polyOffFactor = 0.0f;
static float s_polyOffUnits  = 0.0f;
static void _applyPolyOffset() {
	float slope = s_polyOffFill ? s_polyOffFactor : 0.0f;
	float bias  = s_polyOffFill ? (s_polyOffUnits * (1.0f / 16777216.0f)) : 0.0f;
	_rs(D3DRS_SLOPESCALEDEPTHBIAS, *reinterpret_cast<DWORD*>(&slope));
	_rs(D3DRS_DEPTHBIAS,           *reinterpret_cast<DWORD*>(&bias));
}
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
	_flushEntityBatch();   // [J2X-PERF] pending entity batch was gathered under the OLD state -- draw it first
	switch (cap) {
		case GL_TEXTURE_2D: g_texEnabled = true; break;
		case GL_BLEND: g_blend = true; _rs(D3DRS_ALPHABLENDENABLE, TRUE); break;
		case GL_DEPTH_TEST: g_depth = true; _rs(D3DRS_ZENABLE, TRUE); break;
		case GL_CULL_FACE: g_cull = true; _rs(D3DRS_CULLMODE, D3DCULL_CW); break;   // CW: GL RH front-face -> D3D LH is CW. Was CCW = culled the visible faces (missing ground). Matches the working hand-port.
		case GL_ALPHA_TEST: g_alphaTest = true; break;   // ref stays whatever glAlphaFunc set (see g_alphaRef)
		case GL_FOG: g_fog = true; break;
		case GL_POLYGON_OFFSET_FILL: s_polyOffFill = true; _applyPolyOffset(); break;
		default: break;
	}
}
void glDisable(int cap) {
	_flushEntityBatch();   // [J2X-PERF] flush geometry gathered under the old state before it changes
	switch (cap) {
		case GL_TEXTURE_2D: g_texEnabled = false; break;
		case GL_BLEND: g_blend = false; _rs(D3DRS_ALPHABLENDENABLE, FALSE); break;
		case GL_DEPTH_TEST: g_depth = false; _rs(D3DRS_ZENABLE, FALSE); break;
		case GL_CULL_FACE: g_cull = false; _rs(D3DRS_CULLMODE, D3DCULL_NONE); break;
		case GL_ALPHA_TEST: g_alphaTest = false; break;   // keep all texels; the shader gates on g_alphaTest
		case GL_FOG: g_fog = false; break;
		case GL_POLYGON_OFFSET_FILL: s_polyOffFill = false; _applyPolyOffset(); break;
		default: break;
	}
}
void glBlendFunc(int s, int d) { _flushEntityBatch(); _rs(D3DRS_SRCBLEND, _blendArg(s)); _rs(D3DRS_DESTBLEND, _blendArg(d)); }
void glDepthFunc(int f) {
	_flushEntityBatch();   // [J2X-PERF]
	DWORD c = D3DCMP_LESSEQUAL;
	if (f == GL_LESS) c = D3DCMP_LESS; else if (f == GL_GREATER) c = D3DCMP_GREATER;
	else if (f == GL_GEQUAL) c = D3DCMP_GREATEREQUAL; else if (f == GL_EQUAL) c = D3DCMP_EQUAL;
	else J2X_UNHANDLED("glDepthFunc", f);   // report the gap; c keeps its D3DCMP_LESSEQUAL default (sensible fallback)
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
	_flushEntityBatch();   // [J2X-PERF]
	if (!g_dev) return;
	DWORD flags = 0;
	if (mask & GL_COLOR_BUFFER_BIT) flags |= D3DCLEAR_TARGET;
	if (mask & GL_DEPTH_BUFFER_BIT) flags |= D3DCLEAR_ZBUFFER;
	D3DCOLOR c = D3DCOLOR_COLORVALUE(g_clearR, g_clearG, g_clearB, g_clearA);
	g_dev->Clear(0, 0, flags, c, g_clearDepth, 0);
}
void glHint(int, int) {}
void glPolygonOffset(float factor, float units) {   // FIXED BUG crack-overlay-zfight: real D3D9 depth bias (was a no-op stub -> mining crack z-fought the block face)
	s_polyOffFactor = factor; s_polyOffUnits = units;
	if (s_polyOffFill) _applyPolyOffset();
}
void glLineWidth(float) {}
void glColorMaterial(int, int) {}

// =========================================================================== fog
void glFogf(int pname, float param) {
	_flushEntityBatch();   // [J2X-PERF]
	if (pname == GL_FOG_START) g_fogStart = param; else if (pname == GL_FOG_END) g_fogEnd = param;
	else if (pname == GL_FOG_DENSITY) g_fogDensity = param;   // J2X-BUGFIX exp-fog: was dropped -> dim highlight
	else J2X_UNHANDLED("glFogf", pname);
}
void glFogi(int pname, int param) { if (pname == GL_FOG_MODE) g_fogMode = param; else glFogf(pname, (float)param); }
void glFog(int pname, FloatBuffer* p) {
	_flushEntityBatch();   // [J2X-PERF]
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
void glBindTexture(int, int id) { if (id != g_boundTex) _flushEntityBatch(); g_boundTex = id; if (g_recordingList > 0 && g_recordingList < (int)g_lists.size()) g_lists[g_recordingList].ownsTex = true; }   // [J2X-PERF] texture change ends a batch; [font-fix] this list now owns its texture
void glTexParameteri(int, int pname, int param) {
	GLTex* t = boundTex(); if (!t) return;
	if (pname == GL_TEXTURE_WRAP_S) t->wrapS = param; else if (pname == GL_TEXTURE_WRAP_T) t->wrapT = param;
	else if (pname == GL_TEXTURE_MIN_FILTER) t->minF = param; else if (pname == GL_TEXTURE_MAG_FILTER) t->magF = param;
	else J2X_UNHANDLED("glTexParameteri", pname);
}
void glTexParameterf(int target, int pname, float param) { glTexParameteri(target, pname, (int)param); }

// NOTE (tex-endian-byte-read): a DWORD-swizzle helper used to live here to turn the game's texel into ARGB.
// It was ENDIAN-BROKEN: RenderEngine.setupTexture writes the source as a BYTE ARRAY [R,G,B,A], and reading
// four bytes as a DWORD depends on endianness (big-endian 360 -> 0xRRGGBBAA, little-endian PC -> 0xAABBGGRR).
// The correct, portable approach is to read the bytes DIRECTLY (see _uploadRGBA / glTexSubImage2D).

// Upload a texture into a fresh LINEAR texture bound to the current id (see J2X_TEXFMT).
// NOTE (terrain mips -- DIVERGENCE from the hand-port, deliberately not yet ported): the hand-port gives the
// terrain atlas a 3-level mip chain so distant terrain stops aliasing (Textures::loadTerrainMipmapped +
// MipmapGenerator::generateAtlasMipmaps). It can do that because it loads by FILENAME and knows which texture
// is the 16x16-tile atlas, and it downsamples PER TILE to avoid cross-tile colour bleed. This memory-upload
// path receives only (w,h,pixels) with no atlas hint, and terrain/gui/items are all 256x256, so it can't tell
// the atlas apart -- and a naive whole-image mip would bleed neighbouring tiles together. To port it safely,
// thread an "isAtlas + tileSize" hint from the caller (RenderEngine.setupTexture overlay) into glTexImage2D,
// then CreateTexture(w,h,3,...) and run the per-tile alpha-weighted box filter here. Left as 1-level for now.
static void _uploadRGBA(GLTex* t, int w, int h, const unsigned char* rgba) {
	if (!g_dev || !t || w <= 0 || h <= 0) return;
	if (t->tex) { t->tex->Release(); t->tex = 0; }
	// Match the Beta port's proven upload: unbind first (360 forbids releasing a still-set texture),
	// LINEAR format (J2X_TEXFMT), and bracket CreateTexture with EndScene/BeginScene -- the Nexia360
	// D3D12 backend rejects CreateTexture mid-scene.
	g_dev->SetTexture(0, 0);
	if (t->tex) { t->tex->Release(); t->tex = 0; }
	g_dev->EndScene();
	HRESULT hr = g_dev->CreateTexture(w, h, 1, 0, J2X_TEXFMT, J2X_TEXPOOL, &t->tex, 0);
	g_dev->BeginScene();
	if (FAILED(hr) || !t->tex) { t->tex = 0; return; }
	t->w = w; t->h = h;
	D3DLOCKED_RECT lr;
	if (FAILED(t->tex->LockRect(0, &lr, 0, 0))) return;
	// J2X-BUGFIX tex-endian-byte-read: the source is a BYTE ARRAY [R,G,B,A] (RenderEngine.setupTexture writes it
	// byte by byte). Reading it as a DWORD + swizzling is ENDIAN-DEPENDENT: on the big-endian 360 those bytes
	// read back as 0xRRGGBBAA, which the little-endian _texelToARGB mis-swizzled -> r forced to 0xFF = the red
	// noise. Read the bytes DIRECTLY (endian-independent), matching the working hand-port's copyRGBAtoARGB.
	for (int y = 0; y < h; ++y) {
		DWORD* dst = (DWORD*)((unsigned char*)lr.pBits + y * lr.Pitch);
		const unsigned char* s = rgba + y * w * 4;   // R,G,B,A
		for (int x = 0; x < w; ++x) dst[x] = D3DCOLOR_ARGB(s[x*4+3], s[x*4+0], s[x*4+1], s[x*4+2]);
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
	g_dev->SetTexture(0, 0);   // unbind: 360 forbids LockRect on a texture still set in the device
	D3DLOCKED_RECT lr; RECT rc; rc.left = xoff; rc.top = yoff; rc.right = xoff + w; rc.bottom = yoff + h;
	if (FAILED(t->tex->LockRect(0, &lr, &rc, 0))) return;
	// J2X-BUGFIX tex-endian-byte-read: byte-array [R,G,B,A] source read directly (see _uploadRGBA).
	const unsigned char* src = (const unsigned char*)pixels->base();
	for (int y = 0; y < h; ++y) {
		DWORD* dst = (DWORD*)((unsigned char*)lr.pBits + y * lr.Pitch);
		for (int x = 0; x < w; ++x) { const unsigned char* p = src + (y * w + x) * 4; dst[x] = D3DCOLOR_ARGB(p[3], p[0], p[1], p[2]); }
	}
	t->tex->UnlockRect(0);
}
void glPixelStorei(int, int) {}

// =========================================================================== vertex arrays
void glEnableClientState(int cap) {
	if (cap == GL_VERTEX_ARRAY) g_vaVertex = true; else if (cap == GL_COLOR_ARRAY) g_vaColor = true;
	else if (cap == GL_TEXTURE_COORD_ARRAY) g_vaTex = true;
	// GL_NORMAL_ARRAY: accepted and ignored -- see glNormalPointer. Handled explicitly so the Beta
	// Tessellator's per-chunk enable/disable pair does not trip the fail-loud path every draw.
	else if (cap == GL_NORMAL_ARRAY) J2X_NOOP("no fixed-function lighting on Xenos; light is in vertex colour");
	else J2X_UNHANDLED("glEnableClientState", cap);
}
void glDisableClientState(int cap) {
	if (cap == GL_VERTEX_ARRAY) g_vaVertex = false; else if (cap == GL_COLOR_ARRAY) g_vaColor = false;
	else if (cap == GL_TEXTURE_COORD_ARRAY) g_vaTex = false;
	else if (cap == GL_NORMAL_ARRAY) J2X_NOOP("no fixed-function lighting on Xenos; light is in vertex colour");
	else J2X_UNHANDLED("glDisableClientState", cap);
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
// LWJGL 4-arg colour form. `unsignedBytes` (true for the Beta Tessellator) states the bytes are
// normalized 0..255 -- exactly what the 3-arg ByteBuffer overload already assumes, so delegate. A false
// here would mean un-normalized bytes, which no converted game emits; report it rather than guess.
void glColorPointer(int size, bool unsignedBytes, int stride, ByteBuffer* buf) {
	if (!unsignedBytes) J2X_UNHANDLED("glColorPointer(unsignedBytes=false)", size);
	glColorPointer(size, stride, buf);
}
// Normals are accepted and DISCARDED: GLVert carries no normal, Xenos has no fixed-function lighting
// (D3DRS_LIGHTING does not exist), and this layer bakes light into the vertex colour -- the same reason
// glNormal3f is a reviewed no-op. Taking the pointer without using it keeps glEnableClientState
// (GL_NORMAL_ARRAY) + glNormalPointer + glDrawArrays a well-formed sequence for the caller.
void glNormalPointer(int /*stride*/, ByteBuffer* /*buf*/) {
	J2X_NOOP("normals unused: light is baked into the vertex colour, no fixed-function lighting on Xenos");
}

// --------------------------------------------------------------------------- VBO-offset forms (dead)
// These take a byte OFFSET into the currently-bound GL_ARRAY_BUFFER. This layer implements no buffer
// objects and reports GL_ARB_vertex_buffer_object = false, so a game's `useVBO` branch is never taken --
// they exist so that branch COMPILES. If one is ever reached the capability flag lied, and silently
// treating an offset as a pointer would read wild memory, so fail loud instead.
static void _vboFormUnsupported(const char* fn) { J2X_UNHANDLED(fn, 0); }
void glVertexPointer(int, int, int, long long)   { _vboFormUnsupported("glVertexPointer(VBO offset)"); }
void glColorPointer(int, int, int, long long)    { _vboFormUnsupported("glColorPointer(VBO offset)"); }
void glTexCoordPointer(int, int, int, long long) { _vboFormUnsupported("glTexCoordPointer(VBO offset)"); }
void glNormalPointer(int, int, long long)        { _vboFormUnsupported("glNormalPointer(VBO offset)"); }
static unsigned char _u8(float f) { int i = (int)(f * 255.0f + 0.5f); return (unsigned char)(i < 0 ? 0 : i > 255 ? 255 : i); }
void glDrawArrays(int mode, int first, int count) {
	if (count <= 0) return;
	// J2X-BUGFIX runaway-vertex-count: a legit Tessellator batch is <=~262K verts. A count this huge is a
	// corrupted/uninitialized vertexCount (the chest/TileEntity render path hit this) -> the reserve() below
	// would try to allocate hundreds of MB -> bad_alloc/hard crash. Skip + log instead, like the hand-port.
	if (count > 1048576) { DbgPrintF("[J2X] glDrawArrays: runaway vertexCount %d -- skipped\n", count); return; }
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
			} else if (g_cpSize >= 4) {
				// J2X-BUGFIX color-endian-int-read: the Tessellator packs each vertex colour as a NATIVE
				// int (setColor: 0xAABBGGRR = a<<24|b<<16|g<<8|r), stores it in an int[] rawBuffer, and
				// uploads it through IntBuffer::put -- which writes `*(int*)bytes = v` in NATIVE memory
				// order (see NIOBuffers). glColorPointer(GL_UNSIGNED_BYTE) then reads those 4 bytes. A
				// POSITIONAL byte read (c[0..3] as R,G,B,A) is endian-dependent: on the big-endian 360 the
				// bytes are [A,B,G,R], so it rotates opaque alpha (0xFF) into red -> the red menu/dirt
				// tint. Read the packed int back the SAME way it was written (as an int) and shift-extract
				// -- endian-neutral, the dual of the byte-array texture read above.
				const unsigned int packed = *(const unsigned int*)(g_cpPtr + idx * g_cpStride);
				v.color = D3DCOLOR_ARGB((unsigned char)(packed >> 24), (unsigned char)packed,
				                        (unsigned char)(packed >> 8), (unsigned char)(packed >> 16));
			} else {
				// size-3 byte colour (no packed alpha): positional read is correct here.
				const unsigned char* c = (const unsigned char*)(g_cpPtr + idx * g_cpStride);
				v.color = D3DCOLOR_ARGB(255, c[0], c[1], c[2]);
			}
		}
		g.push_back(v);
	}
	_drawGather(mode, &g[0], (int)g.size(), false);   // Tesselator (highlight/terrain-lists): direct/recorded, never batched
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
	// [J2X-PERF terrain-load] Was this list ALREADY on screen (had live VBs)? If so it's a re-build of a
	// visible chunk (block edit) -> glEndList uploads immediately so it never blanks for a frame (no flash).
	// A first build (initial load) has no live VBs -> deferred to the batched per-frame pump.
	g_recordHadVB = false;
	{ GLList& L = g_lists[list]; for (size_t i = 0; i < L.batches.size(); ++i) if (L.batches[i].vb) { g_recordHadVB = true; break; } }
	_freeBatchVBs(g_lists[list]);        // releasing the old VBs before re-recording
	g_recordingList = list;
	g_lists[list].used = true;
	g_lists[list].ownsTex = false;   // [font-fix] reset; set true if glBindTexture is called during this recording
	g_recordBaseMV = top(g_mv);   // [font] baseline: glEndList records glTranslatef-inside-list as a net delta off this
}
// Upload one list's staged CPU geometry into GPU vertex buffers. The DEVICE MUST BE OUT OF SCENE across the
// call (the 360 backend rejects mid-scene creates); callers bracket it with EndScene/BeginScene.
static void _createListVBs(GLList& L) {
	// Lock() on a buffer the GPU may still be reading stalls the driver; a stale binding also makes the
	// validator inspect a slot whose resource we are mid-write on. Unbind once for the whole upload pass.
	_unbindStream();
	for (size_t i = 0; i < L.batches.size(); ++i) {
		GLBatch& b = L.batches[i];
		if (b.vb || b.tris.empty()) continue;           // already uploaded (or nothing to upload)
		UINT bytes = (UINT)(b.tris.size() * sizeof(GLVert));
		if (SUCCEEDED(g_dev->CreateVertexBuffer(bytes, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &b.vb, 0)) && b.vb) {
			void* p = 0;
			if (SUCCEEDED(b.vb->Lock(0, bytes, &p, 0))) { memcpy(p, &b.tris[0], bytes); b.vb->Unlock(); }
			b.triCount = (int)(b.tris.size() / 3);
		}
		std::vector<GLVert>().swap(b.tris);             // free the CPU staging copy now that it's on the GPU
	}
}
// [J2X-PERF terrain-load] STAGE the recorded geometry (CPU tris already in the batches). A FIRST build (initial
// load) is deferred to the once-per-frame _pumpPendingVBs so a load frame pays ONE device stall for all its
// chunks instead of one per chunk. A RE-build of an already-visible chunk (block edit, g_recordHadVB) uploads
// NOW so it draws this same frame -- deferring it would leave the chunk blank for one frame (a visible flash).
void glEndList() {
	if (g_recordingList > 0 && g_recordingList < (int)g_lists.size() && g_dev) {
		bool hasGeom = false;
		GLList& L = g_lists[g_recordingList];
		for (size_t i = 0; i < L.batches.size(); ++i) if (!L.batches[i].tris.empty()) { hasGeom = true; break; }
		if (hasGeom) {
			if (g_recordHadVB) { g_dev->EndScene(); _createListVBs(L); g_dev->BeginScene(); }   // edit: no flash
			else g_pendingVB.push_back(g_recordingList);                                        // load: defer
		}
		// [font] record the net modelview delta applied during this list (FontRenderer's per-glyph
		// glTranslatef pen advance). delta = end * base^-1 (row-vector: top = T * base), replayed at
		// glCallList. Identity for geometry-only lists -> hasXform false. Computed even for a blank
		// glyph (e.g. space) so the pen still advances.
		D3DXMATRIX inv;
		if (D3DXMatrixInverse(&inv, 0, &g_recordBaseMV)) {
			D3DXMATRIX delta = top(g_mv) * inv, idm; D3DXMatrixIdentity(&idm);
			L.xform = delta;
			L.hasXform = (memcmp(&delta, &idm, sizeof(D3DXMATRIX)) != 0);
		}
	}
	g_recordingList = 0;
}
// Create all pending display-list vertex buffers in ONE out-of-scene window (called per frame from glFlush).
static void _pumpPendingVBs() {
	if (g_pendingVB.empty() || !g_dev) { g_pendingVB.clear(); return; }
	g_dev->EndScene();                                  // ONE stall for every chunk built this frame
	for (size_t k = 0; k < g_pendingVB.size(); ++k) {
		int id = g_pendingVB[k];
		if (id > 0 && id < (int)g_lists.size()) _createListVBs(g_lists[id]);
	}
	g_dev->BeginScene();
	g_pendingVB.clear();
}
void glCallList(int list) {
	_flushEntityBatch();   // [J2X-PERF] flush pending entity geometry before drawing a display list (world chunks etc.)
	if (list <= 0 || list >= (int)g_lists.size() || !g_lists[list].used || !g_dev || !g_vs) return;
	GLList& L = g_lists[list];
	int saveTex = g_boundTex; bool saveEn = g_texEnabled;
	// [font-fix] A list that bound its OWN texture during recording (terrain chunks) replays each batch with
	// that recorded texture. A texture-AGNOSTIC list -- one that never called glBindTexture inside glNewList,
	// e.g. FontRenderer's glyph lists, which bind the font texture at glCallLists TIME -- must replay with the
	// caller's CURRENT (live) texture + enable state, or the glyphs draw untextured (white boxes). This matches
	// the hand-port's live-state display-list replay.
	for (size_t i = 0; i < L.batches.size(); ++i) {
		GLBatch& b = L.batches[i];
		if (!b.vb || b.triCount <= 0) continue;
		if (L.ownsTex) { g_boundTex = b.tex;   g_texEnabled = b.texEnabled; }
		else           { g_boundTex = saveTex; g_texEnabled = saveEn; }
		// [font colour + item/world-brightness] EVERY batch inherits the CURRENT glColor via the gCol PS
		// constant. Colourless (white-baked) font batches need it to take the text colour; COLOURED batches
		// (terrain lighting, or item/entity meshes recorded into a list) need glColor as a global block-light
		// multiplier ON TOP of their baked vertex colours -- without it list-recorded coloured geometry renders
		// full-bright, the display-list twin of the direct-draw fix above. glColor is white for terrain/GUI
		// (tint == no-op there), so this only dims geometry the game actually dims. Matches issueDrawListOp.
		g_modR = g_cr; g_modG = g_cg; g_modB = g_cb; g_modA = g_ca;
		_applyDrawState();
		_bindStream(b.vb);
		g_dev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, b.triCount);
	}
	g_modR = g_modG = g_modB = g_modA = 1.0f;   // restore white for subsequent non-list draws
	g_boundTex = saveTex; g_texEnabled = saveEn;
	// [font] replay the list's recorded in-list translate onto the live modelview so a glCallLists run
	// steps each glyph across (row-vector: top = delta * top). No-op for geometry-only lists (chunks).
	// This is bracketed by the caller's glPushMatrix/glPopMatrix (FontRenderer.renderString), so the
	// accumulation is discarded after the string.
	if (L.hasXform) { D3DXMATRIX& m = top(g_mv); m = L.xform * m; }
	// Leave stream 0 empty. Holding the last list's VB bound past the draw is what let a later
	// Release/Lock hit a live resource, and it also collides with DrawPrimitiveUP's internal buffer.
	_unbindStream();
}
// glCallLists(IntBuffer): execute the ids in [position, limit) -- base() already folds the byte position
// in, so read straight from it (the same contract glDrawArrays relies on). FontRenderer put()s glyph list
// ids then flip()s before calling, so remaining() is exactly the count to replay.
void glCallLists(IntBuffer* lists) {
	if (!lists) return;
	const int n = lists->remaining();
	const int* ids = (const int*)lists->base();
	if (!ids || n <= 0) return;
	for (int i = 0; i < n; ++i) glCallList(ids[i]);
}
void glDeleteLists(int list, int n) {
	for (int i = 0; i < n; ++i) {
		int id = list + i;
		if (id > 0 && id < (int)g_lists.size()) { _freeBatchVBs(g_lists[id]); g_lists[id].used = false; }
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

} // namespace d3d9
