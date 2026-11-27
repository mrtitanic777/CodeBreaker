// GpuD3D12.cpp -- the GL layer's Direct3D 12 backend (Gpu.h), the PC built with --graphics d3d12. It carries out what GL11.cpp
// decided, on a Direct3D 12 device and queue the Display made: GL's fixed-function pipeline is the shared HLSL in GpuShaders.h,
// wrapped here in shader model 5 (feature level 11_0 and up, and Windows' software rasterizer, WARP).
//
// Direct3D 12 leaves to the program what earlier versions did for it, and this file is that work:
//  - the frame is recorded into one command list, run when the Display presents (two allocators, one per swap-chain buffer, each
//    reused once the fence says the GPU is done with the frame recorded in it);
//  - the fixed-function state is a pipeline state object per distinct setting (blend, depth, rasterizer and primitive type
//    together), made on first use and kept;
//  - the root signature: the two stages' constants as root constant-buffer views, each texture unit's texture and sampler a
//    one-descriptor table;
//  - memory the CPU writes (immediate geometry, the constants, texture and display-list uploads) comes from one upload ring that
//    is reused as the fence passes it;
//  - a texture lives in a default heap with its shader view in a fixed slot of the shader-visible heap, moved between the copy
//    and shader states as it is written and drawn with;
//  - display lists share default-heap pages of 8 MB, each list a range of one (a resource apiece would be thousands of small
//    allocations, each a trip to the video memory manager that can block while the GPU frees memory); their uploads go on a
//    second command list run just before the frame's, so a page is only ever copied to in one list and read in the next and
//    needs no barrier (Indev builds lists as it draws: in one list, uploads and draws alternated a hundred barriers a frame);
//  - a resource the game lets go of is released once the GPU has finished the frames that used it.
// Textures are B8G8R8A8: the layer's ARGB words are those bytes in memory, so they go up as they are.
#include "stdafx.h"
#include "GpuD3D12.h"
#include "GpuShaders.h"
#include "Common.h"     // DbgPrintF / J2X_UNHANDLED
#if defined(__MINGW32__) && !defined(WIDL_EXPLICIT_AGGREGATE_RETURNS)
#define WIDL_EXPLICIT_AGGREGATE_RETURNS   // (MinGW's headers: GetDesc and the heap-start handles return their structures as Direct3D 12 does)
#endif
#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <deque>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <string.h>

namespace j2xgl {
namespace gpu {

struct TextureObject {   // (gpu::Texture)
	ID3D12Resource* res;
	D3D12_RESOURCE_STATES state;
	UINT slot;           // its shader view in the shader-visible heap
	int w, h, levels;
};
// A default-heap buffer that display lists share: free ranges by offset. It is copied to only in the upload list and read only
// in the frame's list; a buffer starts each list's execution in COMMON and is promoted implicitly to each, so no barrier.
struct Page {
	ID3D12Resource* res;
	D3D12_GPU_VIRTUAL_ADDRESS va;
	UINT64 bytes;
	std::map<UINT64, UINT64> free;   // offset -> size
};
struct BufferObject {    // (gpu::Buffer): a range of a page
	Page* page;
	UINT64 offset;
	UINT64 size;                     // (the range, aligned)
	UINT bytes;                      // (the vertices)
};

// =========================================================================== the device and the frame
static const UINT kFrames = 2;                    // the swap chain's buffers, each with an allocator
static const UINT kSrvSlots = 8192;               // textures alive at once (slot 0: the empty view an unused unit reads)
static const UINT kSamplers = 48;                 // every Sampler setting (see _samplerIndex)
static const UINT64 kRingBytes = 32ull << 20;     // the upload ring
static const DXGI_FORMAT kColorFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
static const DXGI_FORMAT kDepthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

static ID3D12Device* g_dev = 0;
static ID3D12CommandQueue* g_queue = 0;
static IDXGISwapChain3* g_swap = 0;
static ID3D12CommandAllocator* g_alloc[kFrames];
static UINT64 g_allocFence[kFrames];             // the fence value that says the frame recorded with each allocator is done
static ID3D12GraphicsCommandList* g_list = 0;
static bool g_open = false;                      // the list is recording
static ID3D12CommandAllocator* g_upAlloc[kFrames];
static ID3D12GraphicsCommandList* g_upList = 0;  // the display-list uploads, run before g_list
static bool g_upOpen = false;
static ID3D12Fence* g_fence = 0;
static HANDLE g_event = 0;
static UINT64 g_fenceValue = 0;                  // the last value signalled
static ID3D12Resource* g_back[kFrames];
static UINT g_frame = 0;                         // the swap-chain buffer being drawn
static ID3D12Resource* g_depth = 0;
static ID3D12DescriptorHeap* g_rtvHeap = 0;
static ID3D12DescriptorHeap* g_dsvHeap = 0;
static ID3D12DescriptorHeap* g_srvHeap = 0;      // shader-visible: every texture's view
static ID3D12DescriptorHeap* g_sampHeap = 0;     // shader-visible: every sampler setting
static UINT g_rtvStep = 0, g_srvStep = 0, g_sampStep = 0;
static ID3D12RootSignature* g_root = 0;
static ID3DBlob* g_vsCode = 0;
static ID3DBlob* g_psCode = 0;
static int g_w = 854, g_h = 480;

static ID3D12Resource* g_ring = 0;
static unsigned char* g_ringCpu = 0;
static UINT64 g_ringGpu = 0;
static UINT64 g_head = 0, g_tail = 0;            // the ring: written up to head; the GPU may still read from tail on
static std::deque<std::pair<UINT64, UINT64> > g_inflight;   // (fence value, head when it was signalled), oldest first

static std::vector<UINT> g_freeSlots;
static UINT g_nextSlot = 1;
struct Doomed { UINT64 fence; IUnknown* obj; UINT slot; Page* page; UINT64 offset, size; };   // (an object, or a page's range)
static std::vector<Doomed> g_doomed;             // let go of by the game, released when the fence passes
static std::vector<Page*> g_pages;
static const UINT64 kPageBytes = 8ull << 20;
static const UINT64 kListAlign = 16;

static void _pageFree(Page* pg, UINT64 at, UINT64 size) {   // back into the free ranges, joined with its neighbours
	std::map<UINT64, UINT64>::iterator next = pg->free.lower_bound(at);
	if (next != pg->free.end() && at + size == next->first) { size += next->second; pg->free.erase(next++); }
	if (next != pg->free.begin()) {
		std::map<UINT64, UINT64>::iterator prev = next;
		--prev;
		if (prev->first + prev->second == at) { prev->second += size; return; }
	}
	pg->free[at] = size;
}

template<class T> static void _release(T*& p) { if (p) { p->Release(); p = 0; } }

// J2X_D3D_DEBUG=1 (the Display turned the debug layer on before making the device): the layer's errors and warnings on stderr,
// each kind once, as frames are presented.
static ID3D12InfoQueue* g_info = 0;
static void _debugMessages() {
	if (!g_info) return;
	static std::set<int> said;
	const UINT64 n = g_info->GetNumStoredMessages();
	for (UINT64 i = 0; i < n; ++i) {
		SIZE_T len = 0;
		if (FAILED(g_info->GetMessage(i, 0, &len)) || !len) continue;
		std::vector<char> buf(len);
		D3D12_MESSAGE* m = (D3D12_MESSAGE*)&buf[0];
		if (FAILED(g_info->GetMessage(i, m, &len))) continue;
		if (m->Severity > D3D12_MESSAGE_SEVERITY_WARNING || !said.insert((int)m->ID).second) continue;
		DbgPrintF("[J2X] Direct3D 12 debug layer (%s): %s\n", m->Severity <= D3D12_MESSAGE_SEVERITY_ERROR ? "error" : "warning", m->pDescription);
	}
	g_info->ClearStoredMessages();
}

static D3D12_CPU_DESCRIPTOR_HANDLE _cpu(ID3D12DescriptorHeap* heap, UINT step, UINT i) {
	D3D12_CPU_DESCRIPTOR_HANDLE h = heap->GetCPUDescriptorHandleForHeapStart();
	h.ptr += (SIZE_T)i * step;
	return h;
}
static D3D12_GPU_DESCRIPTOR_HANDLE _gpu(ID3D12DescriptorHeap* heap, UINT step, UINT i) {
	D3D12_GPU_DESCRIPTOR_HANDLE h = heap->GetGPUDescriptorHandleForHeapStart();
	h.ptr += (UINT64)i * step;
	return h;
}

static void _barrier(ID3D12Resource* r, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) {
	if (from == to || !r) return;
	D3D12_RESOURCE_BARRIER b;
	memset(&b, 0, sizeof(b));
	b.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	b.Transition.pResource = r;
	b.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	b.Transition.StateBefore = from;
	b.Transition.StateAfter = to;
	g_list->ResourceBarrier(1, &b);
}
static void _textureState(Texture t, D3D12_RESOURCE_STATES s) {
	_barrier(t->res, t->state, s);
	t->state = s;
}

// =========================================================================== running what was recorded
static void _wait(UINT64 value) {
	if (!g_fence || g_fence->GetCompletedValue() >= value) return;
	if (SUCCEEDED(g_fence->SetEventOnCompletion(value, g_event))) WaitForSingleObject(g_event, INFINITE);
}
static void _collect() {   // release what the GPU has finished with
	const UINT64 done = g_fence ? g_fence->GetCompletedValue() : ~0ull;
	size_t keep = 0;
	for (size_t i = 0; i < g_doomed.size(); ++i) {
		if (g_doomed[i].fence <= done) {
			if (g_doomed[i].page) _pageFree(g_doomed[i].page, g_doomed[i].offset, g_doomed[i].size);
			else g_doomed[i].obj->Release();
			if (g_doomed[i].slot) g_freeSlots.push_back(g_doomed[i].slot);
		} else {
			g_doomed[keep++] = g_doomed[i];
		}
	}
	g_doomed.resize(keep);
	while (!g_inflight.empty() && g_inflight.front().first <= done) { g_tail = g_inflight.front().second; g_inflight.pop_front(); }
}
static void _retire(IUnknown* obj, UINT slot) {   // the GPU may still use it in what is recorded now: the next signal frees it
	Doomed d;
	d.fence = g_fenceValue + 1; d.obj = obj; d.slot = slot; d.page = 0; d.offset = d.size = 0;
	g_doomed.push_back(d);
}

static bool s_known = false;   // the list's state is what s_* below say
static void _reopen(UINT allocator) {
	g_list->Reset(g_alloc[allocator], 0);
	ID3D12DescriptorHeap* heaps[2] = { g_srvHeap, g_sampHeap };
	g_list->SetDescriptorHeaps(2, heaps);
	g_open = true;
	s_known = false;
}
// The upload list, opened on its first copy since the last submit (with this frame's upload allocator).
static ID3D12GraphicsCommandList* _uploads() {
	if (!g_upOpen) {
		g_upList->Reset(g_upAlloc[g_frame], 0);
		g_upOpen = true;
	}
	return g_upList;
}
static UINT64 _submit() {
	if (g_upOpen) {   // first, on its own (its buffers decay to COMMON after it, ready to be promoted to reads in the next)
		g_upList->Close();
		ID3D12CommandList* up[1] = { g_upList };
		g_queue->ExecuteCommandLists(1, up);
		g_upOpen = false;
	}
	if (g_open) {
		g_list->Close();
		ID3D12CommandList* lists[1] = { g_list };
		g_queue->ExecuteCommandLists(1, lists);
		g_open = false;
	}
	g_queue->Signal(g_fence, ++g_fenceValue);
	g_inflight.push_back(std::make_pair(g_fenceValue, g_head));
	return g_fenceValue;
}
// Run what is recorded and wait for it (a read back, or the ring filled by one frame), then go on recording with the same frame.
static void _flush() {
	_wait(_submit());
	g_inflight.clear();
	g_head = g_tail = 0;
	g_alloc[g_frame]->Reset();
	g_upAlloc[g_frame]->Reset();
	_reopen(g_frame);
	_collect();
}

// `bytes` of the upload ring at `align`: false only when it is bigger than the ring. Waits for the GPU when the ring is full;
// when this frame alone has filled it, runs the frame so far first (_flush: the list's state is then set again).
static bool _ringAlloc(UINT64 bytes, UINT64 align, UINT64& at) {
	if (!g_ring || bytes > kRingBytes) return false;
	for (;;) {
		const bool empty = g_inflight.empty() && g_head == g_tail;
		if (empty) g_head = g_tail = 0;
		UINT64 pos = (g_head + align - 1) & ~(align - 1);
		bool fits;
		if (empty) {
			fits = pos + bytes <= kRingBytes;
		} else if (g_head >= g_tail) {                       // free: head..end, then 0..tail
			fits = pos + bytes <= kRingBytes;
			if (!fits && bytes < g_tail) { pos = 0; fits = true; }
		} else {                                             // free: head..tail
			fits = pos + bytes < g_tail;
		}
		if (fits) { at = pos; g_head = pos + bytes; return true; }
		if (!g_inflight.empty()) {
			_wait(g_inflight.front().first);
			g_tail = g_inflight.front().second;
			g_inflight.pop_front();
		} else {
			_flush();
		}
	}
}

// An upload buffer of its own, for what is bigger than the ring (released when the GPU is done with it).
static ID3D12Resource* _uploadBuffer(const void* data, UINT64 bytes) {
	D3D12_HEAP_PROPERTIES heap;
	memset(&heap, 0, sizeof(heap));
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC d;
	memset(&d, 0, sizeof(d));
	d.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; d.Width = bytes; d.Height = 1; d.DepthOrArraySize = 1; d.MipLevels = 1;
	d.SampleDesc.Count = 1; d.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	ID3D12Resource* r = 0;
	if (FAILED(g_dev->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &d, D3D12_RESOURCE_STATE_GENERIC_READ, 0, __uuidof(ID3D12Resource), (void**)&r)) || !r) return 0;
	void* p = 0;
	D3D12_RANGE none = { 0, 0 };
	if (FAILED(r->Map(0, &none, &p))) { r->Release(); return 0; }
	memcpy(p, data, (size_t)bytes);
	r->Unmap(0, 0);
	return r;
}

// =========================================================================== the shaders and the root signature
// GL's pipeline (GpuShaders.h) in shader model 5: the constants as two constant buffers laid out in GL11.cpp's order (a float4x4
// is four float4s, column-major as Direct3D 9's registers), the entry points with Direct3D 12's semantics (Direct3D 11's).
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
		if (dll) compile = (pD3DCompile)GetProcAddress(dll, "D3DCompile");
	}
	if (!compile) { DbgPrintF("[J2X] Direct3D 12: no shader compiler (d3dcompiler_47.dll)\n"); return 0; }
	ID3DBlob* code = 0;
	ID3DBlob* err = 0;
	const HRESULT hr = compile(src.c_str(), src.size(), 0, 0, 0, "main", profile, D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &code, &err);
	if (FAILED(hr)) DbgPrintF("[J2X] Direct3D 12 %s compile: %s\n", profile, err ? (const char*)err->GetBufferPointer() : "failed");
	if (err) err->Release();
	return SUCCEEDED(hr) ? code : 0;
}

// Root parameters: 0 the vertex constants (b0), 1 the fragment constants (b0), 2 and 3 texture units 0 and 1 (t0, t1), 4 and 5
// their samplers (s0, s1).
static ID3D12RootSignature* _makeRootSignature() {
	D3D12_DESCRIPTOR_RANGE ranges[4];
	memset(ranges, 0, sizeof(ranges));
	for (int i = 0; i < 4; ++i) {
		ranges[i].RangeType = i < 2 ? D3D12_DESCRIPTOR_RANGE_TYPE_SRV : D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
		ranges[i].NumDescriptors = 1;
		ranges[i].BaseShaderRegister = (UINT)(i & 1);
	}
	D3D12_ROOT_PARAMETER params[6];
	memset(params, 0, sizeof(params));
	params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	for (int i = 0; i < 4; ++i) {
		params[2 + i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[2 + i].DescriptorTable.NumDescriptorRanges = 1;
		params[2 + i].DescriptorTable.pDescriptorRanges = &ranges[i];
		params[2 + i].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	}
	D3D12_ROOT_SIGNATURE_DESC d;
	memset(&d, 0, sizeof(d));
	d.NumParameters = 6; d.pParameters = params;
	d.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	ID3DBlob* blob = 0;
	ID3DBlob* err = 0;
	if (FAILED(D3D12SerializeRootSignature(&d, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &err))) {
		DbgPrintF("[J2X] Direct3D 12 root signature: %s\n", err ? (const char*)err->GetBufferPointer() : "failed");
		_release(err);
		return 0;
	}
	_release(err);
	ID3D12RootSignature* root = 0;
	g_dev->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&root);
	blob->Release();
	return root;
}

// =========================================================================== samplers: every setting made once, in its slot
static UINT _samplerIndex(const Sampler& sm) {
	return (sm.wrapU ? 1u : 0u) | (sm.wrapV ? 2u : 0u) | ((unsigned)sm.mag << 2) | ((unsigned)sm.min << 3) | ((unsigned)(sm.mip + 1) << 4);
}
static void _makeSamplers() {
	for (UINT i = 0; i < kSamplers; ++i) {
		const bool minL = (i >> 3) & 1, magL = (i >> 2) & 1;
		const int mip = (int)(i >> 4) - 1;
		if (mip > kLinear) continue;
		const bool mipL = mip == kLinear;
		// Direct3D 12's filter is one enum for the three: MIN, MAG, MIP each POINT or LINEAR.
		static const D3D12_FILTER kFilter[8] = {
			D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR, D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
			D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR, D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT, D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
			D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D12_FILTER_MIN_MAG_MIP_LINEAR };
		D3D12_SAMPLER_DESC d;
		memset(&d, 0, sizeof(d));
		d.Filter = kFilter[(minL ? 4 : 0) | (magL ? 2 : 0) | (mipL ? 1 : 0)];
		d.AddressU = (i & 1) ? D3D12_TEXTURE_ADDRESS_MODE_WRAP : D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		d.AddressV = (i & 2) ? D3D12_TEXTURE_ADDRESS_MODE_WRAP : D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		d.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		d.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		d.MinLOD = 0;
		d.MaxLOD = mip < 0 ? 0.0f : D3D12_FLOAT32_MAX;   // (no mip filter: the top level only)
		g_dev->CreateSampler(&d, _cpu(g_sampHeap, g_sampStep, i));
	}
}

// =========================================================================== pipeline states, made once per setting
static const D3D12_BLEND kBlend[] = { D3D12_BLEND_ZERO, D3D12_BLEND_ONE, D3D12_BLEND_SRC_COLOR, D3D12_BLEND_INV_SRC_COLOR, D3D12_BLEND_SRC_ALPHA,
                                      D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_DEST_ALPHA, D3D12_BLEND_INV_DEST_ALPHA, D3D12_BLEND_DEST_COLOR,
                                      D3D12_BLEND_INV_DEST_COLOR, D3D12_BLEND_SRC_ALPHA_SAT };
// The alpha channel's factors: a colour factor has no place there in Direct3D 12, so it takes the matching alpha one (as GL does).
static D3D12_BLEND _alphaBlend(int b) {
	switch (b) {
		case kSrcColor: return D3D12_BLEND_SRC_ALPHA;
		case kInvSrcColor: return D3D12_BLEND_INV_SRC_ALPHA;
		case kDstColor: return D3D12_BLEND_DEST_ALPHA;
		case kInvDstColor: return D3D12_BLEND_INV_DEST_ALPHA;
		case kSrcAlphaSat: return D3D12_BLEND_ONE;
		default: return kBlend[b];
	}
}

typedef std::pair<unsigned long long, unsigned> PsoKey;
static std::map<PsoKey, ID3D12PipelineState*> s_psos;

static int _depthBias(const RenderState& rs) {   // back to depth-buffer steps (D24)
	return (int)(rs.depthBias * 16777216.0f + (rs.depthBias >= 0 ? 0.5f : -0.5f));
}
static ID3D12PipelineState* _pso(Prim prim, const RenderState& rs) {
	const int bias = _depthBias(rs);
	unsigned slopeBits;
	memcpy(&slopeBits, &rs.slopeBias, 4);
	const unsigned long long bits = (rs.blend ? 1ull : 0ull) | ((unsigned long long)rs.srcBlend << 1) | ((unsigned long long)rs.dstBlend << 5) |
	                                ((unsigned long long)(rs.colorWrite & 15) << 9) | (rs.depthTest ? 1ull << 13 : 0ull) |
	                                ((unsigned long long)rs.depthFunc << 14) | (rs.depthWrite ? 1ull << 17 : 0ull) |
	                                ((unsigned long long)rs.cull << 18) | (prim == kLines ? 1ull << 20 : 0ull) |
	                                ((unsigned long long)(unsigned)(bias & 0xFFFFFF) << 21);
	const PsoKey key(bits, slopeBits);
	std::map<PsoKey, ID3D12PipelineState*>::iterator it = s_psos.find(key);
	if (it != s_psos.end()) return it->second;

	static const D3D12_INPUT_ELEMENT_DESC layout[] = {   // gpu::Vertex
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_B8G8R8A8_UNORM,  0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },   // (ARGB words: B, G, R, A bytes)
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },   // the provoking normal
		{ "COLOR",    1, DXGI_FORMAT_B8G8R8A8_UNORM,  0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },   // the provoking colour
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d;
	memset(&d, 0, sizeof(d));
	d.pRootSignature = g_root;
	d.VS.pShaderBytecode = g_vsCode->GetBufferPointer(); d.VS.BytecodeLength = g_vsCode->GetBufferSize();
	d.PS.pShaderBytecode = g_psCode->GetBufferPointer(); d.PS.BytecodeLength = g_psCode->GetBufferSize();
	D3D12_RENDER_TARGET_BLEND_DESC& t = d.BlendState.RenderTarget[0];
	t.BlendEnable = rs.blend ? TRUE : FALSE;
	t.SrcBlend = kBlend[rs.srcBlend]; t.DestBlend = kBlend[rs.dstBlend]; t.BlendOp = D3D12_BLEND_OP_ADD;
	t.SrcBlendAlpha = _alphaBlend(rs.srcBlend); t.DestBlendAlpha = _alphaBlend(rs.dstBlend); t.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	t.LogicOp = D3D12_LOGIC_OP_NOOP;
	t.RenderTargetWriteMask = (UINT8)(((rs.colorWrite & 1) ? D3D12_COLOR_WRITE_ENABLE_RED : 0) | ((rs.colorWrite & 2) ? D3D12_COLOR_WRITE_ENABLE_GREEN : 0) |
	                                  ((rs.colorWrite & 4) ? D3D12_COLOR_WRITE_ENABLE_BLUE : 0) | ((rs.colorWrite & 8) ? D3D12_COLOR_WRITE_ENABLE_ALPHA : 0));
	d.SampleMask = 0xFFFFFFFFu;
	d.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	// The layer names the on-screen winding NOT drawn; Direct3D 12 names the face culled, front being clockwise.
	d.RasterizerState.CullMode = rs.cull == kCullCW ? D3D12_CULL_MODE_FRONT : rs.cull == kCullCCW ? D3D12_CULL_MODE_BACK : D3D12_CULL_MODE_NONE;
	d.RasterizerState.FrontCounterClockwise = FALSE;
	d.RasterizerState.DepthBias = bias;
	d.RasterizerState.SlopeScaledDepthBias = rs.slopeBias;
	d.RasterizerState.DepthClipEnable = TRUE;
	// GL: with the depth test off nothing is written to depth either.
	d.DepthStencilState.DepthEnable = rs.depthTest ? TRUE : FALSE;
	d.DepthStencilState.DepthWriteMask = rs.depthWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
	d.DepthStencilState.DepthFunc = (D3D12_COMPARISON_FUNC)(D3D12_COMPARISON_FUNC_NEVER + rs.depthFunc);   // (NEVER..ALWAYS, the layer's order)
	d.InputLayout.pInputElementDescs = layout;
	d.InputLayout.NumElements = 7;
	d.PrimitiveTopologyType = prim == kLines ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE : D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d.NumRenderTargets = 1;
	d.RTVFormats[0] = kColorFormat;
	d.DSVFormat = kDepthFormat;
	d.SampleDesc.Count = 1;
	ID3D12PipelineState* pso = 0;
	const HRESULT hr = g_dev->CreateGraphicsPipelineState(&d, __uuidof(ID3D12PipelineState), (void**)&pso);
	if (FAILED(hr)) DbgPrintF("[J2X] Direct3D 12: a pipeline state could not be made (0x%08x)\n", (unsigned)hr);
	s_psos[key] = pso;
	return pso;
}

// =========================================================================== the frame's targets
static void _makeTargets(int w, int h) {
	for (UINT i = 0; i < kFrames; ++i) {
		g_back[i] = 0;
		if (SUCCEEDED(g_swap->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&g_back[i])) && g_back[i])
			g_dev->CreateRenderTargetView(g_back[i], 0, _cpu(g_rtvHeap, g_rtvStep, i));
	}
	D3D12_HEAP_PROPERTIES heap;
	memset(&heap, 0, sizeof(heap));
	heap.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC d;
	memset(&d, 0, sizeof(d));
	d.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; d.Width = (UINT64)w; d.Height = (UINT)h; d.DepthOrArraySize = 1; d.MipLevels = 1;
	d.Format = kDepthFormat; d.SampleDesc.Count = 1; d.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	D3D12_CLEAR_VALUE clear;
	memset(&clear, 0, sizeof(clear));
	clear.Format = kDepthFormat;
	clear.DepthStencil.Depth = 1.0f;
	if (SUCCEEDED(g_dev->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &d, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear, __uuidof(ID3D12Resource), (void**)&g_depth)) && g_depth)
		g_dev->CreateDepthStencilView(g_depth, 0, _cpu(g_dsvHeap, 0, 0));
	g_w = w; g_h = h;
}
static void _dropTargets() {
	for (UINT i = 0; i < kFrames; ++i) _release(g_back[i]);
	_release(g_depth);
}
// The frame starts on the buffer the swap chain gives: its allocator, once the GPU is done with the frame last recorded in it.
static void _beginFrame() {
	g_frame = g_swap->GetCurrentBackBufferIndex() % kFrames;
	_wait(g_allocFence[g_frame]);
	g_alloc[g_frame]->Reset();
	g_upAlloc[g_frame]->Reset();
	_reopen(g_frame);
	_barrier(g_back[g_frame], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	_collect();
}

// =========================================================================== attach / detach
bool attachD3D12(ID3D12Device* device, ID3D12CommandQueue* queue, IDXGISwapChain3* swapChain, int w, int h) {
	detachD3D12();
	if (!device || !queue || !swapChain) return false;
	g_dev = device; g_queue = queue; g_swap = swapChain;
	g_dev->AddRef(); g_queue->AddRef(); g_swap->AddRef();
	if (FAILED(g_dev->QueryInterface(__uuidof(ID3D12InfoQueue), (void**)&g_info))) g_info = 0;   // (there with the debug layer only)
	if (g_info) DbgPrintF("[J2X] Direct3D 12: the debug layer is on\n");
	g_vsCode = _compile(std::string(kVSHead) + kGlVertexStage + kVSMain, "vs_5_0");
	g_psCode = _compile(std::string(kPSHead) + kGlFragmentStage + kPSMain, "ps_5_0");
	g_root = _makeRootSignature();
	bool ok = g_vsCode && g_psCode && g_root;
	for (UINT i = 0; i < kFrames; ++i) {
		g_allocFence[i] = 0;
		if (FAILED(g_dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&g_alloc[i]))) ok = false;
		if (FAILED(g_dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&g_upAlloc[i]))) ok = false;
	}
	if (ok && FAILED(g_dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_alloc[0], 0, __uuidof(ID3D12GraphicsCommandList), (void**)&g_list))) ok = false;
	if (g_list) { g_list->Close(); g_open = false; }
	if (ok && FAILED(g_dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_upAlloc[0], 0, __uuidof(ID3D12GraphicsCommandList), (void**)&g_upList))) ok = false;
	if (g_upList) { g_upList->Close(); g_upOpen = false; }
	if (FAILED(g_dev->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&g_fence))) ok = false;
	g_event = CreateEventA(0, FALSE, FALSE, 0);
	g_fenceValue = 0;

	D3D12_DESCRIPTOR_HEAP_DESC hd;
	memset(&hd, 0, sizeof(hd));
	hd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; hd.NumDescriptors = kFrames;
	if (FAILED(g_dev->CreateDescriptorHeap(&hd, __uuidof(ID3D12DescriptorHeap), (void**)&g_rtvHeap))) ok = false;
	hd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; hd.NumDescriptors = 1;
	if (FAILED(g_dev->CreateDescriptorHeap(&hd, __uuidof(ID3D12DescriptorHeap), (void**)&g_dsvHeap))) ok = false;
	hd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; hd.NumDescriptors = kSrvSlots; hd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	if (FAILED(g_dev->CreateDescriptorHeap(&hd, __uuidof(ID3D12DescriptorHeap), (void**)&g_srvHeap))) ok = false;
	hd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER; hd.NumDescriptors = kSamplers;
	if (FAILED(g_dev->CreateDescriptorHeap(&hd, __uuidof(ID3D12DescriptorHeap), (void**)&g_sampHeap))) ok = false;
	if (!ok) { DbgPrintF("[J2X] Direct3D 12: setup FAILED (see above)\n"); return false; }
	g_rtvStep = g_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	g_srvStep = g_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	g_sampStep = g_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	D3D12_SHADER_RESOURCE_VIEW_DESC nullView;   // slot 0: an empty view, which samples as 0 (what an unused unit reads, as Direct3D 11's)
	memset(&nullView, 0, sizeof(nullView));
	nullView.Format = kColorFormat;
	nullView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	nullView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	nullView.Texture2D.MipLevels = 1;
	g_dev->CreateShaderResourceView(0, &nullView, _cpu(g_srvHeap, g_srvStep, 0));
	g_freeSlots.clear();
	g_nextSlot = 1;
	_makeSamplers();

	D3D12_HEAP_PROPERTIES heap;
	memset(&heap, 0, sizeof(heap));
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC rd;
	memset(&rd, 0, sizeof(rd));
	rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; rd.Width = kRingBytes; rd.Height = 1; rd.DepthOrArraySize = 1; rd.MipLevels = 1;
	rd.SampleDesc.Count = 1; rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	void* cpu = 0;
	D3D12_RANGE none = { 0, 0 };
	if (FAILED(g_dev->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, 0, __uuidof(ID3D12Resource), (void**)&g_ring)) ||
	    !g_ring || FAILED(g_ring->Map(0, &none, &cpu))) {
		DbgPrintF("[J2X] Direct3D 12: the upload ring could not be made\n");
		return false;
	}
	g_ringCpu = (unsigned char*)cpu;   // (an upload heap stays mapped)
	g_ringGpu = g_ring->GetGPUVirtualAddress();
	g_head = g_tail = 0;
	g_inflight.clear();

	_makeTargets(w, h);
	_beginFrame();
	DbgPrintF("[J2X] Direct3D 12: shaders, root signature and buffers ready\n");
	return g_back[g_frame] && g_depth;
}

void detachD3D12() {
	if (g_queue && g_fence) { _wait(_submit()); }
	if (g_ring) g_ring->Unmap(0, 0);
	g_ringCpu = 0;
	_release(g_ring);
	for (std::map<PsoKey, ID3D12PipelineState*>::iterator i = s_psos.begin(); i != s_psos.end(); ++i) if (i->second) i->second->Release();
	s_psos.clear();
	for (size_t i = 0; i < g_doomed.size(); ++i) if (g_doomed[i].obj) g_doomed[i].obj->Release();
	g_doomed.clear();
	for (size_t i = 0; i < g_pages.size(); ++i) { g_pages[i]->res->Release(); delete g_pages[i]; }
	g_pages.clear();
	g_inflight.clear();
	_dropTargets();
	_release(g_rtvHeap); _release(g_dsvHeap); _release(g_srvHeap); _release(g_sampHeap);
	_release(g_root); _release(g_vsCode); _release(g_psCode);
	_release(g_list);
	g_open = false;
	_release(g_upList);
	g_upOpen = false;
	for (UINT i = 0; i < kFrames; ++i) { _release(g_alloc[i]); _release(g_upAlloc[i]); }
	_release(g_fence);
	if (g_event) { CloseHandle(g_event); g_event = 0; }
	_debugMessages();
	_release(g_info);
	_release(g_swap); _release(g_queue); _release(g_dev);
}

long presentD3D12(unsigned syncInterval) {
	if (!g_swap || !g_list || !g_open) return E_FAIL;
	_barrier(g_back[g_frame], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	g_allocFence[g_frame] = _submit();
	const HRESULT hr = g_swap->Present(syncInterval, 0);
	_beginFrame();
	_debugMessages();
	return hr;
}

bool resizeD3D12(int w, int h) {
	if (!g_swap || w < 1 || h < 1) return false;
	_wait(_submit());   // (the frame just begun: only its buffer's move to a render target is in it; the buffers are dropped next)
	for (UINT i = 0; i < kFrames; ++i) g_allocFence[i] = 0;
	g_inflight.clear();
	g_head = g_tail = 0;
	_dropTargets();
	const HRESULT hr = g_swap->ResizeBuffers(kFrames, (UINT)w, (UINT)h, DXGI_FORMAT_UNKNOWN, 0);
	if (FAILED(hr)) DbgPrintF("[J2X] Direct3D 12: ResizeBuffers failed: 0x%08x\n", (unsigned)hr);
	DXGI_SWAP_CHAIN_DESC sd;
	if (SUCCEEDED(g_swap->GetDesc(&sd))) { w = (int)sd.BufferDesc.Width; h = (int)sd.BufferDesc.Height; }
	_makeTargets(w, h);
	_beginFrame();
	return SUCCEEDED(hr) && g_back[g_frame] && g_depth;
}

// =========================================================================== what is on the list
static ID3D12PipelineState* s_pso = 0;
static D3D12_PRIMITIVE_TOPOLOGY s_topo = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
static D3D12_VIEWPORT s_vp;
static D3D12_RECT s_sc;
static float s_vsc[kVSConsts * 4], s_psc[kPSConsts * 4];
static D3D12_GPU_VIRTUAL_ADDRESS s_vsAddr = 0, s_psAddr = 0;
static UINT s_slot[2], s_samp[2];
static D3D12_GPU_VIRTUAL_ADDRESS s_vb = 0;

bool ready() { return g_dev && g_list && g_open && g_root && g_vsCode && g_psCode && g_ring && g_back[g_frame] && g_depth; }
const char* name() { return "Direct3D 12"; }
int targetWidth() { return g_w; }
int targetHeight() { return g_h; }
bool halfPixelOffset() { return false; }   // (Direct3D 10 and later centre pixels as GL does)
void forgetState() { s_known = false; }

// The constants a draw reads, in the ring when they changed: the VS block (592 bytes) at `at`, the PS block 768 bytes on (each
// constant buffer view starts on 256 bytes).
static const UINT64 kVSBlock = (kVSConsts * 16 + 255) & ~255;
static const UINT64 kConstBytes = kVSBlock + 256;
static void _constants(const Constants& c, UINT64 at) {
	if (!s_known || memcmp(s_vsc, c.vs, sizeof(s_vsc)) != 0) {
		memcpy(g_ringCpu + at, c.vs, sizeof(s_vsc));
		memcpy(s_vsc, c.vs, sizeof(s_vsc));
		s_vsAddr = g_ringGpu + at;
		g_list->SetGraphicsRootConstantBufferView(0, s_vsAddr);
	}
	if (!s_known || memcmp(s_psc, c.ps, sizeof(s_psc)) != 0) {
		memcpy(g_ringCpu + at + kVSBlock, c.ps, sizeof(s_psc));
		memcpy(s_psc, c.ps, sizeof(s_psc));
		s_psAddr = g_ringGpu + at + kVSBlock;
		g_list->SetGraphicsRootConstantBufferView(1, s_psAddr);
	}
}

// Everything a draw needs set on the list. `constAt`: kConstBytes of ring for the constants (taken before, as taking ring space
// may run the list so far).
static void _apply(Prim prim, const RenderState& rs, const Constants& c, const Bindings& b, UINT64 constAt) {
	if (!s_known) {
		g_list->SetGraphicsRootSignature(g_root);
		const D3D12_CPU_DESCRIPTOR_HANDLE rtv = _cpu(g_rtvHeap, g_rtvStep, g_frame), dsv = _cpu(g_dsvHeap, 0, 0);
		g_list->OMSetRenderTargets(1, &rtv, FALSE, &dsv);
		s_pso = 0; s_topo = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED; s_vb = 0;
		s_slot[0] = s_slot[1] = s_samp[0] = s_samp[1] = ~0u;
		memset(&s_vp, 0, sizeof(s_vp));
		memset(&s_sc, 0, sizeof(s_sc));
	}
	ID3D12PipelineState* pso = _pso(prim, rs);
	if (pso != s_pso) { g_list->SetPipelineState(pso); s_pso = pso; }
	const D3D12_PRIMITIVE_TOPOLOGY topo = prim == kLines ? D3D_PRIMITIVE_TOPOLOGY_LINELIST : D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	if (topo != s_topo) { g_list->IASetPrimitiveTopology(topo); s_topo = topo; }
	D3D12_VIEWPORT vp;
	vp.TopLeftX = (float)rs.vpX; vp.TopLeftY = (float)rs.vpY; vp.Width = (float)rs.vpW; vp.Height = (float)rs.vpH; vp.MinDepth = 0.0f; vp.MaxDepth = 1.0f;
	if (!s_known || memcmp(&vp, &s_vp, sizeof(vp)) != 0) { g_list->RSSetViewports(1, &vp); s_vp = vp; }
	D3D12_RECT sc;   // (Direct3D 12 always scissors: with GL's test off, the whole target)
	if (rs.scissor) { sc.left = rs.scX; sc.top = rs.scY; sc.right = rs.scX + rs.scW; sc.bottom = rs.scY + rs.scH; }
	else { sc.left = 0; sc.top = 0; sc.right = g_w; sc.bottom = g_h; }
	if (!s_known || memcmp(&sc, &s_sc, sizeof(sc)) != 0) { g_list->RSSetScissorRects(1, &sc); s_sc = sc; }
	_constants(c, constAt);
	for (int u = 0; u < 2; ++u) {
		Texture t = b.tex[u];
		if (t) _textureState(t, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		const UINT slot = t ? t->slot : 0;
		if (slot != s_slot[u]) { g_list->SetGraphicsRootDescriptorTable(2 + u, _gpu(g_srvHeap, g_srvStep, slot)); s_slot[u] = slot; }
		const UINT samp = _samplerIndex(b.samp[u]);
		if (samp != s_samp[u]) { g_list->SetGraphicsRootDescriptorTable(4 + u, _gpu(g_sampHeap, g_sampStep, samp)); s_samp[u] = samp; }
	}
	s_known = true;
}
static void _bindVB(D3D12_GPU_VIRTUAL_ADDRESS at, UINT bytes) {
	D3D12_VERTEX_BUFFER_VIEW v;
	v.BufferLocation = at; v.SizeInBytes = bytes; v.StrideInBytes = sizeof(Vertex);
	g_list->IASetVertexBuffers(0, 1, &v);
	s_vb = at;
}

// =========================================================================== drawing
void draw(Prim prim, const Vertex* v, int count, const RenderState& rs, const Constants& c, const Bindings& b) {
	if (!ready() || count <= 0) return;
	const UINT bytes = (UINT)count * sizeof(Vertex);
	const UINT64 vbBytes = ((UINT64)bytes + 255) & ~255ull;
	UINT64 at = 0;
	if (vbBytes + kConstBytes <= kRingBytes / 2) {
		if (!_ringAlloc(vbBytes + kConstBytes, 256, at)) return;
		memcpy(g_ringCpu + at + kConstBytes, v, bytes);
		_apply(prim, rs, c, b, at);
		_bindVB(g_ringGpu + at + kConstBytes, bytes);
	} else {   // bigger than the ring holds well: a buffer of its own for this draw
		ID3D12Resource* once = _uploadBuffer(v, bytes);
		if (!once || !_ringAlloc(kConstBytes, 256, at)) { if (once) once->Release(); return; }
		_apply(prim, rs, c, b, at);
		_bindVB(once->GetGPUVirtualAddress(), bytes);
		_retire(once, 0);
	}
	g_list->DrawInstanced((UINT)count, 1, 0, 0);
}

static Page* _newPage(UINT64 bytes) {
	D3D12_HEAP_PROPERTIES heap;
	memset(&heap, 0, sizeof(heap));
	heap.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC d;
	memset(&d, 0, sizeof(d));
	d.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; d.Width = bytes; d.Height = 1; d.DepthOrArraySize = 1; d.MipLevels = 1;
	d.SampleDesc.Count = 1; d.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	ID3D12Resource* res = 0;
	if (FAILED(g_dev->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &d, D3D12_RESOURCE_STATE_COMMON, 0, __uuidof(ID3D12Resource), (void**)&res)) || !res) {
		DbgPrintF("[J2X] Direct3D 12: a %u KB buffer page could not be made\n", (unsigned)(bytes >> 10));
		return 0;
	}
	Page* pg = new Page;
	pg->res = res; pg->va = res->GetGPUVirtualAddress(); pg->bytes = bytes;
	pg->free[0] = bytes;
	g_pages.push_back(pg);
	return pg;
}
// `size` bytes of some page (the first free range that holds it), a new page when none has room.
static bool _pageAlloc(UINT64 size, Page*& page, UINT64& at) {
	for (size_t i = 0; i < g_pages.size(); ++i) {
		Page* pg = g_pages[i];
		for (std::map<UINT64, UINT64>::iterator f = pg->free.begin(); f != pg->free.end(); ++f) {
			if (f->second < size) continue;
			at = f->first;
			const UINT64 rest = f->second - size;
			pg->free.erase(f);
			if (rest) pg->free[at + size] = rest;
			page = pg;
			return true;
		}
	}
	Page* pg = _newPage(size > kPageBytes ? size : kPageBytes);
	if (!pg) return false;
	at = 0;
	pg->free.clear();
	if (pg->bytes > size) pg->free[size] = pg->bytes - size;
	page = pg;
	return true;
}

Buffer createBuffer(const Vertex* v, int count) {
	if (!ready() || count <= 0) return 0;
	const UINT bytes = (UINT)count * sizeof(Vertex);
	const UINT64 size = ((UINT64)bytes + kListAlign - 1) & ~(kListAlign - 1);
	Page* pg = 0;
	UINT64 offset = 0;
	if (!_pageAlloc(size, pg, offset)) return 0;
	UINT64 at = 0;
	ID3D12Resource* from = g_ring;
	if (bytes <= kRingBytes / 2 && _ringAlloc(bytes, 16, at)) {
		memcpy(g_ringCpu + at, v, bytes);
	} else {
		from = _uploadBuffer(v, bytes);
		if (!from) { _pageFree(pg, offset, size); return 0; }
		at = 0;
		_retire(from, 0);
	}
	_uploads()->CopyBufferRegion(pg->res, offset, from, at, bytes);   // (runs before this frame's draws: see _submit)
	BufferObject* o = new BufferObject;
	o->page = pg; o->offset = offset; o->size = size; o->bytes = bytes;
	return o;
}
void releaseBuffer(Buffer buf) {
	if (!buf) return;
	if (buf->page) {
		if (s_vb == buf->page->va + buf->offset) s_vb = 0;
		Doomed d;
		d.fence = g_fenceValue + 1; d.obj = 0; d.slot = 0; d.page = buf->page; d.offset = buf->offset; d.size = buf->size;
		g_doomed.push_back(d);
	}
	delete buf;
}
void drawBuffer(Prim prim, Buffer buf, int count, const RenderState& rs, const Constants& c, const Bindings& b) {
	if (!ready() || !buf || !buf->page || count <= 0) return;
	UINT64 at = 0;
	if (!_ringAlloc(kConstBytes, 256, at)) return;
	_apply(prim, rs, c, b, at);
	const D3D12_GPU_VIRTUAL_ADDRESS va = buf->page->va + buf->offset;
	if (s_vb != va) _bindVB(va, buf->bytes);
	g_list->DrawInstanced((UINT)count, 1, 0, 0);
}
void beginUploads() {}   // (uploads are recorded in the frame's list, in order with the draws)
void endUploads() {}

// GL clears the whole target where the masks let it (decided by the layer), inside the scissor box when that test is on.
void clear(bool color, bool depth, bool stencil, float r, float g, float b, float a, float z, const RenderState& rs) {
	if (!ready()) return;
	D3D12_RECT box;
	UINT rects = 0;
	if (rs.scissor) {
		box.left = rs.scX < 0 ? 0 : rs.scX; box.top = rs.scY < 0 ? 0 : rs.scY;
		box.right = rs.scX + rs.scW > g_w ? g_w : rs.scX + rs.scW; box.bottom = rs.scY + rs.scH > g_h ? g_h : rs.scY + rs.scH;
		if (box.right <= box.left || box.bottom <= box.top) return;
		rects = 1;
	}
	if (color) { const float c[4] = { r, g, b, a }; g_list->ClearRenderTargetView(_cpu(g_rtvHeap, g_rtvStep, g_frame), c, rects, rects ? &box : 0); }
	if (depth || stencil) {
		const D3D12_CLEAR_FLAGS flags = (D3D12_CLEAR_FLAGS)((depth ? D3D12_CLEAR_FLAG_DEPTH : 0) | (stencil ? D3D12_CLEAR_FLAG_STENCIL : 0));
		g_list->ClearDepthStencilView(_cpu(g_dsvHeap, 0, 0), flags, z, 0, rects, rects ? &box : 0);
	}
}

// =========================================================================== textures
Texture createTexture(int w, int h, int levels) {
	if (!ready() || w <= 0 || h <= 0) return 0;
	UINT slot = 0;
	if (!g_freeSlots.empty()) { slot = g_freeSlots.back(); g_freeSlots.pop_back(); }
	else if (g_nextSlot < kSrvSlots) slot = g_nextSlot++;
	else { J2X_UNHANDLED("more textures alive at once than the Direct3D 12 backend's view heap holds", (int)kSrvSlots); return 0; }
	D3D12_HEAP_PROPERTIES heap;
	memset(&heap, 0, sizeof(heap));
	heap.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC d;
	memset(&d, 0, sizeof(d));
	d.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; d.Width = (UINT64)w; d.Height = (UINT)h; d.DepthOrArraySize = 1; d.MipLevels = (UINT16)levels;
	d.Format = kColorFormat;   // (the layer's ARGB words, as they lie in memory)
	d.SampleDesc.Count = 1;
	ID3D12Resource* res = 0;
	if (FAILED(g_dev->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &d, D3D12_RESOURCE_STATE_COPY_DEST, 0, __uuidof(ID3D12Resource), (void**)&res)) || !res) {
		DbgPrintF("[J2X] Direct3D 12: a %dx%d texture could not be made\n", w, h);
		g_freeSlots.push_back(slot);
		return 0;
	}
	g_dev->CreateShaderResourceView(res, 0, _cpu(g_srvHeap, g_srvStep, slot));
	TextureObject* t = new TextureObject;
	t->res = res; t->state = D3D12_RESOURCE_STATE_COPY_DEST; t->slot = slot; t->w = w; t->h = h; t->levels = levels;
	return t;
}
void releaseTexture(Texture t) {
	if (!t) return;
	for (int u = 0; u < 2; ++u) if (s_slot[u] == t->slot) s_slot[u] = ~0u;
	if (t->res) _retire(t->res, t->slot);
	delete t;
}
bool writeTexture(Texture t, int level, int x, int y, int w, int h, const unsigned int* argb) {
	if (!t || !t->res || !ready() || w <= 0 || h <= 0) return false;
	const UINT pitch = ((UINT)w * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1) & ~(UINT)(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1);
	UINT64 at = 0;
	if (!_ringAlloc((UINT64)pitch * h, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT, at)) {
		J2X_UNHANDLED("a texture upload bigger than the Direct3D 12 backend's upload ring", w * h);
		return false;
	}
	for (int r = 0; r < h; ++r) memcpy(g_ringCpu + at + (size_t)r * pitch, argb + (size_t)r * w, (size_t)w * 4);
	_textureState(t, D3D12_RESOURCE_STATE_COPY_DEST);
	D3D12_TEXTURE_COPY_LOCATION dst, src;
	memset(&dst, 0, sizeof(dst));
	memset(&src, 0, sizeof(src));
	dst.pResource = t->res; dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX; dst.SubresourceIndex = (UINT)level;
	src.pResource = g_ring; src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint.Offset = at;
	src.PlacedFootprint.Footprint.Format = kColorFormat;
	src.PlacedFootprint.Footprint.Width = (UINT)w; src.PlacedFootprint.Footprint.Height = (UINT)h; src.PlacedFootprint.Footprint.Depth = 1;
	src.PlacedFootprint.Footprint.RowPitch = pitch;
	g_list->CopyTextureRegion(&dst, (UINT)x, (UINT)y, 0, &src, 0);
	return true;
}
// A copy of one subresource (a texture level, or the frame) into a buffer the CPU reads, after the GPU has run up to it.
static bool _readBack(ID3D12Resource* src, UINT subresource, std::vector<unsigned int>& argb, int& w, int& h) {
	D3D12_RESOURCE_DESC desc = src->GetDesc();
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT fp;
	UINT rows = 0;
	UINT64 rowBytes = 0, total = 0;
	g_dev->GetCopyableFootprints(&desc, subresource, 1, 0, &fp, &rows, &rowBytes, &total);
	D3D12_HEAP_PROPERTIES heap;
	memset(&heap, 0, sizeof(heap));
	heap.Type = D3D12_HEAP_TYPE_READBACK;
	D3D12_RESOURCE_DESC d;
	memset(&d, 0, sizeof(d));
	d.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; d.Width = total; d.Height = 1; d.DepthOrArraySize = 1; d.MipLevels = 1;
	d.SampleDesc.Count = 1; d.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	ID3D12Resource* rb = 0;
	if (FAILED(g_dev->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &d, D3D12_RESOURCE_STATE_COPY_DEST, 0, __uuidof(ID3D12Resource), (void**)&rb)) || !rb) return false;
	D3D12_TEXTURE_COPY_LOCATION to, from;
	memset(&to, 0, sizeof(to));
	memset(&from, 0, sizeof(from));
	to.pResource = rb; to.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT; to.PlacedFootprint = fp;
	from.pResource = src; from.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX; from.SubresourceIndex = subresource;
	g_list->CopyTextureRegion(&to, 0, 0, 0, &from, 0);
	_flush();
	w = (int)fp.Footprint.Width;
	h = (int)fp.Footprint.Height;
	void* p = 0;
	D3D12_RANGE all = { 0, (SIZE_T)total };
	bool ok = false;
	if (SUCCEEDED(rb->Map(0, &all, &p))) {
		argb.resize((size_t)w * h);
		for (int r = 0; r < h; ++r) memcpy(&argb[(size_t)r * w], (const char*)p + fp.Offset + (size_t)r * fp.Footprint.RowPitch, (size_t)w * 4);
		D3D12_RANGE none = { 0, 0 };
		rb->Unmap(0, &none);
		ok = true;
	}
	rb->Release();
	return ok;
}
bool readTexture(Texture t, int level, std::vector<unsigned int>& argb, int& w, int& h) {
	if (!t || !t->res || !ready()) return false;
	_textureState(t, D3D12_RESOURCE_STATE_COPY_SOURCE);
	return _readBack(t->res, (UINT)level, argb, w, h);
}

// =========================================================================== the frame
bool readFrame(std::vector<unsigned int>& argb, int& w, int& h) {
	if (!ready()) return false;
	ID3D12Resource* back = g_back[g_frame];
	_barrier(back, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE);
	// (_readBack runs the list; the buffer goes back to a render target in the list that follows)
	const bool ok = _readBack(back, 0, argb, w, h);
	_barrier(back, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	if (!ok) return false;
	for (size_t i = 0; i < argb.size(); ++i) argb[i] |= 0xFF000000u;   // (the frame is opaque, as GL's default framebuffer reads)
	return true;
}

}  // namespace gpu
}  // namespace j2xgl
