// GpuD3D11.cpp -- the GL layer's Direct3D 11 backend (Gpu.h), the PC built with --graphics d3d11. It carries out what GL11.cpp
// decided, on a Direct3D 11 device the Display made: GL's fixed-function pipeline is the shared HLSL in GpuShaders.h, wrapped
// here in shader model 4 (feature level 10_0 and up, so any Direct3D 11 card and Windows' software rasterizer, WARP, run it).
//
// Direct3D 11 keeps state in immutable objects: a blend, depth-stencil, rasterizer or sampler state is made once per distinct
// setting and reused (the GL layer's few combinations make a handful of each). Immediate geometry streams through one dynamic
// vertex buffer (written without waiting while there is room, renamed when it wraps); display lists get immutable buffers.
// Textures are B8G8R8A8: the layer's ARGB words are those bytes in memory, so they go up as they are.
#include "stdafx.h"
#include "GpuD3D11.h"
#include "GpuShaders.h"
#include "Common.h"     // DbgPrintF / J2X_UNHANDLED
#include <d3d11.h>
#include <d3dcompiler.h>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <string.h>

namespace j2xgl {
namespace gpu {

struct TextureObject {   // (gpu::Texture)
	ID3D11Texture2D* tex;
	ID3D11ShaderResourceView* srv;
	int w, h, levels;
};
struct BufferObject {    // (gpu::Buffer)
	ID3D11Buffer* vb;
};

// =========================================================================== the device
static ID3D11Device* g_dev = 0;
static ID3D11DeviceContext* g_ctx = 0;
static ID3D11RenderTargetView* g_rtv = 0;
static ID3D11DepthStencilView* g_dsv = 0;
static ID3D11Texture2D* g_back = 0;
static int g_w = 854, g_h = 480;

static ID3D11VertexShader* g_vs = 0;
static ID3D11PixelShader* g_ps = 0;
static ID3D11InputLayout* g_layout = 0;
static ID3D11Buffer* g_vsCB = 0;
static ID3D11Buffer* g_psCB = 0;
static ID3D11Buffer* g_stream = 0;             // the dynamic buffer immediate geometry goes through
static const UINT kStreamBytes = 4u << 20;     // 4 MB: ~70,000 vertices before it wraps
static UINT g_streamAt = 0;

template<class T> static void _release(T*& p) { if (p) { p->Release(); p = 0; } }

// J2X_D3D_DEBUG=1 (the Display made the device with the debug layer): the layer's errors and warnings on stderr, each kind once,
// as frames are presented.
static ID3D11InfoQueue* g_info = 0;
static void _debugMessages() {
	if (!g_info) return;
	static std::set<int> said;
	const UINT64 n = g_info->GetNumStoredMessages();
	for (UINT64 i = 0; i < n; ++i) {
		SIZE_T len = 0;
		if (FAILED(g_info->GetMessage(i, 0, &len)) || !len) continue;
		std::vector<char> buf(len);
		D3D11_MESSAGE* m = (D3D11_MESSAGE*)&buf[0];
		if (FAILED(g_info->GetMessage(i, m, &len))) continue;
		if (m->Severity > D3D11_MESSAGE_SEVERITY_WARNING || !said.insert((int)m->ID).second) continue;
		DbgPrintF("[J2X] Direct3D 11 debug layer (%s): %s\n", m->Severity <= D3D11_MESSAGE_SEVERITY_ERROR ? "error" : "warning", m->pDescription);
	}
	g_info->ClearStoredMessages();
}

// =========================================================================== the shaders
// GL's pipeline (GpuShaders.h) in shader model 4: the constants as two constant buffers laid out in GL11.cpp's order (a float4x4
// is four float4s, column-major as Direct3D 9's registers), the entry points with Direct3D 11's semantics.
static const char* const kVSHead =
	"cbuffer GLVertex : register(b0) {\n"
	"  float4x4 gWVP; float4x4 gMV; float4x4 gT0; float4x4 gT1;\n"
	"  float4 gLit; float4 gAmb; float4 gL0p; float4 gL0d; float4 gL0a; float4 gL1p; float4 gL1d; float4 gL1a;\n"
	"  float4 gMa; float4 gMd; float4 gTint; float4 gShade;\n"
	"  float4 gGen; float4 gGenO[4]; float4 gGenE[4];\n"
	"};\n";
static const char* const kVSMain =
	"void main(float3 p : POSITION, float3 vn : NORMAL, float4 vc : COLOR0, float2 t0 : TEXCOORD0, float2 t1 : TEXCOORD1,\n"
	"          float3 fn : TEXCOORD2, float4 fc : COLOR1,\n"
	"          out float4 op : SV_Position, out float4 oc : COLOR0, out float4 ot0 : TEXCOORD0, out float2 ot1 : TEXCOORD1,\n"
	"          out float ofz : TEXCOORD2) {\n"
	"  glVertex(p, vn, vc, t0, t1, fn, fc, op, oc, ot0, ot1, ofz);\n"
	"}\n";
static const char* const kPSHead =
	"cbuffer GLFragment : register(b0) { float4 gTex; float4 gAlpha; float4 gFog; float4 gFogC; };\n"
	"Texture2D tex0 : register(t0);\n"
	"Texture2D tex1 : register(t1);\n"
	"SamplerState samp0 : register(s0);\n"
	"SamplerState samp1 : register(s1);\n";
static const char* const kPSMain =
	"float4 main(float4 pos : SV_Position, float4 c : COLOR0, float4 t0 : TEXCOORD0, float2 t1 : TEXCOORD1, float fz : TEXCOORD2) : SV_Target {\n"
	"  bool keep;\n"
	"  float4 o = glFragment(c, tex0.Sample(samp0, t0.xy / t0.w), tex1.Sample(samp1, t1), fz, keep);\n"
	"  clip(keep ? 1 : -1);\n"
	"  return o;\n"
	"}\n";

// D3DCompile from d3dcompiler_47.dll, loaded on first use (no import library: any Windows compiler links this file).
static ID3DBlob* _compile(const std::string& src, const char* profile) {
	static pD3DCompile compile = 0;
	static bool tried = false;
	if (!tried) {
		tried = true;
		HMODULE dll = LoadLibraryA("d3dcompiler_47.dll");
		if (!dll) dll = LoadLibraryA("d3dcompiler_43.dll");
		if (dll) compile = (pD3DCompile)GetProcAddress(dll, "D3DCompile");
	}
	if (!compile) { DbgPrintF("[J2X] Direct3D 11: no shader compiler (d3dcompiler_47.dll)\n"); return 0; }
	ID3DBlob* code = 0;
	ID3DBlob* err = 0;
	const HRESULT hr = compile(src.c_str(), src.size(), 0, 0, 0, "main", profile, D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &code, &err);
	if (FAILED(hr)) DbgPrintF("[J2X] Direct3D 11 %s compile: %s\n", profile, err ? (const char*)err->GetBufferPointer() : "failed");
	if (err) err->Release();
	return SUCCEEDED(hr) ? code : 0;
}

static ID3D11Buffer* _makeBuffer(UINT bytes, D3D11_USAGE usage, UINT bind, UINT cpu, const void* data) {
	D3D11_BUFFER_DESC d;
	memset(&d, 0, sizeof(d));
	d.ByteWidth = bytes; d.Usage = usage; d.BindFlags = bind; d.CPUAccessFlags = cpu;
	D3D11_SUBRESOURCE_DATA init;
	memset(&init, 0, sizeof(init));
	init.pSysMem = data;
	ID3D11Buffer* b = 0;
	if (FAILED(g_dev->CreateBuffer(&d, data ? &init : 0, &b))) return 0;
	return b;
}

bool attachD3D11(ID3D11Device* device, ID3D11DeviceContext* context) {
	detachD3D11();
	g_dev = device; g_ctx = context;
	if (!g_dev || !g_ctx) return false;
	g_dev->AddRef(); g_ctx->AddRef();
	ID3DBlob* vs = _compile(std::string(kVSHead) + kGlVertexStage + kVSMain, "vs_4_0");
	ID3DBlob* ps = _compile(std::string(kPSHead) + kGlFragmentStage + kPSMain, "ps_4_0");
	if (vs) g_dev->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), 0, &g_vs);
	if (ps) g_dev->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), 0, &g_ps);
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {   // gpu::Vertex
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_B8G8R8A8_UNORM,  0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },   // (ARGB words: B, G, R, A bytes)
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },   // the provoking normal
		{ "COLOR",    1, DXGI_FORMAT_B8G8R8A8_UNORM,  0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },   // the provoking colour
	};
	if (vs) g_dev->CreateInputLayout(layout, 7, vs->GetBufferPointer(), vs->GetBufferSize(), &g_layout);
	_release(vs);
	_release(ps);
	if (FAILED(g_dev->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&g_info))) g_info = 0;   // (there with the debug layer only)
	if (g_info) DbgPrintF("[J2X] Direct3D 11: the debug layer is on\n");
	g_vsCB = _makeBuffer(kVSConsts * 16, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0);
	g_psCB = _makeBuffer(kPSConsts * 16, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0);
	g_stream = _makeBuffer(kStreamBytes, D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, 0);
	g_streamAt = kStreamBytes;   // (the first write renames it)
	const bool ok = g_vs && g_ps && g_layout && g_vsCB && g_psCB && g_stream;
	DbgPrintF(ok ? "[J2X] Direct3D 11: shaders, layout and buffers ready\n" : "[J2X] Direct3D 11: setup FAILED (see above)\n");
	forgetState();
	return ok;
}

void setTargetsD3D11(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, ID3D11Texture2D* backbuffer, int w, int h) {
	if (g_ctx) g_ctx->OMSetRenderTargets(0, 0, 0);
	_release(g_rtv); _release(g_dsv); _release(g_back);
	g_rtv = rtv; g_dsv = dsv; g_back = backbuffer;
	if (g_rtv) g_rtv->AddRef();
	if (g_dsv) g_dsv->AddRef();
	if (g_back) g_back->AddRef();
	if (w > 0) g_w = w;
	if (h > 0) g_h = h;
	if (g_ctx && g_rtv) g_ctx->OMSetRenderTargets(1, &g_rtv, g_dsv);
	forgetState();
}

// =========================================================================== state objects, made once per setting
static std::map<unsigned, ID3D11BlendState*> s_blends;
static std::map<unsigned, ID3D11DepthStencilState*> s_depths;
static std::map<unsigned long long, ID3D11RasterizerState*> s_rasters;
static std::map<unsigned, ID3D11SamplerState*> s_samplers;

void detachD3D11() {
	for (std::map<unsigned, ID3D11BlendState*>::iterator i = s_blends.begin(); i != s_blends.end(); ++i) i->second->Release();
	for (std::map<unsigned, ID3D11DepthStencilState*>::iterator i = s_depths.begin(); i != s_depths.end(); ++i) i->second->Release();
	for (std::map<unsigned long long, ID3D11RasterizerState*>::iterator i = s_rasters.begin(); i != s_rasters.end(); ++i) i->second->Release();
	for (std::map<unsigned, ID3D11SamplerState*>::iterator i = s_samplers.begin(); i != s_samplers.end(); ++i) i->second->Release();
	s_blends.clear(); s_depths.clear(); s_rasters.clear(); s_samplers.clear();
	_release(g_vs); _release(g_ps); _release(g_layout); _release(g_vsCB); _release(g_psCB); _release(g_stream);
	_release(g_rtv); _release(g_dsv); _release(g_back);
	_debugMessages();
	_release(g_info);
	_release(g_ctx); _release(g_dev);
}

static const D3D11_BLEND kBlend[] = { D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_SRC_COLOR, D3D11_BLEND_INV_SRC_COLOR, D3D11_BLEND_SRC_ALPHA,
                                      D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_DEST_ALPHA, D3D11_BLEND_INV_DEST_ALPHA, D3D11_BLEND_DEST_COLOR,
                                      D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_SRC_ALPHA_SAT };
// The alpha channel's factors: a colour factor has no place there in Direct3D 11, so it takes the matching alpha one (as GL does).
static D3D11_BLEND _alphaBlend(int b) {
	switch (b) {
		case kSrcColor: return D3D11_BLEND_SRC_ALPHA;
		case kInvSrcColor: return D3D11_BLEND_INV_SRC_ALPHA;
		case kDstColor: return D3D11_BLEND_DEST_ALPHA;
		case kInvDstColor: return D3D11_BLEND_INV_DEST_ALPHA;
		case kSrcAlphaSat: return D3D11_BLEND_ONE;
		default: return kBlend[b];
	}
}
static ID3D11BlendState* _blendState(const RenderState& rs) {
	const unsigned key = (rs.blend ? 1u : 0u) | ((unsigned)rs.srcBlend << 1) | ((unsigned)rs.dstBlend << 6) | (rs.colorWrite << 11);
	std::map<unsigned, ID3D11BlendState*>::iterator it = s_blends.find(key);
	if (it != s_blends.end()) return it->second;
	D3D11_BLEND_DESC d;
	memset(&d, 0, sizeof(d));
	D3D11_RENDER_TARGET_BLEND_DESC& t = d.RenderTarget[0];
	t.BlendEnable = rs.blend ? TRUE : FALSE;
	t.SrcBlend = kBlend[rs.srcBlend]; t.DestBlend = kBlend[rs.dstBlend]; t.BlendOp = D3D11_BLEND_OP_ADD;
	t.SrcBlendAlpha = _alphaBlend(rs.srcBlend); t.DestBlendAlpha = _alphaBlend(rs.dstBlend); t.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	t.RenderTargetWriteMask = (UINT8)(((rs.colorWrite & 1) ? D3D11_COLOR_WRITE_ENABLE_RED : 0) | ((rs.colorWrite & 2) ? D3D11_COLOR_WRITE_ENABLE_GREEN : 0) |
	                                  ((rs.colorWrite & 4) ? D3D11_COLOR_WRITE_ENABLE_BLUE : 0) | ((rs.colorWrite & 8) ? D3D11_COLOR_WRITE_ENABLE_ALPHA : 0));
	ID3D11BlendState* s = 0;
	g_dev->CreateBlendState(&d, &s);
	s_blends[key] = s;
	return s;
}
static ID3D11DepthStencilState* _depthState(const RenderState& rs) {
	const unsigned key = (rs.depthTest ? 1u : 0u) | ((unsigned)rs.depthFunc << 1) | (rs.depthWrite ? 16u : 0u);
	std::map<unsigned, ID3D11DepthStencilState*>::iterator it = s_depths.find(key);
	if (it != s_depths.end()) return it->second;
	D3D11_DEPTH_STENCIL_DESC d;
	memset(&d, 0, sizeof(d));
	// GL: with the depth test off nothing is written to depth either.
	d.DepthEnable = rs.depthTest ? TRUE : FALSE;
	d.DepthWriteMask = rs.depthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	d.DepthFunc = (D3D11_COMPARISON_FUNC)(D3D11_COMPARISON_NEVER + rs.depthFunc);   // (NEVER..ALWAYS, the layer's order)
	ID3D11DepthStencilState* s = 0;
	g_dev->CreateDepthStencilState(&d, &s);
	s_depths[key] = s;
	return s;
}
static ID3D11RasterizerState* _rasterState(const RenderState& rs) {
	const int bias = (int)(rs.depthBias * 16777216.0f + (rs.depthBias >= 0 ? 0.5f : -0.5f));   // back to depth-buffer steps (D24)
	unsigned slopeBits; memcpy(&slopeBits, &rs.slopeBias, 4);
	const unsigned long long key = (unsigned long long)rs.cull | ((rs.scissor ? 1ull : 0ull) << 2) | ((unsigned long long)(unsigned)(bias & 0xFFFFFF) << 3) |
	                               ((unsigned long long)slopeBits << 27);
	std::map<unsigned long long, ID3D11RasterizerState*>::iterator it = s_rasters.find(key);
	if (it != s_rasters.end()) return it->second;
	D3D11_RASTERIZER_DESC d;
	memset(&d, 0, sizeof(d));
	d.FillMode = D3D11_FILL_SOLID;
	// The layer names the on-screen winding NOT drawn; Direct3D 11 names the face culled, front being clockwise.
	d.CullMode = rs.cull == kCullCW ? D3D11_CULL_FRONT : rs.cull == kCullCCW ? D3D11_CULL_BACK : D3D11_CULL_NONE;
	d.FrontCounterClockwise = FALSE;
	d.DepthBias = bias;
	d.SlopeScaledDepthBias = rs.slopeBias;
	d.DepthClipEnable = TRUE;
	d.ScissorEnable = rs.scissor ? TRUE : FALSE;
	ID3D11RasterizerState* s = 0;
	g_dev->CreateRasterizerState(&d, &s);
	s_rasters[key] = s;
	return s;
}
static ID3D11SamplerState* _samplerState(const Sampler& sm) {
	const unsigned key = (sm.wrapU ? 1u : 0u) | (sm.wrapV ? 2u : 0u) | ((unsigned)sm.mag << 2) | ((unsigned)sm.min << 3) | ((unsigned)(sm.mip + 1) << 4);
	std::map<unsigned, ID3D11SamplerState*>::iterator it = s_samplers.find(key);
	if (it != s_samplers.end()) return it->second;
	D3D11_SAMPLER_DESC d;
	memset(&d, 0, sizeof(d));
	const bool minL = sm.min == kLinear, magL = sm.mag == kLinear, mipL = sm.mip == kLinear;
	// Direct3D 11's filter is one enum for the three: MIN, MAG, MIP each POINT or LINEAR.
	static const D3D11_FILTER kFilter[8] = {
		D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT, D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D11_FILTER_MIN_MAG_MIP_LINEAR };
	d.Filter = kFilter[(minL ? 4 : 0) | (magL ? 2 : 0) | (mipL ? 1 : 0)];
	d.AddressU = sm.wrapU ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP;
	d.AddressV = sm.wrapV ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP;
	d.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	d.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d.MinLOD = 0;
	d.MaxLOD = sm.mip < 0 ? 0.0f : D3D11_FLOAT32_MAX;   // (no mip filter: the top level only)
	ID3D11SamplerState* s = 0;
	g_dev->CreateSamplerState(&d, &s);
	s_samplers[key] = s;
	return s;
}

// =========================================================================== what is on the device
static bool s_known = false;
static ID3D11BlendState* s_blend = 0;
static ID3D11DepthStencilState* s_depth = 0;
static ID3D11RasterizerState* s_raster = 0;
static D3D11_VIEWPORT s_vp;
static D3D11_RECT s_sc;
static float s_vsc[kVSConsts * 4], s_psc[kPSConsts * 4];
static ID3D11ShaderResourceView* s_srv[2];
static ID3D11SamplerState* s_samp[2];
static D3D11_PRIMITIVE_TOPOLOGY s_topo = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
static ID3D11Buffer* s_vb = 0;

bool ready() { return g_dev && g_ctx && g_rtv && g_vs && g_ps && g_layout && g_vsCB && g_psCB && g_stream; }
const char* name() { return "Direct3D 11"; }
int targetWidth() { return g_w; }
int targetHeight() { return g_h; }
bool halfPixelOffset() { return false; }   // (Direct3D 10 and later centre pixels as GL does)
void forgetState() { s_known = false; }

static void _apply(Prim prim, const RenderState& rs, const Constants& c, const Bindings& b) {
	if (!s_known) {
		g_ctx->IASetInputLayout(g_layout);
		g_ctx->VSSetShader(g_vs, 0, 0);
		g_ctx->PSSetShader(g_ps, 0, 0);
		g_ctx->VSSetConstantBuffers(0, 1, &g_vsCB);
		g_ctx->PSSetConstantBuffers(0, 1, &g_psCB);
		g_ctx->OMSetRenderTargets(1, &g_rtv, g_dsv);
		s_blend = 0; s_depth = 0; s_raster = 0; s_vb = 0;
		s_srv[0] = s_srv[1] = (ID3D11ShaderResourceView*)~(size_t)0;
		s_samp[0] = s_samp[1] = (ID3D11SamplerState*)~(size_t)0;
		s_topo = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
		memset(&s_vp, 0, sizeof(s_vp));
		memset(&s_sc, 0, sizeof(s_sc));
	}
	ID3D11BlendState* bs = _blendState(rs);
	if (bs != s_blend) { const float f[4] = { 0, 0, 0, 0 }; g_ctx->OMSetBlendState(bs, f, 0xFFFFFFFF); s_blend = bs; }
	ID3D11DepthStencilState* ds = _depthState(rs);
	if (ds != s_depth) { g_ctx->OMSetDepthStencilState(ds, 0); s_depth = ds; }
	ID3D11RasterizerState* ras = _rasterState(rs);
	if (ras != s_raster) { g_ctx->RSSetState(ras); s_raster = ras; }
	D3D11_VIEWPORT vp;
	vp.TopLeftX = (float)rs.vpX; vp.TopLeftY = (float)rs.vpY; vp.Width = (float)rs.vpW; vp.Height = (float)rs.vpH; vp.MinDepth = 0.0f; vp.MaxDepth = 1.0f;
	if (!s_known || memcmp(&vp, &s_vp, sizeof(vp)) != 0) { g_ctx->RSSetViewports(1, &vp); s_vp = vp; }
	if (rs.scissor) {
		D3D11_RECT sc;
		sc.left = rs.scX; sc.top = rs.scY; sc.right = rs.scX + rs.scW; sc.bottom = rs.scY + rs.scH;
		if (!s_known || memcmp(&sc, &s_sc, sizeof(sc)) != 0) { g_ctx->RSSetScissorRects(1, &sc); s_sc = sc; }
	}
	D3D11_MAPPED_SUBRESOURCE m;
	if (!s_known || memcmp(s_vsc, c.vs, sizeof(s_vsc)) != 0) {
		if (SUCCEEDED(g_ctx->Map(g_vsCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) { memcpy(m.pData, c.vs, sizeof(s_vsc)); g_ctx->Unmap(g_vsCB, 0); }
		memcpy(s_vsc, c.vs, sizeof(s_vsc));
	}
	if (!s_known || memcmp(s_psc, c.ps, sizeof(s_psc)) != 0) {
		if (SUCCEEDED(g_ctx->Map(g_psCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) { memcpy(m.pData, c.ps, sizeof(s_psc)); g_ctx->Unmap(g_psCB, 0); }
		memcpy(s_psc, c.ps, sizeof(s_psc));
	}
	for (int u = 0; u < 2; ++u) {
		ID3D11ShaderResourceView* srv = b.tex[u] ? b.tex[u]->srv : 0;
		if (srv != s_srv[u]) { g_ctx->PSSetShaderResources(u, 1, &srv); s_srv[u] = srv; }
		ID3D11SamplerState* ss = _samplerState(b.samp[u]);
		if (ss != s_samp[u]) { g_ctx->PSSetSamplers(u, 1, &ss); s_samp[u] = ss; }
	}
	const D3D11_PRIMITIVE_TOPOLOGY topo = prim == kLines ? D3D11_PRIMITIVE_TOPOLOGY_LINELIST : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	if (topo != s_topo) { g_ctx->IASetPrimitiveTopology(topo); s_topo = topo; }
	s_known = true;
}
static void _bindVB(ID3D11Buffer* vb, UINT offset) {
	const UINT stride = sizeof(Vertex);
	g_ctx->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	s_vb = vb;
}

// =========================================================================== drawing
void draw(Prim prim, const Vertex* v, int count, const RenderState& rs, const Constants& c, const Bindings& b) {
	if (!ready() || count <= 0) return;
	const UINT bytes = (UINT)count * sizeof(Vertex);
	ID3D11Buffer* vb = g_stream;
	ID3D11Buffer* once = 0;
	UINT offset = 0;
	if (bytes > kStreamBytes) {   // bigger than the stream: a buffer of its own for this draw
		once = _makeBuffer(bytes, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, v);
		if (!once) return;
		vb = once;
	} else {
		D3D11_MAPPED_SUBRESOURCE m;
		const bool wrap = g_streamAt + bytes > kStreamBytes;
		if (wrap) g_streamAt = 0;
		if (FAILED(g_ctx->Map(g_stream, 0, wrap ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE, 0, &m))) return;
		memcpy((char*)m.pData + g_streamAt, v, bytes);
		g_ctx->Unmap(g_stream, 0);
		offset = g_streamAt;
		g_streamAt += bytes;
	}
	_apply(prim, rs, c, b);
	_bindVB(vb, offset);
	g_ctx->Draw((UINT)count, 0);
	if (once) { _bindVB(0, 0); once->Release(); }
}

Buffer createBuffer(const Vertex* v, int count) {
	if (!g_dev || count <= 0) return 0;
	ID3D11Buffer* vb = _makeBuffer((UINT)count * sizeof(Vertex), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, v);
	if (!vb) return 0;
	BufferObject* o = new BufferObject;
	o->vb = vb;
	return o;
}
void releaseBuffer(Buffer buf) {
	if (!buf) return;
	if (s_vb == buf->vb && g_ctx) _bindVB(0, 0);
	if (buf->vb) buf->vb->Release();
	delete buf;
}
void drawBuffer(Prim prim, Buffer buf, int count, const RenderState& rs, const Constants& c, const Bindings& b) {
	if (!ready() || !buf || !buf->vb || count <= 0) return;
	_apply(prim, rs, c, b);
	if (s_vb != buf->vb) _bindVB(buf->vb, 0);
	g_ctx->Draw((UINT)count, 0);
}
void beginUploads() {}   // (Direct3D 11 makes resources at any time)
void endUploads() {}

// GL clears the whole target where the masks let it (decided by the layer), inside the scissor box when that test is on.
void clear(bool color, bool depth, bool stencil, float r, float g, float b, float a, float z, const RenderState& rs) {
	if (!g_ctx || !g_rtv) return;
	if (rs.scissor && (rs.scX > 0 || rs.scY > 0 || rs.scW < g_w || rs.scH < g_h))
		J2X_UNHANDLED("glClear inside a scissor box on Direct3D 11 (clears the whole target)", 0);
	if (color) { const float c[4] = { r, g, b, a }; g_ctx->ClearRenderTargetView(g_rtv, c); }
	if ((depth || stencil) && g_dsv)
		g_ctx->ClearDepthStencilView(g_dsv, (depth ? D3D11_CLEAR_DEPTH : 0) | (stencil ? D3D11_CLEAR_STENCIL : 0), z, 0);
}

// =========================================================================== textures
static void _unbindTexture(Texture t) {
	for (int u = 0; u < 2; ++u)
		if (g_ctx && s_srv[u] == t->srv) { ID3D11ShaderResourceView* none = 0; g_ctx->PSSetShaderResources(u, 1, &none); s_srv[u] = 0; }
}
Texture createTexture(int w, int h, int levels) {
	if (!g_dev || w <= 0 || h <= 0) return 0;
	D3D11_TEXTURE2D_DESC d;
	memset(&d, 0, sizeof(d));
	d.Width = (UINT)w; d.Height = (UINT)h; d.MipLevels = (UINT)levels; d.ArraySize = 1;
	d.Format = DXGI_FORMAT_B8G8R8A8_UNORM;   // (the layer's ARGB words, as they lie in memory)
	d.SampleDesc.Count = 1;
	d.Usage = D3D11_USAGE_DEFAULT;
	d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	ID3D11Texture2D* tex = 0;
	if (FAILED(g_dev->CreateTexture2D(&d, 0, &tex)) || !tex) { DbgPrintF("[J2X] Direct3D 11: a %dx%d texture could not be made\n", w, h); return 0; }
	ID3D11ShaderResourceView* srv = 0;
	if (FAILED(g_dev->CreateShaderResourceView(tex, 0, &srv))) { tex->Release(); return 0; }
	TextureObject* t = new TextureObject;
	t->tex = tex; t->srv = srv; t->w = w; t->h = h; t->levels = levels;
	return t;
}
void releaseTexture(Texture t) {
	if (!t) return;
	_unbindTexture(t);
	_release(t->srv);
	_release(t->tex);
	delete t;
}
bool writeTexture(Texture t, int level, int x, int y, int w, int h, const unsigned int* argb) {
	if (!t || !t->tex || !g_ctx || w <= 0 || h <= 0) return false;
	D3D11_BOX box;
	box.left = (UINT)x; box.top = (UINT)y; box.front = 0; box.right = (UINT)(x + w); box.bottom = (UINT)(y + h); box.back = 1;
	g_ctx->UpdateSubresource(t->tex, D3D11CalcSubresource((UINT)level, 0, (UINT)t->levels), &box, argb, (UINT)w * 4, 0);
	return true;
}
// A copy of one level (or the frame) through a staging texture the CPU can read.
static bool _readBack(ID3D11Texture2D* src, UINT subresource, int w, int h, std::vector<unsigned int>& argb) {
	D3D11_TEXTURE2D_DESC d;
	src->GetDesc(&d);
	d.Width = (UINT)w; d.Height = (UINT)h; d.MipLevels = 1; d.ArraySize = 1; d.SampleDesc.Count = 1; d.SampleDesc.Quality = 0;
	d.Usage = D3D11_USAGE_STAGING; d.BindFlags = 0; d.CPUAccessFlags = D3D11_CPU_ACCESS_READ; d.MiscFlags = 0;
	ID3D11Texture2D* staging = 0;
	if (FAILED(g_dev->CreateTexture2D(&d, 0, &staging)) || !staging) return false;
	g_ctx->CopySubresourceRegion(staging, 0, 0, 0, 0, src, subresource, 0);
	D3D11_MAPPED_SUBRESOURCE m;
	bool ok = false;
	if (SUCCEEDED(g_ctx->Map(staging, 0, D3D11_MAP_READ, 0, &m))) {
		argb.resize((size_t)w * h);
		for (int r = 0; r < h; ++r) memcpy(&argb[(size_t)r * w], (const char*)m.pData + (size_t)r * m.RowPitch, (size_t)w * 4);
		g_ctx->Unmap(staging, 0);
		ok = true;
	}
	staging->Release();
	return ok;
}
bool readTexture(Texture t, int level, std::vector<unsigned int>& argb, int& w, int& h) {
	if (!t || !t->tex || !g_ctx) return false;
	w = t->w >> level; if (w < 1) w = 1;
	h = t->h >> level; if (h < 1) h = 1;
	return _readBack(t->tex, D3D11CalcSubresource((UINT)level, 0, (UINT)t->levels), w, h, argb);
}

// =========================================================================== the frame
// The Display calls this after presenting a frame.
void frameDoneD3D11() { _debugMessages(); }

bool readFrame(std::vector<unsigned int>& argb, int& w, int& h) {
	if (!g_back || !g_ctx) return false;
	w = g_w; h = g_h;
	if (!_readBack(g_back, 0, w, h, argb)) return false;
	for (size_t i = 0; i < argb.size(); ++i) argb[i] |= 0xFF000000u;   // (the frame is opaque, as GL's default framebuffer reads)
	return true;
}

}  // namespace gpu
}  // namespace j2xgl
