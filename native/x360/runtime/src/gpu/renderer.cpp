// Direct3D 12 renderer (see renderer.h). Register semantics: analysis/xenos/01a (VGT, PA, viewport/scissor §5),
// 01b (RB, constants), 04 (vertex fetch), 05a (texture fetch constants), 06a/06b (EDRAM). Code is our own; Xenia
// (BSD-3) was read for behaviour where noted.

#include "gpu/renderer.h"

#include "gpu/d3d12_device.h"
#include "gpu/draw_trace.h"
#include "gpu/gpu.h"
#include "gpu/output_merger.h"
#include "gpu/present.h"
#include "gpu/presenter.h"
#include "gpu/resolve.h"
#include "gpu/host_targets_hlsl.h"
#include "gpu/shader_cache.h"
#include "gpu/shader_translator.h"
#include "gpu/texture.h"
#include "log.h"
#include "xui/system_ui.h"
#include "memory.h"

#include <wrl/client.h>

#include <algorithm>
#include <array>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <format>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

using Microsoft::WRL::ComPtr;

namespace gpu
{
    namespace
    {
        uint32_t Reg(uint32_t index) { return ReadRegister(index); }
        float RegFloat(uint32_t index) { return std::bit_cast<float>(ReadRegister(index)); }
        int32_t SignExtend(uint32_t value, uint32_t bits)
        {
            const uint32_t shift = 32 - bits;
            return int32_t(value << shift) >> shift;
        }

        const bool g_disabled = [] { const char* v = std::getenv("CB_NO_RENDER"); return v && v[0] == '1'; }();
        // CB_HOST_1X=1 (experiment): host targets of multisampled guest surfaces are single-sampled (every guest sample
        // stores the pixel's value), to tell sample-conversion problems from the rest.
        const bool g_hostSingleSample = [] { const char* v = std::getenv("CB_HOST_1X"); return v && v[0] == '1'; }();
        bool HostMs(uint32_t msaa) { return msaa != 0 && !g_hostSingleSample; }
        // CB_LOCAL_MEMORY=1 (gpu.h LocalMemory): pages of guest physical memory written since the GPU's copy was last
        // brought up to date, one bit per 4 KiB page; everything is written at the start.
        const bool g_localMemory = [] { const char* v = std::getenv("CB_LOCAL_MEMORY"); return v && v[0] == '1'; }();
        constexpr uint32_t kMemoryPageShift = 12, kMemoryPageWords = (guest::kPhysicalSize >> kMemoryPageShift) / 64;
        std::atomic<uint64_t> g_memoryWritten[kMemoryPageWords];
        std::atomic<bool> g_memoryAnyWritten{ false };
        // CB_EDRAM_DUMP_AT=<frame>:<draw>;...: the EDRAM (host targets stored to it first) as edram_frame<frame*10000+draw>.bin
        // just before that draw, to compare the output mergers draw by draw.
        const std::vector<std::pair<uint64_t, uint32_t>> g_edramDumpAt = [] {
            std::vector<std::pair<uint64_t, uint32_t>> at;
            const char* v = std::getenv("CB_EDRAM_DUMP_AT");
            for (std::string_view rest = v ? v : ""; !rest.empty();)
            {
                const size_t end = rest.find(';');
                const std::string entry(rest.substr(0, end));
                unsigned long long frame = 0;
                unsigned draw = 0;
                if (std::sscanf(entry.c_str(), "%llu:%u", &frame, &draw) == 2)
                    at.emplace_back(frame, draw);
                rest = end == std::string_view::npos ? std::string_view() : rest.substr(end + 1);
            }
            return at;
        }();
        // CB_HOST_LOG=<frame>: that frame's draws say which output merger they took and why, and host targets log their
        // loads and stores.
        const uint64_t g_hostLogFrame = [] { const char* v = std::getenv("CB_HOST_LOG"); return v ? std::strtoull(v, nullptr, 10) : 0ull; }();
        // CB_TEXTURE_DUMP=1: every untiled texture; =<frame> (2 or more): only those untiled while that frame is built.
        const uint64_t g_dumpTexturesFrame = [] { const char* v = std::getenv("CB_TEXTURE_DUMP"); return v ? std::strtoull(v, nullptr, 10) : 0ull; }();
        const bool g_dumpTextures = g_dumpTexturesFrame != 0;
        const bool g_magentaTextures = [] { const char* v = std::getenv("CB_TEXTURE_MAGENTA"); return v && v[0] == '1'; }();
        const uint64_t g_dumpFrame = [] {
            const char* v = std::getenv("CB_EDRAM_DUMP");
            return v ? std::strtoull(v, nullptr, 10) : 0ull;
        }();
        // CB_SKIP_DRAWS=<frame>:<first>-<last>;...: in frame <frame> (as CB_GPU_TRACE and CB_PRESENT_DUMP number them),
        // the draws <first>..<last> (the trace's draw numbers) are not drawn; resolves still run. Consecutive frames with
        // different ranges bisect one scene in a single run.
        struct SkipDraws
        {
            uint64_t frame;
            uint32_t first, last;
        };
        const std::vector<SkipDraws> g_skipDraws = [] {
            std::vector<SkipDraws> skips;
            const char* v = std::getenv("CB_SKIP_DRAWS");
            for (std::string_view rest = v ? v : ""; !rest.empty();)
            {
                const size_t end = rest.find(';');
                const std::string entry(rest.substr(0, end));
                unsigned long long frame = 0;
                unsigned first = 0, last = 0;
                if (std::sscanf(entry.c_str(), "%llu:%u-%u", &frame, &first, &last) == 3)
                    skips.push_back({ frame, first, last });
                rest = end == std::string_view::npos ? std::string_view() : rest.substr(end + 1);
            }
            return skips;
        }();
        const uint64_t g_frontDumpFrame = [] {
            const char* v = std::getenv("CB_FRONT_DUMP");
            return v ? std::strtoull(v, nullptr, 10) : 0ull;
        }();
        // CB_PRESENT_DUMP: frames separated by commas; "*<n>" means every n-th frame.
        bool IsPresentDumpFrame(uint64_t frame)
        {
            static const std::vector<std::pair<bool, uint64_t>> entries = [] {
                std::vector<std::pair<bool, uint64_t>> list;
                const char* v = std::getenv("CB_PRESENT_DUMP");
                for (const char* p = v; p && *p;)
                {
                    const bool every = *p == '*';
                    char* end = nullptr;
                    const uint64_t n = std::strtoull(p + (every ? 1 : 0), &end, 10);
                    if (n != 0)
                        list.emplace_back(every, n);
                    p = *end == ',' ? end + 1 : end;
                    if (end == p && *p != 0)
                        break;
                }
                return list;
            }();
            for (const auto& [every, n] : entries)
                if (every ? frame % n == 0 : frame == n)
                    return true;
            return false;
        }

        // CB_GPU_STATS: GPU waits by reason (calls, submissions, time blocked).
        struct SyncStat
        {
            uint64_t calls = 0, submits = 0, waitNs = 0;
        };
        SyncStat g_syncStats[size_t(SyncReason::kCount)];

        // Each distinct unimplemented case is logged once.
        void LogOnce(const std::string& text)
        {
            static std::set<std::string> seen;
            if (seen.insert(text).second)
                Log("GPU: {}", text);
        }

        constexpr uint32_t kTableSize = 16;            // texture / sampler descriptors per stage
        constexpr UINT kSrvHeapSize = 65536;
        constexpr UINT kSamplerHeapSize = 2048;
        // Slot 0 guest memory (SRV), 1 EDRAM (UAV), 2 guest memory (UAV), 3-5 guest-output textures (UAV, presenter.h),
        // 6 the system UI overlay (SRV, present pass).
        constexpr UINT kGuestOutputUavs = 3;
        constexpr UINT kOverlaySrv = 6;
        constexpr UINT kFixedSrvs = 7;
        constexpr uint64_t kUploadPage = 16 * 1024 * 1024;
        constexpr uint32_t kColorInfo[4] = { 0x2001, 0x2003, 0x2004, 0x2005 };
        constexpr uint32_t kBlendControl[4] = { 0x2201, 0x2209, 0x220A, 0x220B };

        enum RootParameter : UINT
        {
            kRootSystem, kRootFloat, kRootBoolLoop, kRootFetch, kRootOutput,
            kRootMemory, kRootEdram, kRootVsTextures, kRootPsTextures, kRootVsSamplers, kRootPsSamplers,
            kRootCount
        };

        // Rectangle lists: three vertices per rectangle; the fourth is the third corner mirrored across the longest
        // edge (the diagonal), for the position and every interpolant (Xenia's geometry shader; 01b §6.1).
        constexpr char kRectListGeometryShader[] = R"(
struct XeVertex
{
    float4 position : SV_Position;
    float4 o0 : TEXCOORD0; float4 o1 : TEXCOORD1; float4 o2 : TEXCOORD2; float4 o3 : TEXCOORD3;
    float4 o4 : TEXCOORD4; float4 o5 : TEXCOORD5; float4 o6 : TEXCOORD6; float4 o7 : TEXCOORD7;
    float4 o8 : TEXCOORD8; float4 o9 : TEXCOORD9; float4 o10 : TEXCOORD10; float4 o11 : TEXCOORD11;
    float4 o12 : TEXCOORD12; float4 o13 : TEXCOORD13; float4 o14 : TEXCOORD14; float4 o15 : TEXCOORD15;
    float4 clip0 : SV_ClipDistance0;
    float2 clip1 : SV_ClipDistance1;
};

[maxvertexcount(4)]
void main(triangle XeVertex v[3], inout TriangleStream<XeVertex> stream)
{
    float2 e12 = v[2].position.xy - v[1].position.xy;
    float2 e20 = v[0].position.xy - v[2].position.xy;
    float2 e01 = v[1].position.xy - v[0].position.xy;
    float l12 = dot(e12, e12), l20 = dot(e20, e20), l01 = dot(e01, e01);
    uint a, b, c;
    if (l20 < l12 && l01 < l12) { a = 0u; b = 1u; c = 2u; }
    else if (l01 < l20) { a = 1u; b = 2u; c = 0u; }
    else { a = 2u; b = 0u; c = 1u; }
    stream.Append(v[a]);
    stream.Append(v[b]);
    stream.Append(v[c]);
    XeVertex d;
#define XE_MIRROR(f) d.f = (v[b].f - v[a].f) + v[c].f
    XE_MIRROR(position);
    XE_MIRROR(o0); XE_MIRROR(o1); XE_MIRROR(o2); XE_MIRROR(o3); XE_MIRROR(o4); XE_MIRROR(o5); XE_MIRROR(o6);
    XE_MIRROR(o7); XE_MIRROR(o8); XE_MIRROR(o9); XE_MIRROR(o10); XE_MIRROR(o11); XE_MIRROR(o12); XE_MIRROR(o13);
    XE_MIRROR(o14); XE_MIRROR(o15); XE_MIRROR(clip0); XE_MIRROR(clip1);
    stream.Append(d);
}
)";

        // Point lists: each point becomes a screen-aligned square sprite of PA_SU_POINT_SIZE (no per-vertex size:
        // VS export mode 0), with the same interpolants at every corner and the sprite coordinates (0,0 top left to
        // 1,1 bottom right) before them for PsParamGen. Points of size 0 draw nothing (Xenia: point lists with size
        // 0 are only used for their vertex shader).
        // Compiled after kXenosHlslLibrary (system constants).
        constexpr char kPointListGeometryShader[] = R"(
struct XeVertex
{
    float4 position : SV_Position;
    float4 o0 : TEXCOORD0; float4 o1 : TEXCOORD1; float4 o2 : TEXCOORD2; float4 o3 : TEXCOORD3;
    float4 o4 : TEXCOORD4; float4 o5 : TEXCOORD5; float4 o6 : TEXCOORD6; float4 o7 : TEXCOORD7;
    float4 o8 : TEXCOORD8; float4 o9 : TEXCOORD9; float4 o10 : TEXCOORD10; float4 o11 : TEXCOORD11;
    float4 o12 : TEXCOORD12; float4 o13 : TEXCOORD13; float4 o14 : TEXCOORD14; float4 o15 : TEXCOORD15;
    float4 clip0 : SV_ClipDistance0;
    float2 clip1 : SV_ClipDistance1;
};

struct XeSpriteVertex
{
    float4 position : SV_Position;
    float4 pointCoord : TEXCOORD16;
    float4 o0 : TEXCOORD0; float4 o1 : TEXCOORD1; float4 o2 : TEXCOORD2; float4 o3 : TEXCOORD3;
    float4 o4 : TEXCOORD4; float4 o5 : TEXCOORD5; float4 o6 : TEXCOORD6; float4 o7 : TEXCOORD7;
    float4 o8 : TEXCOORD8; float4 o9 : TEXCOORD9; float4 o10 : TEXCOORD10; float4 o11 : TEXCOORD11;
    float4 o12 : TEXCOORD12; float4 o13 : TEXCOORD13; float4 o14 : TEXCOORD14; float4 o15 : TEXCOORD15;
    float4 clip0 : SV_ClipDistance0;
    float2 clip1 : SV_ClipDistance1;
};

[maxvertexcount(4)]
void main(point XeVertex v[1], inout TriangleStream<XeSpriteVertex> stream)
{
    if (!(xe_sys_point.x > 0.0) || !(xe_sys_point.y > 0.0))
        return;
    float2 radius = xe_sys_point.xy * xe_sys_point.zw * v[0].position.w;
    XeSpriteVertex s;
    s.o0 = v[0].o0; s.o1 = v[0].o1; s.o2 = v[0].o2; s.o3 = v[0].o3; s.o4 = v[0].o4; s.o5 = v[0].o5;
    s.o6 = v[0].o6; s.o7 = v[0].o7; s.o8 = v[0].o8; s.o9 = v[0].o9; s.o10 = v[0].o10; s.o11 = v[0].o11;
    s.o12 = v[0].o12; s.o13 = v[0].o13; s.o14 = v[0].o14; s.o15 = v[0].o15;
    s.clip0 = v[0].clip0;
    s.clip1 = v[0].clip1;
    [unroll] for (uint i = 0u; i < 4u; i++)
    {
        // Top left, top right, bottom left, bottom right (host NDC Y goes up, sprite V goes down).
        float2 corner = float2(float(i & 1u), float(i >> 1));
        s.pointCoord = float4(corner, 0.0, 0.0);
        s.position = float4(v[0].position.xy + (corner * 2.0 - 1.0) * float2(radius.x, -radius.y), v[0].position.zw);
        stream.Append(s);
    }
}
)";

        struct PipelineKey
        {
            const void* vs;
            const void* ps;
            bool rectList;
            bool pointList;
            bool stripCut = false; // strips drawn through a list with 0xFFFFFFFF cuts (primitive reset)
            D3D12_PRIMITIVE_TOPOLOGY_TYPE topology;
            D3D12_CULL_MODE cull;
            bool frontCounterClockwise;
            bool depthClip;
            // ROV draws with MSAA: 4 = rasterized on 4x MSAA against a dummy target (msaaDummy_), whose coverage
            // (SV_Coverage) picks the guest samples; 0 = one sample per pixel.
            uint32_t forcedSamples;
            // Host render targets (else the ROV path: no colour / depth targets).
            bool host = false;
            uint32_t samples = 1;
            bool color = false, depthStencil = false;
            uint32_t blend = 0, colorMask = 0, depthControl = 0, stencilMasks = 0;
            auto operator<=>(const PipelineKey&) const = default;
        };

        // A host texture: every level of one decoded fetch constant (texture.h), untiled from guest memory on the GPU.
        struct Texture
        {
            ComPtr<ID3D12Resource> resource;
            D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
            DXGI_FORMAT uavFormat = DXGI_FORMAT_UNKNOWN;
            TextureFetch fetch;
            TextureLayout layout;
            uint64_t hash = 0;              // of the guest bytes at the last untile (when hashValid)
            bool hashValid = false;         // false when the bytes were not readable on the CPU then (pending resolve)
            bool dirty = true;              // not untiled since creation or since a resolve wrote over it
            uint64_t validatedFrame = ~0ull;
            D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;
        };

        constexpr D3D12_RESOURCE_STATES kShaderResource =
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;

        // Host formats of a texture (texture.h): fractions keep the raw components in a typeless resource written
        // through a UINT view and read through a UNORM or SNORM view; integers are floats. The view repeats the
        // last component into the ones the format lacks.
        struct HostTextureFormat
        {
            DXGI_FORMAT resource, uav, srv;
            UINT mapping;
            uint32_t bytes; // per texel, as read back by the tests
        };

        HostTextureFormat HostFormatOf(const TextureFetch& f)
        {
            if (f.numbers == TextureNumbers::kInteger)
                return { DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT,
                         D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING, 16 };
            const bool snorm = f.numbers == TextureNumbers::kSigned;
            switch (f.format)
            {
            case 22: // k_24_8: the depth as a float (the untile converts it)
                return { DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_R32_FLOAT,
                         D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(0, 0, 0, 0), 4 };
            case 2: case 8: case 9:
                return { DXGI_FORMAT_R8_TYPELESS, DXGI_FORMAT_R8_UINT, snorm ? DXGI_FORMAT_R8_SNORM : DXGI_FORMAT_R8_UNORM,
                         D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(0, 0, 0, 0), 1 };
            case 10:
                return { DXGI_FORMAT_R8G8_TYPELESS, DXGI_FORMAT_R8G8_UINT,
                         snorm ? DXGI_FORMAT_R8G8_SNORM : DXGI_FORMAT_R8G8_UNORM,
                         D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(0, 1, 1, 1), 2 };
            case 25:
                return { DXGI_FORMAT_R16G16_TYPELESS, DXGI_FORMAT_R16G16_UINT,
                         snorm ? DXGI_FORMAT_R16G16_SNORM : DXGI_FORMAT_R16G16_UNORM,
                         D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(0, 1, 1, 1), 4 };
            default:
                return { DXGI_FORMAT_R8G8B8A8_TYPELESS, DXGI_FORMAT_R8G8B8A8_UINT,
                         snorm ? DXGI_FORMAT_R8G8B8A8_SNORM : DXGI_FORMAT_R8G8B8A8_UNORM,
                         D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING, 4 };
            }
        }

        // Every level of a texture copied back to the CPU (tests, CB_TEXTURE_DUMP): tightly packed rows.
        struct TextureReadback
        {
            ComPtr<ID3D12Resource> buffer;
            std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> footprints;
            std::vector<uint32_t> present;  // level indices copied
            uint32_t bytes = 0;
            uint32_t guestFormat = 0;
            std::string dumpName;           // CB_TEXTURE_DUMP: written as BMPs at the next Sync
        };

        // Four independent lanes over 32-byte blocks (the CPU overlaps their multiplies; one chain ran at ~6 GB/s, and
        // every texture in use is hashed once per frame), then the tail as before.
        uint64_t HashBytes(const uint8_t* data, size_t size)
        {
            uint64_t h = 0x9E3779B97F4A7C15ull ^ size;
            size_t i = 0;
            if (size >= 64)
            {
                uint64_t lane[4] = { h, h ^ 0x632BE59BD9B4E019ull, h ^ 0x8CB92BA72F3D8DD7ull, h ^ 0xC2B2AE3D27D4EB4Full };
                for (; i + 32 <= size; i += 32)
                    for (int k = 0; k < 4; k++)
                    {
                        uint64_t v;
                        std::memcpy(&v, data + i + k * 8, 8);
                        lane[k] ^= v * 0xFF51AFD7ED558CCDull;
                        lane[k] = std::rotl(lane[k], 31) * 0x9E3779B97F4A7C15ull;
                    }
                for (int k = 0; k < 4; k++)
                    h = std::rotl(h ^ lane[k] * 0xFF51AFD7ED558CCDull, 27) * 0x9E3779B97F4A7C15ull;
            }
            for (; i + 8 <= size; i += 8)
            {
                uint64_t v;
                std::memcpy(&v, data + i, 8);
                h ^= v * 0xFF51AFD7ED558CCDull;
                h = std::rotl(h, 31) * 0x9E3779B97F4A7C15ull;
            }
            for (; i < size; i++)
                h = (h ^ data[i]) * 0x100000001B3ull;
            return h ^ (h >> 29);
        }

        // GPUCLAMP -> host address mode (05b §4.2). With ClampPolicy OGL (D3DSAMP_POINTBORDERENABLE false) and point
        // filtering, the border modes act as their clamp-to-edge counterparts, which makes them exact; the half-border
        // and mirror-to-border modes need shader-side addressing otherwise (not implemented: returns false).
        bool AddressMode(uint32_t clamp, bool pointPolicy, D3D12_TEXTURE_ADDRESS_MODE& mode)
        {
            switch (clamp & 7)
            {
            case 0: mode = D3D12_TEXTURE_ADDRESS_MODE_WRAP; return true;
            case 1: mode = D3D12_TEXTURE_ADDRESS_MODE_MIRROR; return true;
            case 2: mode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; return true;
            case 3: mode = D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE; return true;
            case 6:
                mode = pointPolicy ? D3D12_TEXTURE_ADDRESS_MODE_CLAMP : D3D12_TEXTURE_ADDRESS_MODE_BORDER;
                return true;
            case 4: mode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; return pointPolicy;
            default: mode = D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE; return pointPolicy; // 5, 7
            }
        }

        void WriteBmp(const std::string& path, uint32_t width, uint32_t height, const std::vector<uint32_t>& bgra);

        class Renderer
        {
        public:
            Renderer();
            void Draw(const DrawPacket& draw, const Shader* vs, const Shader* ps);
            void Sync(SyncReason reason = SyncReason::kOther);
            uint64_t SubmitPending();
            bool Reached(uint64_t value, uint32_t ms);
            void Swap(uint64_t frame, uint32_t frontBuffer, uint32_t width, uint32_t height, const GuestOverlay& overlay);
            int SelfTest();
            int TextureTests();

        private:
            struct Upload
            {
                uint8_t* cpu;
                D3D12_GPU_VIRTUAL_ADDRESS gpu;
                ID3D12Resource* resource;
                uint64_t offset;
            };

            ID3D12Device3* device_ = nullptr;
            ComPtr<ID3D12CommandQueue> queue_;
            // Recorded work is submitted in batches while the command processor goes on (Submit), so the GPU runs
            // alongside it; each allocator is reused once the GPU has passed the fence value of its last submission.
            struct Allocator
            {
                ComPtr<ID3D12CommandAllocator> allocator;
                uint64_t fenceValue = 0;
            };
            std::vector<Allocator> allocators_;
            size_t allocatorIndex_ = 0;
            // GPU time: a timestamp at the start and end of each command list (slot = allocator index), read back
            // once the allocator's fence has passed; summed for the frame-rate report (TakeGpuMilliseconds).
            // Inside a list, segments by category (draws, resolves, untiles, present: GpuScope) get their own pairs.
            static constexpr size_t kTimedLists = 256, kQueriesPerList = 256, kSegments = (kQueriesPerList - 2) / 2;
            static constexpr int kCategories = 4; // 0 draw, 1 resolve, 2 texture untile, 3 present
            ComPtr<ID3D12QueryHeap> timestamps_;
            ComPtr<ID3D12Resource> timestampReadback_;
            uint64_t timestampFrequency_ = 0;
            struct TimedList
            {
                bool timed = false;
                uint32_t segments = 0;
                uint8_t category[kSegments] = {};
                uint64_t label[kSegments] = {}; // frame << 16 | draw of the frame (CB_GPU_DRAW_TIMES)
            };
            std::vector<TimedList> timed_ = std::vector<TimedList>(kTimedLists);
            std::atomic<uint64_t> gpuTicks_ = 0;
            std::atomic<uint64_t> categoryTicks_[kCategories] = {};
            std::vector<int> scopeStack_;
            bool segmentOpen_ = false;
            uint32_t drawOfFrame_ = 0; // draws and resolves since the last swap (CB_GPU_DRAW_TIMES labels)
            uint64_t drawOfFrameSwap_ = 0;
            int segmentCategory_ = 0;
            bool Timing() const { return timestamps_ && timestampFrequency_ && allocatorIndex_ < kTimedLists; }
            void OpenSegment(int category);
            void CloseSegment();
            void CollectTimestamp(size_t slot);
        public:
            double TakeGpuMilliseconds();
            std::string TakeGpuBreakdown();
            void BeginScope(int category);
            void EndScope();
            struct GpuScope
            {
                Renderer& r;
                GpuScope(Renderer& renderer, int category) : r(renderer) { r.BeginScope(category); }
                ~GpuScope() { r.EndScope(); }
            };
        private:
            ComPtr<ID3D12GraphicsCommandList> list_;
            ComPtr<ID3D12Fence> fence_;
            HANDLE fenceEvent_ = nullptr;
            HANDLE waitEvent_ = nullptr; // Reached (a timed wait must not leave fenceEvent_ armed for another value)
            uint64_t fenceValue_ = 0;  // last value signalled
            uint64_t syncedValue_ = 0; // last value Sync waited for: the GPU is idle when it equals fenceValue_
            bool recording_ = false;
            uint32_t recordedDraws_ = 0; // draws and resolves recorded since the last submission
            // Transient allocations (uploads, descriptors) restart at every Sync; a constant block's upload is reused
            // while its version (gpu.h ConstantVersion) and the allocation generation are unchanged.
            uint64_t uploadGeneration_ = 0;
            struct ConstantUpload
            {
                uint64_t version = ~0ull, generation = ~0ull;
                D3D12_GPU_VIRTUAL_ADDRESS address = 0;
            };
            ConstantUpload constantUploads_[size_t(ConstantBlock::kCount)];

            ComPtr<ID3D12RootSignature> rootSignature_, computeRootSignature_, presentRootSignature_;
            ComPtr<ID3D12PipelineState> presentPipeline_;
            // The system UI overlay (xui/system_ui.h): an RGBA8 texture the size of the front buffer, uploaded when its
            // content changes; overlayPixels_ keeps the bytes for the CPU reference of CB_PRESENT_DUMP.
            ComPtr<ID3D12Resource> overlay_;
            uint32_t overlayWidth_ = 0, overlayHeight_ = 0;
            std::vector<uint8_t> overlayPixels_;
            bool UpdateOverlay(uint32_t width, uint32_t height);
            uint64_t gammaWritesLogged_ = ~0ull;
            std::map<ResolveKey, ComPtr<ID3D12PipelineState>> resolvePipelines_;
            ComPtr<ID3D12PipelineState> clearPipeline_;
            // Guest memory is read through an SRV by draws and written through a UAV by resolves: its state is tracked
            // explicitly within a recording and returned to COMMON before every submission.
            D3D12_RESOURCE_STATES memoryState_ = D3D12_RESOURCE_STATE_COMMON;
            // Destination ranges resolves wrote in the work recorded since the last Sync: guest memory as the CPU sees
            // it is stale there until the GPU has finished, so texture hashes cannot be taken there.
            std::vector<std::pair<uint32_t, uint32_t>> pendingResolves_;
            ComPtr<ID3D12PipelineState> untilePipelines_[2]; // fractions (UINT view), integers (float view)
            std::vector<TextureReadback> pendingDumps_;
            ComPtr<ID3D12Heap> memoryHeap_;
            // memory_ is what shaders read and resolves write: guest physical memory itself (a placed buffer over the
            // host's pages, read across the bus), or with LocalMemory() a buffer in video memory that UploadMemory
            // brings up to date from memorySystem_, the guest's pages.
            ComPtr<ID3D12Resource> memory_, memorySystem_;
            void UploadMemory();
            uint64_t memoryUploaded_ = 0; // bytes, for the frame log
            ComPtr<ID3D12Resource> edram_;
            // Host index lists for fans and quad lists over vertex positions 0..count-1: the same for every draw, so
            // built once for the largest count (VGT_DRAW_INITIATOR's 16-bit NumIndices).
            ComPtr<ID3D12Resource> fanIndices_, quadIndices_;
            static constexpr uint32_t kMaxDrawCount = 0xFFFF;
            static constexpr uint32_t kFanIndexCount = (kMaxDrawCount - 2) * 3, kQuadIndexCount = kMaxDrawCount / 4 * 6;
            std::vector<uint8_t> rectListGs_;
            std::vector<uint8_t> pointListGs_;

            ComPtr<ID3D12DescriptorHeap> srvHeap_, samplerHeap_, nullHeap_, defaultSamplerHeap_;
            UINT srvSize_ = 0, samplerSize_ = 0;
            UINT srvNext_ = kFixedSrvs, samplerNext_ = 0;
            std::map<std::vector<ID3D12Resource*>, UINT> srvTables_;
            std::map<std::vector<uint8_t>, UINT> samplerTables_;

            std::vector<ComPtr<ID3D12Resource>> uploadPages_;
            std::vector<uint8_t*> uploadMapped_;
            size_t uploadPage_ = 0;
            uint64_t uploadOffset_ = 0;

            std::map<PipelineKey, ComPtr<ID3D12PipelineState>> pipelines_;

            // ---- Pipeline library ----
            // Draw pipelines are kept in a D3D12 pipeline library saved as shader_cache/pipelines.bin, so a pipeline is
            // built by the driver once, not at its first draw in every run (drivers without their own disk cache pay
            // tens of milliseconds per pipeline on the render thread). The driver rejects a library from another
            // driver or GPU; it then starts empty. CB_SHADER_CACHE=0 turns it off.
            ComPtr<ID3D12PipelineLibrary> pipelineLibrary_;
            std::vector<uint8_t> pipelineLibraryBlob_; // the loaded file: the library reads from it while it lives
            std::filesystem::path pipelineLibraryPath_;
            bool pipelineLibraryDirty_ = false;
            std::chrono::steady_clock::time_point pipelineLibrarySaved_;
            uint32_t pipelinesCreated_ = 0, pipelinesLoaded_ = 0;
            double pipelineCreateMs_ = 0, pipelineLoadMs_ = 0;
            void OpenPipelineLibrary();
            void SavePipelineLibrary(bool force);

            // ---- Host render targets ----
            // Draws the host can do with fixed-function blending and depth/stencil go to host render targets instead of
            // the ROV output merger. The EDRAM buffer stays the truth: a target is a cache of the EDRAM tiles it covers
            // (base, pitch, MSAA, rows), loaded from them when they changed since (tileWritten_ > synced) and stored
            // back (dirty) before anything else reads or writes those tiles: a ROV draw, a resolve, another target.
            struct HostTarget
            {
                bool depth = false;
                uint32_t base = 0, pitch = 0, msaa = 0, pitchTiles = 0, rows = 0, width = 0, height = 0;
                ComPtr<ID3D12Resource> resource;
                UINT view = 0;
                D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;
                bool dirty = false;
                uint64_t synced = 0;
                uint32_t Tiles() const { return pitchTiles * rows; }
            };
            std::vector<std::unique_ptr<HostTarget>> hostTargets_;
            ComPtr<ID3D12DescriptorHeap> rtvHeap_, dsvHeap_;
            // The ROV path's 4x MSAA rasterization target (PipelineKey::forcedSamples): R8, never written, grown to the
            // largest surface drawn so far; its own one-view RTV heap.
            ComPtr<ID3D12Resource> msaaDummy_;
            ComPtr<ID3D12DescriptorHeap> msaaDummyHeap_;
            uint32_t msaaDummyWidth_ = 0, msaaDummyHeight_ = 0;
            std::vector<ComPtr<ID3D12Resource>> msaaDummyRetired_;
            D3D12_CPU_DESCRIPTOR_HANDLE MsaaDummy(uint32_t width, uint32_t height)
            {
                width = std::max<uint32_t>(width, 1);
                height = std::max<uint32_t>(height, 1);
                if (!msaaDummy_ || width > msaaDummyWidth_ || height > msaaDummyHeight_)
                {
                    if (msaaDummy_)
                        msaaDummyRetired_.push_back(msaaDummy_); // recorded work may still use it: freed at the next Sync
                    msaaDummyWidth_ = std::max(width, msaaDummyWidth_);
                    msaaDummyHeight_ = std::max(height, msaaDummyHeight_);
                    D3D12_HEAP_PROPERTIES heap = { D3D12_HEAP_TYPE_DEFAULT };
                    D3D12_RESOURCE_DESC desc = {};
                    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                    desc.Width = msaaDummyWidth_;
                    desc.Height = msaaDummyHeight_;
                    desc.DepthOrArraySize = 1;
                    desc.MipLevels = 1;
                    desc.Format = DXGI_FORMAT_R8_UNORM;
                    desc.SampleDesc.Count = 4;
                    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
                    msaaDummy_.Reset();
                    CheckHr(device_->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc,
                                                             D3D12_RESOURCE_STATE_RENDER_TARGET, nullptr,
                                                             IID_PPV_ARGS(&msaaDummy_)), "MSAA dummy target");
                    if (!msaaDummyHeap_)
                    {
                        D3D12_DESCRIPTOR_HEAP_DESC h = { D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 1 };
                        CheckHr(device_->CreateDescriptorHeap(&h, IID_PPV_ARGS(&msaaDummyHeap_)), "MSAA dummy RTV heap");
                    }
                    device_->CreateRenderTargetView(msaaDummy_.Get(), nullptr,
                                                   msaaDummyHeap_->GetCPUDescriptorHandleForHeapStart());
                    Log("GPU: ROV MSAA rasterization target {}x{} (4x)", msaaDummyWidth_, msaaDummyHeight_);
                }
                return msaaDummyHeap_->GetCPUDescriptorHandleForHeapStart();
            }
            UINT rtvSize_ = 0, dsvSize_ = 0, rtvNext_ = 0, dsvNext_ = 0;
            static constexpr UINT kHostTargetViews = 512;
            uint64_t edramClock_ = 1;
            std::vector<uint64_t> tileWritten_ = std::vector<uint64_t>(2048, 0);
            ComPtr<ID3D12RootSignature> hostStoreRootSignature_;
            std::map<uint32_t, ComPtr<ID3D12PipelineState>> hostPassPipelines_;
            bool stencilRefSupported_ = false;
            bool hostTargetsEnabled_ = true;
            // CB_RENDER_PATH=host: depth/stencil and multisampled draws go to host targets too (only formats and
            // states the host cannot do stay on the ROV), so a depth buffer is never compared both ways.
            bool hostAll_ = false;
            uint64_t hostDraws_ = 0, rovDraws_ = 0;
            static uint32_t TileHeight(uint32_t msaa) { return msaa == 0 ? 16 : 8; }
            HostTarget* Target(bool depth, uint32_t base, uint32_t pitch, uint32_t msaa, uint32_t rows);
            bool Stale(const HostTarget& t) const;
            void TransitionTarget(HostTarget& t, D3D12_RESOURCE_STATES state);
            UINT TargetSrvs(HostTarget& t);
            void StoreTarget(HostTarget& t);
            void LoadTarget(HostTarget& t);
            void StoreOverlapping(uint32_t first, uint32_t count, const HostTarget* except);
            void StoreAllDirty();
            void EdramWritten(uint32_t first, uint32_t count);
            ID3D12PipelineState* HostPassPipeline(uint32_t kind, uint32_t msaa, uint32_t bit);
            D3D12_CPU_DESCRIPTOR_HANDLE TargetView(const HostTarget& t) const;
            std::vector<uint8_t> PassShader(const std::string& source, const char* target, const char* name);
            uint64_t skippedDraws_ = 0; // draws skipped while their shaders compiled (async shaders)
            std::map<OutputMergerKey, std::vector<uint8_t>> outputMergerOnly_;
            std::map<TextureFetch, Texture> textures_;
            Texture placeholder_;
            uint64_t frame_ = 0;

            // What CB_EDRAM_DUMP shows: the last colour surface drawn with RT0, and its depth buffer.
            struct DumpSurface
            {
                bool valid = false;
                uint32_t base = 0, pitch = 0, msaa = 0, format = 0, depthBase = 0, height = 0;
            } dump_;

            void BeginRecording();
            void Submit();
            void SubmitBatchIfDue();
            D3D12_GPU_VIRTUAL_ADDRESS UploadConstants(ConstantBlock block, uint32_t first, uint32_t count);
            Upload Allocate(uint64_t size, uint64_t alignment);
            D3D12_GPU_VIRTUAL_ADDRESS UploadData(const void* data, uint64_t size);
            D3D12_CPU_DESCRIPTOR_HANDLE SrvCpu(UINT index) const;
            D3D12_GPU_DESCRIPTOR_HANDLE SrvGpu(UINT index) const;
            D3D12_GPU_DESCRIPTOR_HANDLE TextureTable(const HostShader& shader);
            D3D12_GPU_DESCRIPTOR_HANDLE SamplerTable(const HostShader& shader);
            const Texture& TextureFor(uint32_t fetchConstant);
            void UntileTexture(Texture& texture);
            bool OverlapsPendingResolve(const TextureLayout& layout) const;
            TextureReadback RecordTextureReadback(Texture& texture);
            std::vector<std::vector<uint8_t>> FinishTextureReadback(const TextureReadback& readback);
            void WriteTextureDumps();
            ID3D12PipelineState* Pipeline(const PipelineKey& key, const std::vector<uint8_t>& vs, const std::vector<uint8_t>& ps);
            const std::vector<uint8_t>& OutputMergerOnlyShader(const OutputMergerKey& key);
            void Transition(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
            void UavBarrier(ID3D12Resource* resource);
            void SetMemoryState(D3D12_RESOURCE_STATES state);
            void Resolve();
            ID3D12PipelineState* ComputePipeline(const std::string& source, const char* name);
            // The renderer's own shaders made from keys (resolves, the resolve clear, host-target load / store passes,
            // depth-only output mergers): every key seen is listed in shader_cache/helpers.bin, and PrewarmHelpers
            // builds them all again at start-up (with this build's code), before the boot animation plays.
        public:
            void PrewarmHelpers();
        private:
            void RecordHelper(uint32_t type, const void* key, uint32_t size);
            std::ofstream helpers_;
            bool prewarming_ = false;
            int TextureSamplingTests();
            bool quietTextures_ = false; // tests: no per-tuple log lines
            std::set<std::array<uint32_t, 6>> describedTuples_; // fetch constants already passed to LogOnce
            void DumpEdram(uint64_t frame);
            void DumpFrontBuffer(uint64_t frame, uint32_t frontBuffer, uint32_t width, uint32_t height);
            uint32_t Present(const PresentConstants& constants, std::vector<uint32_t>* readback);
            void LogGamma(uint64_t frame);
        };

        Renderer::Renderer()
        {
            device_ = SharedDevice();
            D3D12_COMMAND_QUEUE_DESC queueDesc = { D3D12_COMMAND_LIST_TYPE_DIRECT };
            CheckHr(device_->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue_)), "renderer CreateCommandQueue");
            allocators_.emplace_back();
            CheckHr(device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                    IID_PPV_ARGS(&allocators_[0].allocator)), "renderer CreateCommandAllocator");
            CheckHr(device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocators_[0].allocator.Get(), nullptr,
                                               IID_PPV_ARGS(&list_)), "renderer CreateCommandList");
            list_->Close();
            CheckHr(device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)), "renderer CreateFence");
            fenceEvent_ = CreateEventW(nullptr, FALSE, FALSE, nullptr);
            waitEvent_ = CreateEventW(nullptr, FALSE, FALSE, nullptr);

            // Root signature (shader_translator.h bindings).
            D3D12_DESCRIPTOR_RANGE memoryRange = { D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, 0 };
            D3D12_DESCRIPTOR_RANGE edramRange = { D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, 0 };
            D3D12_DESCRIPTOR_RANGE textureRange = { D3D12_DESCRIPTOR_RANGE_TYPE_SRV, kTableSize, 0, 1, 0 };
            D3D12_DESCRIPTOR_RANGE samplerRange = { D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, kTableSize, 0, 0, 0 };
            D3D12_ROOT_PARAMETER params[kRootCount] = {};
            for (UINT i = 0; i <= kRootOutput; i++)
            {
                params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
                params[i].Descriptor.ShaderRegister = i;
                params[i].ShaderVisibility = i == kRootOutput ? D3D12_SHADER_VISIBILITY_PIXEL : D3D12_SHADER_VISIBILITY_ALL;
            }
            auto table = [&](RootParameter p, const D3D12_DESCRIPTOR_RANGE* range, D3D12_SHADER_VISIBILITY visibility) {
                params[p].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[p].DescriptorTable = { 1, range };
                params[p].ShaderVisibility = visibility;
            };
            table(kRootMemory, &memoryRange, D3D12_SHADER_VISIBILITY_ALL);
            table(kRootEdram, &edramRange, D3D12_SHADER_VISIBILITY_PIXEL);
            table(kRootVsTextures, &textureRange, D3D12_SHADER_VISIBILITY_VERTEX);
            table(kRootPsTextures, &textureRange, D3D12_SHADER_VISIBILITY_PIXEL);
            table(kRootVsSamplers, &samplerRange, D3D12_SHADER_VISIBILITY_VERTEX);
            table(kRootPsSamplers, &samplerRange, D3D12_SHADER_VISIBILITY_PIXEL);
            D3D12_ROOT_SIGNATURE_DESC rootDesc = { kRootCount, params };
            ComPtr<ID3DBlob> blob, error;
            if (FAILED(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error)))
                Fatal("GPU: root signature: {}", error ? static_cast<const char*>(error->GetBufferPointer()) : "?");
            CheckHr(device_->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(),
                                                 IID_PPV_ARGS(&rootSignature_)), "CreateRootSignature");

            // Compute (resolves and clears, resolve.h): constants b5, EDRAM u0 and guest memory u1 (heap slots 1-2), and
            // for a resolve from a host render target its views t0-t1 (set only by those).
            D3D12_DESCRIPTOR_RANGE computeRange = { D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 2, 0, 0, 0 };
            D3D12_DESCRIPTOR_RANGE computeSourceRange = { D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0, 0, 0 };
            D3D12_ROOT_PARAMETER computeParams[3] = {};
            computeParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            computeParams[0].Descriptor.ShaderRegister = 5;
            computeParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            computeParams[1].DescriptorTable = { 1, &computeRange };
            computeParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            computeParams[2].DescriptorTable = { 1, &computeSourceRange };
            D3D12_ROOT_SIGNATURE_DESC computeDesc = { 3, computeParams };
            blob.Reset();
            error.Reset();
            if (FAILED(D3D12SerializeRootSignature(&computeDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error)))
                Fatal("GPU: compute root signature: {}", error ? static_cast<const char*>(error->GetBufferPointer()) : "?");
            CheckHr(device_->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(),
                                                 IID_PPV_ARGS(&computeRootSignature_)), "CreateRootSignature (compute)");

            // Present pass (present.h): constants b0, guest memory t0 (heap slot 0), guest output u0 (slots 3-5), the
            // system UI overlay t1 (slot 6).
            D3D12_DESCRIPTOR_RANGE presentSrvRange = { D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, 0 };
            D3D12_DESCRIPTOR_RANGE presentUavRange = { D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, 0 };
            D3D12_DESCRIPTOR_RANGE presentOverlayRange = { D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, 0, 0 };
            D3D12_ROOT_PARAMETER presentParams[4] = {};
            presentParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            presentParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            presentParams[1].DescriptorTable = { 1, &presentSrvRange };
            presentParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            presentParams[2].DescriptorTable = { 1, &presentUavRange };
            presentParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            presentParams[3].DescriptorTable = { 1, &presentOverlayRange };
            D3D12_ROOT_SIGNATURE_DESC presentDesc = { 4, presentParams };
            blob.Reset();
            error.Reset();
            if (FAILED(D3D12SerializeRootSignature(&presentDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error)))
                Fatal("GPU: present root signature: {}", error ? static_cast<const char*>(error->GetBufferPointer()) : "?");
            CheckHr(device_->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(),
                                                 IID_PPV_ARGS(&presentRootSignature_)), "CreateRootSignature (present)");
            {
                std::vector<uint8_t> bytecode;
                std::string errors;
                if (!CompileHlsl(kPresentHlsl, "cs_5_1", "present", bytecode, errors))
                    Fatal("GPU: present shader failed to compile: {}", errors);
                D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
                desc.pRootSignature = presentRootSignature_.Get();
                desc.CS = { bytecode.data(), bytecode.size() };
                CheckHr(device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&presentPipeline_)),
                        "CreateComputePipelineState (present)");
            }
            // The texture untile pass (texture.h) has the same bindings as the present pass.
            for (int integer = 0; integer < 2; integer++)
            {
                std::vector<uint8_t> bytecode;
                std::string errors;
                const std::string source = std::string(integer ? "#define XE_UNTILE_FLOAT\n" : "") + kTextureUntileHlsl;
                if (!CompileHlsl(source, "cs_5_1", integer ? "texture_untile_float" : "texture_untile", bytecode, errors))
                    Fatal("GPU: texture untile shader failed to compile: {}", errors);
                D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
                desc.pRootSignature = presentRootSignature_.Get();
                desc.CS = { bytecode.data(), bytecode.size() };
                CheckHr(device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&untilePipelines_[integer])),
                        "CreateComputePipelineState (texture untile)");
            }
            // Typed UAV stores into the untile destinations (required on feature level 11_0; checked to fail clearly).
            for (DXGI_FORMAT format : { DXGI_FORMAT_R8_UINT, DXGI_FORMAT_R8G8_UINT, DXGI_FORMAT_R8G8B8A8_UINT,
                                        DXGI_FORMAT_R16G16_UINT, DXGI_FORMAT_R32G32B32A32_FLOAT })
            {
                D3D12_FEATURE_DATA_FORMAT_SUPPORT support = { format };
                if (FAILED(device_->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &support, sizeof(support))) ||
                    !(support.Support2 & D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE))
                    Fatal("GPU: the device cannot store DXGI format {} through a typed UAV (texture untile)", int(format));
            }

            // Guest physical memory: the section's 64 KiB-page window is physical address 0.
            void* physical = guest::g_memory.Translate(guest::kPhysicalWindow64k);
            CheckHr(device_->OpenExistingHeapFromAddress(physical, IID_PPV_ARGS(&memoryHeap_)),
                    "OpenExistingHeapFromAddress (guest physical memory)");
            const D3D12_HEAP_DESC heapDesc = memoryHeap_->GetDesc();
            D3D12_RESOURCE_DESC memoryDesc = {};
            memoryDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            memoryDesc.Width = guest::kPhysicalSize;
            memoryDesc.Height = memoryDesc.DepthOrArraySize = memoryDesc.MipLevels = 1;
            memoryDesc.SampleDesc.Count = 1;
            memoryDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            memoryDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            if (heapDesc.Flags & D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER)
                memoryDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER;
            CheckHr(device_->CreatePlacedResource(memoryHeap_.Get(), 0, &memoryDesc, D3D12_RESOURCE_STATE_COMMON, nullptr,
                                                  IID_PPV_ARGS(&memorySystem_)), "CreatePlacedResource (guest physical memory)");
            if (g_localMemory)
            {
                memory_.Attach(CreateBuffer(guest::kPhysicalSize, D3D12_HEAP_TYPE_DEFAULT,
                                            D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, "guest physical memory (video memory)"));
                MarkMemoryWritten(0, guest::kPhysicalSize);
                Log("GPU: guest physical memory is kept in video memory (CB_LOCAL_MEMORY)");
            }
            else
                memory_ = memorySystem_;

            edram_.Attach(CreateBuffer(kEdramBytes, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
                                       "EDRAM buffer"));

            D3D12_DESCRIPTOR_HEAP_DESC srvDesc = { D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kSrvHeapSize,
                                                   D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE };
            CheckHr(device_->CreateDescriptorHeap(&srvDesc, IID_PPV_ARGS(&srvHeap_)), "SRV heap");
            D3D12_DESCRIPTOR_HEAP_DESC samplerDesc = { D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, kSamplerHeapSize,
                                                       D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE };
            CheckHr(device_->CreateDescriptorHeap(&samplerDesc, IID_PPV_ARGS(&samplerHeap_)), "sampler heap");
            D3D12_DESCRIPTOR_HEAP_DESC nullDesc = { D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kTableSize };
            CheckHr(device_->CreateDescriptorHeap(&nullDesc, IID_PPV_ARGS(&nullHeap_)), "null SRV heap");
            D3D12_DESCRIPTOR_HEAP_DESC defaultSamplerDesc = { D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 1 };
            CheckHr(device_->CreateDescriptorHeap(&defaultSamplerDesc, IID_PPV_ARGS(&defaultSamplerHeap_)), "sampler staging heap");
            srvSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
            samplerSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

            D3D12_SHADER_RESOURCE_VIEW_DESC memorySrv = {};
            memorySrv.Format = DXGI_FORMAT_R32_TYPELESS;
            memorySrv.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
            memorySrv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            memorySrv.Buffer.NumElements = guest::kPhysicalSize / 4;
            memorySrv.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
            device_->CreateShaderResourceView(memory_.Get(), &memorySrv, SrvCpu(0));
            D3D12_UNORDERED_ACCESS_VIEW_DESC edramUav = {};
            edramUav.Format = DXGI_FORMAT_R32_TYPELESS;
            edramUav.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
            edramUav.Buffer.NumElements = kEdramBytes / 4;
            edramUav.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
            device_->CreateUnorderedAccessView(edram_.Get(), nullptr, &edramUav, SrvCpu(1));
            D3D12_UNORDERED_ACCESS_VIEW_DESC memoryUav = edramUav;
            memoryUav.Buffer.NumElements = guest::kPhysicalSize / 4;
            device_->CreateUnorderedAccessView(memory_.Get(), nullptr, &memoryUav, SrvCpu(2));
            D3D12_SHADER_RESOURCE_VIEW_DESC nullSrv = {};
            nullSrv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            nullSrv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            nullSrv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            nullSrv.Texture2D.MipLevels = 1;
            D3D12_CPU_DESCRIPTOR_HANDLE nullHandle = nullHeap_->GetCPUDescriptorHandleForHeapStart();
            for (UINT i = 0; i < kTableSize; i++, nullHandle.ptr += srvSize_)
                device_->CreateShaderResourceView(nullptr, &nullSrv, nullHandle);
            D3D12_SAMPLER_DESC defaultSampler = {};
            defaultSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
            defaultSampler.AddressU = defaultSampler.AddressV = defaultSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            defaultSampler.MaxLOD = D3D12_FLOAT32_MAX;
            device_->CreateSampler(&defaultSampler, defaultSamplerHeap_->GetCPUDescriptorHandleForHeapStart());

            // Placeholder: opaque magenta (never silently black) for unimplemented textures behind CB_TEXTURE_MAGENTA=1,
            // and the binding of constants without data whose swizzle reads only 0/1 (nothing is sampled from it).
            D3D12_HEAP_PROPERTIES defaultHeap = { D3D12_HEAP_TYPE_DEFAULT };
            D3D12_RESOURCE_DESC placeholderDesc = { D3D12_RESOURCE_DIMENSION_TEXTURE2D, 0, 1, 1, 1, 1,
                                                    DXGI_FORMAT_R8G8B8A8_UNORM, { 1, 0 }, D3D12_TEXTURE_LAYOUT_UNKNOWN,
                                                    D3D12_RESOURCE_FLAG_NONE };
            CheckHr(device_->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &placeholderDesc,
                                                     D3D12_RESOURCE_STATE_COPY_DEST, nullptr,
                                                     IID_PPV_ARGS(&placeholder_.resource)), "placeholder texture");
            placeholder_.srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            placeholder_.srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            placeholder_.srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            placeholder_.srv.Texture2D.MipLevels = 1;
            placeholder_.dirty = false;
            placeholder_.state = kShaderResource;
            BeginRecording();
            const uint32_t magenta = 0xFFFF00FFu; // R=FF G=00 B=FF A=FF
            Upload up = Allocate(256, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
            std::memcpy(up.cpu, &magenta, 4);
            D3D12_TEXTURE_COPY_LOCATION dst = { placeholder_.resource.Get(), D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX };
            D3D12_TEXTURE_COPY_LOCATION src = { up.resource, D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT };
            src.PlacedFootprint = { up.offset, { DXGI_FORMAT_R8G8B8A8_UNORM, 1, 1, 1, 256 } };
            list_->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
            Transition(placeholder_.resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, kShaderResource);
            // Fans as (v1, v2, v0), (v2, v3, v0) like Direct3D (Xenia); quads split as (q, q+1, q+2), (q, q+2, q+3)
            // (Xenia's split; the hardware's is not documented).
            auto staticIndices = [&](ComPtr<ID3D12Resource>& buffer, uint32_t count, auto fill, const char* name) {
                buffer.Attach(CreateBuffer(uint64_t(count) * 4, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, name));
                Upload data = Allocate(uint64_t(count) * 4, 4);
                fill(reinterpret_cast<uint32_t*>(data.cpu));
                Transition(buffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
                list_->CopyBufferRegion(buffer.Get(), 0, data.resource, data.offset, uint64_t(count) * 4);
                Transition(buffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
            };
            staticIndices(fanIndices_, kFanIndexCount, [](uint32_t* out) {
                for (uint32_t i = 2; i < kMaxDrawCount; i++, out += 3)
                    out[0] = i - 1, out[1] = i, out[2] = 0;
            }, "fan indices");
            staticIndices(quadIndices_, kQuadIndexCount, [](uint32_t* out) {
                for (uint32_t q = 0; q + 4 <= kMaxDrawCount; q += 4, out += 6)
                    out[0] = q, out[1] = q + 1, out[2] = q + 2, out[3] = q, out[4] = q + 2, out[5] = q + 3;
            }, "quad indices");
            // The EDRAM stays a UAV; its initial content is zero (CreateCommittedResource zeroes default heaps).
            Transition(edram_.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
            Sync();

            std::string errors;
            if (!CompileHlsl(kRectListGeometryShader, "gs_5_1", "rect_list_gs", rectListGs_, errors))
                Fatal("GPU: rectangle-list geometry shader: {}", errors);
            if (!CompileHlsl(std::string(kXenosHlslLibrary) + kPointListGeometryShader, "gs_5_1", "point_list_gs",
                    pointListGs_, errors))
                Fatal("GPU: point-list geometry shader: {}", errors);
            // Host render targets: view heaps, the store pass's root signature (b0, t0-t1, EDRAM u0), SV_StencilRef.
            {
                D3D12_DESCRIPTOR_HEAP_DESC rtv = { D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kHostTargetViews };
                D3D12_DESCRIPTOR_HEAP_DESC dsv = { D3D12_DESCRIPTOR_HEAP_TYPE_DSV, kHostTargetViews };
                CheckHr(device_->CreateDescriptorHeap(&rtv, IID_PPV_ARGS(&rtvHeap_)), "RTV heap");
                CheckHr(device_->CreateDescriptorHeap(&dsv, IID_PPV_ARGS(&dsvHeap_)), "DSV heap");
                rtvSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
                dsvSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
                D3D12_DESCRIPTOR_RANGE srvs = { D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0, 0, 0 };
                D3D12_DESCRIPTOR_RANGE uav = { D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, 0 };
                D3D12_ROOT_PARAMETER p[3] = {};
                p[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
                p[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                p[1].DescriptorTable = { 1, &srvs };
                p[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                p[2].DescriptorTable = { 1, &uav };
                D3D12_ROOT_SIGNATURE_DESC desc = { 3, p };
                ComPtr<ID3DBlob> rsBlob, rsError;
                if (FAILED(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &rsBlob, &rsError)))
                    Fatal("GPU: host target root signature: {}", rsError ? static_cast<const char*>(rsError->GetBufferPointer()) : "?");
                CheckHr(device_->CreateRootSignature(0, rsBlob->GetBufferPointer(), rsBlob->GetBufferSize(),
                                                     IID_PPV_ARGS(&hostStoreRootSignature_)), "CreateRootSignature (host store)");
                D3D12_FEATURE_DATA_D3D12_OPTIONS options = {};
                if (SUCCEEDED(device_->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options))))
                    stencilRefSupported_ = options.PSSpecifiedStencilRefSupported;
                if (const char* path = std::getenv("CB_RENDER_PATH"); path && std::string(path) == "rov")
                    hostTargetsEnabled_ = false;
                else if (path && std::string(path) == "host")
                    hostAll_ = true;
                Log("GPU: render path: {} (SV_StencilRef {})", !hostTargetsEnabled_ ? "ROV output merger only (CB_RENDER_PATH=rov)"
                    : hostAll_ ? "host render targets for everything they can do (CB_RENDER_PATH=host)"
                    : "host render targets, ROV for the rest", stencilRefSupported_ ? "supported" : "not supported");
            }
            OpenPipelineLibrary();
            Log("GPU: renderer ready (guest memory heap flags {:X}, EDRAM {} bytes)", uint32_t(heapDesc.Flags), kEdramBytes);
        }

        void Renderer::OpenPipelineLibrary()
        {
            if (const char* value = std::getenv("CB_SHADER_CACHE"); value && std::string(value) == "0")
                return;
            pipelineLibraryPath_ = std::filesystem::path("shader_cache") / "pipelines.bin";
            if (std::ifstream file{ pipelineLibraryPath_, std::ios::binary })
                pipelineLibraryBlob_.assign(std::istreambuf_iterator<char>(file), {});
            HRESULT hr = E_FAIL;
            if (!pipelineLibraryBlob_.empty())
                hr = device_->CreatePipelineLibrary(pipelineLibraryBlob_.data(), pipelineLibraryBlob_.size(),
                                                    IID_PPV_ARGS(&pipelineLibrary_));
            if (FAILED(hr))
            {
                if (!pipelineLibraryBlob_.empty())
                    Log("GPU: pipeline cache {} not usable with this driver/GPU (HRESULT {:08X}); starting it again",
                        pipelineLibraryPath_.string(), uint32_t(hr));
                pipelineLibraryBlob_.clear();
                pipelineLibrary_.Reset();
                if (FAILED(hr = device_->CreatePipelineLibrary(nullptr, 0, IID_PPV_ARGS(&pipelineLibrary_))))
                {
                    Log("GPU: no pipeline library on this driver (HRESULT {:08X}); pipelines are built every run", uint32_t(hr));
                    pipelineLibrary_.Reset();
                    return;
                }
            }
            else
                Log("GPU: pipeline cache {} loaded ({} KB)", pipelineLibraryPath_.string(), pipelineLibraryBlob_.size() / 1024);
            pipelineLibrarySaved_ = std::chrono::steady_clock::now();
        }

        // Written a few seconds after the last new pipeline (checked at every present), through a temporary file.
        void Renderer::SavePipelineLibrary(bool force)
        {
            if (!pipelineLibrary_ || !pipelineLibraryDirty_)
                return;
            const auto now = std::chrono::steady_clock::now();
            if (!force && now - pipelineLibrarySaved_ < std::chrono::seconds(3))
                return;
            pipelineLibrarySaved_ = now;
            pipelineLibraryDirty_ = false;
            std::vector<uint8_t> data(pipelineLibrary_->GetSerializedSize());
            if (FAILED(pipelineLibrary_->Serialize(data.data(), data.size())))
                return;
            std::error_code error;
            std::filesystem::create_directories(pipelineLibraryPath_.parent_path(), error);
            const std::filesystem::path temporary = pipelineLibraryPath_.string() + ".tmp";
            {
                std::ofstream file(temporary, std::ios::binary | std::ios::trunc);
                file.write(reinterpret_cast<const char*>(data.data()), std::streamsize(data.size()));
                if (!file)
                    return;
            }
            std::filesystem::rename(temporary, pipelineLibraryPath_, error);
            Log("GPU: pipeline cache saved ({} KB); this run: {} pipelines built in {:.1f} ms, {} loaded from the cache in {:.1f} ms",
                data.size() / 1024, pipelinesCreated_, pipelineCreateMs_, pipelinesLoaded_, pipelineLoadMs_);
        }

        void Renderer::OpenSegment(int category)
        {
            if (!recording_ || !Timing() || segmentOpen_ || timed_[allocatorIndex_].segments >= kSegments)
                return;
            const uint32_t n = timed_[allocatorIndex_].segments;
            list_->EndQuery(timestamps_.Get(), D3D12_QUERY_TYPE_TIMESTAMP, UINT(allocatorIndex_ * kQueriesPerList + 2 + 2 * n));
            segmentOpen_ = true;
            segmentCategory_ = category;
            timed_[allocatorIndex_].label[n] = SwapCount() << 16 | (drawOfFrame_ & 0xFFFF);
        }

        void Renderer::CloseSegment()
        {
            if (!segmentOpen_)
                return;
            segmentOpen_ = false;
            if (!recording_ || !Timing())
                return;
            TimedList& t = timed_[allocatorIndex_];
            list_->EndQuery(timestamps_.Get(), D3D12_QUERY_TYPE_TIMESTAMP,
                            UINT(allocatorIndex_ * kQueriesPerList + 3 + 2 * t.segments));
            t.category[t.segments++] = uint8_t(segmentCategory_);
        }

        void Renderer::BeginScope(int category)
        {
            CloseSegment();
            scopeStack_.push_back(category);
            OpenSegment(category);
        }

        void Renderer::EndScope()
        {
            CloseSegment();
            if (!scopeStack_.empty())
                scopeStack_.pop_back();
            if (!scopeStack_.empty())
                OpenSegment(scopeStack_.back());
        }

        void Renderer::CollectTimestamp(size_t slot)
        {
            if (slot >= kTimedLists || !timed_[slot].timed)
                return;
            TimedList& list = timed_[slot];
            list.timed = false;
            const size_t bytes = (2 + 2 * size_t(list.segments)) * 8, offset = slot * kQueriesPerList * 8;
            const D3D12_RANGE range{ offset, offset + bytes };
            void* data = nullptr;
            if (FAILED(timestampReadback_->Map(0, &range, &data)))
                return;
            std::vector<uint64_t> t(bytes / 8);
            std::memcpy(t.data(), static_cast<uint8_t*>(data) + offset, bytes);
            const D3D12_RANGE none{ 0, 0 };
            timestampReadback_->Unmap(0, &none);
            if (t[1] > t[0])
                gpuTicks_ += t[1] - t[0];
            // CB_GPU_DRAW_TIMES=<frame>: each draw's (and resolve's ...) GPU time in that frame, by its index in the
            // frame (the index CB_GPU_TRACE prints for the frame after it: the swap counts the frame being built).
            static const uint64_t drawTimesFrame = [] { const char* v = std::getenv("CB_GPU_DRAW_TIMES"); return v ? std::strtoull(v, nullptr, 0) : 0ull; }();
            for (uint32_t i = 0; i < list.segments; i++)
                if (t[3 + 2 * i] > t[2 + 2 * i])
                {
                    categoryTicks_[list.category[i]] += t[3 + 2 * i] - t[2 + 2 * i];
                    if (drawTimesFrame && (list.label[i] >> 16) == drawTimesFrame)
                        Log("GPU: frame {} item {} ({}): {:.3f} ms", drawTimesFrame, list.label[i] & 0xFFFF,
                            int(list.category[i]), double(t[3 + 2 * i] - t[2 + 2 * i]) * 1000.0 / double(timestampFrequency_));
                }
        }

        std::string Renderer::TakeGpuBreakdown()
        {
            static const char* const kNames[kCategories] = { "draws", "resolves + target loads/stores", "untiles", "present" };
            std::string out = std::format("{} host-target draws, {} ROV draws; ", hostDraws_, rovDraws_);
            hostDraws_ = rovDraws_ = 0;
            for (int i = 0; i < kCategories; i++)
            {
                const uint64_t ticks = categoryTicks_[i].exchange(0);
                out += std::format("{} {:.2f} ms; ", kNames[i],
                                   timestampFrequency_ ? double(ticks) * 1000.0 / double(timestampFrequency_) : 0.0);
            }
            return out;
        }

        double Renderer::TakeGpuMilliseconds()
        {
            const uint64_t ticks = gpuTicks_.exchange(0);
            return timestampFrequency_ ? double(ticks) * 1000.0 / double(timestampFrequency_) : 0.0;
        }

        void Renderer::BeginRecording()
        {
            if (recording_)
                return;
            const uint64_t completed = fence_->GetCompletedValue();
            size_t free = allocators_.size();
            for (size_t i = 0; i < allocators_.size() && free == allocators_.size(); i++)
                if (allocators_[(allocatorIndex_ + i) % allocators_.size()].fenceValue <= completed)
                    free = (allocatorIndex_ + i) % allocators_.size();
            if (free == allocators_.size())
            {
                allocators_.emplace_back();
                CheckHr(device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                        IID_PPV_ARGS(&allocators_.back().allocator)),
                        "renderer CreateCommandAllocator");
            }
            allocatorIndex_ = free;
            CollectTimestamp(allocatorIndex_); // its previous list is done (fence passed)
            ID3D12CommandAllocator* allocator = allocators_[allocatorIndex_].allocator.Get();
            CheckHr(allocator->Reset(), "renderer allocator Reset");
            CheckHr(list_->Reset(allocator, nullptr), "renderer list Reset");
            if (!timestamps_)
            {
                D3D12_QUERY_HEAP_DESC q{ D3D12_QUERY_HEAP_TYPE_TIMESTAMP, UINT(kTimedLists * kQueriesPerList), 0 };
                D3D12_HEAP_PROPERTIES readback{ D3D12_HEAP_TYPE_READBACK };
                D3D12_RESOURCE_DESC buffer{ D3D12_RESOURCE_DIMENSION_BUFFER, 0, kTimedLists * kQueriesPerList * 8, 1, 1, 1,
                                            DXGI_FORMAT_UNKNOWN, { 1, 0 }, D3D12_TEXTURE_LAYOUT_ROW_MAJOR };
                if (SUCCEEDED(device_->CreateQueryHeap(&q, IID_PPV_ARGS(&timestamps_))) &&
                    SUCCEEDED(device_->CreateCommittedResource(&readback, D3D12_HEAP_FLAG_NONE, &buffer,
                        D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&timestampReadback_))))
                    queue_->GetTimestampFrequency(&timestampFrequency_);
            }
            if (Timing())
            {
                list_->EndQuery(timestamps_.Get(), D3D12_QUERY_TYPE_TIMESTAMP, UINT(allocatorIndex_ * kQueriesPerList));
                timed_[allocatorIndex_].segments = 0;
            }
            recording_ = true;
            if (!scopeStack_.empty())
                OpenSegment(scopeStack_.back()); // a scope that spans a submit goes on in the new list
            recording_ = false;
            ID3D12DescriptorHeap* heaps[] = { srvHeap_.Get(), samplerHeap_.Get() };
            list_->SetDescriptorHeaps(2, heaps);
            recording_ = true;
        }

        // Hands the recorded work to the GPU without waiting. Transient allocations stay in use until the next Sync.
        void Renderer::Submit()
        {
            SetMemoryState(D3D12_RESOURCE_STATE_COMMON);
            CloseSegment();
            if (Timing())
            {
                const UINT first = UINT(allocatorIndex_ * kQueriesPerList);
                list_->EndQuery(timestamps_.Get(), D3D12_QUERY_TYPE_TIMESTAMP, first + 1);
                list_->ResolveQueryData(timestamps_.Get(), D3D12_QUERY_TYPE_TIMESTAMP, first,
                                        2 + 2 * timed_[allocatorIndex_].segments, timestampReadback_.Get(),
                                        uint64_t(first) * 8);
                timed_[allocatorIndex_].timed = true;
            }
            CheckHr(list_->Close(), "renderer list Close");
            ID3D12CommandList* lists[] = { list_.Get() };
            queue_->ExecuteCommandLists(1, lists);
            CheckHr(queue_->Signal(fence_.Get(), ++fenceValue_), "renderer Signal");
            allocators_[allocatorIndex_].fenceValue = fenceValue_;
            recording_ = false;
            recordedDraws_ = 0;
        }

        // Called after each recorded draw or resolve. Waiting for the GPU only at the points where the CPU could see
        // its progress left it idle while the command processor recorded, and the command processor idle while it
        // ran; submitting in batches lets both work at once. What the guest observes is unchanged: every sync point
        // still waits for all work before it. A batch goes when the GPU has run out of work or when it is large.
        void Renderer::SubmitBatchIfDue()
        {
            constexpr uint32_t kMinBatch = 32, kMaxBatch = 256;
            if (++recordedDraws_ < kMinBatch)
                return;
            if (recordedDraws_ >= kMaxBatch || (recordedDraws_ % kMinBatch == 0 && fence_->GetCompletedValue() == fenceValue_))
                Submit();
        }

        // A constant block as the draw's root constant buffer: uploaded again only when a value in it changed.
        D3D12_GPU_VIRTUAL_ADDRESS Renderer::UploadConstants(ConstantBlock block, uint32_t first, uint32_t count)
        {
            ConstantUpload& cached = constantUploads_[size_t(block)];
            const uint64_t version = ConstantVersion(block);
            if (cached.version != version || cached.generation != uploadGeneration_)
            {
                if (block == ConstantBlock::kFloat)
                {
                    Upload upload = Allocate(count * 4, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
                    CopyFlushingDenormals(reinterpret_cast<uint32_t*>(upload.cpu), HostRegisters(first), count);
                    cached.address = upload.gpu;
                }
                else
                    cached.address = UploadData(HostRegisters(first), count * 4);
                cached.version = version;
                cached.generation = uploadGeneration_;
            }
            return cached.address;
        }

        void Renderer::Sync(SyncReason reason)
        {
            SyncStat& stat = g_syncStats[size_t(reason)];
            stat.calls++;
            if (!recording_ && syncedValue_ == fenceValue_)
                return;
            stat.submits++;
            if (recording_)
                Submit();
            if (fence_->GetCompletedValue() < fenceValue_)
            {
                const auto started = std::chrono::steady_clock::now();
                CheckHr(fence_->SetEventOnCompletion(fenceValue_, fenceEvent_), "renderer SetEventOnCompletion");
                WaitForSingleObject(fenceEvent_, INFINITE);
                stat.waitNs += uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::steady_clock::now() - started).count());
            }
            syncedValue_ = fenceValue_;
            msaaDummyRetired_.clear();
            LogDebugMessages();
            pendingResolves_.clear(); // guest memory now holds everything the GPU wrote
            WriteTextureDumps();
            // The GPU is idle: every transient allocation can be reused.
            uploadGeneration_++;
            uploadPage_ = 0;
            uploadOffset_ = 0;
            srvNext_ = kFixedSrvs;
            samplerNext_ = 0;
            srvTables_.clear();
            samplerTables_.clear();
        }

        uint64_t Renderer::SubmitPending()
        {
            if (recording_)
                Submit();
            return fenceValue_;
        }

        bool Renderer::Reached(uint64_t value, uint32_t ms)
        {
            if (fence_->GetCompletedValue() >= value)
                return true;
            if (ms == 0)
                return false;
            CheckHr(fence_->SetEventOnCompletion(value, waitEvent_), "renderer SetEventOnCompletion (deferred)");
            WaitForSingleObject(waitEvent_, ms);
            return fence_->GetCompletedValue() >= value;
        }

        Renderer::Upload Renderer::Allocate(uint64_t size, uint64_t alignment)
        {
            if (size > kUploadPage)
                Fatal("GPU: upload of {} bytes exceeds the upload page size", size);
            uploadOffset_ = (uploadOffset_ + alignment - 1) & ~(alignment - 1);
            if (uploadPage_ < uploadPages_.size() && uploadOffset_ + size > kUploadPage)
            {
                uploadPage_++;
                uploadOffset_ = 0;
            }
            if (uploadPage_ == uploadPages_.size())
            {
                ComPtr<ID3D12Resource> page;
                page.Attach(CreateBuffer(kUploadPage, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE, "upload page"));
                uint8_t* mapped = nullptr;
                CheckHr(page->Map(0, nullptr, reinterpret_cast<void**>(&mapped)), "upload page Map");
                uploadPages_.push_back(page);
                uploadMapped_.push_back(mapped);
                uploadOffset_ = 0;
            }
            ID3D12Resource* page = uploadPages_[uploadPage_].Get();
            Upload upload{ uploadMapped_[uploadPage_] + uploadOffset_, page->GetGPUVirtualAddress() + uploadOffset_, page,
                           uploadOffset_ };
            uploadOffset_ += size;
            return upload;
        }

        D3D12_GPU_VIRTUAL_ADDRESS Renderer::UploadData(const void* data, uint64_t size)
        {
            Upload upload = Allocate(size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
            std::memcpy(upload.cpu, data, size);
            return upload.gpu;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE Renderer::SrvCpu(UINT index) const
        {
            D3D12_CPU_DESCRIPTOR_HANDLE handle = srvHeap_->GetCPUDescriptorHandleForHeapStart();
            handle.ptr += SIZE_T(index) * srvSize_;
            return handle;
        }

        D3D12_GPU_DESCRIPTOR_HANDLE Renderer::SrvGpu(UINT index) const
        {
            D3D12_GPU_DESCRIPTOR_HANDLE handle = srvHeap_->GetGPUDescriptorHandleForHeapStart();
            handle.ptr += UINT64(index) * srvSize_;
            return handle;
        }

        void Renderer::Transition(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
        {
            if (before == after)
                return;
            D3D12_RESOURCE_BARRIER barrier = { D3D12_RESOURCE_BARRIER_TYPE_TRANSITION };
            barrier.Transition = { resource, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, before, after };
            list_->ResourceBarrier(1, &barrier);
        }

        void Renderer::UavBarrier(ID3D12Resource* resource)
        {
            D3D12_RESOURCE_BARRIER barrier = { D3D12_RESOURCE_BARRIER_TYPE_UAV };
            barrier.UAV.pResource = resource;
            list_->ResourceBarrier(1, &barrier);
        }

        // Copies the written pages of guest memory into the GPU's copy (LocalMemory), in stream order before the work
        // recorded after it. A page written again while the copy waits to run is copied again next time.
        void Renderer::UploadMemory()
        {
            if (!g_localMemory || !g_memoryAnyWritten.exchange(false, std::memory_order_acq_rel))
                return;
            BeginRecording();
            uint64_t runStart = 0, runPages = 0, bytes = 0;
            auto flush = [&] {
                if (!runPages)
                    return;
                SetMemoryState(D3D12_RESOURCE_STATE_COPY_DEST);
                list_->CopyBufferRegion(memory_.Get(), runStart << kMemoryPageShift, memorySystem_.Get(),
                                        runStart << kMemoryPageShift, runPages << kMemoryPageShift);
                bytes += runPages << kMemoryPageShift;
                runPages = 0;
            };
            for (uint32_t w = 0; w < kMemoryPageWords; w++)
            {
                uint64_t bits = g_memoryWritten[w].load(std::memory_order_relaxed);
                if (bits)
                    bits = g_memoryWritten[w].exchange(0, std::memory_order_acq_rel);
                if (!bits)
                {
                    flush();
                    continue;
                }
                for (uint32_t b = 0; b < 64; b++)
                {
                    const uint64_t page = uint64_t(w) * 64 + b;
                    if ((bits >> b) & 1)
                    {
                        if (!runPages)
                            runStart = page;
                        runPages++;
                    }
                    else
                        flush();
                }
            }
            flush();
            memoryUploaded_ += bytes;
        }

        void Renderer::SetMemoryState(D3D12_RESOURCE_STATES state)
        {
            if (memoryState_ == state)
            {
                if (state == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
                    UavBarrier(memory_.Get()); // resolves stay in stream order when their destinations overlap
                return;
            }
            Transition(memory_.Get(), memoryState_, state);
            memoryState_ = state;
        }

        // ---- Textures (texture.h: decode, layout, GPU untile) ------------------------------------------------------

        bool Renderer::OverlapsPendingResolve(const TextureLayout& layout) const
        {
            for (const auto& [start, end] : layout.extents)
                for (const auto& [rs, re] : pendingResolves_)
                    if (start < re && rs < end)
                        return true;
            return false;
        }

        // Every present level from guest memory into the host texture: one compute dispatch per level, recorded in
        // stream order, so a texture a resolve wrote earlier in the same recording reads the resolved data.
        void Renderer::UntileTexture(Texture& texture)
        {
            if (!recording_)
                Fatal("GPU: texture untile outside a recording");
            GpuScope scope(*this, 2);
            const HostTextureFormat host = HostFormatOf(texture.fetch);
            SetMemoryState(kShaderResource);
            Transition(texture.resource.Get(), texture.state, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
            list_->SetComputeRootSignature(presentRootSignature_.Get());
            list_->SetPipelineState(untilePipelines_[texture.fetch.numbers == TextureNumbers::kInteger ||
                                                    TextureUntileKind(texture.fetch.format) == kUntileDepth24].Get());
            list_->SetComputeRootDescriptorTable(1, SrvGpu(0));
            for (uint32_t level = 0; level < texture.layout.levelCount; level++)
                for (uint32_t face = 0; face < texture.fetch.faces; face++)
                {
                    const TextureLevelSource& source = texture.layout.levels[level];
                    if (!source.present)
                        continue;
                    D3D12_UNORDERED_ACCESS_VIEW_DESC uav = {};
                    uav.Format = host.uav;
                    uav.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;   // one slice: the face (a 2D texture: 0)
                    uav.Texture2DArray.MipSlice = level;
                    uav.Texture2DArray.FirstArraySlice = face;
                    uav.Texture2DArray.ArraySize = 1;
                    const UINT index = srvNext_++;
                    device_->CreateUnorderedAccessView(texture.resource.Get(), nullptr, &uav, SrvCpu(index));
                    TextureUntileConstants constants = UntileConstants(texture.fetch, source);
                    constants.storage += face * source.faceStride;
                    list_->SetComputeRootConstantBufferView(0, UploadData(&constants, sizeof(constants)));
                    list_->SetComputeRootDescriptorTable(2, SrvGpu(index));
                    list_->Dispatch((source.width + 7) / 8, (source.height + 7) / 8, 1);
                }
            texture.state = kShaderResource;
            Transition(texture.resource.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, texture.state);
            if (g_dumpTextures && (g_dumpTexturesFrame == 1 || SwapCount() + 1 == g_dumpTexturesFrame))
            {
                TextureReadback readback = RecordTextureReadback(texture);
                readback.dumpName = std::format("tex_{:08X}_{}x{}{}_f{}", texture.fetch.baseAddress ? texture.fetch.baseAddress
                                                                                                    : texture.fetch.mipAddress,
                                                texture.fetch.width, texture.fetch.height, texture.fetch.tiled ? "_tiled" : "",
                                                frame_);
                pendingDumps_.push_back(std::move(readback));
            }
        }

        const Texture& Renderer::TextureFor(uint32_t tf)
        {
            uint32_t d[6];
            for (uint32_t i = 0; i < 6; i++)
                d[i] = Reg(0x4800 + tf * 6 + i);
            TextureFetch fetch;
            const std::string problem = DecodeTextureFetch(d, fetch);
            if (!problem.empty())
            {
                // A constant whose swizzle selects only the constants 0 and 1 reads nothing (05a §3).
                bool constantsOnly = true;
                for (uint32_t c = 0; c < 4; c++)
                    constantsOnly &= ((d[3] >> (1 + 3 * c)) & 7) >= 4;
                if (constantsOnly)
                    return placeholder_;
                LogOnce(std::format("texture not implemented ({}): {}", problem, DescribeTextureTuple(d)));
                if (g_magentaTextures)
                    return placeholder_;
                Fatal("GPU: texture fetch constant {} is not implemented: {} ({}; {:08X} {:08X} {:08X} {:08X} {:08X} {:08X})",
                      tf, problem, DescribeTextureTuple(d), d[0], d[1], d[2], d[3], d[4], d[5]);
            }
            // Each distinct tuple once (05b §6.1 runtime check); described only for fetch constants not seen before.
            if (!quietTextures_ && describedTuples_.insert({ d[0], d[1], d[2], d[3], d[4], d[5] }).second)
                LogOnce("texture: " + DescribeTextureTuple(d));
            const uint32_t bytes = TextureBytesPerTexel(fetch.format);
            if ((bytes == 1 && fetch.endian != 0) || (bytes == 2 && fetch.endian >= 2))
                LogOnce(std::format("texture endian {} on {}-byte texels: applied to 32-bit words (05b §1.5; Xenia "
                                    "swaps per texel; unverified)", fetch.endian, bytes));

            Texture& texture = textures_[fetch];
            if (!texture.resource)
            {
                texture.fetch = fetch;
                texture.layout = ComputeTextureLayout(fetch);
                for (const auto& [start, end] : texture.layout.extents)
                    if (uint64_t(end) > guest::kPhysicalSize || end < start)
                        Fatal("GPU: texture {:08X}-{:08X} is outside physical memory ({})", start, end, DescribeTextureTuple(d));
                const HostTextureFormat host = HostFormatOf(fetch);
                D3D12_HEAP_PROPERTIES heap = { D3D12_HEAP_TYPE_DEFAULT };
                D3D12_RESOURCE_DESC desc = { D3D12_RESOURCE_DIMENSION_TEXTURE2D, 0, fetch.width, fetch.height,
                                             UINT16(fetch.faces), UINT16(texture.layout.levelCount), host.resource,
                                             { 1, 0 }, D3D12_TEXTURE_LAYOUT_UNKNOWN,
                                             D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS };
                CheckHr(device_->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON,
                                                         nullptr, IID_PPV_ARGS(&texture.resource)), "texture");
                texture.state = D3D12_RESOURCE_STATE_COMMON;
                texture.uavFormat = host.uav;
                texture.srv.Format = host.srv;
                texture.srv.Shader4ComponentMapping = host.mapping;
                if (fetch.faces == 6)
                {
                    texture.srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
                    texture.srv.TextureCube.MipLevels = texture.layout.levelCount;
                }
                else
                {
                    texture.srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                    texture.srv.Texture2D.MipLevels = texture.layout.levelCount;
                }
            }
            // Untiled when new or written by a resolve (in stream order, no CPU wait), and when the guest bytes changed:
            // hashed once per frame, but only where no recorded resolve is still to
            // write (there the CPU's view of memory is stale; the next use after the GPU finished checks again).
            auto hash = [&] {
                uint64_t h = 0;
                for (const auto& [start, end] : texture.layout.extents)
                    h = h * 0x100000001B3ull ^ HashBytes(guest::g_memory.Translate(guest::kPhysicalWindow64k + start), end - start);
                return h;
            };
            const bool pending = OverlapsPendingResolve(texture.layout);
            if (texture.dirty)
            {
                UntileTexture(texture);
                texture.dirty = false;
                texture.hashValid = !pending;
                if (!pending)
                    texture.hash = hash();
                texture.validatedFrame = frame_;
            }
            else if (texture.validatedFrame != frame_ && !pending)
            {
                const uint64_t h = hash();
                if (!texture.hashValid || h != texture.hash)
                    UntileTexture(texture);
                texture.hash = h;
                texture.hashValid = true;
                texture.validatedFrame = frame_;
            }
            return texture;
        }

        TextureReadback Renderer::RecordTextureReadback(Texture& texture)
        {
            TextureReadback readback;
            readback.bytes = HostFormatOf(texture.fetch).bytes;
            readback.guestFormat = texture.fetch.format;
            const D3D12_RESOURCE_DESC desc = texture.resource->GetDesc();
            const UINT levels = desc.MipLevels;
            readback.footprints.resize(levels);
            UINT64 total = 0;
            device_->GetCopyableFootprints(&desc, 0, levels, 0, readback.footprints.data(), nullptr, nullptr, &total);
            readback.buffer.Attach(CreateBuffer(total, D3D12_HEAP_TYPE_READBACK, D3D12_RESOURCE_FLAG_NONE, "texture readback"));
            BeginRecording();
            Transition(texture.resource.Get(), texture.state, D3D12_RESOURCE_STATE_COPY_SOURCE);
            for (UINT level = 0; level < levels; level++)
            {
                if (!texture.layout.levels[level].present)
                    continue;
                // The copy reads the typeless resource as its UINT format (the footprint's format must be castable).
                D3D12_TEXTURE_COPY_LOCATION dst = { readback.buffer.Get(), D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT };
                dst.PlacedFootprint = readback.footprints[level];
                D3D12_TEXTURE_COPY_LOCATION src = { texture.resource.Get(), D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX };
                src.SubresourceIndex = level;
                list_->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
                readback.present.push_back(level);
            }
            Transition(texture.resource.Get(), D3D12_RESOURCE_STATE_COPY_SOURCE, texture.state);
            return readback;
        }

        // After the Sync that executed RecordTextureReadback: per level, rows of width * bytes (empty when absent).
        std::vector<std::vector<uint8_t>> Renderer::FinishTextureReadback(const TextureReadback& readback)
        {
            std::vector<std::vector<uint8_t>> levels(readback.footprints.size());
            const uint8_t* mapped = nullptr;
            CheckHr(readback.buffer->Map(0, nullptr, reinterpret_cast<void**>(const_cast<uint8_t**>(&mapped))),
                    "texture readback Map");
            for (uint32_t level : readback.present)
            {
                const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& f = readback.footprints[level];
                const size_t row = size_t(f.Footprint.Width) * readback.bytes;
                levels[level].resize(row * f.Footprint.Height);
                for (UINT y = 0; y < f.Footprint.Height; y++)
                    std::memcpy(levels[level].data() + y * row, mapped + f.Offset + size_t(y) * f.Footprint.RowPitch, row);
            }
            readback.buffer->Unmap(0, nullptr);
            return levels;
        }

        // CB_TEXTURE_DUMP=1: every untiled texture as <name>_L<level>.bmp, data components X, Y, Z, W as R, G, B, A
        // (integers clamped to bytes, 16-bit components shown by their high byte).
        void Renderer::WriteTextureDumps()
        {
            std::vector<TextureReadback> dumps = std::move(pendingDumps_);
            pendingDumps_.clear();
            for (const TextureReadback& readback : dumps)
            {
                const std::vector<std::vector<uint8_t>> levels = FinishTextureReadback(readback);
                for (uint32_t level : readback.present)
                {
                    const D3D12_SUBRESOURCE_FOOTPRINT& f = readback.footprints[level].Footprint;
                    std::vector<uint32_t> bgra(size_t(f.Width) * f.Height);
                    for (size_t i = 0; i < bgra.size(); i++)
                    {
                        const uint8_t* t = levels[level].data() + i * readback.bytes;
                        uint8_t c[4];
                        for (uint32_t k = 0; k < 4; k++)
                        {
                            if (readback.bytes == 16)
                            {
                                float v;
                                std::memcpy(&v, t + 4 * k, 4);
                                c[k] = uint8_t(std::clamp(v, 0.0f, 255.0f));
                            }
                            else if (readback.guestFormat == 25)
                                c[k] = t[std::min(k, 1u) * 2 + 1];
                            else
                                c[k] = t[std::min<uint32_t>(k, readback.bytes - 1)];
                        }
                        bgra[i] = uint32_t(c[2]) | uint32_t(c[1]) << 8 | uint32_t(c[0]) << 16 | uint32_t(c[3]) << 24;
                    }
                    WriteBmp(std::format("{}_L{}.bmp", readback.dumpName, level), f.Width, f.Height, bgra);
                }
            }
        }

        D3D12_GPU_DESCRIPTOR_HANDLE Renderer::TextureTable(const HostShader& shader)
        {
            std::vector<const Texture*> textures(kTableSize, nullptr);
            if (shader.textures.size() > kTableSize)
                Fatal("GPU: a shader uses {} textures (at most {} are bound)", shader.textures.size(), kTableSize);
            for (size_t i = 0; i < shader.textures.size(); i++)
            {
                if (shader.textures[i].dimension == 0)
                    Fatal("GPU: 1D texture fetches are not implemented (tf{})", shader.textures[i].fetchConstant);
                // A fetch constant the title never set up as a texture (type 0: all zero) while its shader still
                // samples it: the null descriptor, which reads 0 (titles rely on it: a shader shared by draws that
                // bind fewer textures).
                if ((Reg(0x4800 + shader.textures[i].fetchConstant * 6) & 3) != 2)
                {
                    LogOnce(std::format("texture: tf{} is not a texture; sampled as zero", shader.textures[i].fetchConstant));
                    continue;
                }
                textures[i] = &TextureFor(shader.textures[i].fetchConstant);
                if ((shader.textures[i].dimension == 3) != (textures[i]->fetch.faces == 6))
                    Fatal("GPU: tf{} is fetched as {} but its fetch constant is {}", shader.textures[i].fetchConstant,
                          shader.textures[i].dimension == 3 ? "a cube" : "2D", textures[i]->fetch.faces == 6 ? "a cube" : "2D");
            }
            std::vector<ID3D12Resource*> resources(kTableSize, nullptr);
            for (UINT i = 0; i < kTableSize; i++)
                resources[i] = textures[i] ? textures[i]->resource.Get() : nullptr;
            auto it = srvTables_.find(resources);
            if (it != srvTables_.end())
                return SrvGpu(it->second);
            const UINT index = srvNext_;
            srvNext_ += kTableSize;
            device_->CopyDescriptorsSimple(kTableSize, SrvCpu(index), nullHeap_->GetCPUDescriptorHandleForHeapStart(),
                                           D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
            for (UINT i = 0; i < kTableSize; i++)
                if (textures[i])
                    device_->CreateShaderResourceView(resources[i], &textures[i]->srv, SrvCpu(index + i));
            srvTables_.emplace(std::move(resources), index);
            return SrvGpu(index);
        }

        // The host sampler for a fetch constant and an instruction's overrides (05a §2, 05b §4). Stops on what the
        // host sampler cannot express exactly.
        D3D12_SAMPLER_DESC SamplerFor(const SamplerBinding& binding)
        {
            uint32_t d[6];
            for (uint32_t i = 0; i < 6; i++)
                d[i] = Reg(0x4800 + binding.fetchConstant * 6 + i);
            const uint32_t mag = binding.magFilter == 3 ? (d[3] >> 19) & 3 : binding.magFilter;
            const uint32_t min = binding.minFilter == 3 ? (d[3] >> 21) & 3 : binding.minFilter;
            const uint32_t mip = binding.mipFilter == 3 ? (d[3] >> 23) & 3 : binding.mipFilter;
            const uint32_t aniso = binding.anisoFilter == 7 ? (d[3] >> 25) & 7 : binding.anisoFilter;
            auto stop = [&](const std::string& what) {
                Fatal("GPU: texture sampler of tf{}: {} ({})", binding.fetchConstant, what, DescribeTextureTuple(d));
            };
            if (mag > 1 || min > 1)
                stop(std::format("min/mag filter {}/{}", min, mag));
            if (mip > 2)
                stop(std::format("mip filter {}", mip));
            if (aniso > 5)
                stop(std::format("aniso filter {}", aniso));
            D3D12_SAMPLER_DESC desc = {};
            if (aniso != 0)
            {
                desc.Filter = D3D12_FILTER_ANISOTROPIC;
                desc.MaxAnisotropy = 1u << (aniso - 1); // 1:1 .. 16:1 (Xenia)
            }
            else
            {
                desc.Filter = D3D12_ENCODE_BASIC_FILTER(min == 1 ? D3D12_FILTER_TYPE_LINEAR : D3D12_FILTER_TYPE_POINT,
                                                        mag == 1 ? D3D12_FILTER_TYPE_LINEAR : D3D12_FILTER_TYPE_POINT,
                                                        mip == 1 ? D3D12_FILTER_TYPE_LINEAR : D3D12_FILTER_TYPE_POINT,
                                                        D3D12_FILTER_REDUCTION_TYPE_STANDARD);
            }
            // ClampPolicy OGL applies to point filtering (05a §1.1); with mixed point/linear the border modes are open.
            const bool policy = (d[1] >> 11) & 1;
            const bool point = aniso == 0 && min == 0 && mag == 0;
            const uint32_t clamps[3] = { (d[0] >> 10) & 7, (d[0] >> 13) & 7, (d[0] >> 16) & 7 };
            D3D12_TEXTURE_ADDRESS_MODE* modes[3] = { &desc.AddressU, &desc.AddressV, &desc.AddressW };
            for (uint32_t axis = 0; axis < 3; axis++)
            {
                if (axis == 2)
                {
                    *modes[axis] = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 2D: no W coordinate
                    continue;
                }
                if (policy && !point && clamps[axis] >= 4)
                    stop(std::format("address mode {} with ClampPolicy OGL and min/mag filter {}/{}", clamps[axis], min, mag));
                if (!AddressMode(clamps[axis], policy && point, *modes[axis]))
                    stop(std::format("address mode {} (half border / mirror to border)", clamps[axis]));
            }
            // Border colour: D3D writes only ABGR black or white (05b §4.3); it replaces the data components.
            if ((d[5] & 3) > 1)
                stop(std::format("border colour {} (YCbCr black)", d[5] & 3));
            const float border = (d[5] & 3) == 1 ? 1.0f : 0.0f;
            desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = border;
            desc.BorderColor[3] = ((d[5] >> 2) & 1) ? 1.0f : border;
            // Level range of the constant (05a §3); BASEMAP samples the first of them only (Xenia's MaxLOD + 0.25 keeps
            // the min/mag decision). LOD bias is applied in the shader.
            TextureFetch fetch;
            DecodeTextureFetch(d, fetch); // TextureFor already stopped on an unimplemented constant
            desc.MinLOD = float(fetch.minLevel);
            desc.MaxLOD = mip == 2 ? float(fetch.minLevel) + 0.25f : float(fetch.maxLevel);
            return desc;
        }

        D3D12_GPU_DESCRIPTOR_HANDLE Renderer::SamplerTable(const HostShader& shader)
        {
            std::vector<D3D12_SAMPLER_DESC> descs;
            for (const SamplerBinding& binding : shader.samplers)
                descs.push_back(SamplerFor(binding));
            if (descs.size() > kTableSize)
                Fatal("GPU: a shader uses {} samplers (at most {} are bound)", descs.size(), kTableSize);
            std::vector<uint8_t> key(descs.size() * sizeof(D3D12_SAMPLER_DESC));
            if (!descs.empty())
                std::memcpy(key.data(), descs.data(), key.size());
            auto gpuHandle = [&](UINT index) {
                D3D12_GPU_DESCRIPTOR_HANDLE handle = samplerHeap_->GetGPUDescriptorHandleForHeapStart();
                handle.ptr += UINT64(index) * samplerSize_;
                return handle;
            };
            auto it = samplerTables_.find(key);
            if (it != samplerTables_.end())
                return gpuHandle(it->second);
            const UINT index = samplerNext_;
            samplerNext_ += kTableSize;
            for (UINT i = 0; i < kTableSize; i++)
            {
                D3D12_CPU_DESCRIPTOR_HANDLE handle = samplerHeap_->GetCPUDescriptorHandleForHeapStart();
                handle.ptr += SIZE_T(index + i) * samplerSize_;
                if (i < descs.size())
                    device_->CreateSampler(&descs[i], handle);
                else
                    device_->CopyDescriptorsSimple(1, handle, defaultSamplerHeap_->GetCPUDescriptorHandleForHeapStart(),
                                                   D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
            }
            samplerTables_.emplace(std::move(key), index);
            return gpuHandle(index);
        }

        // ---- Pipelines -----------------------------------------------------------------------------------------

        const std::vector<uint8_t>& Renderer::OutputMergerOnlyShader(const OutputMergerKey& key)
        {
            auto it = outputMergerOnly_.find(key);
            if (it != outputMergerOnly_.end())
                return it->second;
            RecordHelper(4, &key, sizeof(key));
            const std::string source = std::string(kXenosHlslLibrary) + OutputMergerHlsl(key, OutputMergerTarget::kPixelShader);
            std::vector<uint8_t> bytecode;
            std::string errors;
            if (!CompileHlsl(source, "ps_5_1", "output_merger_only", bytecode, errors))
                Fatal("GPU: output-merger-only pixel shader failed to compile: {}\n{}", errors, source);
            return outputMergerOnly_.emplace(key, std::move(bytecode)).first->second;
        }

        ID3D12PipelineState* Renderer::Pipeline(const PipelineKey& key, const std::vector<uint8_t>& vs,
                                                 const std::vector<uint8_t>& ps)
        {
            auto it = pipelines_.find(key);
            if (it != pipelines_.end())
                return it->second.Get();
            D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
            desc.pRootSignature = rootSignature_.Get();
            desc.VS = { vs.data(), vs.size() };
            desc.PS = { ps.data(), ps.size() };
            if (key.rectList)
                desc.GS = { rectListGs_.data(), rectListGs_.size() };
            else if (key.pointList)
                desc.GS = { pointListGs_.data(), pointListGs_.size() };
            desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0;
            if (key.host)
            {
                desc.PS = ps.empty() ? D3D12_SHADER_BYTECODE{} : D3D12_SHADER_BYTECODE{ ps.data(), ps.size() };
                if (key.color)
                {
                    desc.NumRenderTargets = 1;
                    desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
                    auto& rt = desc.BlendState.RenderTarget[0];
                    rt.RenderTargetWriteMask = UINT8(key.colorMask);
                    const uint32_t b = key.blend;
                    // RB_BLENDCONTROL factors (01b §6.4) -> D3D12 (HostBlendable checked they exist).
                    auto factor = [](uint32_t f, bool alpha) {
                        switch (f)
                        {
                        case 0: return D3D12_BLEND_ZERO;
                        case 1: return D3D12_BLEND_ONE;
                        case 4: return alpha ? D3D12_BLEND_SRC_ALPHA : D3D12_BLEND_SRC_COLOR;
                        case 5: return alpha ? D3D12_BLEND_INV_SRC_ALPHA : D3D12_BLEND_INV_SRC_COLOR;
                        case 6: return D3D12_BLEND_SRC_ALPHA;
                        case 7: return D3D12_BLEND_INV_SRC_ALPHA;
                        case 8: return alpha ? D3D12_BLEND_DEST_ALPHA : D3D12_BLEND_DEST_COLOR;
                        case 9: return alpha ? D3D12_BLEND_INV_DEST_ALPHA : D3D12_BLEND_INV_DEST_COLOR;
                        case 10: return D3D12_BLEND_DEST_ALPHA;
                        case 11: return D3D12_BLEND_INV_DEST_ALPHA;
                        case 12: return D3D12_BLEND_BLEND_FACTOR;
                        case 13: return D3D12_BLEND_INV_BLEND_FACTOR;
                        default: return D3D12_BLEND_SRC_ALPHA_SAT; // 16
                        }
                    };
                    auto op = [](uint32_t o) {
                        switch (o)
                        {
                        case 0: return D3D12_BLEND_OP_ADD;
                        case 1: return D3D12_BLEND_OP_SUBTRACT;
                        case 2: return D3D12_BLEND_OP_MIN;
                        case 3: return D3D12_BLEND_OP_MAX;
                        default: return D3D12_BLEND_OP_REV_SUBTRACT; // 4
                        }
                    };
                    rt.BlendEnable = b != 0x00010001u;
                    rt.SrcBlend = factor(b & 0x1F, false);
                    rt.BlendOp = op((b >> 5) & 7);
                    rt.DestBlend = factor((b >> 8) & 0x1F, false);
                    rt.SrcBlendAlpha = factor((b >> 16) & 0x1F, true);
                    rt.BlendOpAlpha = op((b >> 21) & 7);
                    rt.DestBlendAlpha = factor((b >> 24) & 0x1F, true);
                }
                if (key.depthStencil)
                {
                    desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
                    const uint32_t dc = key.depthControl;
                    auto& ds = desc.DepthStencilState;
                    ds.DepthEnable = (dc >> 1) & 1;
                    ds.DepthWriteMask = ((dc >> 2) & 1) ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
                    ds.DepthFunc = D3D12_COMPARISON_FUNC(((dc >> 4) & 7) + 1);
                    ds.StencilEnable = dc & 1;
                    ds.StencilReadMask = UINT8(key.stencilMasks & 0xFF);
                    ds.StencilWriteMask = UINT8(key.stencilMasks >> 8);
                    auto face = [&](uint32_t shift) {
                        D3D12_DEPTH_STENCILOP_DESC f;
                        f.StencilFunc = D3D12_COMPARISON_FUNC(((dc >> shift) & 7) + 1);
                        f.StencilFailOp = D3D12_STENCIL_OP(((dc >> (shift + 3)) & 7) + 1);
                        f.StencilPassOp = D3D12_STENCIL_OP(((dc >> (shift + 6)) & 7) + 1);
                        f.StencilDepthFailOp = D3D12_STENCIL_OP(((dc >> (shift + 9)) & 7) + 1);
                        return f;
                    };
                    ds.FrontFace = face(8);
                    ds.BackFace = ((dc >> 7) & 1) ? face(20) : face(8);
                }
            }
            desc.SampleMask = UINT_MAX;
            desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
            desc.RasterizerState.CullMode = key.cull;
            desc.RasterizerState.FrontCounterClockwise = key.frontCounterClockwise;
            desc.RasterizerState.DepthClipEnable = key.depthClip;
            // The ROV path's MSAA draws rasterize on a real 4x MSAA target (a dummy, nothing written) rather than with
            // ForcedSampleCount: under target-independent rasterization this GPU gave neighbouring triangles some of
            // the same samples near their shared edge (RTX 3060, 2026-10-19), so those samples were blended twice:
            // dark 2x2 specks along the dashboard background's quad diagonal and dashes on the boot ribbons.
            desc.RasterizerState.ForcedSampleCount = 0;
            if (!key.host)
            {
                desc.DepthStencilState.DepthEnable = FALSE;
                desc.DepthStencilState.StencilEnable = FALSE;
            }
            desc.PrimitiveTopologyType = key.topology;
            if (key.stripCut)
                desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF;
            if (!key.host)
                desc.NumRenderTargets = 0;
            desc.SampleDesc.Count = key.host ? key.samples : 1;
            if (!key.host && key.forcedSamples)
            {
                desc.NumRenderTargets = 1;
                desc.RTVFormats[0] = DXGI_FORMAT_R8_UNORM;
                desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0;
                desc.SampleDesc.Count = key.forcedSamples;
            }
            ComPtr<ID3D12PipelineState> pipeline;
            // The library name: every input of the description (the shaders by content, the key's other fields).
            uint64_t hash = 0xCBF29CE484222325ull;
            auto mix = [&hash](const void* data, size_t size) {
                for (size_t i = 0; i < size; i++)
                    hash = (hash ^ static_cast<const uint8_t*>(data)[i]) * 0x100000001B3ull;
            };
            for (const D3D12_SHADER_BYTECODE& code : { desc.VS, desc.PS, desc.GS })
            {
                mix(&code.BytecodeLength, sizeof(code.BytecodeLength));
                if (code.BytecodeLength)
                    mix(code.pShaderBytecode, code.BytecodeLength);
            }
            const uint32_t fields[] = { key.rectList, key.pointList, uint32_t(key.topology), uint32_t(key.cull),
                                        key.frontCounterClockwise, key.depthClip, key.forcedSamples, key.host, key.samples,
                                        key.color, key.depthStencil, key.blend, key.colorMask, key.depthControl,
                                        key.stencilMasks, key.stripCut, 3 /* description layout version */ };
            mix(fields, sizeof(fields));
            const std::wstring name = std::format(L"{:016X}", hash);
            const auto start = std::chrono::steady_clock::now();
            if (pipelineLibrary_ && SUCCEEDED(pipelineLibrary_->LoadGraphicsPipeline(name.c_str(), &desc, IID_PPV_ARGS(&pipeline))))
            {
                pipelinesLoaded_++;
                pipelineLoadMs_ += std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
            }
            else
            {
                pipeline.Reset();
                CheckHr(device_->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipeline)), "CreateGraphicsPipelineState");
                pipelinesCreated_++;
                pipelineCreateMs_ += std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
                if (pipelineLibrary_ && SUCCEEDED(pipelineLibrary_->StorePipeline(name.c_str(), pipeline.Get())))
                {
                    pipelineLibraryDirty_ = true;
                    pipelineLibrarySaved_ = std::chrono::steady_clock::now();
                }
            }
            return pipelines_.emplace(key, pipeline).first->second.Get();
        }

        // ---- Host render targets ----------------------------------------------------------------------------------

        D3D12_CPU_DESCRIPTOR_HANDLE Renderer::TargetView(const HostTarget& t) const
        {
            D3D12_CPU_DESCRIPTOR_HANDLE h = (t.depth ? dsvHeap_ : rtvHeap_)->GetCPUDescriptorHandleForHeapStart();
            h.ptr += SIZE_T(t.view) * (t.depth ? dsvSize_ : rtvSize_);
            return h;
        }

        bool Renderer::Stale(const HostTarget& t) const
        {
            for (uint32_t i = 0; i < t.Tiles(); i++)
                if (tileWritten_[(t.base + i) & 2047u] > t.synced)
                    return true;
            return false;
        }

        void Renderer::TransitionTarget(HostTarget& t, D3D12_RESOURCE_STATES state)
        {
            Transition(t.resource.Get(), t.state, state);
            t.state = state;
        }

        void Renderer::EdramWritten(uint32_t first, uint32_t count)
        {
            ++edramClock_;
            for (uint32_t i = 0; i < std::min<uint32_t>(count, 2048); i++)
                tileWritten_[(first + i) & 2047u] = edramClock_;
        }

        void Renderer::StoreOverlapping(uint32_t first, uint32_t count, const HostTarget* except)
        {
            count = std::min<uint32_t>(count, 2048);
            for (auto& t : hostTargets_)
            {
                if (!t->dirty || t.get() == except)
                    continue;
                // Ranges on the 2048-tile ring.
                const uint32_t a = (t->base - first) & 2047u, b = (first - t->base) & 2047u;
                if (a < count || b < t->Tiles())
                    StoreTarget(*t);
            }
        }

        void Renderer::StoreAllDirty()
        {
            for (auto& t : hostTargets_)
                if (t->dirty)
                    StoreTarget(*t);
        }

        std::vector<uint8_t> Renderer::PassShader(const std::string& source, const char* target, const char* name)
        {
            std::vector<uint8_t> bytecode;
            std::string errors;
            if (!CompileHlsl(std::string(kEdramAddressHlsl) + kHostTargetCommonHlsl + source, target, name, bytecode, errors))
                Fatal("GPU: {} failed to compile: {}", name, errors);
            return bytecode;
        }

        // kind: 0 colour load, 1 depth+stencil load (SV_StencilRef), 2 depth load (stencil = 0), 3 one stencil bit
        // (`bit`), 4 colour store, 5 depth/stencil store.
        ID3D12PipelineState* Renderer::HostPassPipeline(uint32_t kind, uint32_t msaa, uint32_t bit)
        {
            const uint32_t key = kind << 16 | msaa << 8 | bit;
            ComPtr<ID3D12PipelineState>& pipeline = hostPassPipelines_[key];
            if (pipeline)
                return pipeline.Get();
            RecordHelper(3, &key, sizeof(key));
            if (kind >= 4)
            {
                const std::string ms = msaa != 0 ? "#define XE_HT_MS 1\n" : "#define XE_HT_MS 0\n";
                const std::vector<uint8_t> cs = PassShader(ms + (kind == 4 ? kHostTargetStoreColorCs : kHostTargetStoreDepthCs),
                                                           "cs_5_1", kind == 4 ? "host_store_color" : "host_store_depth");
                D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
                desc.pRootSignature = hostStoreRootSignature_.Get();
                desc.CS = { cs.data(), cs.size() };
                CheckHr(device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipeline)), "host store pipeline");
                return pipeline.Get();
            }
            static const std::vector<uint8_t> vs = PassShader(kHostTargetLoadVs, "vs_5_1", "host_load_vs");
            const char* const sources[4] = { kHostTargetLoadColorPs, kHostTargetLoadDepthStencilRefPs, kHostTargetLoadDepthPs,
                                             kHostTargetLoadStencilBitPs };
            const std::vector<uint8_t> ps = PassShader(sources[kind], "ps_5_1", "host_load");
            D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
            desc.pRootSignature = rootSignature_.Get();
            desc.VS = { vs.data(), vs.size() };
            desc.PS = { ps.data(), ps.size() };
            desc.SampleMask = UINT_MAX;
            desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
            desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
            desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            desc.SampleDesc.Count = HostMs(msaa) ? 4 : 1;
            if (kind == 0)
            {
                desc.NumRenderTargets = 1;
                desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
                desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            }
            else
            {
                desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
                auto& ds = desc.DepthStencilState;
                ds.DepthEnable = kind != 3;
                ds.DepthWriteMask = kind != 3 ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
                ds.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
                ds.StencilEnable = TRUE;
                ds.StencilReadMask = 0xFF;
                ds.StencilWriteMask = kind == 3 ? UINT8(1u << bit) : 0xFF;
                ds.FrontFace = { D3D12_STENCIL_OP_REPLACE, D3D12_STENCIL_OP_REPLACE, D3D12_STENCIL_OP_REPLACE,
                                 D3D12_COMPARISON_FUNC_ALWAYS };
                ds.BackFace = ds.FrontFace;
            }
            CheckHr(device_->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipeline)), "host load pipeline");
            return pipeline.Get();
        }

        Renderer::HostTarget* Renderer::Target(bool depth, uint32_t base, uint32_t pitch, uint32_t msaa, uint32_t rows)
        {
            for (auto it = hostTargets_.begin(); it != hostTargets_.end(); ++it)
            {
                HostTarget& t = **it;
                if (t.depth != depth || t.base != base || t.pitch != pitch || t.msaa != msaa)
                    continue;
                if (t.rows >= rows)
                    return &t;
                if (t.dirty)
                    StoreTarget(t); // a taller one replaces it, loaded from the EDRAM
                rows = std::max(rows, t.rows);
                hostTargets_.erase(it);
                break;
            }
            auto t = std::make_unique<HostTarget>();
            t->depth = depth;
            t->base = base;
            t->pitch = pitch;
            t->msaa = msaa;
            t->pitchTiles = (pitch * (msaa == 2 ? 2 : 1) + 79) / 80;
            t->rows = rows;
            t->width = pitch;
            t->height = rows * TileHeight(msaa);
            D3D12_HEAP_PROPERTIES heap = { D3D12_HEAP_TYPE_DEFAULT };
            D3D12_RESOURCE_DESC desc = {};
            desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            desc.Width = t->width;
            desc.Height = t->height;
            desc.DepthOrArraySize = 1;
            desc.MipLevels = 1;
            desc.Format = depth ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R8G8B8A8_TYPELESS;
            desc.SampleDesc.Count = HostMs(msaa) ? 4 : 1;
            desc.Flags = depth ? D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL : D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
            t->state = depth ? D3D12_RESOURCE_STATE_DEPTH_WRITE : D3D12_RESOURCE_STATE_RENDER_TARGET;
            CheckHr(device_->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, t->state, nullptr,
                                                     IID_PPV_ARGS(&t->resource)), "host render target");
            UINT& next = depth ? dsvNext_ : rtvNext_;
            if (next >= kHostTargetViews)
                Fatal("GPU: more than {} host render target views", kHostTargetViews);
            t->view = next++;
            if (depth)
            {
                D3D12_DEPTH_STENCIL_VIEW_DESC v = {};
                v.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
                v.ViewDimension = HostMs(msaa) ? D3D12_DSV_DIMENSION_TEXTURE2DMS : D3D12_DSV_DIMENSION_TEXTURE2D;
                device_->CreateDepthStencilView(t->resource.Get(), &v, TargetView(*t));
            }
            else
            {
                D3D12_RENDER_TARGET_VIEW_DESC v = {};
                v.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                v.ViewDimension = HostMs(msaa) ? D3D12_RTV_DIMENSION_TEXTURE2DMS : D3D12_RTV_DIMENSION_TEXTURE2D;
                device_->CreateRenderTargetView(t->resource.Get(), &v, TargetView(*t));
            }
            LogOnce(std::format("host render target: {} at tile {}, {} x {} ({}x MSAA)", depth ? "D24S8" : "8_8_8_8",
                                base, t->width, t->height, 1u << msaa));
            hostTargets_.push_back(std::move(t));
            return hostTargets_.back().get();
        }

        struct HostTargetConstants
        {
            uint32_t base, pitchTiles, msaa, bit;
            uint32_t width, height, pad0, pad1;
        };

        void Renderer::StoreTarget(HostTarget& t)
        {
            if (!t.dirty)
                return;
            if (g_hostLogFrame && SwapCount() + 1 == g_hostLogFrame)
                Log("host-log   store {} at {} ({} rows, msaa {}) before draw {}", t.depth ? "depth" : "colour", t.base, t.rows,
                    t.msaa, drawOfFrame_);
            const UINT index = TargetSrvs(t);
            BeginRecording();
            GpuScope scope(*this, 1);
            TransitionTarget(t, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
            UavBarrier(edram_.Get());
            const HostTargetConstants c = { t.base, t.pitchTiles, t.msaa, 0, t.width, t.height, 0, 0 };
            list_->SetPipelineState(HostPassPipeline(t.depth ? 5 : 4, t.msaa, 0));
            list_->SetComputeRootSignature(hostStoreRootSignature_.Get());
            list_->SetComputeRootConstantBufferView(0, UploadData(&c, sizeof(c)));
            list_->SetComputeRootDescriptorTable(1, SrvGpu(index));
            list_->SetComputeRootDescriptorTable(2, SrvGpu(1));
            list_->Dispatch((t.width + 7) / 8, (t.height + 7) / 8, 1);
            UavBarrier(edram_.Get());
            t.dirty = false;
            EdramWritten(t.base, t.Tiles());
            t.synced = edramClock_;
        }

        // Two shader resource views of a target in the SRV ring (colour: R8G8B8A8_UINT twice; depth: depth, stencil).
        UINT Renderer::TargetSrvs(HostTarget& t)
        {
            if (srvNext_ + 2 > kSrvHeapSize)
                Sync(SyncReason::kDescriptors);
            BeginRecording();
            const UINT index = srvNext_;
            srvNext_ += 2;
            D3D12_SHADER_RESOURCE_VIEW_DESC v = {};
            v.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            v.ViewDimension = HostMs(t.msaa) ? D3D12_SRV_DIMENSION_TEXTURE2DMS : D3D12_SRV_DIMENSION_TEXTURE2D;
            v.Texture2D.MipLevels = 1;
            if (t.depth)
            {
                v.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
                device_->CreateShaderResourceView(t.resource.Get(), &v, SrvCpu(index));
                v.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
                if (!HostMs(t.msaa))
                    v.Texture2D.PlaneSlice = 1;
                device_->CreateShaderResourceView(t.resource.Get(), &v, SrvCpu(index + 1));
            }
            else
            {
                v.Format = DXGI_FORMAT_R8G8B8A8_UINT;
                device_->CreateShaderResourceView(t.resource.Get(), &v, SrvCpu(index));
                device_->CreateShaderResourceView(t.resource.Get(), &v, SrvCpu(index + 1));
            }
            return index;
        }

        void Renderer::LoadTarget(HostTarget& t)
        {
            if (g_hostLogFrame && SwapCount() + 1 == g_hostLogFrame)
                Log("host-log   load {} at {} ({} rows, msaa {}) before draw {}", t.depth ? "depth" : "colour", t.base, t.rows,
                    t.msaa, drawOfFrame_);
            BeginRecording();
            GpuScope scope(*this, 1);
            UavBarrier(edram_.Get());
            TransitionTarget(t, t.depth ? D3D12_RESOURCE_STATE_DEPTH_WRITE : D3D12_RESOURCE_STATE_RENDER_TARGET);
            const D3D12_CPU_DESCRIPTOR_HANDLE view = TargetView(t);
            if (t.depth)
                list_->OMSetRenderTargets(0, nullptr, FALSE, &view);
            else
                list_->OMSetRenderTargets(1, &view, FALSE, nullptr);
            list_->SetGraphicsRootSignature(rootSignature_.Get());
            list_->SetGraphicsRootDescriptorTable(kRootEdram, SrvGpu(1));
            const D3D12_VIEWPORT viewport = { 0.0f, 0.0f, float(t.width), float(t.height), 0.0f, 1.0f };
            const D3D12_RECT scissor = { 0, 0, LONG(t.width), LONG(t.height) };
            list_->RSSetViewports(1, &viewport);
            list_->RSSetScissorRects(1, &scissor);
            list_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            auto pass = [&](uint32_t kind, uint32_t bit, UINT ref) {
                const HostTargetConstants c = { t.base, t.pitchTiles, t.msaa, bit, t.width, t.height, 0, 0 };
                list_->SetPipelineState(HostPassPipeline(kind, t.msaa, bit));
                list_->SetGraphicsRootConstantBufferView(kRootSystem, UploadData(&c, sizeof(c)));
                list_->OMSetStencilRef(ref);
                list_->DrawInstanced(3, 1, 0, 0);
            };
            if (!t.depth)
                pass(0, 0, 0);
            else if (stencilRefSupported_)
                pass(1, 0, 0);
            else
            {
                pass(2, 0, 0); // depth, stencil 0
                for (uint32_t bit = 0; bit < 8; bit++)
                    pass(3, bit, 0xFF);
            }
            t.dirty = false;
            t.synced = edramClock_;
        }

        // Whether host render targets can draw this exactly enough: one 8_8_8_8 colour target (or none) without an
        // exponent bias, D3D-expressible blending, D24S8 depth/stencil without polygon offset and with the same
        // stencil reference and masks for both faces. Everything else stays on the ROV output merger.
        bool HostEligible(const OutputMergerKey& om, uint32_t stencilFront, uint32_t stencilBack, bool all)
        {
            for (uint32_t rt = 1; rt < 4; rt++)
                if (om.colorFormat[rt] != kRtDisabled)
                    return false;
            if (om.colorFormat[0] != kRtDisabled)
            {
                if (om.colorFormat[0] != 0 || om.expBias[0] != 0)
                    return false;
                const uint32_t b = om.blendControl[0];
                for (uint32_t f : { b & 0x1F, (b >> 8) & 0x1F, (b >> 16) & 0x1F, (b >> 24) & 0x1F })
                    if (f == 2 || f == 3 || f == 14 || f == 15 || f > 16)
                        return false;
                if (((b >> 5) & 7) > 4 || ((b >> 21) & 7) > 4)
                    return false;
            }
            // Depth or stencil tested / written: the ROV, so a depth buffer is only ever quantised and compared one way.
            // Mixed with ROV draws on the same buffer, the host's D24 conversion and compare differed by a step from
            // the ROV's round-to-nearest, and hidden ribbon edges showed through the boot animation's orb as thin
            // "crack" curves.
            if (!all && (om.depthControl & 0x7)) // stencil enable, z enable, z write
                return false;
            if (om.depthControl)
            {
                if (om.depthFormat != 0 || om.polyOffsetFront || om.polyOffsetBack)
                    return false;
                const bool twoSidedStencil = (om.depthControl & 1) && om.polygonal && ((om.depthControl >> 7) & 1);
                if (twoSidedStencil && (stencilFront & 0xFFFFFF) != (stencilBack & 0xFFFFFF))
                    return false;
            }
            return true;
        }

        // ---- Draws ---------------------------------------------------------------------------------------------

        void Renderer::Draw(const DrawPacket& draw, const Shader* vs, const Shader* ps)
        {
            UploadMemory();
            if (const uint64_t swap = SwapCount(); swap != drawOfFrameSwap_)
            {
                drawOfFrameSwap_ = swap;
                drawOfFrame_ = 0;
            }
            else
                drawOfFrame_++;
            for (const auto& [frame, draw] : g_edramDumpAt)
                if (SwapCount() + 1 == frame && drawOfFrame_ == draw)
                {
                    StoreAllDirty();
                    DumpEdram(frame * 10000 + draw);
                }
            GpuScope scope(*this, 0);
            const uint32_t mode = Reg(0x2208) & 7; // RB_MODECONTROL
            if (mode == 6)
            {
                Resolve(); // COPY (07_resolve.md)
                return;
            }
            for (const SkipDraws& skip : g_skipDraws)
                if (SwapCount() + 1 == skip.frame && drawOfFrame_ >= skip.first && drawOfFrame_ <= skip.last)
                    return; // the frame being built is the one after `SwapCount()` swaps (draw_trace.cpp)
            if (mode != 4 && mode != 5)
                return; // nothing is rasterized (01b §1)
            const uint32_t surfaceInfo = Reg(0x2000), pitch = surfaceInfo & 0x3FFF, msaa = (surfaceInfo >> 16) & 3;
            if (pitch == 0)
                return;
            if (msaa == 3)
                Fatal("GPU: RB_SURFACE_INFO MsaaSamples 3 is undefined ({:08X})", surfaceInfo);
            const uint32_t program = Reg(0x2180);
            const uint32_t exportMode = (program >> 24) & 7;
            if (exportMode == 7)
                return; // MULTIPASS: every primitive is killed
            if (exportMode != 0)
                Fatal("GPU: VS export mode {} (point size / edge flag / kill outputs) is not implemented", exportMode);
            if ((Reg(0x2284) & 3) == 1)
                Fatal("GPU: tessellation (VGT_OUTPUT_PATH_CNTL {:08X}) is not implemented", Reg(0x2284));
            if (!vs || vs->words.empty())
                Fatal("GPU: draw without a vertex shader");
            if (vs->analysis.memoryExport || (ps && ps->analysis.memoryExport))
                Fatal("GPU: memory export (eA/eM) is not implemented (vertex shader {}, pixel shader {})",
                    vs->analysis.memoryExport, ps && ps->analysis.memoryExport);

            // Primitive type -> host topology (01b §6.1). Index positions (0..count-1) are what the host draws; the
            // vertex shader turns a position into the guest index (xe_vertex_index).
            const uint32_t prim = draw.initiator & 0x3F, count = draw.initiator >> 16;
            const uint32_t source = (draw.initiator >> 6) & 3;
            const uint32_t suScMode = Reg(0x2205);
            D3D12_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            bool polygonal = true, rectList = false, pointList = false;
            ID3D12Resource* indices = nullptr; // a static index list (fans, quads) over positions 0..count-1
            D3D12_GPU_VIRTUAL_ADDRESS drawIndices = 0; // or one made for this draw (primitive reset)
            bool stripCut = false;
            uint32_t hostCount = count;

            // Primitive reset (PA_SU_SC_MODE_CNTL multi_prim_ib_ena): in a strip or fan read from an index buffer, the
            // index VGT_MULTI_PRIM_IB_RESET_INDX ends the primitive and the next index starts a new one (Up's level
            // geometry). The host draws positions, so the positions are listed here: a reset becomes the host's
            // strip cut (0xFFFFFFFF), and a fan is listed as triangles per segment, hub first.
            if ((suScMode >> 21) & 1 && source == 0 && (prim == 3 || prim == 5 || prim == 6) && count)
            {
                const bool index32 = (draw.initiator >> 11) & 1;
                const uint32_t address = (draw.dmaBase & 0x1FFFFFFF) & ~(index32 ? 3u : 1u);
                const uint32_t endian = draw.dmaSize >> 30;
                const uint32_t reset = Reg(0x2103) & (index32 ? 0xFFFFFFu : 0xFFFFu);
                auto swap = [](uint32_t v, uint32_t e) {
                    if (e == 1) return ((v << 8) & 0xFF00FF00u) | ((v >> 8) & 0x00FF00FFu);
                    if (e == 2) return __builtin_bswap32(v);
                    if (e == 3) return (v << 16) | (v >> 16);
                    return v;
                };
                auto guestIndex = [&](uint32_t position) {
                    if (index32)
                    {
                        uint32_t word;
                        std::memcpy(&word, guest::g_memory.Translate(guest::kPhysicalWindow64k + address + position * 4), 4);
                        return swap(word, endian) & 0xFFFFFFu;
                    }
                    const uint32_t at = address + position * 2;
                    uint32_t word;
                    std::memcpy(&word, guest::g_memory.Translate(guest::kPhysicalWindow64k + (at & ~3u)), 4);
                    word = swap(word, endian == 2 ? 1 : endian == 3 ? 0 : endian);
                    return (at & 2) ? word >> 16 : word & 0xFFFFu;
                };
                std::vector<uint32_t> list;
                list.reserve(count + count / 2);
                if (prim == 5)
                {
                    uint32_t hub = UINT32_MAX, previous = UINT32_MAX;
                    for (uint32_t i = 0; i < count; i++)
                    {
                        if (guestIndex(i) == reset)
                        {
                            hub = previous = UINT32_MAX;
                            continue;
                        }
                        if (hub == UINT32_MAX)
                            hub = i;
                        else if (previous == UINT32_MAX)
                            previous = i;
                        else
                        {
                            list.insert(list.end(), { previous, i, hub });
                            previous = i;
                        }
                    }
                }
                else
                {
                    for (uint32_t i = 0; i < count; i++)
                        list.push_back(guestIndex(i) == reset ? 0xFFFFFFFFu : i);
                    stripCut = true;
                }
                hostCount = uint32_t(list.size());
                if (hostCount == 0)
                    return;
                const Upload upload = Allocate(uint64_t(hostCount) * 4, 4);
                std::memcpy(upload.cpu, list.data(), uint64_t(hostCount) * 4);
                drawIndices = upload.gpu;
            }
            switch (prim)
            {
            case 4: hostCount = count - count % 3; break;
            case 6: topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
            case 5: // fan -> triangle list (fanIndices_, or listed above with primitive reset)
                if (!drawIndices)
                {
                    indices = fanIndices_.Get();
                    hostCount = count >= 3 ? (count - 2) * 3 : 0;
                }
                break;
            case 13: // quad list -> two triangles per whole quad (quadIndices_)
                indices = quadIndices_.Get();
                hostCount = count / 4 * 6;
                break;
            case 8:
                rectList = true;
                polygonal = false;
                hostCount = count - count % 3;
                break;
            case 1: // point list -> sprites (kPointListGeometryShader)
                topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
                topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
                polygonal = false;
                pointList = true;
                break;
            case 2: topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST; topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE; polygonal = false; break;
            case 3: topology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP; topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE; polygonal = false; break;
            default:
                Fatal("GPU: primitive type {} is not implemented by the renderer (initiator {:08X})", prim, draw.initiator);
            }
            if (prim == 2 || prim == 3)
                LogOnce("lines are drawn as host 1-pixel lines (width and Xenos line rasterization not emulated)");
            if (hostCount == 0)
                return;

            // Culling and fill (01a §5).
            const bool cullFront = suScMode & 1, cullBack = (suScMode >> 1) & 1;
            if (polygonal && cullFront && cullBack)
                return;
            const uint32_t polyMode = (suScMode >> 3) & 3;
            if (polygonal && polyMode == 1)
                Fatal("GPU: polygon fill mode (PA_SU_SC_MODE_CNTL {:08X}) is not implemented", suScMode);

            const uint32_t colorControl = Reg(0x2202);
            if ((colorControl >> 4) & 1)
                Fatal("GPU: alpha-to-mask (RB_COLORCONTROL {:08X}) is not implemented", colorControl);
            const uint32_t clipControl = Reg(0x2204);
            const bool clipDisable = (clipControl >> 16) & 1;
            if (!clipDisable && (clipControl & 0x3F) && ((clipControl >> 17) & 1))
                Fatal("GPU: cull-only user clip planes (PA_CL_CLIP_CNTL {:08X}) are not implemented", clipControl);

            // Output merger key (output_merger.h).
            const bool runsShader = mode == 4 && ps && !ps->words.empty();
            OutputMergerKey om;
            om.msaa = msaa;
            om.runsShader = runsShader;
            om.polygonal = polygonal;
            const uint32_t colorMask = Reg(0x2104);
            if (runsShader)
            {
                for (uint32_t rt = 0; rt < 4; rt++)
                {
                    const uint32_t mask = (colorMask >> (4 * rt)) & 0xF;
                    if (!mask || !((ps->analysis.colorsWritten >> rt) & 1))
                        continue;
                    const uint32_t info = Reg(kColorInfo[rt]), format = (info >> 16) & 0xF;
                    if (!OutputMergerSupportsColorFormat(format))
                        Fatal("GPU: render target {} format {} ({}) is not implemented by the EDRAM output merger", rt,
                              format, EdramColorFormatName(format));
                    om.colorFormat[rt] = format;
                    om.colorMask[rt] = format == 4 ? mask & 3 : mask;
                    if (om.colorMask[rt] == 0)
                    {
                        om.colorFormat[rt] = kRtDisabled;
                        continue;
                    }
                    om.blendControl[rt] = Reg(kBlendControl[rt]);
                    om.expBias[rt] = SignExtend((info >> 20) & 0x3F, 6);
                }
                om.alphaTest = (colorControl & 8) ? colorControl & 0xF : 0;
                om.shaderDepth = ps->analysis.writesDepth;
            }
            const uint32_t depthControl = Reg(0x2200);
            om.depthControl = (depthControl & 3) ? depthControl : 0;
            om.depthFormat = (Reg(0x2002) >> 16) & 1;
            if (om.depthControl)
            {
                om.polyOffsetFront = polygonal ? (suScMode >> 11) & 1 : (suScMode >> 13) & 1;
                om.polyOffsetBack = polygonal && ((suScMode >> 12) & 1);
            }
            bool anyColor = false;
            for (uint32_t rt = 0; rt < 4; rt++)
                anyColor |= om.colorFormat[rt] != kRtDisabled;
            if (!anyColor && !om.depthControl)
                return; // nothing would be written
            if (std::getenv("CB_OM_SURVEY"))
                LogOnce(std::format("OM survey: msaa {} rt0 fmt {} mask {:X} blend {:08X} rt1 fmt {} depth {:08X} dfmt {} "
                                    "alphatest {:X} shaderdepth {} runsShader {}", om.msaa, om.colorFormat[0],
                                    om.colorMask[0], om.blendControl[0], om.colorFormat[1], om.depthControl,
                                    om.depthFormat, om.alphaTest, om.shaderDepth, om.runsShader));

            // Shaders.
            TranslationOptions vsOptions;
            vsOptions.registerCount = (program & 0x3F) + 1;
            const HostShader* hostVsReady = TryCompiledShader(*vs, vsOptions);
            if (!hostVsReady)
            {
                skippedDraws_++;
                return; // its vertex shader is still compiling (shader_cache.h)
            }
            const HostShader& hostVs = *hostVsReady;

            // Viewport, depth range and scissor (01a §5 "Viewport / scissor / half-pixel derivation").
            const uint32_t vte = Reg(0x2206), vtxControl = Reg(0x2302);
            const float sx = (vte & 1) ? RegFloat(0x210F) : 1.0f, ox = (vte & 2) ? RegFloat(0x2110) : 0.0f;
            const float sy = (vte & 4) ? RegFloat(0x2111) : 1.0f, oy = (vte & 8) ? RegFloat(0x2112) : 0.0f;
            const float sz = (vte & 16) ? RegFloat(0x2113) : 1.0f, oz = (vte & 32) ? RegFloat(0x2114) : 0.0f;
            const uint32_t windowOffset = Reg(0x2080);
            const int32_t windowX = SignExtend(windowOffset & 0x7FFF, 15), windowY = SignExtend((windowOffset >> 16) & 0x7FFF, 15);
            float addX = 0.0f, addY = 0.0f;
            if ((suScMode >> 16) & 1)
            {
                addX += float(windowX);
                addY += float(windowY);
            }
            if ((vtxControl & 1) == 0) // PixCenter ZERO: D3D9 integer pixel centres; host centres are at +0.5
            {
                addX += 0.5f;
                addY += 0.5f;
            }
            constexpr float kMaxExtent = 8192.0f;
            XeSystemConstants system = {};
            D3D12_VIEWPORT viewport = { 0.0f, 0.0f, kMaxExtent, kMaxExtent, 0.0f, 1.0f };
            if (clipDisable)
            {
                // No clipping: the whole guest transform is done in the shader over a fixed host viewport.
                system.ndcScale[0] = sx * 2.0f / kMaxExtent;
                system.ndcOffset[0] = (ox + addX) * 2.0f / kMaxExtent - 1.0f;
                system.ndcScale[1] = -sy * 2.0f / kMaxExtent;
                system.ndcOffset[1] = 1.0f - (oy + addY) * 2.0f / kMaxExtent;
                system.ndcScale[2] = sz;
                system.ndcOffset[2] = oz;
            }
            else
            {
                // The guest clip volume maps exactly onto an integer host viewport; the fraction goes to the shader.
                auto axis = [&](float s, float o, float add, float& start, float& size, float& scale, float& offset) {
                    const float centre = o + add, extent = std::fabs(s);
                    const float a0 = std::floor(std::clamp(centre - extent, 0.0f, kMaxExtent));
                    const float a1 = std::floor(std::clamp(centre + extent, 0.0f, kMaxExtent));
                    start = a0;
                    size = a1 - a0;
                    if (size <= 0.0f)
                        return false;
                    scale = s * 2.0f / size;
                    offset = (centre - (a0 + size * 0.5f)) * 2.0f / size;
                    return true;
                };
                float scaleX, offsetX, scaleY, offsetY;
                if (!axis(sx, ox, addX, viewport.TopLeftX, viewport.Width, scaleX, offsetX) ||
                    !axis(sy, oy, addY, viewport.TopLeftY, viewport.Height, scaleY, offsetY))
                    return; // empty viewport: nothing is drawn
                system.ndcScale[0] = scaleX;
                system.ndcOffset[0] = offsetX;
                system.ndcScale[1] = -scaleY; // render-target Y goes down, host NDC Y goes up
                system.ndcOffset[1] = -offsetY;
                // Depth: D3D clip space (z in 0..w) maps to [oz, oz + sz]; GL clip space (-w..w) to [oz - sz, oz + sz].
                float minDepth, maxDepth, scaleZ, offsetZ;
                if ((clipControl >> 19) & 1)
                {
                    minDepth = oz;
                    maxDepth = oz + sz;
                    scaleZ = 1.0f;
                    offsetZ = 0.0f;
                }
                else
                {
                    minDepth = oz - sz;
                    maxDepth = oz + sz;
                    scaleZ = 0.5f;
                    offsetZ = 0.5f;
                }
                if (minDepth > maxDepth)
                {
                    // Reversed range: swap the bounds and mirror z (z' = w - z) so the host still maps to the guest.
                    std::swap(minDepth, maxDepth);
                    scaleZ = -scaleZ;
                    offsetZ = 1.0f - offsetZ;
                }
                if (minDepth < 0.0f || maxDepth > 1.0f)
                    LogOnce(std::format("viewport depth range [{}, {}] outside [0, 1] is clamped", minDepth, maxDepth));
                viewport.MinDepth = std::clamp(minDepth, 0.0f, 1.0f);
                viewport.MaxDepth = std::clamp(maxDepth, 0.0f, 1.0f);
                system.ndcScale[2] = scaleZ;
                system.ndcOffset[2] = offsetZ;
            }
            const uint32_t scissorTl = Reg(0x2081), scissorBr = Reg(0x2082);
            int32_t left = scissorTl & 0x3FFF, top = (scissorTl >> 16) & 0x3FFF;
            int32_t right = scissorBr & 0x3FFF, bottom = (scissorBr >> 16) & 0x3FFF;
            if (!(scissorTl >> 31))
            {
                left += windowX;
                right += windowX;
                top += windowY;
                bottom += windowY;
            }
            const uint32_t screenTl = Reg(0x200E), screenBr = Reg(0x200F);
            left = std::max(left, SignExtend(screenTl & 0x7FFF, 15));
            top = std::max(top, SignExtend((screenTl >> 16) & 0x7FFF, 15));
            right = std::min(right, SignExtend(screenBr & 0x7FFF, 15));
            bottom = std::min(bottom, SignExtend((screenBr >> 16) & 0x7FFF, 15));
            right = std::min(right, int32_t(pitch));
            left = std::max(left, 0);
            top = std::max(top, 0);
            if (right <= left || bottom <= top)
                return;
            const D3D12_RECT scissor = { left, top, right, bottom };

            // Host render targets or the ROV output merger ("Host render targets"): the targets' rows reach the bottom
            // of what this draw can touch (scissor, and the viewport when clipping).
            const uint32_t pitchTiles = (pitch * (msaa == 2 ? 2 : 1) + 79) / 80;
            int32_t drawBottom = bottom;
            if (!clipDisable)
                drawBottom = std::min(drawBottom, int32_t(viewport.TopLeftY + viewport.Height) + 1);
            const uint32_t rows = (uint32_t(std::max(drawBottom, 1)) + TileHeight(msaa) - 1) / TileHeight(msaa);
            const uint32_t colorBase0 = Reg(kColorInfo[0]) & 0x7FF, depthBase0 = Reg(0x2002) & 0x7FF;
            const bool colorOn = om.colorFormat[0] != kRtDisabled;
            bool host = hostTargetsEnabled_ && HostEligible(om, Reg(0x210D), Reg(0x210C), hostAll_);
            const char* rovReason = !hostTargetsEnabled_ ? "rov only" : host ? "" : "state or format";
            // Multisampled draws stay on the ROV output merger: the host targets' depth under MSAA does not match the
            // samples the ROV draws leave in EDRAM (the boot animation's X, depth-tested and blended at 4x, came out
            // wrong). The dashboard's UI is not multisampled.
            if (host && msaa != 0 && !hostAll_)
                host = false, rovReason = "msaa";
            {
                auto fits = [&](uint32_t base) { return pitchTiles != 0 && base + pitchTiles * rows <= 2048; };
                if (host && colorOn && !fits(colorBase0))
                    host = false, rovReason = "colour wraps the EDRAM";
                if (host && om.depthControl && !fits(depthBase0))
                    host = false, rovReason = "depth wraps the EDRAM";
                if (host && colorOn && om.depthControl)
                {
                    const uint32_t span = pitchTiles * rows;
                    if (colorBase0 < depthBase0 + span && depthBase0 < colorBase0 + span)
                        host = false, rovReason = "colour and depth overlap";
                }
            }
            if (g_hostLogFrame && SwapCount() + 1 == g_hostLogFrame)
                Log("host-log draw {}: {} {} | msaa {} pitch {} rows {} colour {}@{} depth {:08X}@{}", drawOfFrame_,
                    host ? "host" : "rov", rovReason, msaa, pitch, rows, colorOn ? om.colorFormat[0] : 99, colorBase0,
                    om.depthControl, depthBase0);

            const HostShader* hostPs = nullptr;
            const std::vector<uint8_t>* psBytecode;
            if (runsShader)
            {
                const uint32_t misc = Reg(0x2181), interpolators = Reg(0x2182);
                TranslationOptions psOptions;
                psOptions.registerCount = ((program >> 8) & 0x3F) + 1;
                psOptions.paramGen = (program >> 18) & 1;
                psOptions.paramGenRegister = (misc >> 8) & 0xFF;
                // SQ_INTERPOLATOR_CNTL.ParamShade is not flat shading: TU46 sets bit 0 on every UI draw whose texture
                // coordinates must interpolate (flat made the title screen single texels), D3DRS_SHADEMODE does not
                // exist on Xbox 360, and Xenia never reads the field. All interpolators are smooth.
                psOptions.flatInterpolators = 0;
                const uint32_t sampleControl = (misc >> 2) & 3;
                if (msaa != 0 && sampleControl == 0)
                    psOptions.centroidInterpolators = 0xFFFF;
                else if (msaa != 0 && sampleControl == 2)
                    psOptions.centroidInterpolators = ~(interpolators >> 16) & 0xFFFF;
                psOptions.pointSprite = pointList;
                psOptions.useOutputMerger = !host;
                if (host)
                    psOptions.alphaTest = om.alphaTest;
                else
                    psOptions.outputMerger = om;
                hostPs = TryCompiledShader(*ps, psOptions);
                if (!hostPs)
                {
                    skippedDraws_++;
                    return; // its pixel shader is still compiling
                }
                psBytecode = &hostPs->bytecode;
            }
            else
            {
                static const std::vector<uint8_t> kNoPixelShader;
                psBytecode = host ? &kNoPixelShader : &OutputMergerOnlyShader(om);
            }

            // System constants (shader_translator.h).
            system.flags = (vte >> 8) & 7;
            system.vertexIndexOffset = Reg(0x2102) & 0xFFFFFF;
            system.minVertexIndex = Reg(0x2101) & 0xFFFFFF;
            system.maxVertexIndex = Reg(0x2100) & 0xFFFFFF;
            if (source == 0)
            {
                const bool index32 = (draw.initiator >> 11) & 1;
                system.indexAddress = (draw.dmaBase & 0x1FFFFFFF) & ~(index32 ? 3u : 1u);
                system.indexFormat = index32 ? 2 : 1;
                system.indexEndian = draw.dmaSize >> 30;
            }
            const uint32_t vsConst = Reg(0x2307), psConst = Reg(0x2308);
            system.vsConstantBase = vsConst & 0x1FF;
            system.vsConstantCount = ((vsConst >> 12) & 0x1FF) + 1;
            system.psConstantBase = psConst & 0x1FF;
            system.psConstantCount = ((psConst >> 12) & 0x1FF) + 1;
            for (uint32_t i = 0; i < 6; i++)
                for (uint32_t c = 0; c < 4; c++)
                    system.clipPlanes[i][c] = RegFloat(0x2388 + i * 4 + c);
            system.clipPlaneMask = clipDisable ? 0 : clipControl & 0x3F;
            system.interpolatorCount = 16;
            system.paramGenFlags = polygonal && !rectList ? 1 : pointList ? 2 :
                (topologyType == D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE ? 4 : 0);
            if (pointList)
            {
                // PA_SU_POINT_SIZE: half height (bits 0-15) and half width (16-31) in 12.4 fixed point.
                const uint32_t pointSize = Reg(0x2280);
                system.pointSize[0] = float(pointSize >> 16) * (2.0f / 16.0f);
                system.pointSize[1] = float(pointSize & 0xFFFF) * (2.0f / 16.0f);
                // A diameter of d pixels is a radius of d / extent in host NDC (the NDC range 2 spans the extent).
                system.pointSize[2] = 1.0f / std::max(viewport.Width, 1.0f);
                system.pointSize[3] = 1.0f / std::max(viewport.Height, 1.0f);
            }

            OutputMergerConstants output = {};
            for (uint32_t rt = 0; rt < 4; rt++)
                output.colorBase[rt] = Reg(kColorInfo[rt]) & 0x7FF; // 11-bit periodic addressing (06a §3.3)
            output.pitchTiles = (pitch * (msaa == 2 ? 2 : 1) + 79) / 80;
            output.depthBase = Reg(0x2002) & 0x7FF;
            output.stencilFront = Reg(0x210D) & 0xFFFFFF;
            output.stencilBack = Reg(0x210C) & 0xFFFFFF;
            for (uint32_t c = 0; c < 4; c++)
                output.blendConstant[c] = RegFloat(0x2105 + c);
            output.alphaRef = RegFloat(0x210E);
            output.polyOffsetFrontScale = RegFloat(0x2380);
            output.polyOffsetFrontOffset = RegFloat(0x2381);
            output.polyOffsetBackScale = RegFloat(0x2382);
            output.polyOffsetBackOffset = RegFloat(0x2383);

            // Record. A draw needs at most two texture and two sampler tables plus one untile view per texture level;
            // when the descriptor rings cannot hold them, the recorded work is finished first (the rings restart once
            // the GPU is idle). Textures are untiled on the GPU in stream order, so resolves need no wait here.
            if (srvNext_ + 2 * kTableSize * (1 + kTextureMaxLevels) + 16 > kSrvHeapSize ||
                samplerNext_ + 2 * kTableSize > kSamplerHeapSize)
                Sync(SyncReason::kDescriptors);
            HostTarget* colorTarget = nullptr;
            HostTarget* depthTarget = nullptr;
            const uint32_t rovSpan = std::min<uint32_t>(pitchTiles * rows, 2048);
            if (host)
            {
                colorTarget = colorOn ? Target(false, colorBase0, pitch, msaa, rows) : nullptr;
                depthTarget = om.depthControl ? Target(true, depthBase0, pitch, msaa, rows) : nullptr;
                for (HostTarget* t : { colorTarget, depthTarget })
                    if (t)
                    {
                        StoreOverlapping(t->base, t->Tiles(), t);
                        if (Stale(*t))
                            LoadTarget(*t);
                    }
            }
            else
            {
                // The ROV reads and writes these tiles: targets over them go back to the EDRAM first.
                for (uint32_t rt = 0; rt < 4; rt++)
                    if (om.colorFormat[rt] != kRtDisabled)
                        StoreOverlapping(Reg(kColorInfo[rt]) & 0x7FF, rovSpan, nullptr);
                if (om.depthControl)
                    StoreOverlapping(depthBase0, rovSpan, nullptr);
            }
            BeginRecording();
            SetMemoryState(kShaderResource);
            const D3D12_GPU_DESCRIPTOR_HANDLE vsTextures = TextureTable(hostVs), vsSamplers = SamplerTable(hostVs);
            D3D12_GPU_DESCRIPTOR_HANDLE psTextures = SrvGpu(kFixedSrvs), psSamplers = samplerHeap_->GetGPUDescriptorHandleForHeapStart();
            if (hostPs)
            {
                psTextures = TextureTable(*hostPs);
                psSamplers = SamplerTable(*hostPs);
            }
            else
            {
                // An output-merger-only shader samples nothing; any valid table will do.
                psTextures = vsTextures;
                psSamplers = vsSamplers;
            }
            PipelineKey pipelineKey = {};
            pipelineKey.vs = &hostVs.bytecode;
            pipelineKey.ps = psBytecode;
            pipelineKey.rectList = rectList;
            pipelineKey.pointList = pointList;
            pipelineKey.topology = topologyType;
            pipelineKey.stripCut = stripCut;
            pipelineKey.cull = !polygonal ? D3D12_CULL_MODE_NONE
                             : cullFront ? D3D12_CULL_MODE_FRONT : cullBack ? D3D12_CULL_MODE_BACK : D3D12_CULL_MODE_NONE;
            pipelineKey.frontCounterClockwise = ((suScMode >> 2) & 1) == 0;
            pipelineKey.depthClip = !clipDisable;
            pipelineKey.forcedSamples = !host && msaa != 0 ? 4 : 0;
            if (host)
            {
                pipelineKey.host = true;
                pipelineKey.samples = HostMs(msaa) ? 4 : 1;
                pipelineKey.color = colorTarget != nullptr;
                pipelineKey.depthStencil = depthTarget != nullptr;
                pipelineKey.blend = colorOn ? om.blendControl[0] : 0;
                pipelineKey.colorMask = colorOn ? om.colorMask[0] : 0;
                pipelineKey.depthControl = om.depthControl;
                pipelineKey.stencilMasks = ((output.stencilFront >> 8) & 0xFF) | (((output.stencilFront >> 16) & 0xFF) << 8);
            }
            ID3D12PipelineState* pipeline = Pipeline(pipelineKey, hostVs.bytecode, *psBytecode);
            const D3D12_CPU_DESCRIPTOR_HANDLE msaaDummyView =
                !host && pipelineKey.forcedSamples ? MsaaDummy(uint32_t(right), uint32_t(std::max(drawBottom, 1))) : D3D12_CPU_DESCRIPTOR_HANDLE{};

            list_->SetPipelineState(pipeline);
            if (host)
            {
                D3D12_CPU_DESCRIPTOR_HANDLE rtv = {}, dsv = {};
                if (colorTarget)
                {
                    TransitionTarget(*colorTarget, D3D12_RESOURCE_STATE_RENDER_TARGET);
                    rtv = TargetView(*colorTarget);
                }
                if (depthTarget)
                {
                    TransitionTarget(*depthTarget, D3D12_RESOURCE_STATE_DEPTH_WRITE);
                    dsv = TargetView(*depthTarget);
                }
                list_->OMSetRenderTargets(colorTarget ? 1 : 0, colorTarget ? &rtv : nullptr, FALSE,
                                          depthTarget ? &dsv : nullptr);
                list_->OMSetStencilRef(output.stencilFront & 0xFF);
                list_->OMSetBlendFactor(output.blendConstant);
            }
            else if (pipelineKey.forcedSamples)
                list_->OMSetRenderTargets(1, &msaaDummyView, FALSE, nullptr);
            else
                list_->OMSetRenderTargets(0, nullptr, FALSE, nullptr);
            list_->SetGraphicsRootSignature(rootSignature_.Get());
            list_->SetGraphicsRootConstantBufferView(kRootSystem, UploadData(&system, sizeof(system)));
            list_->SetGraphicsRootConstantBufferView(kRootFloat, UploadConstants(ConstantBlock::kFloat, 0x4000, 512 * 4));
            list_->SetGraphicsRootConstantBufferView(kRootBoolLoop, UploadConstants(ConstantBlock::kBoolLoop, 0x4900, 40));
            list_->SetGraphicsRootConstantBufferView(kRootFetch, UploadConstants(ConstantBlock::kFetch, 0x4800, 192));
            list_->SetGraphicsRootConstantBufferView(kRootOutput, UploadData(&output, sizeof(output)));
            list_->SetGraphicsRootDescriptorTable(kRootMemory, SrvGpu(0));
            list_->SetGraphicsRootDescriptorTable(kRootEdram, SrvGpu(1));
            list_->SetGraphicsRootDescriptorTable(kRootVsTextures, vsTextures);
            list_->SetGraphicsRootDescriptorTable(kRootPsTextures, psTextures);
            list_->SetGraphicsRootDescriptorTable(kRootVsSamplers, vsSamplers);
            list_->SetGraphicsRootDescriptorTable(kRootPsSamplers, psSamplers);
            list_->RSSetViewports(1, &viewport);
            list_->RSSetScissorRects(1, &scissor);
            list_->IASetPrimitiveTopology(rectList ? D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST : topology);
            if (indices || drawIndices)
            {
                D3D12_INDEX_BUFFER_VIEW view = { drawIndices ? drawIndices : indices->GetGPUVirtualAddress(),
                    UINT(hostCount * 4), DXGI_FORMAT_R32_UINT };
                list_->IASetIndexBuffer(&view);
                list_->DrawIndexedInstanced(hostCount, 1, 0, 0, 0);
            }
            else
            {
                list_->DrawInstanced(hostCount, 1, 0, 0);
            }
            if (host)
            {
                if (colorTarget)
                    colorTarget->dirty = true;
                if (depthTarget)
                    depthTarget->dirty = true;
                hostDraws_++;
                if (g_hostLogFrame && SwapCount() + 1 == g_hostLogFrame)
                    Log("host-log   drew {} host: {} vertices, depth target {} control {:08X} viewport z {}-{} samples {}", drawOfFrame_, hostCount,
                        depthTarget ? depthTarget->base : 9999, om.depthControl, viewport.MinDepth, viewport.MaxDepth, pipelineKey.samples);
            }
            else
            {
                for (uint32_t rt = 0; rt < 4; rt++)
                    if (om.colorFormat[rt] != kRtDisabled)
                        EdramWritten(Reg(kColorInfo[rt]) & 0x7FF, rovSpan);
                if (om.depthControl)
                    EdramWritten(depthBase0, rovSpan);
                rovDraws_++;
            }

            if (runsShader && om.colorFormat[0] != kRtDisabled)
            {
                dump_.valid = true;
                dump_.base = output.colorBase[0];
                dump_.pitch = pitch;
                dump_.msaa = msaa;
                dump_.format = om.colorFormat[0];
                dump_.depthBase = output.depthBase;
                if (bottom < 8192) // full-size scissors (clears with the viewport disabled) say nothing about the size
                    dump_.height = std::max<uint32_t>(dump_.height, uint32_t(bottom));
            }
            SubmitBatchIfDue();
        }

        // ---- Helper warm-up (shader_cache/helpers.bin) -----------------------------------------------------------

        // "MCHP", then the key sizes this build writes (another layout: the file starts again), then per helper:
        // type (1 resolve, 2 resolve clear, 3 host pass, 4 depth-only output merger), key size, key bytes.
        constexpr uint32_t kHelpersMagic = 0x5048434D;
        const std::filesystem::path kHelpersPath = std::filesystem::path("shader_cache") / "helpers.bin";

        void Renderer::RecordHelper(uint32_t type, const void* key, uint32_t size)
        {
            if (prewarming_ || !pipelineLibrary_)
                return; // pipelineLibrary_: off with CB_SHADER_CACHE=0, as the shader caches
            if (!helpers_.is_open())
            {
                std::error_code error;
                std::filesystem::create_directories(kHelpersPath.parent_path(), error);
                const bool fresh = !std::filesystem::exists(kHelpersPath, error);
                helpers_.open(kHelpersPath, std::ios::binary | std::ios::app);
                if (fresh)
                {
                    const uint32_t header[3] = { kHelpersMagic, uint32_t(sizeof(ResolveKey)), uint32_t(sizeof(OutputMergerKey)) };
                    helpers_.write(reinterpret_cast<const char*>(header), sizeof(header));
                }
            }
            const uint32_t head[2] = { type, size };
            helpers_.write(reinterpret_cast<const char*>(head), sizeof(head));
            if (size)
                helpers_.write(static_cast<const char*>(key), size);
            helpers_.flush();
        }

        void Renderer::PrewarmHelpers()
        {
            if (!pipelineLibrary_)
                return;
            std::ifstream in(kHelpersPath, std::ios::binary);
            uint32_t header[3] = {};
            if (!in.read(reinterpret_cast<char*>(header), sizeof(header)))
                return;
            if (header[0] != kHelpersMagic || header[1] != sizeof(ResolveKey) || header[2] != sizeof(OutputMergerKey))
            {
                in.close();
                std::error_code error;
                std::filesystem::remove(kHelpersPath, error);
                return;
            }
            const auto start = std::chrono::steady_clock::now();
            prewarming_ = true;
            size_t built = 0;
            for (;;)
            {
                uint32_t head[2];
                if (!in.read(reinterpret_cast<char*>(head), sizeof(head)) || head[1] > 4096)
                    break;
                std::vector<char> key(head[1]);
                if (head[1] && !in.read(key.data(), head[1]))
                    break;
                if (head[0] == 1 && head[1] == sizeof(ResolveKey))
                {
                    ResolveKey k;
                    std::memcpy(&k, key.data(), sizeof(k));
                    ComPtr<ID3D12PipelineState>& pipeline = resolvePipelines_[k];
                    if (!pipeline)
                    {
                        pipeline.Attach(ComputePipeline(ResolveHlsl(k, ResolveTarget::kRenderer), "resolve"));
                        built++;
                    }
                }
                else if (head[0] == 2 && !clearPipeline_)
                {
                    clearPipeline_.Attach(ComputePipeline(ClearHlsl(ResolveTarget::kRenderer), "resolve_clear"));
                    built++;
                }
                else if (head[0] == 3 && head[1] == 4)
                {
                    uint32_t k;
                    std::memcpy(&k, key.data(), 4);
                    if (!hostPassPipelines_.count(k))
                    {
                        HostPassPipeline(k >> 16, (k >> 8) & 0xFF, k & 0xFF);
                        built++;
                    }
                }
                else if (head[0] == 4 && head[1] == sizeof(OutputMergerKey))
                {
                    OutputMergerKey k;
                    std::memcpy(&k, key.data(), sizeof(k));
                    if (!outputMergerOnly_.count(k))
                    {
                        OutputMergerOnlyShader(k);
                        built++;
                    }
                }
            }
            prewarming_ = false;
            Log("GPU: {} helper pipeline(s) prepared in {} ms", built,
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count());
        }

        // ---- Resolves (resolve.h) -------------------------------------------------------------------------------

        ID3D12PipelineState* Renderer::ComputePipeline(const std::string& source, const char* name)
        {
            std::vector<uint8_t> bytecode;
            std::string errors;
            if (!CompileHlsl(std::string(kXenosHlslLibrary) + source, "cs_5_1", name, bytecode, errors))
                Fatal("GPU: {} shader failed to compile: {}\n{}", name, errors, source);
            D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
            desc.pRootSignature = computeRootSignature_.Get();
            desc.CS = { bytecode.data(), bytecode.size() };
            ID3D12PipelineState* pipeline = nullptr;
            CheckHr(device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipeline)), "CreateComputePipelineState");
            return pipeline;
        }

        // Copy, then the clears of the same rectangle (07 §12). The EDRAM barriers order the dispatches after the
        // draws before them, the clears after the copy's reads, and the draws after the clears; guest memory goes
        // to the UAV state for the copy and back to a shader-resource state at the next draw.
        void Renderer::Resolve()
        {
            const ResolveInfo info = DecodeResolve();
            if (info.empty)
                return;
            UploadMemory();
            LogOnce(DescribeResolve(info));
            if (g_hostLogFrame && SwapCount() + 1 == g_hostLogFrame)
                Log("host-log   resolve before draw {}: {}", drawOfFrame_, DescribeResolve(info));
            // Host render targets stay the newest copy where they can (the EDRAM is written back only when something
            // else needs those tiles): the copy reads a dirty target of the source surface directly, and a clear also
            // clears the target of the cleared surface, which so stays in step with the EDRAM.
            auto rowsOf = [](uint32_t base, uint32_t pitchTiles, uint32_t msaa, uint32_t y0, uint32_t height,
                             uint32_t& first, uint32_t& count) {
                const uint32_t top = y0 / TileHeight(msaa), bottom = (y0 + height + TileHeight(msaa) - 1) / TileHeight(msaa);
                first = base + top * pitchTiles;
                count = std::min<uint32_t>((bottom - top) * pitchTiles, 2048);
            };
            auto surfaceTarget = [&](bool depth, uint32_t base, uint32_t pitchTiles, uint32_t msaa, uint32_t x1,
                                     uint32_t y1) -> HostTarget* {
                for (auto& t : hostTargets_)
                    if (t->depth == depth && t->base == base && t->pitchTiles == pitchTiles && t->msaa == msaa &&
                        x1 <= t->width && y1 <= t->height)
                        return t.get();
                return nullptr;
            };
            ResolveKey key = info.key;
            HostTarget* source = nullptr;
            if (info.copy)
            {
                const ResolveConstants& c = info.constants;
                source = surfaceTarget(key.depth, c.sourceBase, c.pitchTiles, key.msaa, c.x0 + c.width, c.y0 + c.height);
                if (source && !source->dirty)
                    source = nullptr; // the EDRAM holds the same data
                uint32_t first, count;
                rowsOf(c.sourceBase, c.pitchTiles, key.msaa, c.y0, c.height, first, count);
                StoreOverlapping(first, count, source);
                if (source)
                    key.hostSource = (source->depth ? 3 : 1) + (HostMs(source->msaa) ? 1 : 0);
            }
            struct ClearOfTarget
            {
                const ClearConstants* clear;
                HostTarget* target; // the target of the cleared surface, or none
                bool inStep;        // it was in step with the EDRAM (or newer) before the clear
                uint32_t first, count;
            };
            std::vector<ClearOfTarget> clears;
            for (const ClearConstants* clear : { &info.depthClear, &info.colorClear })
            {
                if (clear == &info.depthClear ? !info.clearDepth : !info.clearColor)
                    continue;
                ClearOfTarget c = { clear, surfaceTarget(clear->depth != 0, clear->base, clear->pitchTiles, clear->msaa,
                                                         clear->x0 + clear->width, clear->y0 + clear->height) };
                c.inStep = c.target && (c.target->dirty || !Stale(*c.target));
                rowsOf(clear->base, clear->pitchTiles, clear->msaa, clear->y0, clear->height, c.first, c.count);
                StoreOverlapping(c.first, c.count, c.target);
                clears.push_back(c);
            }
            const UINT sourceViews = source ? TargetSrvs(*source) : 0;
            BeginRecording();
            GpuScope scope(*this, 1);
            UavBarrier(edram_.Get());
            list_->SetComputeRootSignature(computeRootSignature_.Get());
            list_->SetComputeRootDescriptorTable(1, SrvGpu(1));
            if (info.copy)
            {
                SetMemoryState(D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
                ComPtr<ID3D12PipelineState>& pipeline = resolvePipelines_[key];
                if (!pipeline)
                {
                    pipeline.Attach(ComputePipeline(ResolveHlsl(key, ResolveTarget::kRenderer), "resolve"));
                    RecordHelper(1, &key, sizeof(key));
                }
                if (source)
                {
                    TransitionTarget(*source, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
                    list_->SetComputeRootDescriptorTable(2, SrvGpu(sourceViews));
                }
                list_->SetPipelineState(pipeline.Get());
                list_->SetComputeRootConstantBufferView(0, UploadData(&info.constants, sizeof(info.constants)));
                list_->Dispatch(info.constants.width / 8, info.constants.height / 8, 1);
                // Stream-order invalidation (07 §10, 05b §5): textures over the written range are untiled again at their
                // next use, by a dispatch recorded after this copy.
                pendingResolves_.emplace_back(info.extentStart, info.extentEnd);
                for (auto& [k, texture] : textures_)
                    for (const auto& [start, end] : texture.layout.extents)
                        if (start < info.extentEnd && info.extentStart < end)
                            texture.dirty = true;
            }
            if (!clears.empty())
            {
                if (!clearPipeline_)
                {
                    clearPipeline_.Attach(ComputePipeline(ClearHlsl(ResolveTarget::kRenderer), "resolve_clear"));
                    RecordHelper(2, nullptr, 0);
                }
                list_->SetPipelineState(clearPipeline_.Get());
                for (const ClearOfTarget& c : clears)
                {
                    UavBarrier(edram_.Get());
                    list_->SetComputeRootConstantBufferView(0, UploadData(c.clear, sizeof(*c.clear)));
                    list_->Dispatch(c.clear->width / 8, c.clear->height / 8, 1);
                }
                for (const ClearOfTarget& c : clears)
                {
                    EdramWritten(c.first, c.count); // other targets over these tiles reload from the cleared EDRAM
                    if (!c.target)
                        continue;
                    HostTarget& t = *c.target;
                    const D3D12_RECT rect = { LONG(c.clear->x0), LONG(c.clear->y0), LONG(c.clear->x0 + c.clear->width),
                                              LONG(c.clear->y0 + c.clear->height) };
                    const uint32_t v = c.clear->value;
                    if (t.depth)
                    {
                        TransitionTarget(t, D3D12_RESOURCE_STATE_DEPTH_WRITE);
                        list_->ClearDepthStencilView(TargetView(t), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
                                                     float(v >> 8) / 16777215.0f, UINT8(v & 0xFF), 1, &rect);
                    }
                    else
                    {
                        TransitionTarget(t, D3D12_RESOURCE_STATE_RENDER_TARGET);
                        const float color[4] = { float(v & 0xFF) / 255.0f, float((v >> 8) & 0xFF) / 255.0f,
                                                 float((v >> 16) & 0xFF) / 255.0f, float(v >> 24) / 255.0f };
                        list_->ClearRenderTargetView(TargetView(t), color, 1, &rect);
                    }
                    if (c.inStep)
                        t.synced = edramClock_;
                }
            }
            UavBarrier(edram_.Get());
            SubmitBatchIfDue();
        }

        // ---- EDRAM dump (CB_EDRAM_DUMP) ------------------------------------------------------------------------

        void WriteBmp(const std::string& path, uint32_t width, uint32_t height, const std::vector<uint32_t>& bgra)
        {
            std::ofstream file(path, std::ios::binary);
            const uint32_t imageSize = width * height * 4;
            uint8_t header[54] = { 'B', 'M' };
            auto put32 = [&](uint32_t offset, uint32_t value) { std::memcpy(header + offset, &value, 4); };
            put32(2, 54 + imageSize);
            put32(10, 54);
            put32(14, 40);
            put32(18, width);
            put32(22, uint32_t(-int32_t(height))); // top-down
            header[26] = 1;
            header[28] = 32;
            put32(34, imageSize);
            file.write(reinterpret_cast<const char*>(header), sizeof(header));
            file.write(reinterpret_cast<const char*>(bgra.data()), imageSize);
        }

        void Renderer::DumpEdram(uint64_t frame)
        {
            Sync();
            ComPtr<ID3D12Resource> readback;
            readback.Attach(CreateBuffer(kEdramBytes, D3D12_HEAP_TYPE_READBACK, D3D12_RESOURCE_FLAG_NONE, "EDRAM readback"));
            BeginRecording();
            Transition(edram_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
            list_->CopyBufferRegion(readback.Get(), 0, edram_.Get(), 0, kEdramBytes);
            Transition(edram_.Get(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
            Sync();
            std::vector<uint32_t> edram(kEdramBytes / 4);
            void* mapped = nullptr;
            D3D12_RANGE range = { 0, kEdramBytes };
            CheckHr(readback->Map(0, &range, &mapped), "EDRAM readback Map");
            std::memcpy(edram.data(), mapped, kEdramBytes);
            readback->Unmap(0, nullptr);

            const std::string base = std::format("edram_frame{}", frame);
            std::ofstream(base + ".bin", std::ios::binary).write(reinterpret_cast<const char*>(edram.data()), kEdramBytes);
            if (!dump_.valid)
            {
                Log("GPU: EDRAM dumped to {}.bin (no colour surface drawn yet)", base);
                return;
            }
            const uint32_t width = dump_.pitch, height = dump_.height ? dump_.height : 720;
            const uint32_t pitchTiles = (dump_.pitch * (dump_.msaa == 2 ? 2 : 1) + 79) / 80;
            std::vector<uint32_t> color(size_t(width) * height), depth(size_t(width) * height);
            for (uint32_t y = 0; y < height; y++)
                for (uint32_t x = 0; x < width; x++)
                {
                    uint32_t qx, qy;
                    EdramSample(dump_.msaa, x, y, 0, qx, qy);
                    const uint32_t c = edram[EdramAddress(dump_.base, pitchTiles, qx, qy, false) / 4];
                    uint32_t bgra;
                    if (dump_.format == 0) // bytes R, G, B, A -> B, G, R, A
                        bgra = (c & 0xFF00FF00u) | ((c >> 16) & 0xFF) | ((c & 0xFF) << 16);
                    else // 16_16: high bytes of R and G
                        bgra = 0xFF000000u | (((c >> 8) & 0xFF) << 16) | (((c >> 24) & 0xFF) << 8);
                    color[size_t(y) * width + x] = bgra;
                    const uint32_t d = edram[EdramAddress(dump_.depthBase, pitchTiles, qx, qy, true) / 4];
                    const uint32_t g = d >> 24;
                    depth[size_t(y) * width + x] = 0xFF000000u | g << 16 | g << 8 | g;
                }
            WriteBmp(base + ".bmp", width, height, color);
            WriteBmp(base + "_depth.bmp", width, height, depth);
            Log("GPU: EDRAM dumped: {}.bmp ({}x{} {} at tile {}, MSAA {}), {}_depth.bmp (tile {}), {}.bin", base, width,
                height, EdramColorFormatName(dump_.format), dump_.base, 1u << dump_.msaa, base, dump_.depthBase, base);
        }

        // CB_FRONT_DUMP=<frame>: the front buffer as the title hands it to VdSwap (LE_X8R8G8B8, 2D tiled: memory bytes
        // B, G, R, X per pixel, 08_present), detiled on the CPU into front_frame<N>.bmp.
        void Renderer::DumpFrontBuffer(uint64_t frame, uint32_t frontBuffer, uint32_t width, uint32_t height)
        {
            Sync();
            const uint32_t pitchMacro = (width + 31) / 32;
            const uint32_t size = TiledOffset2D((width - 1) & ~31u, (height - 1) & ~31u, pitchMacro, 2) + 0x1000;
            if (width == 0 || height == 0 || uint64_t(frontBuffer) + size > guest::kPhysicalSize)
            {
                Log("GPU: front buffer {:08X} {}x{} cannot be dumped", frontBuffer, width, height);
                return;
            }
            const uint8_t* source = guest::g_memory.Translate(guest::kPhysicalWindow64k + frontBuffer);
            std::vector<uint32_t> bgra(size_t(width) * height);
            for (uint32_t y = 0; y < height; y++)
                for (uint32_t x = 0; x < width; x++)
                {
                    uint32_t v;
                    std::memcpy(&v, source + TiledOffset2D(x, y, pitchMacro, 2), 4);
                    bgra[size_t(y) * width + x] = v | 0xFF000000u;
                }
            const std::string path = std::format("front_frame{}.bmp", frame);
            WriteBmp(path, width, height, bgra);
            Log("GPU: front buffer {:08X} ({}x{}) dumped to {}", frontBuffer, width, height, path);
        }

        // ---- Presentation (present.h, presenter.h) ---------------------------------------------------------------

        // The display gamma table, logged at the first swaps and whenever its content changes (08 §5.3: D3D's
        // SetupGamma writes it from ComplexSwap; the write counts show how often).
        void Renderer::LogGamma(uint64_t frame)
        {
            static uint32_t logged[256];
            static uint64_t loggedWrites = 0, loggedFrame = 0;
            static int changes = 0;
            const DisplayGamma& g = CurrentDisplayGamma();
            const bool changed = std::memcmp(logged, g.table, sizeof(logged)) != 0;
            if (!(frame <= 3 || (changed && changes < 8)))
                return;
            changes += changed && frame > 3;
            bool gray = true, identity = true;
            std::string red;
            for (uint32_t i = 0; i < 256; i++)
            {
                const uint32_t r = (g.table[i] >> 20) & 0x3FF, gr = (g.table[i] >> 10) & 0x3FF, b = g.table[i] & 0x3FF;
                gray &= r == gr && r == b;
                identity &= r == i * 0x3FF / 0xFF && gray;
                red += std::format("{}{}", i ? " " : "", r);
            }
            Log("GPU: display gamma at frame {}: {} component writes since frame {}; {}; {}{}", frame,
                g.tableWrites - loggedWrites, loggedFrame, identity ? "identity table (Xenia's default)"
                    : gray ? "R = G = B" : "channels differ", changed ? "red: " : "unchanged", changed ? red : "");
            std::memcpy(logged, g.table, sizeof(logged));
            loggedWrites = g.tableWrites;
            loggedFrame = frame;
        }

        // Asks the system UI for this frame's overlay and uploads it when it changed (recording). Returns whether
        // there is one to blend; the texture is left in the COMMON state.
        bool Renderer::UpdateOverlay(uint32_t width, uint32_t height)
        {
            std::vector<uint8_t> pixels;
            bool changed = false;
            const auto started = std::chrono::steady_clock::now();
            const bool visible = xui::RenderOverlay(width, height, VblankCount(), pixels, changed);
            if (changed)
            {
                // Drawing cost while the overlay animates (it is drawn on this thread, which the title waits for).
                static double total = 0, worst = 0;
                static int count = 0;
                const double ms =
                    std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - started).count();
                total += ms;
                worst = std::max(worst, ms);
                if (++count == 120)
                {
                    Log("System UI: overlay drawn in {:.1f} ms on average, {:.1f} ms at most (last {} redraws)",
                        total / count, worst, count);
                    total = worst = 0;
                    count = 0;
                }
            }
            if (!overlay_ || overlayWidth_ != width || overlayHeight_ != height)
            {
                D3D12_HEAP_PROPERTIES heap{ D3D12_HEAP_TYPE_DEFAULT };
                D3D12_RESOURCE_DESC desc{ D3D12_RESOURCE_DIMENSION_TEXTURE2D, 0, width, height, 1, 1,
                                          DXGI_FORMAT_R8G8B8A8_UINT, { 1, 0 }, D3D12_TEXTURE_LAYOUT_UNKNOWN,
                                          D3D12_RESOURCE_FLAG_NONE };
                overlay_.Reset();
                CheckHr(device_->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON,
                                                         nullptr, IID_PPV_ARGS(&overlay_)), "system UI overlay texture");
                overlay_->SetName(L"system UI overlay");
                device_->CreateShaderResourceView(overlay_.Get(), nullptr, SrvCpu(kOverlaySrv));
                overlayWidth_ = width;
                overlayHeight_ = height;
                overlayPixels_.clear();
            }
            if (!visible)
            {
                overlayPixels_.clear();
                return false;
            }
            if (changed || overlayPixels_.empty())
            {
                overlayPixels_ = std::move(pixels);
                const uint64_t rowBytes = uint64_t(width) * 4;
                const uint64_t pitch = (rowBytes + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1) &
                    ~uint64_t(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1);
                const Upload upload = Allocate(pitch * height, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
                for (uint32_t y = 0; y < height; y++)
                    std::memcpy(upload.cpu + y * pitch, overlayPixels_.data() + y * rowBytes, rowBytes);
                D3D12_TEXTURE_COPY_LOCATION dst = { overlay_.Get(), D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX };
                D3D12_TEXTURE_COPY_LOCATION src = { upload.resource, D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT };
                src.PlacedFootprint.Offset = upload.offset;
                src.PlacedFootprint.Footprint = { DXGI_FORMAT_R8G8B8A8_UINT, width, height, 1, UINT(pitch) };
                Transition(overlay_.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
                list_->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
                Transition(overlay_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON);
            }
            return true;
        }

        // The front buffer through the display gamma into a guest-output texture, handed to the window. With
        // `readback`, the result (R10G10B10A2 pixels) is also copied back. Returns the guest output's index.
        uint32_t Renderer::Present(const PresentConstants& constants, std::vector<uint32_t>* readback)
        {
            const GuestOutput out = AcquireGuestOutput(constants.width, constants.height);
            SavePipelineLibrary(false);
            if (static uint32_t presents = 0; ++presents == 600)
                Log("GPU: pipelines over the first 600 frames: {} built in {:.1f} ms, {} loaded from the cache in {:.1f} ms",
                    pipelinesCreated_, pipelineCreateMs_, pipelinesLoaded_, pipelineLoadMs_);
            Sync(SyncReason::kPresent);
            // Ordered before the next submission: the window has finished reading this texture.
            CheckHr(queue_->Wait(out.readFence, out.readValue), "renderer queue Wait (guest output)");
            D3D12_UNORDERED_ACCESS_VIEW_DESC uav = {};
            uav.Format = DXGI_FORMAT_R10G10B10A2_UINT; // integer stores: the 10-bit ramp values exactly
            uav.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
            device_->CreateUnorderedAccessView(out.texture, nullptr, &uav, SrvCpu(kGuestOutputUavs + out.index));
            BeginRecording();
            PresentConstants c = constants;
            const bool systemUi = UpdateOverlay(c.width, c.height);
            UploadMemory();
            if (c.overlay != 2) // XAM's own overlay (CB_RECOMP_XAM) replaces the runtime's system UI
                c.overlay = systemUi ? 1 : 0;
            SetMemoryState(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
            Transition(out.texture, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
            Transition(overlay_.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
            list_->SetComputeRootSignature(presentRootSignature_.Get());
            list_->SetPipelineState(presentPipeline_.Get());
            list_->SetComputeRootConstantBufferView(0, UploadData(&c, sizeof(c)));
            list_->SetComputeRootDescriptorTable(1, SrvGpu(0));
            list_->SetComputeRootDescriptorTable(2, SrvGpu(kGuestOutputUavs + out.index));
            list_->SetComputeRootDescriptorTable(3, SrvGpu(kOverlaySrv));
            list_->Dispatch((c.width + 15) / 16, (c.height + 7) / 8, 1);
            Transition(overlay_.Get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COMMON);
            ComPtr<ID3D12Resource> readbackBuffer;
            D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
            if (readback)
            {
                Transition(out.texture, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
                const D3D12_RESOURCE_DESC desc = out.texture->GetDesc();
                UINT64 total = 0;
                device_->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, nullptr, nullptr, &total);
                readbackBuffer.Attach(CreateBuffer(total, D3D12_HEAP_TYPE_READBACK, D3D12_RESOURCE_FLAG_NONE,
                                                   "guest output readback"));
                D3D12_TEXTURE_COPY_LOCATION dst = { readbackBuffer.Get(), D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT };
                dst.PlacedFootprint = footprint;
                D3D12_TEXTURE_COPY_LOCATION src = { out.texture, D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX };
                src.SubresourceIndex = 0;
                list_->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
                Transition(out.texture, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COMMON);
            }
            else
                Transition(out.texture, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON);
            Sync(SyncReason::kPresent);
            SubmitGuestOutput(out.index, fence_.Get(), fenceValue_);
            if (readback)
            {
                readback->resize(size_t(c.width) * c.height);
                const uint8_t* mapped = nullptr;
                CheckHr(readbackBuffer->Map(0, nullptr, reinterpret_cast<void**>(const_cast<uint8_t**>(&mapped))),
                        "guest output readback Map");
                for (uint32_t y = 0; y < c.height; y++)
                    std::memcpy(readback->data() + size_t(y) * c.width,
                                mapped + footprint.Offset + size_t(y) * footprint.Footprint.RowPitch, size_t(c.width) * 4);
                readbackBuffer->Unmap(0, nullptr);
            }
            return out.index;
        }

        void Renderer::Swap(uint64_t frame, uint32_t frontBuffer, uint32_t width, uint32_t height,
                            const GuestOverlay& overlay)
        {
            GpuScope scope(*this, 3);
            frame_ = frame;
            if (g_dumpFrame != 0 && frame == g_dumpFrame)
            {
                StoreAllDirty();
                DumpEdram(frame);
            }
            if (g_frontDumpFrame != 0 && frame == g_frontDumpFrame)
                DumpFrontBuffer(frame, frontBuffer, width, height);
            dump_.height = 0;
            LogGamma(frame);
            // Decoded even without a window, so a front buffer the presenter cannot show stops the run either way.
            PresentConstants constants = DecodePresent(frontBuffer, width, height, CurrentDisplayGamma());
            if (overlay.base != 0)
                SetGuestOverlay(constants, overlay);
            LogOnce(DescribePresent(constants));
            // CB_THUMBNAIL=<frame>:<file>: that frame's output, a quarter of its size each way (box filtered), as a
            // BMP, then the process ends (the launcher's thumbnails of each booted dashboard, launcher.cpp).
            static const auto thumbnail = [] {
                std::pair<uint64_t, std::string> t{ 0, "" };
                const char* v = std::getenv("CB_THUMBNAIL");
                if (const char* colon = v ? std::strchr(v, ':') : nullptr)
                    t = { std::strtoull(v, nullptr, 10), std::string(colon + 1) };
                return t;
            }();
            const bool thumbnailFrame = thumbnail.first != 0 && frame == thumbnail.first;
            const bool dumpPresent = IsPresentDumpFrame(frame) || thumbnailFrame;
            if (!PresenterActive() && !dumpPresent)
            {
                xui::AdvanceOverlay(VblankCount());
                return;
            }
            std::vector<uint32_t> pixels;
            Present(constants, dumpPresent ? &pixels : nullptr);
            if (!dumpPresent)
                return;
            if (thumbnailFrame)
            {
                const uint32_t tw = width / 4, th = height / 4;
                std::vector<uint32_t> picture(size_t(tw) * th);
                for (uint32_t y = 0; y < th; y++)
                    for (uint32_t x = 0; x < tw; x++)
                    {
                        uint32_t sum[3] = {};
                        for (uint32_t dy = 0; dy < 4; dy++)
                            for (uint32_t dx = 0; dx < 4; dx++)
                            {
                                const uint32_t p = pixels[size_t(y * 4 + dy) * width + x * 4 + dx];
                                for (int c = 0; c < 3; c++)
                                    sum[c] += (p >> (10 * c)) & 0x3FF; // R10G10B10A2: red lowest
                            }
                        auto to8 = [](uint32_t v10x16) { return (v10x16 * 255 / 16 + 511) / 1023; };
                        picture[size_t(y) * tw + x] = 0xFF000000u | to8(sum[0]) << 16 | to8(sum[1]) << 8 | to8(sum[2]);
                    }
                WriteBmp(thumbnail.second, tw, th, picture);
                Log("GPU: frame {} written to {} ({}x{}): CB_THUMBNAIL ends the run", frame, thumbnail.second, tw, th);
                logging::Flush();
                TerminateProcess(GetCurrentProcess(), 0);
            }

            // CB_PRESENT_DUMP: exactly what the window gets (before its scaling), checked against the CPU reference.
            std::vector<uint32_t> expected;
            PresentConstants withOverlay = constants;
            if (withOverlay.overlay != 2)
                withOverlay.overlay = overlayPixels_.empty() ? 0 : 1;
            ReferencePresent(withOverlay, guest::g_memory.Translate(guest::kPhysicalWindow64k),
                             overlayPixels_.empty() ? nullptr : overlayPixels_.data(), expected);
            uint32_t mismatches = 0;
            std::vector<uint32_t> bgra(pixels.size());
            for (size_t i = 0; i < pixels.size(); i++)
            {
                mismatches += pixels[i] != expected[i];
                auto to8 = [](uint32_t v10) { return (v10 * 255 + 511) / 1023; }; // UNORM 10 -> 8, round to nearest
                const uint32_t p = pixels[i];
                bgra[i] = 0xFF000000u | to8(p & 0x3FF) << 16 | to8((p >> 10) & 0x3FF) << 8 | to8((p >> 20) & 0x3FF);
            }
            if (withOverlay.overlay == 2)
            {
                // XAM's overlay texture alone (colour, then alpha as grey), detiled as presented.
                const uint8_t* memory = guest::g_memory.Translate(guest::kPhysicalWindow64k);
                const uint32_t ow = withOverlay.ovWidth, oh = withOverlay.ovHeight;
                std::vector<uint32_t> colour(size_t(ow) * oh * 2);
                for (uint32_t y = 0; y < oh; y++)
                    for (uint32_t x = 0; x < ow; x++)
                    {
                        const uint32_t offset = withOverlay.ovTiled ? TiledOffset2D(x, y, withOverlay.ovPitch / 32, 2)
                                                                    : (y * withOverlay.ovPitch + x) * 4;
                        uint32_t v;
                        std::memcpy(&v, memory + withOverlay.ovBase + offset, 4);
                        if (withOverlay.ovEndian == 2)
                            v = __builtin_bswap32(v);
                        colour[size_t(y) * ow + x] = 0xFF000000u | (v & 0xFFFFFF);
                        const uint32_t a = v >> 24;
                        colour[size_t(oh + y) * ow + x] = 0xFF000000u | a << 16 | a << 8 | a;
                    }
                WriteBmp(std::format("xam_overlay_frame{}.bmp", frame), ow, oh * 2, colour);
            }
            const std::string path = std::format("present_frame{}.bmp", frame);
            WriteBmp(path, width, height, bgra);
            Log("GPU: present output of frame {} dumped to {} (10-bit result rounded to 8 bits); {} of {} pixels differ "
                "from the CPU reference (detile + {}gamma table)", frame, path, mismatches, pixels.size(),
                withOverlay.overlay == 2 ? "XAM's overlay + " : overlayPixels_.empty() ? "" : "system UI overlay + ");
        }

        // ---- Integration test (CB_SHADER_TEST=1, after the shader tests) ------------------------------------------
        // Real COPY draws through Draw/Resolve on a fresh renderer: a colour resolve with colour and depth clears, a
        // second resolve of the cleared surface, a depth resolve, and a tiled texture that is uploaded before a
        // resolve overwrites it in the same frame and must be uploaded again with the new data.
        int Renderer::SelfTest()
        {
            int failures = 0;
            auto check = [&](bool ok, const std::string& what) {
                if (!ok)
                {
                    Log("Renderer test: FAILED: {}", what);
                    failures++;
                }
            };
            uint8_t* physical = guest::g_memory.Translate(guest::kPhysicalWindow64k);
            constexpr uint32_t kVertices = 0x00300000, kDestA = 0x00400000, kDestB = 0x00500000, kDestC = 0x00600000;
            constexpr uint32_t kWidth = 256, kHeight = 128, kDepthBase = 100;
            constexpr uint32_t kColorClear = 0x11223344, kDepthClear = 0xAABBCC01;
            std::memset(physical + kDestA, 0xCD, 0x300000);
            const float vertices[6] = { -0.5f, -0.5f, kWidth - 0.5f, -0.5f, kWidth - 0.5f, kHeight - 0.5f };
            for (uint32_t i = 0; i < 6; i++)
            {
                const uint32_t be = __builtin_bswap32(std::bit_cast<uint32_t>(vertices[i]));
                std::memcpy(physical + kVertices + 4 * i, &be, 4);
            }
            SetRegister(0x4800, kVertices | 3);      // vertex fetch constant 0: the rectangle
            SetRegister(0x4801, 0x1000001A);         // 6 dwords, 8in32
            SetRegister(0x2302, 0);                  // PixCenter ZERO
            SetRegister(0x2080, 0);
            SetRegister(0x2205, 0);
            SetRegister(0x2081, 0x80000000u);
            SetRegister(0x2082, 0x20002000u);
            SetRegister(0x200E, 0);
            SetRegister(0x200F, 0x20002000u);
            SetRegister(0x2000, kWidth);             // RB_SURFACE_INFO: pitch 256, 1x
            SetRegister(0x2001, 0);                  // RT0 8_8_8_8 at tile 0
            SetRegister(0x2002, kDepthBase);         // D24S8 at tile 100
            SetRegister(0x2208, 6);                  // COPY
            SetRegister(0x231A, kHeight << 16 | kWidth);
            SetRegister(0x231B, 6u << 7);            // k_8_8_8_8, no swap, no endian swap
            SetRegister(0x231D, kDepthClear);
            SetRegister(0x231E, kColorClear);
            SetRegister(0x231F, kColorClear);
            // Texture fetch constant 1: tiled k_8_8_8_8 over destination B.
            SetRegister(0x4806, 2u | (kWidth / 32) << 22 | 1u << 31);
            SetRegister(0x4807, kDestB | 6u);
            SetRegister(0x4808, (kWidth - 1) | (kHeight - 1) << 13);
            SetRegister(0x4809, 0);
            SetRegister(0x480A, 0);
            SetRegister(0x480B, 1u << 9);
            const DrawPacket copy = { 0x36, 0, 0x00030088, 0, 0, false };

            auto readTexture = [&](const Texture& texture, std::vector<uint32_t>& out) {
                const TextureReadback readback = RecordTextureReadback(const_cast<Texture&>(texture));
                Sync();
                const std::vector<std::vector<uint8_t>> levels = FinishTextureReadback(readback);
                out.resize(levels[0].size() / 4);
                std::memcpy(out.data(), levels[0].data(), levels[0].size());
            };
            auto allTexels = [](const std::vector<uint32_t>& texels, uint32_t value) {
                return std::all_of(texels.begin(), texels.end(), [&](uint32_t t) { return t == value; });
            };
            auto destHolds = [&](uint32_t dest, uint32_t value) {
                uint32_t bad = 0;
                std::vector<bool> written(0x100000);
                for (uint32_t y = 0; y < kHeight; y++)
                    for (uint32_t x = 0; x < kWidth; x++)
                    {
                        const uint32_t offset = TiledOffset2D(x, y, kWidth / 32, 2);
                        uint32_t v;
                        std::memcpy(&v, physical + dest + offset, 4);
                        bad += v != value;
                        for (uint32_t b = 0; b < 4; b++)
                            written[offset + b] = true;
                    }
                for (uint32_t i = 0; i < 0x100000; i++)
                    bad += !written[i] && physical[dest + i] != 0xCD;
                return bad == 0;
            };

            // Texture B before any resolve: the 0xCD fill.
            std::vector<uint32_t> texels;
            BeginRecording(); // as in Draw: untiles are recorded
            const Texture* texture = &TextureFor(1);
            check(texture != &placeholder_, "tiled k_8_8_8_8 texture is supported");
            readTexture(*texture, texels);
            check(texels.size() == kWidth * kHeight && allTexels(texels, 0xCDCDCDCDu),
                  "texture B before the resolve shows the fill pattern");

            // 1: RT0 (all zero) -> A, then clear colour and depth.
            SetRegister(0x2318, 0x00100300u);
            SetRegister(0x2319, kDestA);
            Draw(copy, nullptr, nullptr);
            // 2: RT0 (now the clear colour) -> B, no clears. Texture B must be invalidated and untiled again after the
            // copy in the same recording, without waiting for the GPU.
            SetRegister(0x2318, 0x00100000u);
            SetRegister(0x2319, kDestB);
            Draw(copy, nullptr, nullptr);
            check(texture->dirty, "texture B is marked for untiling by the resolve");
            const bool stillRecording = recording_ && !pendingResolves_.empty();
            check(&TextureFor(1) == texture, "texture B keeps its host texture");
            check(stillRecording && recording_ && !pendingResolves_.empty(), "texture B untiled without a GPU wait");
            readTexture(*texture, texels);
            check(allTexels(texels, kColorClear), "texture B after the resolve shows the resolved data (same frame)");
            // 3: depth -> C (raw, k_24_8).
            SetRegister(0x2318, 0x00100004u);
            SetRegister(0x2319, kDestC);
            Draw(copy, nullptr, nullptr);
            Sync();
            check(destHolds(kDestA, 0), "destination A holds the surface before the clear, nothing outside the rectangle");
            check(destHolds(kDestB, kColorClear), "destination B holds the colour clear value");
            check(destHolds(kDestC, kDepthClear), "destination C holds the depth clear value");

            // The EDRAM afterwards: colour and depth surfaces hold their clear values.
            ComPtr<ID3D12Resource> readback;
            readback.Attach(CreateBuffer(kEdramBytes, D3D12_HEAP_TYPE_READBACK, D3D12_RESOURCE_FLAG_NONE, "test EDRAM readback"));
            BeginRecording();
            Transition(edram_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
            list_->CopyBufferRegion(readback.Get(), 0, edram_.Get(), 0, kEdramBytes);
            Transition(edram_.Get(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
            Sync();
            const uint32_t* edram = nullptr;
            CheckHr(readback->Map(0, nullptr, reinterpret_cast<void**>(const_cast<uint32_t**>(&edram))), "test EDRAM Map");
            uint32_t badColor = 0, badDepth = 0;
            for (uint32_t y = 0; y < kHeight; y++)
                for (uint32_t x = 0; x < kWidth; x++)
                {
                    badColor += edram[EdramAddress(0, 4, x, y, false) / 4] != kColorClear;
                    badDepth += edram[EdramAddress(kDepthBase, 4, x, y, true) / 4] != kDepthClear;
                }
            readback->Unmap(0, nullptr);
            check(badColor == 0, std::format("EDRAM colour surface cleared ({} wrong)", badColor));
            check(badDepth == 0, std::format("EDRAM depth surface cleared ({} wrong)", badDepth));
            Log("Renderer test: resolve/clear/texture integration: {} failures", failures);

            // Display gamma register semantics (command processor, 08 §5.2), then the present pass on the GPU against
            // the CPU reference for tiled and linear front buffers with every endian mode.
            const int before = failures;
            uint32_t seed = 0x2545F491u;
            auto next = [&] { seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; return seed; };
            const DisplayGamma& gamma = CurrentDisplayGamma();
            TestWriteRegister(reg::kDcLutRwMode, 0);
            TestWriteRegister(reg::kDcLutRwIndex, 0);
            TestWriteRegister(reg::kDcLutWriteEnMask, 7);
            std::vector<uint32_t> ramp(256);
            for (uint32_t i = 0; i < 256; i++) // as D3D's SetRawGammaRamp: 30_COLOR, then RW_INDEX = i + 1
            {
                ramp[i] = next() & 0x3FFFFFFFu;
                TestWriteRegister(reg::kDcLut30Color, ramp[i] | 0xC0000000u);
                TestWriteRegister(reg::kDcLutRwIndex, i + 1);
            }
            check(std::equal(ramp.begin(), ramp.end(), gamma.table), "gamma: D3D's 256-entry ramp sequence");
            check(ReadRegister(reg::kDcLutRwIndex) == 0x100, "gamma: RW_INDEX 0x100 after the ramp");
            TestWriteRegister(reg::kDcLutRwIndex, 0x1FE); // 30_COLOR auto-increment wraps in 8 bits, keeps bit 8
            for (uint32_t i = 0; i < 3; i++)
                TestWriteRegister(reg::kDcLut30Color, 0x10000000u + i);
            check(gamma.table[0xFE] == 0x10000000u && gamma.table[0xFF] == 0x10000001u && gamma.table[0] == 0x10000002u &&
                  ReadRegister(reg::kDcLutRwIndex) == 0x101, "gamma: 30_COLOR index auto-increment");
            TestWriteRegister(reg::kDcLutWriteEnMask, 5); // blue and red only
            TestWriteRegister(reg::kDcLutRwIndex, 10);
            const uint32_t old10 = gamma.table[10];
            TestWriteRegister(reg::kDcLut30Color, 0x3FFFFFFFu);
            check(gamma.table[10] == ((old10 & 0x000FFC00u) | 0x3FF003FFu), "gamma: 30_COLOR write-enable mask");
            TestWriteRegister(reg::kDcLutWriteEnMask, 7);
            TestWriteRegister(reg::kDcLutRwIndex, 20); // SEQ_COLOR: R, G, B (bits 6-15), then the next entry
            TestWriteRegister(reg::kDcLutSeqColor, 0x123u << 6 | 0x3F);
            TestWriteRegister(reg::kDcLutSeqColor, 0x045u << 6);
            TestWriteRegister(reg::kDcLutSeqColor, 0x3FEu << 6 | 0xFFFF0000u);
            const uint32_t old21 = gamma.table[21];
            TestWriteRegister(reg::kDcLutWriteEnMask, 2); // green only
            for (uint32_t v : { 0x001u, 0x002u, 0x003u })
                TestWriteRegister(reg::kDcLutSeqColor, v << 6);
            check(gamma.table[20] == (0x123u << 20 | 0x045u << 10 | 0x3FEu) &&
                  gamma.table[21] == ((old21 & ~0x000FFC00u) | 0x002u << 10) && ReadRegister(reg::kDcLutRwIndex) == 22,
                  "gamma: SEQ_COLOR component order, masks and index");
            TestWriteRegister(reg::kDcLutWriteEnMask, 7);
            TestWriteRegister(reg::kDcLutRwMode, 1); // PWL: three components per entry, 7-bit index, bit 7 kept
            TestWriteRegister(reg::kDcLutRwIndex, 0xFF);
            for (uint32_t v : { 0x0200FFFFu, 0x1234ABCDu, 0xFFFFFFFFu })
                TestWriteRegister(reg::kDcLutPwlData, v);
            check(gamma.pwl[0x7F][0] == 0x0200FFC0u && gamma.pwl[0x7F][1] == 0x1200ABC0u && gamma.pwl[0x7F][2] == 0xFFC0FFC0u &&
                  ReadRegister(reg::kDcLutRwIndex) == 0x80, "gamma: PWL data, masking and index wrap");
            TestWriteRegister(reg::kDcLutRwMode, 0);

            struct PresentCase { uint32_t width, height, pitch; bool tiled; uint32_t endian; };
            const PresentCase cases[] = { { 1280, 720, 1280, true, 0 }, { 200, 100, 224, false, 2 }, { 100, 50, 128, true, 1 },
                                          { 64, 40, 64, true, 3 }, { 33, 7, 64, false, 0 } };
            constexpr uint32_t kFront = 0x00700000;
            for (const PresentCase& pc : cases)
            {
                for (uint32_t i = 0; i < 0x400000; i += 4)
                {
                    const uint32_t v = next();
                    std::memcpy(physical + kFront + i, &v, 4);
                }
                DisplayGamma table = {};
                for (uint32_t& t : table.table)
                    t = next() & 0x3FFFFFFFu;
                SetRegister(0x4800, 2u | (pc.pitch / 32) << 22 | (pc.tiled ? 1u : 0u) << 31);
                SetRegister(0x4801, kFront | pc.endian << 6 | 6u);
                SetRegister(0x4802, (pc.width - 1) | (pc.height - 1) << 13);
                SetRegister(0x4803, (2u | 1u << 3 | 0u << 6 | 5u << 9) << 1);
                SetRegister(0x4804, 0);
                SetRegister(0x4805, 1u << 9);
                const PresentConstants constants = DecodePresent(kFront, pc.width, pc.height, table);
                std::vector<uint32_t> got, expected;
                Present(constants, &got);
                ReferencePresent(constants, physical, nullptr, expected);
                uint32_t bad = 0;
                for (size_t i = 0; i < got.size(); i++)
                    bad += got[i] != expected[i];
                check(bad == 0, std::format("present {}x{} pitch {} {} endian {}: {} of {} pixels differ", pc.width,
                                            pc.height, pc.pitch, pc.tiled ? "tiled" : "linear", pc.endian, bad, got.size()));
            }
            Log("Renderer test: display gamma registers and present pass: {} failures", failures - before);
            failures += TextureTests();
            return failures;
        }

        // ---- Texture tests (CB_SHADER_TEST=1, after the integration test) ------------------------------------------
        // 1. Random fetch constants (every implemented format, endian, linear/tiled, sizes incl. non-power-of-two and
        //    tail-only, packed or not, level ranges, number formats): the GPU untiler's output of every level against
        //    the CPU reference, byte for byte, and every byte the reference reads lies inside the texture's extents.
        // 2. The XDK's own layouts (analysis/texture_ref/xdk_texture_cases.bin from tools/xdk_texture_ref: fetch
        //    constants from XGSetTextureHeaderEx, levels tiled by XGTileTextureLevel): every level against the image
        //    that was tiled, through the GPU path and through the CPU reference.
        // 3. Sampling: point-sampled texel centres through the real sampler setup and xe_tex_finish for every exact
        //    address mode, border black/white/force-W, ClampPolicy, level ranges and BASEMAP, swizzles, exp adjust,
        //    bias, signed and integer numbers, against values computed on the CPU.
        int Renderer::TextureTests()
        {
            int failures = 0;
            auto check = [&](bool ok, const std::string& what) {
                if (!ok)
                {
                    Log("Texture test: FAILED: {}", what);
                    failures++;
                }
            };
            quietTextures_ = true;
            uint8_t* physical = guest::g_memory.Translate(guest::kPhysicalWindow64k);
            constexpr uint32_t kArea = 0x01000000, kAreaSize = 0x01000000;
            uint32_t seed = 0x9E3779B9u;
            auto next = [&] { seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; return seed; };
            auto range = [&](uint32_t lo, uint32_t hi) { return lo + next() % (hi - lo + 1); };
            auto setFetch = [&](uint32_t tf, const uint32_t d[6]) {
                for (uint32_t i = 0; i < 6; i++)
                    SetRegister(0x4800 + tf * 6 + i, d[i]);
            };
            auto readLevels = [&](const Texture& texture) {
                const TextureReadback readback = RecordTextureReadback(const_cast<Texture&>(texture));
                Sync();
                return FinishTextureReadback(readback);
            };

            // 1: random constants against the reference.
            {
                for (uint32_t i = 0; i < kAreaSize; i += 4)
                {
                    const uint32_t v = next();
                    std::memcpy(physical + kArea + i, &v, 4);
                }
                const uint32_t formats[4] = { 2, 6, 10, 25 };
                uint32_t tested = 0, levelsCompared = 0, bad = 0;
                std::map<std::string, uint32_t> skipped;
                for (int attempt = 0; attempt < 600 && tested < 400; attempt++)
                {
                    const uint32_t format = formats[range(0, 3)];
                    const uint32_t bucket = range(0, 9);
                    auto dimension = [&] { return bucket < 4 ? range(1, 20) : bucket < 8 ? range(1, 120) : range(1, 700); };
                    const uint32_t width = dimension(), height = dimension();
                    const bool tiled = range(0, 1), packed = range(0, 2) != 0;
                    const uint32_t pitch = (width + 31) / 32 * 32 + 32 * (range(0, 3) == 0 ? range(1, 2) : 0);
                    const uint32_t maxField = range(0, 3) == 0 ? 0 : range(0, 15);
                    const uint32_t minField = range(0, 4) == 0 ? range(0, maxField) : 0;
                    uint32_t signs = 0, numFormat = 0;
                    switch (range(0, 4))
                    {
                    case 0: case 1: break;                                    // unsigned fractions
                    case 2: signs = 0x55; break;                              // signed fractions
                    case 3: for (uint32_t c = 0; c < 4; c++) signs |= range(0, 1) * 2 << (2 * c); break; // bias
                    default: numFormat = 1; for (uint32_t c = 0; c < 4; c++) signs |= range(0, 1) << (2 * c); break;
                    }
                    // Components the format lacks take the last one's sign.
                    const uint32_t components = TextureComponents(format);
                    const uint32_t lastSign = (signs >> (2 * (components - 1))) & 3;
                    for (uint32_t c = components; c < 4; c++)
                        signs = (signs & ~(3u << (2 * c))) | lastSign << (2 * c);
                    const uint32_t basePage = (kArea >> 12) + range(0, 0x800);
                    const uint32_t mipPage = range(0, 3) == 0 ? 0 : (kArea >> 12) + range(0, 0x800);
                    uint32_t d[6];
                    d[0] = 2u | signs << 2 | (pitch / 32) << 22 | (tiled ? 1u : 0u) << 31;
                    d[1] = format | range(0, 3) << 6 | basePage << 12;
                    d[2] = (width - 1) | (height - 1) << 13;
                    d[3] = numFormat | (0u | 1u << 3 | 2u << 6 | 3u << 9) << 1;
                    d[4] = minField << 2 | maxField << 6;
                    d[5] = 1u << 9 | (packed ? 1u : 0u) << 11 | mipPage << 12;
                    TextureFetch fetch;
                    const std::string problem = DecodeTextureFetch(d, fetch);
                    if (!problem.empty())
                    {
                        skipped[problem]++;
                        continue;
                    }
                    const TextureLayout layout = ComputeTextureLayout(fetch);
                    bool inside = true;
                    for (const auto& [start, end] : layout.extents)
                        inside &= start >= kArea && end <= kArea + kAreaSize;
                    if (!inside)
                    {
                        skipped["outside the test area"]++;
                        continue;
                    }
                    setFetch(2, d);
                    frame_++; // a new frame: cached textures are validated against memory again
                    BeginRecording();
                    const Texture& texture = TextureFor(2);
                    const std::vector<std::vector<uint8_t>> got = readLevels(texture);
                    std::vector<std::vector<uint8_t>> expected;
                    std::vector<std::pair<uint32_t, uint32_t>> reads;
                    ReferenceTexture(d, physical, expected, &reads);
                    tested++;
                    uint32_t outside = 0;
                    for (const auto& [rs, re] : reads)
                    {
                        bool covered = false;
                        for (const auto& [start, end] : layout.extents)
                            covered |= rs >= start && re <= end;
                        outside += !covered;
                    }
                    const std::string what = std::format("{} {}x{} pitch {} {} endian {} {} levels {}-{}{} {}",
                                                         TextureFormatName(format), width, height, pitch,
                                                         tiled ? "tiled" : "linear", (d[1] >> 6) & 3,
                                                         packed ? "packed" : "not packed", fetch.minLevel, fetch.maxLevel,
                                                         mipPage ? "" : " (no mip address)",
                                                         numFormat ? "integer" : signs == 0x55 ? "signed" : "unsigned/bias");
                    check(outside == 0, std::format("{}: {} texel reads outside the extents", what, outside));
                    if (got.size() != expected.size())
                    {
                        check(false, std::format("{}: {} levels, reference {}", what, got.size(), expected.size()));
                        bad++;
                        continue;
                    }
                    for (size_t level = 0; level < got.size(); level++)
                    {
                        if (expected[level].empty() && got[level].empty())
                            continue;
                        levelsCompared++;
                        if (got[level] != expected[level])
                        {
                            size_t differ = 0;
                            for (size_t k = 0; k < std::min(got[level].size(), expected[level].size()); k++)
                                differ += got[level][k] != expected[level][k];
                            check(false, std::format("{}: level {} differs ({} of {} bytes, sizes {}/{})", what, level, differ,
                                                     expected[level].size(), got[level].size(), expected[level].size()));
                            bad++;
                        }
                    }
                }
                std::string skips;
                for (const auto& [reason, count] : skipped)
                    skips += std::format("; {} x {}", count, reason);
                Log("Texture test: {} random textures, {} levels compared with the reference: {} differ{}", tested,
                    levelsCompared, bad, skips);
                check(tested >= 300, std::format("only {} random textures could be tested", tested));
            }

            // 2: the XDK's layouts.
            {
                std::ifstream file("analysis/texture_ref/xdk_texture_cases.bin", std::ios::binary);
                std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                size_t at = 0;
                auto u32 = [&] {
                    uint32_t v = 0;
                    if (at + 4 <= data.size())
                        std::memcpy(&v, data.data() + at, 4);
                    at += 4;
                    return v;
                };
                if (data.size() < 12 || std::memcmp(data.data(), "XTRF", 4) != 0)
                {
                    Log("Texture test: XDK layout cases skipped (no analysis/texture_ref/xdk_texture_cases.bin; build it "
                        "with tools/xdk_texture_ref/build.bat)");
                }
                else
                {
                    at = 4;
                    u32(); // version
                    const uint32_t count = u32();
                    uint32_t cases = 0, levelsCompared = 0, bad = 0, badReference = 0;
                    for (uint32_t n = 0; n < count && at < data.size(); n++)
                    {
                        uint32_t d[6];
                        for (uint32_t& v : d)
                            v = u32();
                        const uint32_t d3dFormat = u32(), levels = u32(), flags = u32(), memoryBytes = u32();
                        const uint8_t* image = data.data() + at;
                        at += memoryBytes;
                        if (memoryBytes > kAreaSize || at > data.size())
                        {
                            check(false, std::format("XDK case {} does not fit ({} bytes)", n, memoryBytes));
                            break;
                        }
                        std::memset(physical + kArea, 0xCD, kAreaSize);
                        std::memcpy(physical + kArea, image, memoryBytes);
                        // Relocate the base and mip pages (the generator used 0x100000 as the base offset).
                        const uint32_t base = d[1] & 0xFFFFF000u, mip = d[5] & 0xFFFFF000u;
                        d[1] = (d[1] & 0xFFF) | (kArea + base - 0x100000);
                        if (mip)
                            d[5] = (d[5] & 0xFFF) | (kArea + mip - 0x100000);
                        std::vector<std::vector<uint8_t>> sources(levels);
                        std::vector<uint32_t> widths(levels), heights(levels);
                        uint32_t bytes = 0;
                        for (uint32_t level = 0; level < levels; level++)
                        {
                            widths[level] = u32();
                            heights[level] = u32();
                            bytes = u32();
                            const size_t size = size_t(widths[level]) * heights[level] * bytes;
                            sources[level].assign(data.begin() + at, data.begin() + at + size);
                            at += size;
                        }
                        const std::string what = std::format("XDK case {} (D3DFORMAT {:08X}, {}x{}, {} levels{})", n, d3dFormat,
                                                             widths[0], heights[0], levels, flags ? ", not packed" : "");
                        TextureFetch fetch;
                        const std::string problem = DecodeTextureFetch(d, fetch);
                        if (!problem.empty())
                        {
                            check(false, what + ": " + problem);
                            continue;
                        }
                        if (fetch.minLevel != 0 || fetch.maxLevel + 1 != levels)
                        {
                            check(false, std::format("{}: decoded levels {}-{}", what, fetch.minLevel, fetch.maxLevel));
                            continue;
                        }
                        // What the host texture must hold: each source texel through the endian rule (D3D's formats use
                        // 8in32 on 4-byte texels, 8in16 on 2-byte ones, none on bytes, where words and texels agree).
                        const uint32_t endian = fetch.endian;
                        auto toHost = [&](std::vector<uint8_t> texels) {
                            for (size_t k = 0; k + bytes <= texels.size(); k += bytes)
                            {
                                uint8_t* t = texels.data() + k;
                                if (bytes == 4 && (endian == 1 || endian == 2))
                                {
                                    std::swap(t[0], t[1]);
                                    std::swap(t[2], t[3]);
                                }
                                if (bytes == 4 && (endian == 2 || endian == 3))
                                {
                                    std::swap(t[0], t[2]);
                                    std::swap(t[1], t[3]);
                                }
                                if (bytes == 2 && endian == 1)
                                    std::swap(t[0], t[1]);
                            }
                            return texels;
                        };
                        if ((bytes == 2 && endian > 1) || (bytes == 1 && endian != 0))
                        {
                            check(false, std::format("{}: unexpected endian {} for {}-byte texels", what, endian, bytes));
                            continue;
                        }
                        setFetch(3, d);
                        frame_++;
                        BeginRecording();
                        const std::vector<std::vector<uint8_t>> got = readLevels(TextureFor(3));
                        std::vector<std::vector<uint8_t>> reference;
                        ReferenceTexture(d, physical, reference);
                        cases++;
                        // Where a level differs: the count of differing bytes and the first one (texel x, y, byte).
                        auto where = [&](const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, uint32_t level) {
                            if (a.size() != b.size())
                                return std::format("size {} instead of {}", a.size(), b.size());
                            size_t first = a.size(), count = 0;
                            for (size_t k = 0; k < a.size(); k++)
                                if (a[k] != b[k] && count++ == 0)
                                    first = k;
                            const size_t texel = first / bytes;
                            return std::format("{} bytes differ, first at texel ({}, {}) byte {}: {:02X} instead of {:02X}",
                                               count, texel % widths[level], texel / widths[level], first % bytes,
                                               a[first], b[first]);
                        };
                        for (uint32_t level = 0; level < levels; level++)
                        {
                            const std::vector<uint8_t> expected = toHost(sources[level]);
                            levelsCompared++;
                            if (level >= got.size() || got[level] != expected)
                            {
                                bad++;
                                check(false, std::format("{}: level {} ({}x{}) differs from the image XG tiled: {}", what,
                                                         level, widths[level], heights[level],
                                                         level < got.size() ? where(got[level], expected, level) : "missing"));
                            }
                            if (level >= reference.size() || reference[level] != expected)
                            {
                                badReference++;
                                check(false, std::format("{}: the CPU reference's level {} differs from the image XG tiled: {}",
                                                         what, level, level < reference.size() ?
                                                             where(reference[level], expected, level) : "missing"));
                            }
                        }
                    }
                    Log("Texture test: {} XDK layout cases, {} levels: {} differ on the GPU, {} in the CPU reference", cases,
                        levelsCompared, bad, badReference);
                    check(cases == count, std::format("{} of {} XDK cases ran", cases, count));
                }
            }

            failures += TextureSamplingTests();
            quietTextures_ = false;
            Log("Texture test: {} failures", failures);
            return failures;
        }

        // The sampling probe: a compute shader that samples a texture through the real descriptor setup (TextureTable,
        // SamplerTable) at given coordinates and LODs and applies xe_tex_finish, as a translated tfetch does.
        int Renderer::TextureSamplingTests()
        {
            int failures = 0;
            auto check = [&](bool ok, const std::string& what) {
                if (!ok)
                {
                    Log("Texture sampling test: FAILED: {}", what);
                    failures++;
                }
            };
            // Root signature: b4 probe constants, t0-t15 space1 textures, s0-s15 samplers, u0 results.
            ComPtr<ID3D12RootSignature> signature;
            ComPtr<ID3D12PipelineState> pipeline;
            {
                D3D12_DESCRIPTOR_RANGE textures = { D3D12_DESCRIPTOR_RANGE_TYPE_SRV, kTableSize, 0, 1, 0 };
                D3D12_DESCRIPTOR_RANGE samplers = { D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, kTableSize, 0, 0, 0 };
                D3D12_ROOT_PARAMETER params[4] = {};
                params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
                params[0].Descriptor.ShaderRegister = 4;
                params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[1].DescriptorTable = { 1, &textures };
                params[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                params[2].DescriptorTable = { 1, &samplers };
                params[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
                D3D12_ROOT_SIGNATURE_DESC desc = { 4, params };
                ComPtr<ID3DBlob> blob, error;
                if (FAILED(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error)))
                    Fatal("Texture sampling test: root signature: {}", error ? static_cast<const char*>(error->GetBufferPointer()) : "?");
                CheckHr(device_->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(),
                                                     IID_PPV_ARGS(&signature)), "probe root signature");
                const std::string source = std::string(kXenosHlslLibrary) + R"(
cbuffer XeProbe : register(b4)
{
    uint4 xe_probe_fetch;          // x: fetch dword 0, y: dword 3, z: sample count
    float4 xe_probe_coords[256];   // xy: coordinates, z: LOD
};
Texture2D<float4> xe_probe_texture : register(t0, space1);
SamplerState xe_probe_sampler : register(s0);
RWByteAddressBuffer xe_probe_out : register(u0);

[numthreads(64, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    if (id.x >= xe_probe_fetch.z)
        return;
    float4 c = xe_probe_coords[id.x];
    float4 tv = xe_probe_texture.SampleLevel(xe_probe_sampler, c.xy, c.z);
    xe_probe_out.Store4(id.x * 16u, asuint(xe_tex_finish(tv, xe_probe_fetch.x, xe_probe_fetch.y)));
}
)";
                std::vector<uint8_t> bytecode;
                std::string errors;
                if (!CompileHlsl(source, "cs_5_1", "texture_probe", bytecode, errors))
                    Fatal("Texture sampling test: probe shader: {}", errors);
                D3D12_COMPUTE_PIPELINE_STATE_DESC desc2 = {};
                desc2.pRootSignature = signature.Get();
                desc2.CS = { bytecode.data(), bytecode.size() };
                CheckHr(device_->CreateComputePipelineState(&desc2, IID_PPV_ARGS(&pipeline)), "probe pipeline");
            }
            constexpr uint32_t kMaxSamples = 256;
            ComPtr<ID3D12Resource> out, readback;
            out.Attach(CreateBuffer(kMaxSamples * 16, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, "probe output"));
            readback.Attach(CreateBuffer(kMaxSamples * 16, D3D12_HEAP_TYPE_READBACK, D3D12_RESOURCE_FLAG_NONE, "probe readback"));

            struct Sample
            {
                float u, v, lod;
            };
            // Samples fetch constant 4 at `samples`; returns the 4 results each.
            auto probe = [&](const std::vector<Sample>& samples) {
                struct
                {
                    uint32_t fetch[4];
                    float coords[kMaxSamples][4];
                } constants = {};
                constants.fetch[0] = Reg(0x4800 + 24);
                constants.fetch[1] = Reg(0x4800 + 24 + 3);
                constants.fetch[2] = uint32_t(samples.size());
                for (size_t i = 0; i < samples.size(); i++)
                {
                    constants.coords[i][0] = samples[i].u;
                    constants.coords[i][1] = samples[i].v;
                    constants.coords[i][2] = samples[i].lod;
                }
                frame_++;
                HostShader shader;
                shader.textures.push_back({ 4, 1 });
                shader.samplers.push_back({ 4, 3, 3, 3, 7 });
                BeginRecording();
                const D3D12_GPU_DESCRIPTOR_HANDLE textureTable = TextureTable(shader), samplerTable = SamplerTable(shader);
                Transition(out.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
                list_->SetComputeRootSignature(signature.Get());
                list_->SetPipelineState(pipeline.Get());
                list_->SetComputeRootConstantBufferView(0, UploadData(&constants, sizeof(constants)));
                list_->SetComputeRootDescriptorTable(1, textureTable);
                list_->SetComputeRootDescriptorTable(2, samplerTable);
                list_->SetComputeRootUnorderedAccessView(3, out->GetGPUVirtualAddress());
                list_->Dispatch(UINT((samples.size() + 63) / 64), 1, 1);
                Transition(out.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
                list_->CopyBufferRegion(readback.Get(), 0, out.Get(), 0, kMaxSamples * 16);
                Transition(out.Get(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COMMON);
                Sync();
                std::vector<std::array<float, 4>> results(samples.size());
                const float* mapped = nullptr;
                CheckHr(readback->Map(0, nullptr, reinterpret_cast<void**>(const_cast<float**>(&mapped))), "probe Map");
                std::memcpy(results.data(), mapped, samples.size() * 16);
                readback->Unmap(0, nullptr);
                return results;
            };
            // Float distance in units in the last place (same sign assumed; different signs count as far apart).
            auto ulps = [](float a, float b) -> uint32_t {
                const int32_t ia = std::bit_cast<int32_t>(a), ib = std::bit_cast<int32_t>(b);
                if ((ia < 0) != (ib < 0))
                    return (a == b) ? 0 : 0xFFFFFFFFu;
                return uint32_t(std::abs(int64_t(ia) - int64_t(ib)));
            };

            uint8_t* physical = guest::g_memory.Translate(guest::kPhysicalWindow64k);
            constexpr uint32_t kTexture = 0x02000000;
            constexpr uint32_t kW = 8, kH = 4;
            // Base level texel (x, y) of the 8x4 k_8_8_8_8 test texture: distinct components.
            auto texel = [](uint32_t x, uint32_t y) {
                const uint32_t k = x * 8 + y * 64 + 5;
                return std::array<uint32_t, 4>{ k & 0xFF, (255 - k) & 0xFF, (k * 3 + 17) & 0xFF, (k + 128) & 0xFF };
            };
            // Linear, pitch 32, endian none: memory bytes X, Y, Z, W.
            for (uint32_t y = 0; y < kH; y++)
                for (uint32_t x = 0; x < kW; x++)
                {
                    const auto t = texel(x, y);
                    for (uint32_t c = 0; c < 4; c++)
                        physical[kTexture + (y * 32 + x) * 4 + c] = uint8_t(t[c]);
                }
            // The value of a data component as the host conversion gives it (texture.h).
            auto convert = [](uint32_t v, uint32_t sign, bool integer) -> float {
                if (integer)
                    return sign == 1 ? float(int32_t(v << 24) >> 24) : float(v);
                if (sign == 1)
                    return std::max(float(double(int32_t(v << 24) >> 24) / 127.0), -1.0f);
                return float(double(v) / 255.0);
            };
            auto expectedFor = [&](const std::array<float, 4>& data, uint32_t d0, uint32_t d3) {
                std::array<float, 4> values;
                for (uint32_t c = 0; c < 4; c++)
                    values[c] = ((d0 >> (2 + 2 * c)) & 3) == 2 ? data[c] * 2.0f - 1.0f : data[c];
                std::array<float, 4> r;
                const int32_t exp = int32_t(d3 << 13) >> 26;
                for (uint32_t c = 0; c < 4; c++)
                {
                    const uint32_t select = (d3 >> (1 + 3 * c)) & 7;
                    const float source = select < 4 ? values[select] : float(select & 1);
                    r[c] = std::ldexp(source, exp);
                }
                return r;
            };
            // Texel index along one axis for an address mode (05b §4.2), or -1 for the border.
            auto addressed = [](int32_t i, int32_t size, uint32_t mode) -> int32_t {
                switch (mode)
                {
                case 0: return ((i % size) + size) % size;
                case 1: { const int32_t j = ((i % (2 * size)) + 2 * size) % (2 * size); return j < size ? j : 2 * size - 1 - j; }
                case 2: return std::clamp(i, 0, size - 1);
                case 3: return std::clamp(i < 0 ? -1 - i : i, 0, size - 1);
                default: return i < 0 || i >= size ? -1 : i; // 6 BORDER
                }
            };

            struct Case
            {
                std::string name;
                uint32_t clampU, clampV, policy, border, forceW, swizzle, exp, signs, numFormat;
            };
            std::vector<Case> cases;
            const uint32_t identity = 0u | 1u << 3 | 2u << 6 | 3u << 9;
            for (uint32_t mode : { 0u, 1u, 2u, 3u, 6u })
                for (uint32_t border = 0; border < 2; border++)
                {
                    cases.push_back({ std::format("U mode {} border {}", mode, border), mode, 2, 0, border, 0, identity, 0, 0, 0 });
                    cases.push_back({ std::format("V mode {} border {}", mode, border), 2, mode, 0, border, 0, identity, 0, 0, 0 });
                }
            cases.push_back({ "border force W", 6, 6, 0, 0, 1, identity, 0, 0, 0 });
            for (uint32_t mode : { 4u, 5u, 6u, 7u }) // ClampPolicy OGL with point filtering
                cases.push_back({ std::format("policy mode {}", mode), mode, mode, 1, 1, 0, identity, 0, 0, 0 });
            uint32_t seed = 0x1234567u;
            auto next = [&] { seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; return seed; };
            for (int i = 0; i < 24; i++)
            {
                const uint32_t kind = i % 4;
                uint32_t signs = 0, numFormat = 0;
                if (kind == 1)
                    signs = 0x55;
                else if (kind == 2)
                    signs = (next() & 0x55) << 1; // bias on some components
                else if (kind == 3)
                {
                    numFormat = 1;
                    signs = next() & 0x55;
                }
                cases.push_back({ std::format("swizzle/number case {}", i), next() % 4, next() % 4, 0, next() & 1, 0,
                                  next() & 0xFFF, (next() % 11) - 5, signs, numFormat });
            }
            uint32_t total = 0;
            double worst = 0.0;
            for (const Case& c : cases)
            {
                uint32_t d[6];
                d[0] = 2u | c.signs << 2 | c.clampU << 10 | c.clampV << 13 | 1u << 22; // linear, pitch 32
                d[1] = 6u | kTexture | c.policy << 11;
                d[2] = (kW - 1) | (kH - 1) << 13;
                d[3] = c.numFormat | c.swizzle << 1 | (uint32_t(c.exp) & 0x3F) << 13; // point min/mag/mip
                d[4] = 0;
                d[5] = c.border | c.forceW << 2 | 1u << 9;
                for (uint32_t i = 0; i < 6; i++)
                    SetRegister(0x4800 + 24 + i, d[i]);
                std::vector<Sample> samples;
                std::vector<std::array<int32_t, 2>> where;
                for (int32_t i = -2 * int32_t(kW); i < 3 * int32_t(kW) && samples.size() < kMaxSamples; i++)
                {
                    const bool alongU = c.name[0] != 'V';
                    const int32_t x = alongU ? i : 3, y = alongU ? 1 + i % 2 : i;
                    if (!alongU && (i < -2 * int32_t(kH) || i >= 3 * int32_t(kH)))
                        continue;
                    samples.push_back({ (float(x) + 0.5f) / kW, (float(y) + 0.5f) / kH, 0.0f });
                    where.push_back({ x, y });
                }
                const auto results = probe(samples);
                const bool integer = c.numFormat == 1;
                // Point filtering with the policy: border modes act as their clamp counterparts.
                auto effective = [&](uint32_t mode) {
                    if (!c.policy)
                        return mode;
                    return mode == 4 || mode == 6 ? 2u : mode == 5 || mode == 7 ? 3u : mode;
                };
                uint32_t badSamples = 0;
                for (size_t s = 0; s < samples.size(); s++)
                {
                    const int32_t tx = addressed(where[s][0], kW, effective(c.clampU));
                    const int32_t ty = addressed(where[s][1], kH, effective(c.clampV));
                    std::array<float, 4> data;
                    if (tx < 0 || ty < 0)
                    {
                        const float b = c.border ? 1.0f : 0.0f;
                        data = { b, b, b, c.forceW ? 1.0f : b };
                    }
                    else
                    {
                        const auto t = texel(uint32_t(tx), uint32_t(ty));
                        for (uint32_t k = 0; k < 4; k++)
                            data[k] = convert(t[k], (c.signs >> (2 * k)) & 3, integer);
                    }
                    const auto expected = expectedFor(data, d[0], d[3]);
                    for (uint32_t k = 0; k < 4; k++)
                    {
                        // Exact for integers; fractions within 2^-22 (scaled by the exp adjust) for the host UNORM/SNORM
                        // conversion, far below the 1/255 between neighbouring texels.
                        const double difference = std::fabs(double(results[s][k]) - double(expected[k]));
                        const double tolerance = integer ? 0.0 : std::ldexp(1.0, c.exp - 22);
                        worst = std::max(worst, difference / std::ldexp(1.0, c.exp));
                        if (!(difference <= tolerance))
                            badSamples++;
                    }
                }
                total += uint32_t(samples.size());
                check(badSamples == 0, std::format("{}: {} sample components wrong", c.name, badSamples));
            }

            // Level ranges and BASEMAP: a tiled packed 64x64 texture whose levels hold distinct constants.
            {
                uint32_t d[6];
                d[0] = 2u | 2u << 10 | 2u << 13 | 2u << 22 | 1u << 31;  // clamp, pitch 64, tiled
                d[1] = 6u | kTexture;
                d[2] = 63 | 63 << 13;
                d[3] = identity << 1;                                   // point, mip point
                d[4] = 1u << 2 | 5u << 6;                               // levels 1-5
                d[5] = 1u << 9 | 1u << 11 | (kTexture + 0x10000);
                TextureFetch fetch;
                DecodeTextureFetch(d, fetch);
                const TextureLayout layout = ComputeTextureLayout(fetch);
                std::memset(physical + kTexture, 0, 0x40000);
                for (uint32_t level = 0; level <= 6; level++)
                {
                    if (!layout.levels[level].present)
                        continue;
                    const TextureLevelSource& l = layout.levels[level];
                    for (uint32_t y = 0; y < l.height; y++)
                        for (uint32_t x = 0; x < l.width; x++)
                        {
                            const uint32_t offset = TiledOffset2D(l.offsetX + x, l.offsetY + y, l.pitchBlocks / 32, 2);
                            const uint32_t value = 0x01010101u * (level * 40 + 10);
                            std::memcpy(physical + l.storage + offset, &value, 4);
                        }
                }
                for (int basemap = 0; basemap < 2; basemap++)
                {
                    d[3] = identity << 1 | (basemap ? 2u : 0u) << 23;
                    for (uint32_t i = 0; i < 6; i++)
                        SetRegister(0x4800 + 24 + i, d[i]);
                    std::vector<Sample> samples;
                    for (int lod = -1; lod <= 7; lod++)
                        samples.push_back({ 0.3f, 0.6f, float(lod) });
                    const auto results = probe(samples);
                    for (size_t s = 0; s < samples.size(); s++)
                    {
                        const int lod = int(samples[s].lod);
                        const uint32_t level = basemap ? 1u : uint32_t(std::clamp(lod, 1, 5));
                        const float expected = float(double(level * 40 + 10) / 255.0);
                        check(ulps(results[s][0], expected) <= 1,
                              std::format("{} LOD {}: got {}, level {} holds {}", basemap ? "BASEMAP" : "mip point", lod,
                                          results[s][0], level, expected));
                    }
                    total += uint32_t(samples.size());
                }
            }
            Log("Texture test: {} sampling cases, {} samples: {} failures (largest fraction difference {:.3g})",
                cases.size() + 2, total, failures, worst);
            return failures;
        }

        Renderer* g_renderer = nullptr;

        Renderer& Instance()
        {
            if (!g_renderer)
                g_renderer = new Renderer();
            return *g_renderer;
        }
    }

    bool LocalMemory() { return g_localMemory; }

    void MarkMemoryWritten(uint32_t physical, uint32_t size)
    {
        if (!g_localMemory || size == 0)
            return;
        const uint64_t first = uint64_t(physical & (guest::kPhysicalSize - 1)) >> kMemoryPageShift;
        const uint64_t last = std::min<uint64_t>((uint64_t(physical & (guest::kPhysicalSize - 1)) + size - 1) >> kMemoryPageShift,
                                                 (guest::kPhysicalSize >> kMemoryPageShift) - 1);
        for (uint64_t page = first; page <= last;)
        {
            const uint64_t word = page / 64, bit = page % 64, n = std::min<uint64_t>(64 - bit, last - page + 1);
            const uint64_t mask = (n == 64 ? ~0ull : ((1ull << n) - 1)) << bit;
            g_memoryWritten[word].fetch_or(mask, std::memory_order_relaxed);
            page += n;
        }
        g_memoryAnyWritten.store(true, std::memory_order_release);
    }

    bool RendererEnabled()
    {
        return !g_disabled;
    }

    void PrepareRenderer()
    {
        if (g_disabled)
            return;
        Instance();
        PrefetchShaders(); // shader_cache.h: compiled in the background while the window comes up
        Instance().PrewarmHelpers();
    }

    void RenderDraw(const DrawPacket& draw, const Shader* vertexShader, const Shader* pixelShader)
    {
        Instance().Draw(draw, vertexShader, pixelShader);
    }

    int RunRendererTests()
    {
        return Instance().SelfTest();
    }

    void RenderSync(SyncReason reason)
    {
        if (g_renderer)
            g_renderer->Sync(reason);
    }

    uint64_t RenderSubmit()
    {
        return g_renderer ? g_renderer->SubmitPending() : 0;
    }

    bool RenderReached(uint64_t value, uint32_t ms)
    {
        return !g_renderer || g_renderer->Reached(value, ms);
    }

    double TakeGpuMilliseconds()
    {
        return g_renderer ? g_renderer->TakeGpuMilliseconds() : 0.0;
    }

    std::string TakeGpuBreakdown()
    {
        return g_renderer ? g_renderer->TakeGpuBreakdown() : std::string();
    }

    std::string TakeSyncStats()
    {
        static const char* const kNames[] = { "other", "scratch", "reg-to-mem", "mem-write", "cond-write", "fence",
                                               "extents", "occlusion", "interrupt", "swap", "ring", "descriptors",
                                               "present" };
        static_assert(std::size(kNames) == size_t(SyncReason::kCount));
        std::string text;
        for (size_t i = 0; i < size_t(SyncReason::kCount); i++)
        {
            SyncStat& stat = g_syncStats[i];
            if (stat.calls)
                text += std::format(" {} {}/{} {:.1f} ms;", kNames[i], stat.submits, stat.calls, stat.waitNs / 1e6);
            stat = {};
        }
        return text;
    }

    void RenderSwap(uint64_t frame, uint32_t frontBuffer, uint32_t width, uint32_t height, const GuestOverlay& overlay)
    {
        if (!g_disabled)
            Instance().Swap(frame, frontBuffer, width, height, overlay);
    }
}
