// CB_SHADER_TEST=1: shader translator verification (run instead of the game).
//  1. Translates and compiles every shader dumped in analysis/shaders/ (CB_GPU_DUMP_SHADERS=1).
//  2. Runs single ALU instructions, assembled as microcode and translated with the compute harness, on the GPU and
//     compares every register, ps, p0, a0 and the kill flag bit for bit with a CPU reference of analysis/xenos/03c.
//     Transcendental scalar ops are compared exactly on special values and within a tolerance otherwise.

#include "gpu/shader_tests.h"

#include "gpu/resolve.h"
#include "gpu/shader_translator.h"
#include "gpu/ucode_fields.h"
#include "log.h"

#include <windows.h>
#include <d3d12.h>
#include <wrl/client.h>

#include <bit>
#include <cfloat>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <random>

using Microsoft::WRL::ComPtr;

namespace gpu
{
    namespace
    {
        using namespace ucode;

        void Check(HRESULT hr, const char* what)
        {
            if (FAILED(hr))
                Fatal("Shader test: {} failed (HRESULT {:08X})", what, uint32_t(hr));
        }

        // ---- GPU side: one compute dispatch per shader over a buffer of register states ---------------------------

        constexpr uint32_t kConstantCount = 16;
        constexpr uint32_t kConstantWindow = 12; // c12-c15 are outside the window and read as 0

        class ComputeRunner
        {
        public:
            explicit ComputeRunner(const float (&constants)[kConstantCount][4])
            {
                Check(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_)), "D3D12CreateDevice");
                D3D12_COMMAND_QUEUE_DESC queueDesc = { D3D12_COMMAND_LIST_TYPE_COMPUTE };
                Check(device_->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue_)), "CreateCommandQueue");
                Check(device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&allocator_)), "CreateCommandAllocator");
                Check(device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)), "CreateFence");
                event_ = CreateEventW(nullptr, FALSE, FALSE, nullptr);

                D3D12_ROOT_PARAMETER params[6] = {};
                for (UINT i = 0; i < 5; i++)
                {
                    params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
                    params[i].Descriptor.ShaderRegister = i;
                }
                params[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
                D3D12_ROOT_SIGNATURE_DESC rootDesc = { 6, params };
                ComPtr<ID3DBlob> blob, error;
                Check(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error), "D3D12SerializeRootSignature");
                Check(device_->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_)),
                      "CreateRootSignature");

                // Constant buffers: system (b0), float constants (b1, 8 KiB), bool/loop (b2), fetch (b3).
                constants_ = CreateBuffer(kConstantsSize, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE);
                uint8_t* mapped;
                Check(constants_->Map(0, nullptr, reinterpret_cast<void**>(&mapped)), "Map constants");
                std::memset(mapped, 0, kConstantsSize);
                XeSystemConstants system = {};
                system.vsConstantBase = 0;
                system.vsConstantCount = kConstantWindow;
                system.psConstantBase = 0;
                system.psConstantCount = kConstantWindow;
                std::memcpy(mapped + kSystemOffset, &system, sizeof(system));
                std::memcpy(mapped + kFloatOffset, constants, sizeof(constants));
                FlushDenormals(reinterpret_cast<uint32_t*>(mapped + kFloatOffset), sizeof(constants) / 4);
                constants_->Unmap(0, nullptr);
            }

            // Bool constants (dwords 0-7) and loop constants (8-39) for b2.
            void SetBoolLoop(const uint32_t (&words)[40])
            {
                uint8_t* mapped;
                Check(constants_->Map(0, nullptr, reinterpret_cast<void**>(&mapped)), "Map constants");
                std::memcpy(mapped + kBoolLoopOffset, words, sizeof(words));
                constants_->Unmap(0, nullptr);
            }

            // Output-merger constants (b4) for the EDRAM tests.
            void SetOutput(const OutputMergerConstants& output)
            {
                uint8_t* mapped;
                Check(constants_->Map(0, nullptr, reinterpret_cast<void**>(&mapped)), "Map constants");
                std::memcpy(mapped + kOutputOffset, &output, sizeof(output));
                constants_->Unmap(0, nullptr);
            }

            // Raw bytes for b4 (resolve tests), at most 256.
            void SetOutputBytes(const void* data, size_t size)
            {
                uint8_t* mapped;
                Check(constants_->Map(0, nullptr, reinterpret_cast<void**>(&mapped)), "Map constants");
                std::memcpy(mapped + kOutputOffset, data, size);
                constants_->Unmap(0, nullptr);
            }

            // Dispatches `groupsX` x `groupsY` groups (default: one group per kHarnessCaseBytes of input).
            std::vector<uint8_t> Run(const std::vector<uint8_t>& bytecode, const std::vector<uint8_t>& input,
                                     UINT groupsX = 0, UINT groupsY = 1)
            {
                const UINT64 size = input.size();
                if (size > ioSize_)
                {
                    ioSize_ = size;
                    io_ = CreateBuffer(size, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
                    upload_ = CreateBuffer(size, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE);
                    readback_ = CreateBuffer(size, D3D12_HEAP_TYPE_READBACK, D3D12_RESOURCE_FLAG_NONE);
                }
                void* mapped;
                Check(upload_->Map(0, nullptr, &mapped), "Map upload");
                std::memcpy(mapped, input.data(), size);
                upload_->Unmap(0, nullptr);

                D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
                psoDesc.pRootSignature = rootSignature_.Get();
                psoDesc.CS = { bytecode.data(), bytecode.size() };
                ComPtr<ID3D12PipelineState> pipeline;
                Check(device_->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&pipeline)), "CreateComputePipelineState");

                Check(allocator_->Reset(), "Reset allocator");
                if (!list_)
                    Check(device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, allocator_.Get(), pipeline.Get(),
                                                     IID_PPV_ARGS(&list_)), "CreateCommandList");
                else
                    Check(list_->Reset(allocator_.Get(), pipeline.Get()), "Reset list");
                list_->CopyBufferRegion(io_.Get(), 0, upload_.Get(), 0, size);
                Transition(io_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
                list_->SetComputeRootSignature(rootSignature_.Get());
                const D3D12_GPU_VIRTUAL_ADDRESS cb = constants_->GetGPUVirtualAddress();
                list_->SetComputeRootConstantBufferView(0, cb + kSystemOffset);
                list_->SetComputeRootConstantBufferView(1, cb + kFloatOffset);
                list_->SetComputeRootConstantBufferView(2, cb + kBoolLoopOffset);
                list_->SetComputeRootConstantBufferView(3, cb + kFetchOffset);
                list_->SetComputeRootConstantBufferView(4, cb + kOutputOffset);
                list_->SetComputeRootUnorderedAccessView(5, io_->GetGPUVirtualAddress());
                list_->Dispatch(groupsX ? groupsX : UINT(size / kHarnessCaseBytes), groupsY, 1);
                Transition(io_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
                list_->CopyBufferRegion(readback_.Get(), 0, io_.Get(), 0, size);
                Check(list_->Close(), "Close list");
                ID3D12CommandList* lists[] = { list_.Get() };
                queue_->ExecuteCommandLists(1, lists);
                Check(queue_->Signal(fence_.Get(), ++fenceValue_), "Signal");
                Check(fence_->SetEventOnCompletion(fenceValue_, event_), "SetEventOnCompletion");
                WaitForSingleObject(event_, INFINITE);

                std::vector<uint8_t> output(size);
                D3D12_RANGE range = { 0, size };
                Check(readback_->Map(0, &range, &mapped), "Map readback");
                std::memcpy(output.data(), mapped, size);
                readback_->Unmap(0, nullptr);
                return output;
            }

        private:
            static constexpr UINT64 kSystemOffset = 0, kFloatOffset = 256, kBoolLoopOffset = 256 + 8192,
                                    kFetchOffset = kBoolLoopOffset + 256, kOutputOffset = kFetchOffset + 1024,
                                    kConstantsSize = kOutputOffset + 256;

            ComPtr<ID3D12Device> device_;
            ComPtr<ID3D12CommandQueue> queue_;
            ComPtr<ID3D12CommandAllocator> allocator_;
            ComPtr<ID3D12GraphicsCommandList> list_;
            ComPtr<ID3D12Fence> fence_;
            ComPtr<ID3D12RootSignature> rootSignature_;
            ComPtr<ID3D12Resource> constants_, io_, upload_, readback_;
            UINT64 ioSize_ = 0, fenceValue_ = 0;
            HANDLE event_ = nullptr;

            ComPtr<ID3D12Resource> CreateBuffer(UINT64 size, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_FLAGS flags)
            {
                D3D12_HEAP_PROPERTIES heap = { heapType };
                D3D12_RESOURCE_DESC desc = {};
                desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
                desc.Width = size;
                desc.Height = desc.DepthOrArraySize = desc.MipLevels = 1;
                desc.SampleDesc.Count = 1;
                desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
                desc.Flags = flags;
                const D3D12_RESOURCE_STATES state = heapType == D3D12_HEAP_TYPE_UPLOAD ? D3D12_RESOURCE_STATE_GENERIC_READ
                    : heapType == D3D12_HEAP_TYPE_READBACK ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_COMMON;
                ComPtr<ID3D12Resource> resource;
                Check(device_->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, state, nullptr, IID_PPV_ARGS(&resource)),
                      "CreateCommittedResource");
                return resource;
            }

            void Transition(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
            {
                D3D12_RESOURCE_BARRIER barrier = {};
                barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                barrier.Transition.pResource = resource;
                barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                barrier.Transition.StateBefore = before;
                barrier.Transition.StateAfter = after;
                list_->ResourceBarrier(1, &barrier);
            }
        };

        // ---- CPU reference (03c), written from the spec tables -------------------------------------------------

        struct State
        {
            float r[64][4] = {};
            float ps = 0;
            bool p0 = false;
            int32_t a0 = 0;
            int32_t aL = 0;
            bool killed = false;
        };

        float Flush(float x)
        {
            const uint32_t u = std::bit_cast<uint32_t>(x);
            return (u & 0x7F800000) == 0 ? std::bit_cast<float>(u & 0x80000000) : x;
        }
        bool IsNan(float x) { return std::isnan(x); }
        float Add(float a, float b) { return Flush(a + b); }
        float Sub(float a, float b) { return Flush(a - b); }
        float Mul(float a, float b) { return (a == 0.0f || b == 0.0f) ? 0.0f : Flush(a * b); } // legacy multiply
        float Max(float a, float b) { return a >= b ? a : b; }
        float Min(float a, float b) { return a < b ? a : b; }
        float Sat(float x) { return x > 0.0f ? (x < 1.0f ? x : 1.0f) : 0.0f; }
        int32_t ClampA0(float f) { return !(f >= -256.0f) ? -256 : f > 255.0f ? 255 : int32_t(f); }

        struct Reference
        {
            const float (&c)[kConstantCount][4];

            float Constant(int32_t index, uint32_t component) const
            {
                if (index < 0 || uint32_t(index) >= kConstantWindow)
                    return 0.0f;
                return c[index][component];
            }

            // Raw (unmodified, unflushed) register or constant component of ALU operand i.
            float Raw(const Alu& alu, const State& s, int i, uint32_t component) const
            {
                const uint32_t reg = alu.reg(i);
                if (alu.isTemp(i))
                {
                    const uint32_t index = (reg & 0x40) ? ((reg & 0x3F) + uint32_t(s.aL)) & 63 : reg & 0x3F;
                    return s.r[index][component];
                }
                const int32_t offset = alu.constantRelative(i) ? (alu.relativeToA0() ? s.a0 : s.aL) : 0;
                return Constant(int32_t(reg) + offset, component);
            }

            float Modified(float v, bool abs, bool negate) const
            {
                v = Flush(v);
                if (abs)
                    v = std::fabs(v);
                return negate ? -v : v;
            }

            void Vector(const Alu& alu, const State& s, int i, float out[4]) const
            {
                const uint32_t swizzle = alu.swizzle(i);
                const bool abs = alu.isTemp(i) ? (alu.reg(i) & 0x80) != 0 : alu.absConstants();
                for (uint32_t comp = 0; comp < 4; comp++)
                    out[comp] = Modified(Raw(alu, s, i, ((swizzle >> (2 * comp)) + comp) & 3), abs, alu.negate(i));
            }

            // Returns the scalar operand `a` (0 if the instruction was predicated off).
            float Execute(const Alu& alu, State& s) const
            {
                if (alu.predicated() && s.p0 != alu.predicateCondition())
                    return 0.0f;
                const State before = s;
                float v1[4], v2[4], v3[4];
                Vector(alu, before, 1, v1);
                Vector(alu, before, 2, v2);
                Vector(alu, before, 3, v3);
                const uint32_t swizzle = alu.swizzle(3);
                const uint32_t ca = ((swizzle >> 6) + 3) & 3, cb = swizzle & 3;
                const bool abs3 = alu.isTemp(3) ? (alu.reg(3) & 0x80) != 0 : alu.absConstants();
                float sa = Modified(Raw(alu, before, 3, ca), abs3, alu.negate(3));
                float sb = Modified(Raw(alu, before, 3, cb), abs3, alu.negate(3));
                const uint32_t sop = alu.scalarOp(), vop = alu.vectorOp();
                if (sop >= 42 && sop <= 47)
                {
                    const int32_t offset = alu.constantRelative(3) ? (alu.relativeToA0() ? before.a0 : before.aL) : 0;
                    sa = Modified(Constant(int32_t(alu.reg(3)) + offset, ca), alu.absConstants(), alu.negate(3));
                    sb = Modified(before.r[alu.scalarConstantTempReg()][cb], alu.absConstants(), alu.negate(3));
                }

                // Vector op (03c §5).
                float vr[4] = {};
                auto each = [&](auto f) { for (int k = 0; k < 4; k++) vr[k] = f(k); };
                auto dp = [&](int n) {
                    float sum = Add(Mul(v1[0], v2[0]), Mul(v1[1], v2[1]));
                    if (n >= 3) sum = Add(sum, Mul(v1[2], v2[2]));
                    if (n >= 4) sum = Add(sum, Mul(v1[3], v2[3]));
                    for (float& x : vr) x = sum;
                };
                auto cmp = [](uint32_t kind, float x) { // eq, ne, gt, ge against 0
                    return kind == 0 ? x == 0.0f : kind == 1 ? x != 0.0f : kind == 2 ? x > 0.0f : x >= 0.0f;
                };
                switch (vop)
                {
                case 0: each([&](int k) { return Add(v1[k], v2[k]); }); break;
                case 1: each([&](int k) { return Mul(v1[k], v2[k]); }); break;
                case 2: each([&](int k) { return Max(v1[k], v2[k]); }); break;
                case 3: each([&](int k) { return Min(v1[k], v2[k]); }); break;
                case 4: each([&](int k) { return v1[k] == v2[k] ? 1.0f : 0.0f; }); break;
                case 5: each([&](int k) { return v1[k] > v2[k] ? 1.0f : 0.0f; }); break;
                case 6: each([&](int k) { return v1[k] >= v2[k] ? 1.0f : 0.0f; }); break;
                case 7: each([&](int k) { return v1[k] != v2[k] ? 1.0f : 0.0f; }); break;
                case 8: each([&](int k) { return Sub(v1[k], std::floor(v1[k])); }); break;
                case 9: each([&](int k) { return std::trunc(v1[k]); }); break;
                case 10: each([&](int k) { return std::floor(v1[k]); }); break;
                case 11: each([&](int k) { return Add(Mul(v1[k], v2[k]), v3[k]); }); break;
                case 12: each([&](int k) { return v1[k] == 0.0f ? v2[k] : v3[k]; }); break;
                case 13: each([&](int k) { return v1[k] >= 0.0f ? v2[k] : v3[k]; }); break;
                case 14: each([&](int k) { return v1[k] > 0.0f ? v2[k] : v3[k]; }); break;
                case 15: dp(4); break;
                case 16: dp(3); break;
                case 17:
                {
                    const float sum = Add(Add(Mul(v1[0], v2[0]), Mul(v1[1], v2[1])), v3[0]);
                    for (float& x : vr) x = sum;
                    break;
                }
                case 18:
                {
                    const float z = v1[0], x = v1[2], y = v1[3];
                    float tc, sc, ma, face;
                    if (std::fabs(z) >= std::fabs(x) && std::fabs(z) >= std::fabs(y))
                        { tc = -y; sc = z < 0 ? -x : x; ma = z; face = z < 0 ? 5.0f : 4.0f; }
                    else if (std::fabs(y) >= std::fabs(x))
                        { tc = y < 0 ? -z : z; sc = x; ma = y; face = y < 0 ? 3.0f : 2.0f; }
                    else
                        { tc = -y; sc = x < 0 ? z : -z; ma = x; face = x < 0 ? 1.0f : 0.0f; }
                    vr[0] = tc; vr[1] = sc; vr[2] = Flush(ma * 2.0f); vr[3] = face;
                    break;
                }
                case 19:
                {
                    const float m = Max(Max(Max(v1[0], v1[1]), v1[2]), v1[3]);
                    for (float& x : vr) x = m;
                    break;
                }
                case 20: case 21: case 22: case 23:
                {
                    const uint32_t kind = vop - 20;
                    s.p0 = v1[3] == 0.0f && cmp(kind, v2[3]);
                    const float x = (v1[0] == 0.0f && cmp(kind, v2[0])) ? 0.0f : Add(v1[0], 1.0f);
                    for (float& y : vr) y = x;
                    break;
                }
                case 24: case 25: case 26: case 27:
                {
                    bool hit = false;
                    for (int k = 0; k < 4; k++)
                    {
                        const float a = v1[k], b = v2[k];
                        hit |= vop == 24 ? a == b : vop == 25 ? a > b : vop == 26 ? a >= b : a != b;
                    }
                    if (hit)
                        s.killed = true;
                    for (float& y : vr) y = hit ? 1.0f : 0.0f;
                    break;
                }
                case 28: vr[0] = 1.0f; vr[1] = Mul(v1[1], v2[1]); vr[2] = v1[2]; vr[3] = v2[3]; break;
                case 29:
                    s.a0 = ClampA0(std::floor(Add(v1[3], 0.5f)));
                    each([&](int k) { return Max(v1[k], v2[k]); });
                    break;
                }

                // Scalar op (03c §6).
                const float ps = before.ps;
                float sr = 0.0f;
                switch (sop)
                {
                case 0: sr = Add(sa, sb); break;
                case 1: sr = Add(sa, ps); break;
                case 2: sr = Mul(sa, sb); break;
                case 3: sr = Mul(sa, ps); break;
                case 4: sr = (ps == -FLT_MAX || ps == -INFINITY || IsNan(ps) || sb <= 0.0f || IsNan(sb)) ? -FLT_MAX : Mul(sa, ps); break;
                case 5: sr = Max(sa, sb); break;
                case 6: sr = Min(sa, sb); break;
                case 7: sr = sa == 0.0f ? 1.0f : 0.0f; break;
                case 8: sr = sa > 0.0f ? 1.0f : 0.0f; break;
                case 9: sr = sa >= 0.0f ? 1.0f : 0.0f; break;
                case 10: sr = sa != 0.0f ? 1.0f : 0.0f; break;
                case 11: sr = Sub(sa, std::floor(sa)); break;
                case 12: sr = std::trunc(sa); break;
                case 13: sr = std::floor(sa); break;
                case 14: sr = sa == 0.0f ? 1.0f : Flush(std::exp2(sa)); break;
                case 15: sr = sa == 1.0f ? 0.0f : std::log2(sa); if (sr == -INFINITY) sr = -FLT_MAX; break;
                case 16: sr = sa == 1.0f ? 0.0f : std::log2(sa); break;
                case 17: case 18: case 19:
                    sr = sa == 1.0f ? 1.0f : Flush(1.0f / sa);
                    if (std::isinf(sr) && sop != 19)
                        sr = std::copysign(sop == 17 ? FLT_MAX : 0.0f, sr);
                    break;
                case 20: case 21: case 22:
                    sr = sa == 1.0f ? 1.0f : Flush(1.0f / std::sqrt(sa));
                    if (std::isinf(sr) && sop != 22)
                        sr = std::copysign(sop == 20 ? FLT_MAX : 0.0f, sr);
                    break;
                case 23: s.a0 = ClampA0(std::floor(Add(sa, 0.5f))); sr = Max(sa, sb); break;
                case 24: s.a0 = ClampA0(std::floor(sa)); sr = Max(sa, sb); break;
                case 25: sr = Sub(sa, sb); break;
                case 26: sr = Sub(sa, ps); break;
                case 27: s.p0 = sa == 0.0f; sr = s.p0 ? 0.0f : 1.0f; break;
                case 28: s.p0 = sa != 0.0f; sr = s.p0 ? 0.0f : 1.0f; break;
                case 29: s.p0 = sa > 0.0f; sr = s.p0 ? 0.0f : 1.0f; break;
                case 30: s.p0 = sa >= 0.0f; sr = s.p0 ? 0.0f : 1.0f; break;
                case 31: s.p0 = sa == 1.0f; sr = s.p0 ? 0.0f : (sa == 0.0f ? 1.0f : sa); break;
                case 32: { const float t = Sub(sa, 1.0f); s.p0 = t <= 0.0f; sr = s.p0 ? 0.0f : t; break; }
                case 33: s.p0 = false; sr = FLT_MAX; break;
                case 34: s.p0 = sa == 0.0f; sr = sa; break;
                case 35: case 36: case 37: case 38: case 39:
                {
                    const bool hit = sop == 35 ? sa == 0.0f : sop == 36 ? sa > 0.0f : sop == 37 ? sa >= 0.0f
                                   : sop == 38 ? sa != 0.0f : sa == 1.0f;
                    if (hit)
                        s.killed = true;
                    sr = hit ? 1.0f : 0.0f;
                    break;
                }
                case 40: sr = std::sqrt(sa); break;
                case 42: case 43: sr = Mul(sa, sb); break;
                case 44: case 45: sr = Add(sa, sb); break;
                case 46: case 47: sr = Sub(sa, sb); break;
                case 48: sr = std::sin(sa); break;
                case 49: sr = std::cos(sa); break;
                case 50: sr = ps; break;
                }
                s.ps = sr;
                if (alu.vectorSaturate())
                    for (float& x : vr) x = Sat(x);
                const float srw = alu.scalarSaturate() ? Sat(sr) : sr;

                // Writes (no exports in these tests): vector, then scalar.
                const uint32_t vd = alu.vectorDestRelative() ? (alu.vectorDest() + uint32_t(before.aL)) & 63 : alu.vectorDest();
                const uint32_t sd = alu.scalarDestRelative() ? (alu.scalarDest() + uint32_t(before.aL)) & 63 : alu.scalarDest();
                for (int k = 0; k < 4; k++)
                    if ((alu.vectorMask() >> k) & 1)
                        s.r[vd][k] = vr[k];
                for (int k = 0; k < 4; k++)
                    if ((alu.scalarMask() >> k) & 1)
                        s.r[sd][k] = srw;
                return sa;
            }
        };

        bool IsApproximate(uint32_t sop)
        {
            return (sop >= 14 && sop <= 22) || sop == 40 || sop == 48 || sop == 49;
        }

        bool SameBits(float a, float b)
        {
            return std::bit_cast<uint32_t>(a) == std::bit_cast<uint32_t>(b) || (std::isnan(a) && std::isnan(b));
        }

        // Transcendentals: exact on special results, otherwise within a relative 2^-20 (sin/cos: absolute 2^-16 for
        // |input| <= pi, unchecked beyond, where the spec does not define them).
        bool Close(float gpu, float cpu, uint32_t sop, float input)
        {
            if ((sop == 48 || sop == 49) && std::isfinite(input) && std::fabs(input) > 3.14159265f)
                return true;
            if (std::isnan(cpu) || std::isinf(cpu) || cpu == 0.0f || std::fabs(cpu) == FLT_MAX)
                return SameBits(gpu, cpu);
            if (sop == 48 || sop == 49)
                return std::fabs(input) > 3.14159265f || std::fabs(gpu - cpu) <= 1.0f / 65536.0f;
            // log2: host precision is an absolute error near 1 (D3D: 2^-21 on [0.5, 2]).
            if ((sop == 15 || sop == 16) && std::fabs(gpu - cpu) <= 1.0f / 2097152.0f)
                return true;
            return std::fabs(gpu - cpu) <= std::fabs(cpu) * (1.0f / 1048576.0f);
        }

        // ---- Instruction generation -------------------------------------------------------------------------------

        const float kPool[] = {
            0.0f, -0.0f, 1.0f, -1.0f, 0.5f, -0.5f, 2.0f, 3.5f, -7.25f, 1e-40f, -1e-40f, INFINITY, -INFINITY, NAN,
            FLT_MAX, -FLT_MAX, 1e30f, -1e30f, 1e-30f, 0.99999994f, 255.5f, -256.5f, 100.0f, 1e-20f, 1.5f, -2.5f,
            3.14159265f, 0.25f, 1e20f, 65504.0f, -1e-30f, 12.0f,
        };

        struct Generator
        {
            std::mt19937 rng{ 0x58454E4F };
            uint32_t Next(uint32_t n) { return std::uniform_int_distribution<uint32_t>(0, n - 1)(rng); }
            float Value()
            {
                if (Next(4) == 0)
                    return std::uniform_real_distribution<float>(-1000.0f, 1000.0f)(rng);
                return kPool[Next(uint32_t(std::size(kPool)))];
            }

            // A random valid ALU instruction with vector op `vop` and scalar op `sop` writing r0-r7.
            Alu Make(uint32_t vop, uint32_t sop, uint32_t vmask, uint32_t smask)
            {
                uint32_t w0 = 0, w1 = 0, w2 = 0;
                w0 |= Next(8) | (Next(5) == 0 ? 0x40u : 0) | (Next(4) == 0 ? 0x80u : 0);
                w0 |= Next(8) << 8 | (Next(5) == 0 ? 1u << 14 : 0);
                w0 |= vmask << 16 | smask << 20 | Next(4) / 3 << 24 | Next(4) / 3 << 25 | sop << 26;
                for (int i = 0; i < 3; i++)
                    w1 |= Next(256) << (8 * i);
                w1 |= Next(2) << 24 | Next(2) << 25 | Next(2) << 26;
                w1 |= Next(2) << 27 | (Next(4) == 0 ? 1u << 28 : 0) | Next(2) << 29 | Next(3) / 2 << 30 | Next(3) / 2 << 31;
                for (int i = 0; i < 3; i++)
                {
                    const bool temp = Next(3) != 0;
                    const uint32_t reg = temp ? Next(8) | (Next(6) == 0 ? 0x40u : 0) | (Next(4) == 0 ? 0x80u : 0) : Next(kConstantCount);
                    w2 |= reg << (8 * i);
                    w2 |= uint32_t(temp) << (29 + i);
                }
                w2 |= vop << 24;
                Alu alu{ w0, w1, w2 };
                // mulsc/addsc/subsc build their temp from src3 fields; keep it within the tested registers r0-r7.
                if (sop >= 42 && sop <= 47 && alu.scalarConstantTempReg() >= 8)
                    alu.w1 &= ~0x38u;
                return alu;
            }
        };

        bool WritesSideEffect(uint32_t vop, uint32_t sop)
        {
            const bool v = (vop >= 20 && vop <= 27) || vop == 29;
            const bool s = (sop >= 23 && sop <= 24) || (sop >= 27 && sop <= 39);
            return v && s;
        }

        std::vector<uint32_t> MicrocodeFor(const Alu& alu)
        {
            // CF 0: exece of one ALU instruction at slot 1; CF 1: nop.
            const uint64_t cf0 = 1 | (1ull << 12) | (uint64_t(kCfExecEnd) << 44);
            return { uint32_t(cf0), uint32_t(cf0 >> 32) & 0xFFFF, 0, alu.w0, alu.w1, alu.w2 };
        }

        int TranslateDumpedShaders()
        {
            const std::filesystem::path directory = "analysis/shaders";
            int failures = 0, count = 0;
            if (!std::filesystem::exists(directory))
            {
                Log("Shader test: {} does not exist; run the game with CB_GPU_DUMP_SHADERS=1 first", directory.string());
                return 0;
            }
            for (const auto& entry : std::filesystem::directory_iterator(directory))
            {
                if (entry.path().extension() != ".bin")
                    continue;
                const std::string name = entry.path().stem().string();
                const bool vertex = name.starts_with("vertex_");
                std::ifstream file(entry.path(), std::ios::binary);
                std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(file)), {});
                Shader shader;
                shader.stage = vertex ? Stage::kVertex : Stage::kPixel;
                shader.hash = std::stoull(name.substr(name.find('_') + 1), nullptr, 16);
                for (size_t i = 0; i + 4 <= bytes.size(); i += 4)
                    shader.words.push_back(uint32_t(bytes[i]) << 24 | uint32_t(bytes[i + 1]) << 16 | uint32_t(bytes[i + 2]) << 8 | bytes[i + 3]);
                if (shader.words.empty())
                    continue;
                shader.disassembly = Disassemble(shader.stage, shader.words, shader.analysis);
                TranslationOptions options;
                options.registerCount = 32;
                const TranslatedShader translated = TranslateShader(shader, options);
                std::vector<uint8_t> bytecode;
                std::string errors;
                count++;
                if (!CompileHlsl(translated.hlsl, vertex ? "vs_5_1" : "ps_5_1", name.c_str(), bytecode, errors))
                {
                    failures++;
                    Log("Shader test: {} FAILED to compile: {}", name, errors);
                }
                if (!vertex)
                {
                    // Also through the EDRAM output merger, with the title screen's state: 8_8_8_8 RT0 with
                    // SRCALPHA/INVSRCALPHA blending, alpha test GREATER, D24S8 with stencil and depth.
                    options.useOutputMerger = true;
                    options.outputMerger.colorFormat[0] = 0;
                    options.outputMerger.colorMask[0] = 0xF;
                    options.outputMerger.blendControl[0] = 0x07060706;
                    options.outputMerger.alphaTest = 0xC;
                    options.outputMerger.depthControl = 0x00700237;
                    options.outputMerger.shaderDepth = shader.analysis.writesDepth;
                    const TranslatedShader rov = TranslateShader(shader, options);
                    if (!CompileHlsl(rov.hlsl, "ps_5_1", name.c_str(), bytecode, errors))
                    {
                        failures++;
                        Log("Shader test: {} FAILED to compile with the output merger: {}", name, errors);
                    }
                }
            }
            Log("Shader test: {} dumped shaders translated, {} failed to compile", count, failures);
            return failures;
        }
    }

    namespace
    {
        // ---- Control-flow programs with hand-computed results (03a §3-4) ---------------------------------------

        uint64_t CfExec(uint32_t op, uint32_t address, uint32_t count, uint32_t sequence, uint32_t boolIndex = 0,
                        bool condition = false)
        {
            return uint64_t(address) | uint64_t(count) << 12 | uint64_t(sequence) << 16 | uint64_t(boolIndex) << 34 |
                   uint64_t(condition) << 42 | uint64_t(op) << 44;
        }
        uint64_t CfTarget(uint32_t op, uint32_t target, bool force, uint32_t boolIndex = 0, bool condition = false,
                          uint32_t loopId = 0)
        {
            return uint64_t(target) | uint64_t(force) << 13 | uint64_t(loopId) << 16 | uint64_t(boolIndex) << 34 |
                   uint64_t(condition) << 42 | uint64_t(op) << 44;
        }

        // vector op `vop` rD.xyzw = (rA, src2), src2 = rB or cB; scalar retain_prev, no scalar write.
        Alu VectorAlu(uint32_t vop, uint32_t dest, uint32_t a, uint32_t b, bool bIsConstant, uint32_t mask = 0xF,
                      bool predicated = false, bool condition = false)
        {
            Alu alu{};
            alu.w0 = dest | mask << 16 | 50u << 26;
            alu.w1 = uint32_t(condition) << 27 | uint32_t(predicated) << 28;
            alu.w2 = a << 16 | b << 8 | vop << 24 | 1u << 31 | uint32_t(!bIsConstant) << 30 | 1u << 29;
            return alu;
        }

        std::vector<uint32_t> Program(const std::vector<uint64_t>& cfs, const std::vector<Alu>& slots)
        {
            std::vector<uint64_t> padded = cfs;
            if (padded.size() & 1)
                padded.push_back(0); // nop
            std::vector<uint32_t> words;
            for (size_t i = 0; i < padded.size(); i += 2)
            {
                const uint64_t a = padded[i], b = padded[i + 1];
                words.push_back(uint32_t(a));
                words.push_back(uint32_t(a >> 32 & 0xFFFF) | uint32_t(b & 0xFFFF) << 16);
                words.push_back(uint32_t(b >> 16 & 0xFFFF) | uint32_t(b >> 32 & 0xFFFF) << 16);
            }
            for (const Alu& alu : slots)
            {
                words.push_back(alu.w0);
                words.push_back(alu.w1);
                words.push_back(alu.w2);
            }
            return words;
        }

        struct FlowTest
        {
            const char* name;
            std::vector<uint32_t> words;
            uint32_t boolLoop[40];
            float expected[5]; // r0.x-r4.x
        };

        int RunControlFlowTests(ComputeRunner& runner)
        {
            // c0 = (1,1,1,1) in these tests: replace constant 0 for the run.
            constexpr uint32_t kAdd = 0, kMax = 2;
            std::vector<FlowTest> tests;
            {
                // loop i0 (count 3, start 0, step 1): r0 += 1; r[1+aL] = r0. Then r4 = r0.
                FlowTest t{ "loop with aL-relative writes" };
                Alu relative = VectorAlu(kMax, 1, 0, 0, false);
                relative.w0 |= 0x40; // r[1+aL]
                const uint32_t base = 2; // 4 CFs = 2 pairs
                t.words = Program({ CfTarget(kCfLoopStart, 3, false, 0, false, 0), CfExec(kCfExec, base, 2, 0),
                                    CfTarget(kCfLoopEnd, 1, false, 0, false, 0), CfExec(kCfExecEnd, base + 2, 1, 0) },
                                  { VectorAlu(kAdd, 0, 0, 0, true), relative, VectorAlu(kMax, 4, 0, 0, false) });
                std::memset(t.boolLoop, 0, sizeof(t.boolLoop));
                t.boolLoop[8] = 3 | 0 << 8 | 1 << 16;
                const float e[5] = { 3, 1, 2, 3, 3 };
                std::memcpy(t.expected, e, sizeof(e));
                tests.push_back(t);
            }
            {
                // jmp if b0 (set) over "r1 += 1"; then r2 += 1; r3 += 1 (end).
                FlowTest t{ "bool-conditional jump" };
                const uint32_t base = 2;
                t.words = Program({ CfTarget(kCfCondJmp, 2, false, 0, true), CfExec(kCfExec, base, 1, 0),
                                    CfExec(kCfExec, base + 1, 1, 0), CfExec(kCfExecEnd, base + 2, 1, 0) },
                                  { VectorAlu(kAdd, 1, 1, 0, true), VectorAlu(kAdd, 2, 2, 0, true), VectorAlu(kAdd, 3, 3, 0, true) });
                std::memset(t.boolLoop, 0, sizeof(t.boolLoop));
                t.boolLoop[0] = 1;
                const float e[5] = { 0, 0, 1, 1, 0 };
                std::memcpy(t.expected, e, sizeof(e));
                tests.push_back(t);
            }
            {
                // call CF3 (r3 += 1; ret) -> back to CF1 (r1 += 1) -> CF2 (r2 += 1, end).
                FlowTest t{ "call and return" };
                const uint32_t base = 3; // 5 CFs + nop = 3 pairs
                t.words = Program({ CfTarget(kCfCondCall, 3, true), CfExec(kCfExec, base, 1, 0), CfExec(kCfExecEnd, base + 1, 1, 0),
                                    CfExec(kCfExec, base + 2, 1, 0), uint64_t(kCfReturn) << 44 },
                                  { VectorAlu(kAdd, 1, 1, 0, true), VectorAlu(kAdd, 2, 2, 0, true), VectorAlu(kAdd, 3, 3, 0, true) });
                std::memset(t.boolLoop, 0, sizeof(t.boolLoop));
                const float e[5] = { 0, 1, 1, 1, 0 };
                std::memcpy(t.expected, e, sizeof(e));
                tests.push_back(t);
            }
            {
                // setp_eq on r0.x (0) -> p0; (p0) exec r1 += 1; (!p0) exec r2 += 1; cexec b1 (clear) r3 += 1;
                // predicated ALU (!p0) r4 += 1 inside a plain exece, then r4 += 1 unpredicated.
                FlowTest t{ "predication and bool-conditional exec" };
                const uint32_t base = 3; // 5 CFs + nop
                Alu setp{};
                setp.w0 = 27u << 26;                // setp_eq, no writes
                setp.w2 = kMax << 24 | 1u << 29 | 1u << 30 | 1u << 31; // all operands r0 (temps)
                setp.w1 = 0x40;                      // src3 swizzle W slot: ((1 + 3) & 3) = x
                t.words = Program({ CfExec(kCfExec, base, 1, 0), CfExec(kCfCondExecPred, base + 1, 1, 0, 0, true),
                                    CfExec(kCfCondExecPred, base + 2, 1, 0, 0, false), CfExec(kCfCondExec, base + 3, 1, 0, 1, true),
                                    CfExec(kCfExecEnd, base + 4, 2, 0) },
                                  { setp, VectorAlu(kAdd, 1, 1, 0, true), VectorAlu(kAdd, 2, 2, 0, true), VectorAlu(kAdd, 3, 3, 0, true),
                                    VectorAlu(kAdd, 4, 4, 0, true, 0xF, true, false), VectorAlu(kAdd, 4, 4, 0, true) });
                std::memset(t.boolLoop, 0, sizeof(t.boolLoop));
                const float e[5] = { 0, 1, 0, 0, 1 };
                std::memcpy(t.expected, e, sizeof(e));
                tests.push_back(t);
            }

            int failures = 0;
            for (FlowTest& t : tests)
            {
                Shader shader;
                shader.stage = Stage::kVertex;
                shader.words = t.words;
                shader.disassembly = Disassemble(shader.stage, shader.words, shader.analysis);
                if (!shader.analysis.errors.empty())
                    Fatal("Shader test: flow program '{}' is invalid: {}\n{}", t.name, shader.analysis.errors.front(), shader.disassembly);
                TranslationOptions options;
                options.computeHarness = true;
                const TranslatedShader translated = TranslateShader(shader, options);
                std::vector<uint8_t> bytecode;
                std::string errors;
                if (!CompileHlsl(translated.hlsl, "cs_5_1", "flow_test", bytecode, errors))
                {
                    std::error_code error;
                    std::filesystem::create_directories("analysis/shader_test", error);
                    std::ofstream("analysis/shader_test/flow_failed.hlsl") << translated.hlsl;
                    Fatal("Shader test: flow program '{}' failed to compile (analysis/shader_test/flow_failed.hlsl): {}",
                          t.name, errors);
                }
                runner.SetBoolLoop(t.boolLoop);
                const std::vector<uint8_t> output = runner.Run(bytecode, std::vector<uint8_t>(kHarnessCaseBytes, 0));
                float r[kHarnessRegisters][4];
                std::memcpy(r, output.data(), sizeof(r));
                std::string got;
                bool ok = true;
                for (int i = 0; i < 5; i++)
                {
                    got += std::format(" r{}.x={}", i, r[i][0]);
                    ok &= r[i][0] == t.expected[i];
                }
                if (!ok)
                {
                    std::error_code error;
                    std::filesystem::create_directories("analysis/shader_test", error);
                    std::ofstream(std::format("analysis/shader_test/flow_{}.hlsl", failures)) << translated.hlsl;
                    failures++;
                    Log("Shader test: flow '{}' FAILED:{} (expected {} {} {} {} {})\n{}", t.name, got, t.expected[0], t.expected[1],
                        t.expected[2], t.expected[3], t.expected[4], shader.disassembly);
                }
            }
            Log("Shader test: {} control-flow programs, {} failed", tests.size(), failures);
            return failures;
        }
    }

    namespace
    {
        // ---- EDRAM output merger (output_merger.h) against its CPU reference ------------------------------------
        //
        // Per random key, one compute shader runs the generated per-key functions over random cases: the colour of
        // every enabled RT over a random old dword, the depth encode and the depth/stencil update, and the EDRAM
        // address of a random pixel/sample. Results must match output_merger_reference.cpp bit for bit.

        OutputMergerKey RandomOutputMergerKey(Generator& gen)
        {
            static const uint32_t kFactors[] = { 0, 1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
            static const uint32_t kFixed16Factors[] = { 0, 1, 4, 5, 8, 9, 12, 13, 14, 15 }; // no RT-alpha factors
            OutputMergerKey key;
            key.msaa = gen.Next(3);
            for (uint32_t rt = 0; rt < 4; rt++)
            {
                if (gen.Next(3) == 0)
                    continue;
                const uint32_t format = gen.Next(2) ? 0 : 4;
                key.colorFormat[rt] = format;
                key.colorMask[rt] = format == 4 ? 1 + gen.Next(3) : 1 + gen.Next(15);
                auto factor = [&] {
                    return format == 4 ? kFixed16Factors[gen.Next(uint32_t(std::size(kFixed16Factors)))]
                                       : kFactors[gen.Next(uint32_t(std::size(kFactors)))];
                };
                if (gen.Next(4) == 0)
                    key.blendControl[rt] = 0x00010001; // ONE/ZERO/ADD: no blending
                else
                    key.blendControl[rt] = factor() | gen.Next(5) << 5 | factor() << 8 | factor() << 16 |
                                           gen.Next(5) << 21 | factor() << 24;
                key.expBias[rt] = gen.Next(4) == 0 ? int32_t(gen.Next(9)) - 4 : 0;
            }
            const uint32_t control = gen.Next(65536) << 16 | gen.Next(65536);
            key.depthControl = (control & 3) ? control : 0;
            key.depthFormat = gen.Next(2);
            key.polygonal = gen.Next(2);
            return key;
        }

        float RandomColorComponent(Generator& gen, uint32_t format)
        {
            switch (gen.Next(4))
            {
            case 0: return gen.Value();
            case 1: return std::uniform_real_distribution<float>(-0.25f, 1.25f)(gen.rng);
            case 2: return format == 4 ? std::uniform_real_distribution<float>(-40.0f, 40.0f)(gen.rng)
                                       : float(gen.Next(256)) / 255.0f;
            default: return std::uniform_real_distribution<float>(0.0f, 1.0f)(gen.rng);
            }
        }

        int RunOutputMergerTests(ComputeRunner& runner, Generator& gen)
        {
            constexpr uint32_t kKeys = 400, kCases = 64;
            uint32_t mismatches = 0, compared = 0;
            for (uint32_t k = 0; k < kKeys; k++)
            {
                const OutputMergerKey key = RandomOutputMergerKey(gen);
                const bool depthStencil = key.depthControl != 0;
                std::string source = std::string(kXenosHlslLibrary) + OutputMergerHlsl(key, OutputMergerTarget::kHarness);
                source += std::format(R"(
RWByteAddressBuffer xe_test_io : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{{
    uint base = id.x * {}u;
    float4 color = asfloat(xe_test_io.Load4(base));
    uint4 old = xe_test_io.Load4(base + 16u);
    uint4 ds = xe_test_io.Load4(base + 32u);   // x: z (float bits), y: old depth/stencil, z: front face
    uint4 at = xe_test_io.Load4(base + 48u);   // x, y: pixel, z: sample, w: base | pitch << 16
    uint4 colors = uint4(0u, 0u, 0u, 0u);
    uint4 depth = uint4(0u, 0u, 0u, 0u);
)", kHarnessCaseBytes);
                for (uint32_t rt = 0; rt < 4; rt++)
                    if (key.colorFormat[rt] != kRtDisabled)
                        source += std::format("    colors.{} = xe_om_color_{}(color, old.{});\n", "xyzw"[rt], rt, "xyzw"[rt]);
                if (depthStencil)
                    source += "    depth.z = xe_om_depth_encode(asfloat(ds.x));\n"
                              "    bool passed;\n"
                              "    depth.x = xe_om_depth_stencil(ds.y, depth.z, ds.z != 0u, passed);\n"
                              "    depth.y = passed ? 1u : 0u;\n";
                source += std::format(R"(    uint2 q = xe_edram_sample(at.xy, at.z, {}u);
    uint4 addresses = uint4(xe_edram_address(at.w & 0xFFFFu, at.w >> 16, q, false),
                            xe_edram_address(at.w & 0xFFFFu, at.w >> 16, q, true), q.x, q.y);
    xe_test_io.Store4(base, colors);
    xe_test_io.Store4(base + 16u, depth);
    xe_test_io.Store4(base + 32u, addresses);
}}
)", key.msaa);
                std::vector<uint8_t> bytecode;
                std::string errors;
                if (!CompileHlsl(source, "cs_5_1", "output_merger_test", bytecode, errors))
                    Fatal("Shader test: output-merger harness failed to compile: {}\n{}", errors, source);

                OutputMergerConstants constants = {};
                for (float& c : constants.blendConstant)
                    c = gen.Next(3) == 0 ? gen.Value() : std::uniform_real_distribution<float>(-0.5f, 1.5f)(gen.rng);
                constants.stencilFront = gen.Next(1u << 24);
                constants.stencilBack = gen.Next(1u << 24);
                runner.SetOutput(constants);

                struct Case
                {
                    float color[4];
                    uint32_t old[4];
                    float z;
                    uint32_t ds, front, x, y, sample, base, pitch;
                };
                std::vector<Case> cases(kCases);
                std::vector<uint8_t> input(kCases * kHarnessCaseBytes, 0);
                const uint32_t format0 = key.colorFormat[0] == 4 ? 4 : 0;
                for (uint32_t i = 0; i < kCases; i++)
                {
                    Case& c = cases[i];
                    for (float& v : c.color)
                        v = RandomColorComponent(gen, format0);
                    for (uint32_t& o : c.old)
                        o = gen.Next(65536) << 16 | gen.Next(65536);
                    switch (gen.Next(3))
                    {
                    case 0: c.z = gen.Value(); break;
                    case 1: c.z = float(gen.Next(5)) * 0.25f; break;
                    default: c.z = std::uniform_real_distribution<float>(-0.25f, 1.25f)(gen.rng); break;
                    }
                    c.ds = gen.Next(65536) << 16 | gen.Next(65536);
                    c.front = gen.Next(2);
                    c.x = gen.Next(1280);
                    c.y = gen.Next(720);
                    c.sample = key.msaa == 0 ? 0 : gen.Next(key.msaa == 1 ? 2 : 4);
                    c.base = gen.Next(2048);
                    c.pitch = 1 + gen.Next(40);
                    uint8_t* out = input.data() + i * kHarnessCaseBytes;
                    const uint32_t words[16] = {
                        std::bit_cast<uint32_t>(c.color[0]), std::bit_cast<uint32_t>(c.color[1]),
                        std::bit_cast<uint32_t>(c.color[2]), std::bit_cast<uint32_t>(c.color[3]),
                        c.old[0], c.old[1], c.old[2], c.old[3],
                        std::bit_cast<uint32_t>(c.z), c.ds, c.front, 0,
                        c.x, c.y, c.sample, c.base | c.pitch << 16 };
                    std::memcpy(out, words, sizeof(words));
                }
                const std::vector<uint8_t> output = runner.Run(bytecode, input);

                for (uint32_t i = 0; i < kCases; i++)
                {
                    const Case& c = cases[i];
                    uint32_t got[12];
                    std::memcpy(got, output.data() + i * kHarnessCaseBytes, sizeof(got));
                    std::string problems;
                    for (uint32_t rt = 0; rt < 4; rt++)
                    {
                        if (key.colorFormat[rt] == kRtDisabled)
                            continue;
                        const uint32_t expected = ReferenceColor(key, rt, c.color, c.old[rt], constants);
                        if (got[rt] != expected)
                            problems += std::format(" rt{} ({} mask {:X} blend {:08X} bias {}): gpu {:08X} cpu {:08X};", rt,
                                                    EdramColorFormatName(key.colorFormat[rt]), key.colorMask[rt],
                                                    key.blendControl[rt], key.expBias[rt], got[rt], expected);
                    }
                    if (depthStencil)
                    {
                        const uint32_t z24 = key.depthFormat ? ReferenceDepthFloat24(c.z) : ReferenceDepthUnorm24(c.z);
                        bool passed = false;
                        const uint32_t ds = ReferenceDepthStencil(key, c.ds, z24, c.front != 0, constants, passed);
                        if (got[6] != z24)
                            problems += std::format(" depth encode ({}) of {:08X}: gpu {:06X} cpu {:06X};",
                                                    key.depthFormat ? "20e4" : "unorm24", std::bit_cast<uint32_t>(c.z), got[6], z24);
                        else if (got[4] != ds || (got[5] != 0) != passed)
                            problems += std::format(" depth/stencil (control {:08X} front {}): gpu {:08X}/{} cpu {:08X}/{};",
                                                    key.depthControl, c.front, got[4], got[5], ds, int(passed));
                    }
                    uint32_t sx, sy;
                    EdramSample(key.msaa, c.x, c.y, c.sample, sx, sy);
                    const uint32_t colorAddress = EdramAddress(c.base, c.pitch, sx, sy, false);
                    const uint32_t depthAddress = EdramAddress(c.base, c.pitch, sx, sy, true);
                    if (got[10] != sx || got[11] != sy || got[8] != colorAddress || got[9] != depthAddress)
                        problems += std::format(" address (msaa {} pixel {},{} s{} base {} pitch {}): gpu {},{} {:X}/{:X}"
                                                " cpu {},{} {:X}/{:X};", key.msaa, c.x, c.y, c.sample, c.base, c.pitch,
                                                got[10], got[11], got[8], got[9], sx, sy, colorAddress, depthAddress);
                    compared++;
                    if (!problems.empty() && mismatches++ < 30)
                        Log("Shader test: OUTPUT MERGER MISMATCH key {} case {}: colour ({} {} {} {}) z {}{}", k, i,
                            c.color[0], c.color[1], c.color[2], c.color[3], c.z, problems);
                }
            }
            Log("Shader test: output merger: {} cases over {} keys compared, {} mismatches", compared, kKeys, mismatches);
            return int(mismatches);
        }

        // Resolves: the copy shader over a random EDRAM image into a random guest-memory region, and the clear shader,
        // compared byte for byte with the CPU reference (resolve_reference.cpp). Also checks that every byte the
        // reference writes lies inside the extent that is marked dirty for the texture cache.
        int RunResolveTests(ComputeRunner& runner, Generator& gen)
        {
            constexpr uint32_t kMemoryBytes = 4 * 1024 * 1024;
            constexpr uint32_t kCopies = 72, kClears = 24;
            uint32_t mismatches = 0;

            auto randomEdram = [&](std::vector<uint32_t>& edram) {
                static const uint16_t kSpecial[] = { 0x0000, 0x8000, 0x8001, 0x7FFF, 0xFFFF, 0x0001, 0x00FF, 0xFF00 };
                edram.resize(kEdramBytes / 4);
                for (uint32_t& d : edram)
                {
                    d = uint32_t(gen.rng());
                    if (gen.Next(8) == 0)
                        d = (d & 0xFFFF0000u) | kSpecial[gen.Next(8)];
                    if (gen.Next(8) == 0)
                        d = (d & 0xFFFFu) | uint32_t(kSpecial[gen.Next(8)]) << 16;
                }
            };
            auto run = [&](const std::string& source, const void* constants, size_t constantsSize, std::vector<uint32_t>& edram,
                           std::vector<uint8_t>& memory, UINT groupsX, UINT groupsY) {
                std::vector<uint8_t> bytecode;
                std::string errors;
                if (!CompileHlsl(std::string(kXenosHlslLibrary) + source, "cs_5_1", "resolve_test", bytecode, errors))
                    Fatal("Shader test: resolve harness failed to compile: {}\n{}", errors, source);
                std::vector<uint8_t> io(kEdramBytes + kMemoryBytes);
                std::memcpy(io.data(), edram.data(), kEdramBytes);
                std::memcpy(io.data() + kEdramBytes, memory.data(), kMemoryBytes);
                runner.SetOutputBytes(constants, constantsSize);
                const std::vector<uint8_t> out = runner.Run(bytecode, io, groupsX, groupsY);
                std::memcpy(edram.data(), out.data(), kEdramBytes);
                std::memcpy(memory.data(), out.data() + kEdramBytes, kMemoryBytes);
            };

            static const uint32_t kDestFormats[] = { 6, 14, 50, 7, 54, 16, 55, 17, 56, 13, 25, 36 };
            uint32_t pixels = 0;
            for (uint32_t t = 0; t < kCopies; t++)
            {
                ResolveKey key;
                key.msaa = gen.Next(3);
                key.depth = gen.Next(5) == 0;
                key.sourceFormat = key.depth ? 0 : (gen.Next(2) ? 4 : 0);
                static const uint32_t kSelects[3][7] = { { 0 }, { 0, 1, 4 }, { 0, 1, 2, 3, 4, 5, 6 } };
                const uint32_t selectCount = key.msaa == 0 ? 1 : key.msaa == 1 ? (key.depth ? 2 : 3) : (key.depth ? 4 : 7);
                key.select = kSelects[key.msaa][gen.Next(selectCount)];
                key.endian = gen.Next(4);
                if (key.depth)
                {
                    key.destFormat = 22 + gen.Next(2);
                    key.raw = true;
                }
                else
                {
                    key.destFormat = kDestFormats[gen.Next(uint32_t(std::size(kDestFormats)))];
                    key.destNumber = key.destFormat == 36 ? 7 : gen.Next(4);
                    key.swap = gen.Next(2);
                    key.expBias = gen.Next(3) == 0 ? int32_t(gen.Next(64)) - 32 : gen.Next(2) ? 0 : -5;
                    const bool equivalent = key.sourceFormat == 0 && (key.destFormat == 6 || key.destFormat == 14 || key.destFormat == 50);
                    key.raw = key.expBias == 0 && key.select < 4 && equivalent && key.destNumber == 0;
                }
                // The title's own configurations first: the front buffer and the 16_16 -> G16R16 bias -5 resolves.
                if (t == 0)
                    key = { 0, 0, false, 0, true, 6, 0, true, 0, 0 };
                else if (t == 1 || t == 2)
                    key = { 0, 0, false, 4, false, 25, t == 1 ? 0u : 1u, false, 0, -5 };

                ResolveConstants c = {};
                c.x0 = 8 * gen.Next(81);
                c.y0 = 8 * gen.Next(61);
                c.width = 8 * (1 + gen.Next(40));
                c.height = 8 * (1 + gen.Next(30));
                if (t == 0)
                    c = { 0, 0, 1280, 720 };
                c.sourceBase = gen.Next(2048);
                c.pitchTiles = t == 0 ? 16 : 1 + gen.Next(24);
                c.destBase = 4096 * gen.Next(64);
                c.destPitchMacro = (c.x0 + c.width + 31) / 32 + (t == 0 ? 0 : gen.Next(3));
                uint32_t start, end;
                ResolveExtent(c, start, end);
                if (end > kMemoryBytes)
                    Fatal("Shader test: resolve case {} extent {:X} exceeds the test memory", t, end);

                std::vector<uint32_t> edram;
                randomEdram(edram);
                std::vector<uint8_t> memory(kMemoryBytes);
                for (uint8_t& b : memory)
                    b = uint8_t(gen.rng());
                std::vector<uint8_t> expected = memory;
                ReferenceResolve(key, c, edram.data(), expected.data());
                for (uint32_t i = 0; i < kMemoryBytes; i++)
                    if (expected[i] != memory[i] && (i < start || i >= end))
                        Fatal("Shader test: resolve case {} writes byte {:X} outside its extent {:X}-{:X}", t, i, start, end);
                std::vector<uint32_t> gpuEdram = edram;
                run(ResolveHlsl(key, ResolveTarget::kHarness), &c, sizeof(c), gpuEdram, memory, c.width / 8, c.height / 8);
                pixels += c.width * c.height;

                uint32_t bad = 0;
                for (uint32_t y = c.y0; y < c.y0 + c.height && bad < 4; y++)
                    for (uint32_t x = c.x0; x < c.x0 + c.width && bad < 4; x++)
                    {
                        const uint32_t a = c.destBase + TiledOffset2D(x, y, c.destPitchMacro, 2);
                        uint32_t got, want;
                        std::memcpy(&got, memory.data() + a, 4);
                        std::memcpy(&want, expected.data() + a, 4);
                        if (got != want)
                        {
                            uint32_t qx, qy;
                            EdramSample(key.msaa, x, y, key.select < 4 ? key.select : key.select == 5 ? 2 : 0, qx, qy);
                            const uint32_t source = edram[EdramAddress(c.sourceBase, c.pitchTiles, qx, qy, key.depth) / 4];
                            Log("Shader test: RESOLVE MISMATCH case {} ({} {}x sel {} -> fmt {} num {} swap {} endian {} bias {} {})"
                                " pixel {},{} first sample {:08X}: gpu {:08X} cpu {:08X}", t,
                                key.depth ? "depth" : EdramColorFormatName(key.sourceFormat), 1u << key.msaa, key.select,
                                key.destFormat, key.destNumber, int(key.swap), key.endian, key.expBias,
                                key.raw ? "raw" : "convert", x, y, source, got, want);
                            bad++;
                        }
                    }
                if (!bad && memory != expected)
                {
                    Log("Shader test: RESOLVE MISMATCH case {}: bytes outside the rectangle differ", t);
                    bad++;
                }
                if (bad)
                    mismatches++;
            }

            for (uint32_t t = 0; t < kClears; t++)
            {
                ClearConstants c = {};
                c.x0 = 8 * gen.Next(81);
                c.y0 = 8 * gen.Next(61);
                c.width = 8 * (1 + gen.Next(40));
                c.height = 8 * (1 + gen.Next(30));
                c.base = gen.Next(2048);
                c.pitchTiles = 1 + gen.Next(24);
                c.msaa = gen.Next(3);
                c.depth = gen.Next(2);
                c.value = uint32_t(gen.rng());
                std::vector<uint32_t> edram;
                randomEdram(edram);
                std::vector<uint32_t> expected = edram;
                ReferenceClear(c, expected.data());
                std::vector<uint8_t> memory(kMemoryBytes, 0);
                run(ClearHlsl(ResolveTarget::kHarness), &c, sizeof(c), edram, memory, c.width / 8, c.height / 8);
                if (edram != expected)
                {
                    uint32_t differ = 0;
                    for (size_t i = 0; i < edram.size(); i++)
                        differ += edram[i] != expected[i];
                    Log("Shader test: CLEAR MISMATCH case {} (msaa {} depth {} base {} pitch {} rect {},{} {}x{}): {} dwords differ",
                        t, c.msaa, c.depth, c.base, c.pitchTiles, c.x0, c.y0, c.width, c.height, differ);
                    mismatches++;
                }
            }
            Log("Shader test: resolves: {} copies ({} pixels) and {} clears compared, {} mismatches", kCopies, pixels,
                kClears, mismatches);
            return int(mismatches);
        }
    }

    int RunShaderTests()
    {
        int failures = TranslateDumpedShaders();

        Generator gen;
        float constants[kConstantCount][4];
        for (auto& c : constants)
            for (float& x : c)
                x = gen.Value();
        ComputeRunner runner(constants);
        const Reference reference{ constants };

        // Every vector op (scalar retain_prev, no scalar write), every scalar op (vector max, no vector write), then
        // random pairs; 32 register states each.
        enum class Kind { kVectorOnly, kScalarOnly, kBoth };
        struct Case { uint32_t vop, sop; Kind kind; };
        std::vector<Case> plan;
        for (uint32_t vop = 0; vop < 30; vop++)
            for (int repeat = 0; repeat < 6; repeat++)
                plan.push_back({ vop, 50, Kind::kVectorOnly });
        for (uint32_t sop = 0; sop <= 50; sop++)
            if (sop != 41)
                for (int repeat = 0; repeat < 6; repeat++)
                    plan.push_back({ 2, sop, Kind::kScalarOnly });
        for (int i = 0; i < 1500; i++)
        {
            uint32_t vop = gen.Next(30), sop = gen.Next(51);
            if (sop == 41 || WritesSideEffect(vop, sop) || (sop >= 42 && sop <= 47 && VectorOperandCount(vop) == 3))
                continue;
            plan.push_back({ vop, sop, Kind::kBoth });
        }

        constexpr uint32_t kStates = 32;
        uint32_t tested = 0, mismatches = 0;
        for (size_t p = 0; p < plan.size(); p++)
        {
            const auto [vop, sop, kind] = plan[p];
            const uint32_t vmask = kind == Kind::kScalarOnly ? 0 : gen.Next(16) | 1;
            const uint32_t smask = kind == Kind::kVectorOnly ? 0 : gen.Next(16) | 1;
            const Alu alu = gen.Make(vop, sop, vmask, smask);

            Shader shader;
            shader.stage = Stage::kVertex;
            shader.words = MicrocodeFor(alu);
            shader.disassembly = Disassemble(shader.stage, shader.words, shader.analysis);
            if (!shader.analysis.errors.empty())
                Fatal("Shader test: generated instruction is invalid: {}", shader.analysis.errors.front());
            TranslationOptions options;
            options.computeHarness = true;
            const TranslatedShader translated = TranslateShader(shader, options);
            std::vector<uint8_t> bytecode;
            std::string errors;
            if (!CompileHlsl(translated.hlsl, "cs_5_1", "alu_test", bytecode, errors))
                Fatal("Shader test: harness shader failed to compile: {}\n{}", errors, translated.hlsl);

            std::vector<State> states(kStates);
            std::vector<uint8_t> input(kStates * kHarnessCaseBytes, 0);
            for (uint32_t k = 0; k < kStates; k++)
            {
                State& s = states[k];
                for (uint32_t r = 0; r < kHarnessRegisters; r++)
                    for (float& x : s.r[r])
                        x = gen.Value();
                s.ps = Flush(gen.Value()); // ps is always a flushed ALU result on hardware
                s.p0 = gen.Next(2);
                s.a0 = int32_t(gen.Next(16)) - 3;
                uint8_t* out = input.data() + k * kHarnessCaseBytes;
                std::memcpy(out, s.r, kHarnessRegisters * 16);
                const uint32_t st[4] = { std::bit_cast<uint32_t>(s.ps), s.p0 ? 1u : 0u, uint32_t(s.a0), 0 };
                std::memcpy(out + kHarnessRegisters * 16, st, 16);
            }
            const std::vector<uint8_t> output = runner.Run(bytecode, input);

            for (uint32_t k = 0; k < kStates; k++)
            {
                State expected = states[k];
                const float scalarInput = reference.Execute(alu, expected);
                const uint8_t* got = output.data() + k * kHarnessCaseBytes;
                float r[kHarnessRegisters][4];
                std::memcpy(r, got, sizeof(r));
                uint32_t st[4];
                std::memcpy(st, got + kHarnessRegisters * 16, 16);
                std::string problems;
                const bool approximate = IsApproximate(sop);
                const uint32_t sd = alu.scalarDest();
                for (uint32_t reg = 0; reg < kHarnessRegisters; reg++)
                    for (uint32_t comp = 0; comp < 4; comp++)
                    {
                        const bool scalarWritten = reg == sd && ((alu.scalarMask() >> comp) & 1); // aL = 0 here
                        // The translator keeps registers flushed (a flush per write, not per read), so registers
                        // compare as their flushed values.
                        const bool ok = approximate && scalarWritten
                            ? Close(Flush(r[reg][comp]), Flush(expected.r[reg][comp]), sop, scalarInput)
                            : SameBits(Flush(r[reg][comp]), Flush(expected.r[reg][comp]));
                        if (!ok)
                            problems += std::format(" r{}.{}: gpu {:08X} ({}) cpu {:08X} ({});", reg, "xyzw"[comp],
                                                    std::bit_cast<uint32_t>(r[reg][comp]), r[reg][comp],
                                                    std::bit_cast<uint32_t>(expected.r[reg][comp]), expected.r[reg][comp]);
                    }
                const float gps = std::bit_cast<float>(st[0]);
                if (!(approximate ? Close(gps, expected.ps, sop, scalarInput) : SameBits(gps, expected.ps)))
                    problems += std::format(" ps: gpu {:08X} ({}) cpu {:08X} ({});", st[0], gps,
                                            std::bit_cast<uint32_t>(expected.ps), expected.ps);
                if ((st[1] != 0) != expected.p0)
                    problems += std::format(" p0: gpu {} cpu {};", st[1], int(expected.p0));
                if (int32_t(st[2]) != expected.a0)
                    problems += std::format(" a0: gpu {} cpu {};", int32_t(st[2]), expected.a0);
                if ((st[3] != 0) != expected.killed)
                    problems += std::format(" kill: gpu {} cpu {};", st[3], int(expected.killed));
                tested++;
                if (!problems.empty())
                {
                    if (mismatches++ < 40)
                    {
                        std::error_code error;
                        std::filesystem::create_directories("analysis/shader_test", error);
                        std::ofstream(std::format("analysis/shader_test/mismatch_{}_v{}_s{}.hlsl", p, vop, sop)) << translated.hlsl;
                        Analysis analysis;
                        const std::string text = Disassemble(Stage::kVertex, shader.words, analysis);
                        std::string inputs;
                        for (uint32_t reg = 0; reg < kHarnessRegisters; reg++)
                            inputs += std::format(" r{}=({} {} {} {})", reg, states[k].r[reg][0], states[k].r[reg][1],
                                                  states[k].r[reg][2], states[k].r[reg][3]);
                        Log("Shader test: MISMATCH #{} vop {} sop {} state {}:{}\n  inputs:{} ps={} p0={} a0={}\n{}", p, vop, sop, k,
                            problems, inputs, states[k].ps, int(states[k].p0), states[k].a0,
                            text.substr(text.find("slot 1:")));
                    }
                }
            }
        }
        Log("Shader test: {} instruction states compared over {} instructions, {} mismatches", tested, plan.size(), mismatches);

        float ones[kConstantCount][4];
        for (auto& c : ones)
            for (float& x : c)
                x = 1.0f;
        ComputeRunner flowRunner(ones);
        failures += RunControlFlowTests(flowRunner);
        failures += RunOutputMergerTests(runner, gen);
        failures += RunResolveTests(runner, gen);
        return failures + int(mismatches);
    }
}
