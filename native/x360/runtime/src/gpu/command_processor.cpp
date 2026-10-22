// Headless Xenos command processor and graphics interrupts (see gpu.h).
// Register indices, packet formats and opcode semantics were cross-checked against Xenia Canary (BSD-3); the code is
// our own.

#include "modules.h"
#include "gpu/gpu.h"
#include "guest_clock.h"
#include "gpu/draw_trace.h"
#include "gpu/present.h"
#include "gpu/presenter.h"
#include "gpu/renderer.h"
#include "gpu/shader_cache.h"
#include "gpu/shader_translator.h"

#include "input.h"
#include "kernel/kernel.h"
#include "settings.h"
#include "transition/transition.h"

#include <condition_variable>
#include <mutex>
#include <windows.h>

#include <array>
#include <atomic>
#include <chrono>
#include <cstring>
#include <deque>
#include <format>
#include <map>
#include <string>
#include <mutex>
#include <set>
#include <thread>
#include <vector>

namespace gpu
{
    namespace
    {
        // The title kicks the command processor by writing CP_RB_WPTR; the recompiled MMIO store reports it
        // (PpcMmioStore32) so the idle loop sleeps on this event instead of spinning. Diagnostics: kicks counted.
        HANDLE g_kick = CreateEventW(nullptr, FALSE, FALSE, nullptr);
        std::atomic<uint64_t> g_kicks = 0;

        using Clock = std::chrono::steady_clock;

        std::vector<uint32_t> g_hostRegisters(reg::kCount, 0); // indices >= 0x4000
        uint64_t g_constantVersions[size_t(ConstantBlock::kCount)] = {};

        std::atomic<uint32_t> g_ringBase = 0;        // physical
        std::atomic<uint32_t> g_ringSizeDwords = 0;
        std::atomic<uint32_t> g_readPointerWriteBack = 0;
        uint32_t g_readIndex = 0;                    // dwords, owned by the CP thread

        std::atomic<uint32_t> g_interruptCallback = 0;
        std::atomic<uint32_t> g_interruptUserData = 0;
        // Process type of the thread that registered the callback. D3D's interrupt routine picks its device with
        // KeGetCurrentProcessType (VdGlobalXamDevice for the system process: bootanim, XAM; VdGlobalDevice for a
        // title) without setting the type itself (bootanim 0x9802FD90 -> 0x98041338), so the kernel must run it in
        // its registrant's process.
        std::atomic<uint8_t> g_interruptProcessType = kernel::kProcessTypeTitle;
        std::mutex g_interruptMutex;                 // one graphics interrupt at a time, like the hardware
        // Set after each graphics interrupt the title's callback handled: a WAIT_REG_MEM polls again at once, since
        // the callback (e.g. D3D's vblank handling) is what usually writes the value it waits for.
        HANDLE g_interruptHandled = CreateEventW(nullptr, FALSE, FALSE, nullptr);

        std::atomic<uint64_t> g_swapCount = 0;
        uint64_t g_counter = 0;                      // CP counter: vblanks + swaps (EVENT_WRITE_SHD bit 31)
        std::atomic<uint64_t> g_vblanks = 0;         // vblanks so far (the system UI's clock)
        // Reset value unknown (Xenia: 0x00000000FFFFFFFF). D3D rewrites all four halves every frame (0x60-0x63)
        // and both choices predicate identically before that.
        uint64_t g_binMask = ~0ull;
        uint64_t g_binSelect = ~0ull;
        uint64_t g_zpdSamples = 0;                   // cumulative fake occlusion samples
        uint64_t g_draws = 0;
        uint64_t g_surveyDraws = 0;                  // draws skipped as visibility-survey geometry
        const Shader* g_vertexShader = nullptr;      // set by IM_LOAD / IM_LOAD_IMMEDIATE
        const Shader* g_pixelShader = nullptr;
        // CB_GPU_STATS=1: per-opcode packet counts, logged with the frame counter.
        const bool g_stats = [] { const char* v = std::getenv("CB_GPU_STATS"); return v && v[0] == '1'; }();
        uint64_t g_opcodeCounts[128] = {};
        // CB_GPU_STATS: WAIT_REG_MEM by (info, address, reference, mask, wait): count, polls that failed, time.
        struct WaitStat
        {
            uint64_t count = 0, misses = 0, ns = 0;
        };
        std::map<std::array<uint32_t, 5>, WaitStat> g_waitStats;

        bool g_cpStarted = false;
        bool g_vblankStarted = false;

        // Display gamma LUT (08_present §5.2). Defaults before the title writes anything are Xenia's: the identity
        // table i * 1023 / 255 and a linear PWL.
        DisplayGamma g_gamma = [] {
            DisplayGamma g{};
            for (uint32_t i = 0; i < 256; i++)
            {
                const uint32_t v = i * 0x3FF / 0xFF;
                g.table[i] = v << 20 | v << 10 | v;
            }
            for (uint32_t i = 0; i < 128; i++)
            {
                const uint32_t base = (i * 0xFFFF / 0x7F) & ~0x3Fu, delta = i < 0x7F ? 0x200u : 0u;
                g.pwl[i][0] = g.pwl[i][1] = g.pwl[i][2] = delta << 16 | base;
            }
            return g;
        }();
        uint32_t g_gammaComponent = 0; // R, G, B counter of DC_LUT_SEQ_COLOR / DC_LUT_PWL_DATA accesses

        // ---- Guest memory as seen by the GPU (physical addresses, endian swap modes) -------------------------

        uint8_t* Physical(uint32_t address)
        {
            return guest::g_memory.Translate(guest::kPhysicalWindow64k + (address & 0x1FFFFFFF));
        }

        uint32_t SwapMode(uint32_t value, uint32_t mode)
        {
            switch (mode & 3)
            {
            case 1: return ((value & 0x00FF00FF) << 8) | ((value >> 8) & 0x00FF00FF); // 8-in-16
            case 2: return __builtin_bswap32(value);                                  // 8-in-32
            case 3: return (value << 16) | (value >> 16);                             // 16-in-32
            default: return value;
            }
        }

        // The GPU accesses memory little-endian and applies the address's endian mode (low 2 bits); mode 2 therefore
        // reads/writes ordinary big-endian guest data.
        uint32_t GpuRead(uint32_t address)
        {
            uint32_t raw;
            memcpy(&raw, Physical(address & ~3u), 4);
            return SwapMode(raw, address & 3);
        }

        // Every write the CPU may be polling for (fences, write-backs, GPU memory writes) bumps this, so a guest
        // thread spinning on one (D3D's CBlocker) can sleep until it changes (WaitForProgress).
        std::atomic<uint64_t> g_progress = 0;
        std::mutex g_progressMutex;
        std::condition_variable g_progressChanged;

        void Progressed()
        {
            {
                std::lock_guard lock(g_progressMutex);
                g_progress.fetch_add(1);
            }
            g_progressChanged.notify_all();
        }

        void GpuWrite(uint32_t address, uint32_t value)
        {
            const uint32_t raw = SwapMode(value, address & 3);
            memcpy(Physical(address & ~3u), &raw, 4);
            Progressed();
        }

        // ---- Writes the CPU may only see once the GPU is done with the work before them ---------------------
        //
        // D3D's fences (EVENT_WRITE_SHD), scratch-register write-backs and the ring read pointer tell the title that
        // the GPU has finished with everything before them. Waiting for the host GPU to go idle at each one made the
        // command processor and the GPU take turns (the boot animation's ribbons ran at 30 frames/s: ~13 ms of GPU
        // work plus the recording, every frame). Instead the work so far is submitted and the write waits in this
        // queue until the GPU's fence passes it, while the command processor records on. The title sees each value
        // no earlier than a full wait would have shown it, and in order. Every RenderSync (CpSync) empties the queue.
        struct DeferredWrite
        {
            uint64_t fence;
            uint8_t* host;  // guest memory (already translated)
            uint32_t raw;   // the 4 bytes as stored
        };
        std::deque<DeferredWrite> g_deferred;

        void RetireDeferredWrites(uint32_t waitMs = 0)
        {
            while (!g_deferred.empty() && RenderReached(g_deferred.front().fence, waitMs))
            {
                memcpy(g_deferred.front().host, &g_deferred.front().raw, 4);
                g_deferred.pop_front();
                waitMs = 0;
                Progressed();
            }
        }

        void DeferWrite(uint8_t* host, uint32_t raw)
        {
            const uint64_t fence = RenderSubmit();
            if (g_deferred.empty() && RenderReached(fence, 0))
            {
                memcpy(host, &raw, 4);
                Progressed();
            }
            else
                g_deferred.push_back({ fence, host, raw });
        }

        // A full wait for the GPU: deferred writes are all due afterwards.
        void CpSync(SyncReason reason)
        {
            RenderSync(reason);
            RetireDeferredWrites();
        }

        uint32_t ReadBE(uint32_t physical)
        {
            uint32_t raw;
            memcpy(&raw, Physical(physical), 4);
            return __builtin_bswap32(raw);
        }

        // ---- Register file ----------------------------------------------------------------------------------

        uint32_t* RegisterSlot(uint32_t index)
        {
            return reinterpret_cast<uint32_t*>(guest::g_memory.Translate(kMmioBase + index * 4));
        }

        void StoreRegister(uint32_t index, uint32_t value)
        {
            if (index < 0x4000)
                __atomic_store_n(RegisterSlot(index), __builtin_bswap32(value), __ATOMIC_SEQ_CST);
            else if (index < reg::kCount)
            {
                uint32_t& slot = g_hostRegisters[index];
                if (slot != value)
                {
                    slot = value;
                    if (index < 0x4800)
                        g_constantVersions[size_t(ConstantBlock::kFloat)]++;
                    else if (index < 0x4900)
                        g_constantVersions[size_t(ConstantBlock::kFetch)]++;
                    else if (index < 0x4928)
                        g_constantVersions[size_t(ConstantBlock::kBoolLoop)]++;
                }
            }
            else
                Fatal("GPU register write to index {:X} beyond the register file", index);
        }

        void GammaRegisterWritten(uint32_t index, uint32_t value);

        // Register writes made by the command processor, with their side effects.
        void WriteRegister(uint32_t index, uint32_t value)
        {
            if (index >= reg::kScratchReg0 && index < reg::kScratchReg0 + 8)
            {
                StoreRegister(index, value);
                const uint32_t n = index - reg::kScratchReg0;
                if ((ReadRegister(reg::kScratchUmsk) >> n) & 1)
                {
                    // The CPU can see this write: it waits for the GPU work before it (deferred writes above).
                    DeferWrite(Physical(ReadRegister(reg::kScratchAddr) + n * 4), __builtin_bswap32(value));
                }
                return;
            }
            if (index == reg::kCoherStatusHost)
            {
                StoreRegister(index, value | 0x80000000); // pending until a WAIT_REG_MEM makes memory coherent
                return;
            }
            StoreRegister(index, value);
            if (index >= reg::kDcLutRwMode && index <= reg::kDcLut30Color)
                GammaRegisterWritten(index, value);
        }

        // DC_LUT_* side effects, with Xenia's semantics (08_present §5.2; Nexia command_processor.cc
        // HandleSpecialRegisterWrite): writing RW_INDEX restarts the R, G, B component sequence; SEQ_COLOR and
        // PWL_DATA write one component per access (R, G, B; the write-enable mask is bit 0 blue, 1 green, 2 red) and
        // advance the index after blue; 30_COLOR writes all enabled components of one entry and advances the index.
        // Index increments are 8-bit (PWL: 7-bit, bit 7 kept) and behave as an RW_INDEX write. Writing a component
        // register in the other LUT mode is undocumented and stops.
        void GammaRegisterWritten(uint32_t index, uint32_t value)
        {
            const uint32_t rwIndex = ReadRegister(reg::kDcLutRwIndex);
            const uint32_t enable = ReadRegister(reg::kDcLutWriteEnMask) & 7;
            const bool pwlMode = ReadRegister(reg::kDcLutRwMode) & 1;
            auto setIndex = [](uint32_t v) {
                StoreRegister(reg::kDcLutRwIndex, v);
                g_gammaComponent = 0;
            };
            switch (index)
            {
            case reg::kDcLutRwIndex:
                g_gammaComponent = 0;
                break;
            case reg::kDcLutSeqColor:
            {
                if (pwlMode)
                    Fatal("GPU: DC_LUT_SEQ_COLOR written in PWL mode (value {:08X})", value);
                const uint32_t component = g_gammaComponent; // 0 red, 1 green, 2 blue
                if ((enable >> (2 - component)) & 1)
                {
                    const uint32_t shift = (2 - component) * 10;
                    uint32_t& entry = g_gamma.table[rwIndex & 0xFF];
                    entry = (entry & ~(0x3FFu << shift)) | (((value & 0xFFFF) >> 6) << shift);
                    g_gamma.tableWrites++;
                }
                if (++g_gammaComponent == 3)
                    setIndex((rwIndex & ~0xFFu) | ((rwIndex + 1) & 0xFF));
                break;
            }
            case reg::kDcLutPwlData:
            {
                if (!pwlMode)
                    Fatal("GPU: DC_LUT_PWL_DATA written in 256-entry table mode (value {:08X})", value);
                const uint32_t component = g_gammaComponent, entry = rwIndex & 0x7F;
                if ((enable >> (2 - component)) & 1)
                {
                    g_gamma.pwl[entry][component] = value & 0xFFC0FFC0u; // bits 0-5 of base and delta read as 0
                    g_gamma.pwlWrites++;
                }
                if (++g_gammaComponent == 3)
                    setIndex((rwIndex & ~0x7Fu) | ((entry + 1) & 0x7F));
                break;
            }
            case reg::kDcLut30Color:
            {
                if (pwlMode)
                    Fatal("GPU: DC_LUT_30_COLOR written in PWL mode (value {:08X})", value);
                uint32_t& entry = g_gamma.table[rwIndex & 0xFF];
                for (uint32_t c = 0; c < 3; c++) // bit c of the mask enables bits 10c..10c+9 (blue, green, red)
                    if ((enable >> c) & 1)
                    {
                        entry = (entry & ~(0x3FFu << (c * 10))) | (value & (0x3FFu << (c * 10)));
                        g_gamma.tableWrites++;
                    }
                setIndex((rwIndex & ~0xFFu) | ((rwIndex + 1) & 0xFF));
                break;
            }
            default:
                break;
            }
        }

        // ---- Interrupts -------------------------------------------------------------------------------------

        // Runs the title's graphics interrupt callback on this host thread as interrupt code on `cpu`.
        void DispatchInterrupt(kernel::GuestThread* thread, PPCContext& ctx, uint32_t source, uint32_t cpu)
        {
            const uint32_t callback = g_interruptCallback.load();
            // A title that has ended (ExTerminateTitleProcess unloaded it) has no callback to run.
            if (callback == 0 || !modules::FindByCode(callback))
                return;
            {
                std::lock_guard lock(g_interruptMutex);
                kernel::CallGuestAtDispatchLevel(thread, ctx, callback, { source, g_interruptUserData.load() }, cpu,
                    g_interruptProcessType.load());
            }
            SetEvent(g_interruptHandled);
        }

        kernel::GuestThread* CreateSystemThread()
        {
            kernel::ThreadParams params;
            params.stackSize = 0x10000;
            params.processType = kernel::kProcessTypeSystem; // serves whatever title runs; not ended with it
            return kernel::CreateThread(params);
        }

        // Fixed-value registers (hardware status the title polls).
        void WriteFixedRegisters()
        {
            const ConsoleSettings& s = Settings();
            StoreRegister(reg::kRbEdramTiming, 0x08100748);
            StoreRegister(reg::kRbBcControl, 0x0000200E);
            StoreRegister(reg::kRbHsioInterfaceAlignerValue, 0x00BBBBBB);
            StoreRegister(reg::kD1ModeVblankVlineStatus, 1);
            StoreRegister(reg::kD1ModeViewportSize, (s.videoWidth << 16) | s.videoHeight);
        }

        void VblankThread()
        {
            kernel::GuestThread* thread = CreateSystemThread();
            PPCContext* ctx = kernel::AttachHostThread(thread);
            const auto period = std::chrono::nanoseconds(int64_t(1e9 / Settings().refreshRate));
            auto next = Clock::now() + period;
            for (;;)
            {
                std::this_thread::sleep_until(next);
                next += period;
                guest_clock::Vblank(uint64_t(period.count()));
                if (guest_clock::Fixed())
                    next = std::max(next, Clock::now()); // frame-locked: no catching up after a wait
                WriteFixedRegisters();
                g_counter++;
                g_vblanks++;
                DispatchInterrupt(thread, *ctx, 0, 2); // source 0 = vblank, delivered on hardware thread 2
            }
        }

        // ---- Packet execution -------------------------------------------------------------------------------

        struct Stream
        {
            uint32_t base;        // physical address of dword 0
            uint32_t wrapDwords;  // ring size for wraparound, 0 for a linear buffer
            uint32_t start;       // first dword index
            uint32_t count;       // dwords available
            uint32_t pos = 0;

            uint32_t Remaining() const { return count - pos; }
            // Physical address of the next dword.
            uint32_t Address() const { return base + (wrapDwords ? (start + pos) % wrapDwords : start + pos) * 4; }
            uint32_t Read()
            {
                uint32_t index = start + pos++;
                if (wrapDwords)
                    index %= wrapDwords;
                return ReadBE(base + index * 4);
            }
        };

        // Diagnostic: where a stream is and the dwords around its current position.
        std::string DescribeStream(const Stream& s)
        {
            std::string text = std::format("stream base {:08X} wrap {} start {} count {} pos {}; dwords:",
                s.base, s.wrapDwords, s.start, s.count, s.pos);
            const int64_t first = std::max<int64_t>(0, int64_t(s.pos) - 8);
            for (int64_t i = first; i < std::min<int64_t>(s.count, s.pos + 8); i++)
            {
                uint32_t index = s.start + uint32_t(i);
                if (s.wrapDwords)
                    index %= s.wrapDwords;
                text += std::format("{}{:08X}", i == int64_t(s.pos) - 1 ? " [" : " ", ReadBE(s.base + index * 4));
                if (i == int64_t(s.pos) - 1)
                    text += "]";
            }
            return text;
        }

        // A packet whose payload runs past the end of its buffer: the rest of that buffer is skipped (the behaviour
        // Xenia Canary settled on; what the hardware CP does here is not documented). Logged for later study.
        void ReportTruncated(const std::string& packet, const Stream& s)
        {
            static std::atomic<int> reported = 0;
            if (reported++ < 20)
                Log("GPU: {} runs past the end of its buffer, skipping the rest; {}", packet, DescribeStream(s));
        }

        bool Compare(uint32_t function, uint32_t value, uint32_t reference)
        {
            switch (function & 7)
            {
            case 0: return false;
            case 1: return value < reference;
            case 2: return value <= reference;
            case 3: return value == reference;
            case 4: return value != reference;
            case 5: return value >= reference;
            case 6: return value > reference;
            default: return true;
            }
        }

        // A coherency request (COHER_STATUS_HOST written with its action bits, range in COHER_BASE/SIZE_HOST) is
        // carried out. On hardware the action completes by itself and the status bit then reads 0 to any poller;
        // memory is always coherent here, so it completes when first polled (Xenia does this only for WAIT_REG_MEM,
        // which leaves a COND_WRITE poll seeing it pending forever). Texture/vertex caches must invalidate
        // [base, base + size) here once they exist.
        void MakeCoherent()
        {
            StoreRegister(reg::kCoherStatusHost, 0);
        }

        // WAIT_REG_MEM / COND_WRITE poll source: memory (with endian mode) or a register.
        uint32_t PollValue(uint32_t waitInfo, uint32_t address)
        {
            if (waitInfo & 0x10)
                return GpuRead(address);
            if (address == reg::kCoherStatusHost && (ReadRegister(address) & 0x80000000))
                MakeCoherent();
            return ReadRegister(address);
        }

        uint32_t ConstantBase(uint32_t type)
        {
            switch (type)
            {
            case 0: return 0x4000; // ALU
            case 1: return 0x4800; // fetch
            case 2: return 0x4900; // bool
            case 3: return 0x4908; // loop
            case 4: return 0x2000; // registers
            default: Fatal("SET_CONSTANT type {} is not implemented", type);
            }
        }

        void WriteOcclusionReport()
        {
            const uint32_t address = ReadRegister(reg::kRbSampleCountAddr) & 0x1FFFFFFF;
            if (address == 0)
                return;
            // No rendering yet, so every query reports samples passing (nothing is culled). D3D sums A+B and
            // subtracts the begin report from the end report.
            g_zpdSamples += 100;
            const uint32_t a = uint32_t(g_zpdSamples - g_zpdSamples / 2), b = uint32_t(g_zpdSamples / 2);
            const uint32_t report[8] = { a, b, 0, 0, a, b, 0, 0 }; // Total, ZFail, ZPass, StencilFail (A, B), little-endian
            memcpy(Physical(address), report, 16);
            memcpy(Physical(address + 16), report + 4, 16);
        }

        kernel::GuestThread* g_cpThread = nullptr;
        PPCContext* g_cpContext = nullptr;

        // ---- Shaders and draws ------------------------------------------------------------------------------

        // IM_LOAD / IM_LOAD_IMMEDIATE dword 1: [13:0] size in dwords, [27:16] start (instruction-memory offset,
        // meaningful only for GPULOADTYPE_SHARED) [d3d9gpu.h GPUCOMMAND_LOAD_SHADER] (02 §5.10). Size 0 occurs
        // (D3D loads an empty program, presumably for "no pixel shader") and becomes an empty shader.
        uint32_t MicrocodeSize(uint32_t startSize, const char* packet, uint32_t addressType = 0)
        {
            const uint32_t start = (startSize >> 16) & 0xFFF, size = startSize & 0x3FFF;
            if (startSize & 0xF000C000)
                Fatal("GPU: {} {:08X} size dword {:08X} sets reserved bits", packet, addressType, startSize);
            if (start != 0)
                Fatal("GPU: {} {:08X} with instruction-memory start {} is not implemented ({:08X})", packet, addressType,
                      start, startSize);
            return size;
        }

        // The microcode is copied when the packet executes: the guest may reuse that memory afterwards.
        void SetShader(uint32_t type, std::vector<uint32_t> words, const char* packet)
        {
            switch (type)
            {
            case 0: g_vertexShader = LoadShader(ucode::Stage::kVertex, std::move(words)); break;
            case 1: g_pixelShader = LoadShader(ucode::Stage::kPixel, std::move(words)); break;
            default: Fatal("GPU: {} of shader type {} (GPULOADTYPE_SHARED) is not implemented", packet, type);
            }
        }

        // Translates and compiles the draw's shaders for the current SQ state (01a §3) on first use, so every shader
        // the title draws with is known to translate before the renderer exists.
        void PrepareShaders()
        {
            const uint32_t mode = ReadRegister(0x2208) & 7; // RB_MODECONTROL
            if (mode != 4 && mode != 5)
                return; // copies (resolves) and disabled rendering run no shaders
            const uint32_t program = ReadRegister(0x2180), misc = ReadRegister(0x2181);
            if (g_vertexShader && !g_vertexShader->words.empty())
            {
                TranslationOptions options;
                options.registerCount = (program & 0x3F) + 1;
                RequestCompiledShader(*g_vertexShader, options);
            }
            // Depth-only mode does not run the pixel shader (Xenia's assumption, 01b §1); an empty program is none.
            if (mode == 4 && g_pixelShader && !g_pixelShader->words.empty())
            {
                TranslationOptions options;
                options.registerCount = ((program >> 8) & 0x3F) + 1;
                options.paramGen = (program >> 18) & 1;
                options.paramGenRegister = (misc >> 8) & 0xFF;
                const uint32_t interpolators = ReadRegister(0x2182);
                options.flatInterpolators = 0; // ParamShade is not flat shading (renderer.cpp)
                // Sampling: 1x MSAA or centers-only -> centers; centroids-only -> centroid; else per-interpolator
                // SamplingPattern (0 = centroid) (01a §3).
                const uint32_t msaa = (ReadRegister(0x2000) >> 16) & 3, sampleControl = (misc >> 2) & 3;
                if (msaa != 0 && sampleControl == 0)
                    options.centroidInterpolators = 0xFFFF;
                else if (msaa != 0 && sampleControl == 2)
                    options.centroidInterpolators = ~(interpolators >> 16) & 0xFFFF;
                RequestCompiledShader(*g_pixelShader, options);
            }
        }

        // A draw packet whose registers have been written: traced (CB_GPU_TRACE) and rendered (renderer.h).
        void IssueDraw(DrawPacket draw)
        {
            // Visibility-survey geometry (D3D's conditional-rendering setup) is tested but never drawn (02 §5.7).
            const uint32_t vizQuery = ReadRegister(reg::kPaScVizQuery);
            draw.surveyOnly = (vizQuery & 1) && ((vizQuery >> 7) & 1);
            g_draws++;
            if (draw.surveyOnly)
                g_surveyDraws++;
            if (TraceActive(g_swapCount.load()))
                TraceDraw(draw, g_vertexShader, g_pixelShader);
            if (draw.surveyOnly)
                return;
            if (RendererEnabled())
                RenderDraw(draw, g_vertexShader, g_pixelShader);
            else
                PrepareShaders();
        }

        void ExecutePackets(Stream& s);

        // What the command processor is executing (for stall reports): type-3 opcode and the packet's address.
        std::atomic<uint32_t> g_currentOpcode = 0xFFFFFFFF;
        std::atomic<uint32_t> g_currentPacket = 0;

        // CB_PM4_LOG=<n>: logs the first n type-3 packets (opcode, address, up to 6 dwords) for bring-up.
        const uint64_t g_pm4Log = [] { const char* v = std::getenv("CB_PM4_LOG"); return v ? std::strtoull(v, nullptr, 0) : 0ull; }();
        std::atomic<uint64_t> g_pm4Logged = 0;

        void ExecuteType3(uint32_t opcode, uint32_t count, Stream& s)
        {
            g_currentOpcode = opcode;
            g_currentPacket = s.Address();
            if (g_pm4Log && g_pm4Logged++ < g_pm4Log)
            {
                std::string words;
                for (uint32_t i = 0; i < std::min(count, 6u); i++)
                    words += std::format(" {:08X}", ReadBE(s.Address() + i * 4));
                Log("PM4 {:02X} x{} at {:08X}:{}", opcode, count, s.Address() - 4, words);
            }
            if (g_stats)
                g_opcodeCounts[opcode & 0x7F]++;
            const uint32_t end = s.pos + count;
            auto& mem = guest::g_memory;
            switch (opcode)
            {
            case 0x48: // ME_INIT
            case 0x10: // NOP
            case 0x3B: // INVALIDATE_STATE
            case 0x5E: // CONTEXT_UPDATE
            case 0x26: // WAIT_FOR_IDLE
            case 0x5D: // WAIT_IB_PFD_COMPLETE
                break;
            case 0x27: // IM_LOAD: microcode from memory
            {
                const uint32_t addressType = s.Read(), startSize = s.Read();
                const uint32_t size = MicrocodeSize(startSize, "IM_LOAD", addressType);
                if (addressType & 0x1C)
                    Fatal("GPU: IM_LOAD address {:08X} is not 32-byte aligned (bits 4:2 undefined); {}", addressType,
                          DescribeStream(s));
                const uint32_t address = addressType & ~3u;
                std::vector<uint32_t> words(size);
                for (uint32_t i = 0; i < size; i++)
                    words[i] = ReadBE(address + i * 4);
                SetShader(addressType & 3, std::move(words), "IM_LOAD");
                break;
            }
            case 0x2B: // IM_LOAD_IMMEDIATE: microcode inline in the packet
            {
                const uint32_t type = s.Read(), startSize = s.Read();
                const uint32_t size = MicrocodeSize(startSize, "IM_LOAD_IMMEDIATE");
                if (size > count - 2)
                    Fatal("GPU: IM_LOAD_IMMEDIATE of {} dwords in a {}-dword packet", size, count);
                if (type > 3)
                    Fatal("GPU: IM_LOAD_IMMEDIATE type dword {:08X} is undefined", type);
                std::vector<uint32_t> words(size);
                for (uint32_t& word : words)
                    word = s.Read();
                SetShader(type, std::move(words), "IM_LOAD_IMMEDIATE");
                break;
            }
            case 0x23: // VIZ_QUERY: [5:0] id, [8] end
            {
                const uint32_t info = s.Read(), id = info & 0x3F;
                const bool end = (info & 0x100) != 0;
                StoreRegister(reg::kVgtEventInitiator, end ? 8 : 7); // VIZQUERY_END / VIZQUERY_START
                // Nothing is rasterized yet, so every query reports visible (as the occlusion reports do). Real
                // results need the renderer.
                if (end)
                {
                    const uint32_t status = id < 32 ? reg::kPaScVizQueryStatus0 : reg::kPaScVizQueryStatus1;
                    StoreRegister(status, ReadRegister(status) | (1u << (id & 31)));
                }
                if (TraceActive(g_swapCount.load()))
                    TraceNote(std::format("viz query {} {}", id, end ? "end" : "begin").c_str());
                break;
            }
            case 0x22: // DRAW_INDX: viz token, VGT_DRAW_INITIATOR, then VGT_DMA_BASE/SIZE for an index buffer
            {
                if (count < 2)
                    Fatal("GPU: DRAW_INDX with {} dwords", count);
                DrawPacket draw{ opcode, s.Read(), s.Read(), 0, 0, false };
                const uint32_t source = (draw.initiator >> 6) & 3;
                if (source == 0)
                {
                    if (count < 4)
                        Fatal("GPU: DRAW_INDX from an index buffer with {} dwords", count);
                    draw.dmaBase = s.Read();
                    draw.dmaSize = s.Read();
                    WriteRegister(reg::kVgtDmaBase, draw.dmaBase);
                    WriteRegister(reg::kVgtDmaSize, draw.dmaSize);
                }
                else if (source != 2)
                    Fatal("GPU: DRAW_INDX with index source {} (initiator {:08X}) is not implemented", source, draw.initiator);
                WriteRegister(reg::kVgtDrawInitiator, draw.initiator);
                IssueDraw(draw);
                break;
            }
            case 0x36: // DRAW_INDX_2: VGT_DRAW_INITIATOR, then inline indices for the immediate source
            {
                DrawPacket draw{ opcode, 0, s.Read(), 0, 0, false };
                const uint32_t source = (draw.initiator >> 6) & 3;
                // TU46's D3D only emits auto-index DRAW_INDX_2 (02 §5.11); the packing of inline indices is unverified.
                if (source != 2)
                    Fatal("GPU: DRAW_INDX_2 with index source {} (initiator {:08X}) is not implemented", source, draw.initiator);
                WriteRegister(reg::kVgtDrawInitiator, draw.initiator);
                IssueDraw(draw);
                break;
            }
            case 0x3F: // INDIRECT_BUFFER
            case 0x37: // INDIRECT_BUFFER_PFD
            {
                const uint32_t address = s.Read() & 0x1FFFFFFF;
                const uint32_t length = s.Read() & 0xFFFFF;
                if (length)
                {
                    Stream ib{ address, 0, 0, length };
                    ExecutePackets(ib);
                }
                break;
            }
            case 0x3C: // WAIT_REG_MEM
            {
                const uint32_t info = s.Read(), address = s.Read(), reference = s.Read(), mask = s.Read(), wait = s.Read();
                auto started = Clock::now();
                bool warned = false;
                WaitStat* stat = g_stats ? &g_waitStats[{ info, address, reference, mask, wait }] : nullptr;
                if (stat)
                    stat->count++;
                while (!Compare(info, PollValue(info, address) & mask, reference))
                {
                    if (stat)
                        stat->misses++;
                    // The hardware's poll interval for `wait` is undocumented (Xenia sleeps wait / 0x100 ms, kept here as
                    // the longest gap); the CP also polls again as soon as a graphics interrupt has been handled, so a
                    // value the title's vblank callback writes is seen without a host timer's delay.
                    if (!g_deferred.empty())
                        RetireDeferredWrites(1); // the value may be a deferred write of the command processor's own
                    else if (wait >= 0x100)
                        WaitForSingleObject(g_interruptHandled, wait / 0x100);
                    else
                        SwitchToThread();
                    if (!warned && Clock::now() - started > std::chrono::seconds(5))
                    {
                        warned = true;
                        Log("GPU: WAIT_REG_MEM on {} {:08X} (mask {:08X}, ref {:08X}, fn {}) still waiting after 5 s",
                            (info & 0x10) ? "memory" : "register", address, mask, reference, info & 7);
                    }
                }
                if (stat)
                    stat->ns += uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - started).count());
                break;
            }
            case 0x21: // REG_RMW
            {
                const uint32_t info = s.Read(), andMask = s.Read(), orMask = s.Read();
                const uint32_t target = info & 0x1FFF;
                const uint32_t andValue = (info & 0x80000000) ? ReadRegister(andMask & 0x1FFF) : andMask;
                const uint32_t orValue = (info & 0x40000000) ? ReadRegister(orMask & 0x1FFF) : orMask;
                WriteRegister(target, (ReadRegister(target) & andValue) | orValue);
                break;
            }
            case 0x3E: // REG_TO_MEM
            {
                const uint32_t index = s.Read(), address = s.Read();
                CpSync(SyncReason::kRegToMem);
                GpuWrite(address, ReadRegister(index));
                break;
            }
            case 0x3D: // MEM_WRITE
            {
                const uint32_t address = s.Read();
                CpSync(SyncReason::kMemWrite);
                for (uint32_t i = 0; s.pos < end; i++)
                    GpuWrite(((address & ~3u) + i * 4) | (address & 3), s.Read());
                break;
            }
            case 0x45: // COND_WRITE
            {
                const uint32_t info = s.Read(), poll = s.Read(), reference = s.Read(), mask = s.Read();
                const uint32_t target = s.Read(), data = s.Read();
                if (Compare(info, PollValue(info, poll) & mask, reference))
                {
                    if (info & 0x100)
                    {
                        CpSync(SyncReason::kCondWrite);
                        GpuWrite(target, data);
                    }
                    else
                        WriteRegister(target, data);
                }
                break;
            }
            case 0x46: // EVENT_WRITE
                StoreRegister(reg::kVgtEventInitiator, s.Read() & 0x3F);
                break;
            case 0x58: // EVENT_WRITE_SHD: D3D's fences
            {
                const uint32_t initiator = s.Read(), address = s.Read(), value = s.Read();
                StoreRegister(reg::kVgtEventInitiator, initiator & 0x3F);
                // A fence: everything before it must be complete when the CPU sees the value (deferred write).
                const uint32_t data = (initiator & 0x80000000) ? uint32_t(g_counter) : value;
                const uint32_t target = address & ~3u;
                const uint32_t writebackStart = ReadRegister(reg::kWritebackStart);
                const uint32_t writebackSize = ReadRegister(reg::kWritebackSize);
                if (target > 0x1FFFFFFF && writebackStart != 0 && target >= writebackStart && target - writebackStart < writebackSize)
                {
                    DeferWrite(mem.Translate(kWritebackVirtualBase + (target - writebackStart)), SwapMode(data, address & 3));
                }
                else
                {
                    DeferWrite(Physical(target), SwapMode(data, address & 3));
                }
                break;
            }
            case 0x5A: // EVENT_WRITE_EXT: screen extents report
            {
                const uint32_t initiator = s.Read(), address = s.Read();
                StoreRegister(reg::kVgtEventInitiator, initiator & 0x3F);
                // No GPU wait: the report is a constant (whole screen), so nothing the CPU reads depends on the draws
                // before it. Waiting here drained the GPU 14-25 times a frame (bootanim's ribbons ran at 30 frames/s).
                const uint16_t extents[6] = { 0, 1024, 0, 1024, 0, 1 };
                for (int i = 0; i < 6; i++)
                {
                    const uint16_t be = __builtin_bswap16(extents[i]);
                    memcpy(Physical((address & ~3u) + i * 2), &be, 2);
                }
                break;
            }
            case 0x5B: // EVENT_WRITE_ZPD: occlusion query report
                StoreRegister(reg::kVgtEventInitiator, s.Read() & 0x3F);
                CpSync(SyncReason::kOcclusion);
                WriteOcclusionReport();
                break;
            case 0x2D: // SET_CONSTANT
            {
                const uint32_t header = s.Read();
                const uint32_t base = ConstantBase((header >> 16) & 0xFF) + (header & 0x7FF);
                for (uint32_t i = 0; s.pos < end; i++)
                    WriteRegister(base + i, s.Read());
                break;
            }
            case 0x55: // SET_CONSTANT2
            case 0x56: // SET_SHADER_CONSTANTS
            {
                const uint32_t base = s.Read() & 0xFFFF;
                for (uint32_t i = 0; s.pos < end; i++)
                    WriteRegister(base + i, s.Read());
                break;
            }
            case 0x2F: // LOAD_ALU_CONSTANT
            {
                const uint32_t address = s.Read() & 0x3FFFFFFF, header = s.Read(), size = s.Read() & 0xFFF;
                const uint32_t base = ConstantBase((header >> 16) & 0xFF) + (header & 0x7FF);
                for (uint32_t i = 0; i < size; i++)
                    WriteRegister(base + i, ReadBE(address + i * 4));
                break;
            }
            case 0x54: // INTERRUPT: raise the graphics interrupt (source 1) on each hardware thread in the mask
            {
                const uint32_t cpuMask = s.Read();
                CpSync(SyncReason::kInterrupt);
                for (uint32_t cpu = 0; cpu < 6; cpu++)
                    if (cpuMask & (1u << cpu))
                        DispatchInterrupt(g_cpThread, *g_cpContext, 1, cpu);
                break;
            }
            case 0x50: // SET_BIN_MASK
            {
                const uint64_t hi = s.Read(), lo = s.Read();
                g_binMask = hi << 32 | lo;
                break;
            }
            case 0x51: // SET_BIN_SELECT
            {
                const uint64_t hi = s.Read(), lo = s.Read();
                g_binSelect = hi << 32 | lo;
                break;
            }
            case 0x60: g_binMask = (g_binMask & ~0xFFFFFFFFull) | s.Read(); break;               // SET_BIN_MASK_LO
            case 0x61: g_binMask = (g_binMask & 0xFFFFFFFFull) | uint64_t(s.Read()) << 32; break; // SET_BIN_MASK_HI
            case 0x62: g_binSelect = (g_binSelect & ~0xFFFFFFFFull) | s.Read(); break;           // SET_BIN_SELECT_LO
            case 0x63: g_binSelect = (g_binSelect & 0xFFFFFFFFull) | uint64_t(s.Read()) << 32; break; // SET_BIN_SELECT_HI
            case kOpcodeRuntimeSwap:
            {
                const uint32_t signature = s.Read();
                if (signature != kSwapSignature)
                    Fatal("GPU: malformed runtime swap packet");
                const uint32_t frontBuffer = s.Read(), width = s.Read(), height = s.Read();
                // XAM's system command buffer (kernel/xboxkrnl_video.cpp VdSwap): its identifier goes back once the
                // frame is out, and its overlay is composited into this frame.
                const uint32_t identifierAddress = s.Read(), identifier = s.Read();
                GuestOverlay overlay;
                overlay.base = s.Read();
                overlay.format = s.Read();
                overlay.width = s.Read();
                overlay.height = s.Read();
                overlay.mode = s.Read();
                for (uint32_t& v : overlay.source)
                    v = s.Read();
                CpSync(SyncReason::kSwap);
                const uint64_t frame = ++g_swapCount;
                g_counter++;
                TraceSwap(frame);
                // A launcher's cover goes once the first frame with content is up (the boot animation is ready;
                // native/x360/system/transition): signalled here when headless, by the presenter with a window.
                transition::FrameShown(Physical(frontBuffer), size_t(width) * height * 4, gpu::PresenterActive());
                // Decodes the front buffer by fetch constant 0 (VdSwap wrote it just before this packet), applies the
                // display gamma and hands the result to the window (renderer.cpp, Renderer::Swap).
                RenderSwap(frame, frontBuffer, width, height, overlay);
                if (identifierAddress)
                    guest::g_memory.Write32(identifierAddress, identifier);
                input::OnFrame(frame);
                kernel::DiagnosticsFrame(frame);
                {
                    // Stalls: a swap that comes much later than the 60 Hz pace (load hitches, freezes).
                    static auto previousSwap = std::chrono::steady_clock::now();
                    const auto now = std::chrono::steady_clock::now();
                    const double ms = std::chrono::duration<double, std::milli>(now - previousSwap).count();
                    previousSwap = now;
                    // CB_FRAME_LOG=<ms> lowers the threshold (e.g. 20 lists every frame that missed a vblank).
                    static const double threshold = [] { const char* v = std::getenv("CB_FRAME_LOG"); return v ? std::atof(v) : 100.0; }();
                    // With CB_GPU_STATS=1 as well, the slow frame's draws and GPU syncs are listed too.
                    static uint64_t previousDraws = 0;
                    const uint64_t frameDraws = g_draws - previousDraws;
                    previousDraws = g_draws;
                    const std::string syncs = g_stats && std::getenv("CB_FRAME_LOG") ? TakeSyncStats() : std::string();
                    if (frame > 1 && ms >= threshold)
                        Log("GPU: frame {} came {:.1f} ms after the previous one (vblank {}, {} host shaders, {} draws){}",
                            frame, ms, g_vblanks.load(), CompiledShaderCount(), frameDraws, syncs);
                }
                if (frame == 1 || frame % 600 == 0)
                {
                    // Presentation rate since the previous report (wall clock; the title is paced by the 60 Hz vblank).
                    static auto previousReport = std::chrono::steady_clock::now();
                    const auto now = std::chrono::steady_clock::now();
                    const double seconds = std::chrono::duration<double>(now - previousReport).count();
                    previousReport = now;
                    const double gpuMs = TakeGpuMilliseconds();
                    Log("GPU: frame {} presented ({} draw packets so far, {} survey-only; {} shaders, {} host shaders; "
                        "{:.1f} frames/s; GPU {:.2f} ms/frame)", frame, g_draws, g_surveyDraws, ShaderCount(),
                        CompiledShaderCount(), frame == 1 || seconds <= 0 ? 0.0 : 600 / seconds,
                        frame == 1 ? 0.0 : gpuMs / 600);
                    if (g_stats)
                        Log("GPU: GPU time by kind over the last 600 frames: {}; {} ring kicks reported by MMIO stores",
                            TakeGpuBreakdown(), g_kicks.exchange(0));
                    if (g_stats)
                    {
                        std::string counts;
                        for (uint32_t op = 0; op < 128; op++)
                            if (g_opcodeCounts[op])
                                counts += std::format(" {:02X}:{}", op, g_opcodeCounts[op]);
                        Log("GPU: type-3 packets so far:{}", counts);
                        Log("GPU: syncs (submits/calls, time waiting) since the last report:{}", TakeSyncStats());
                        for (auto& [key, stat] : g_waitStats)
                            if (stat.count)
                                Log("GPU: WAIT_REG_MEM info {:X} address {:08X} ref {:08X} mask {:08X} wait {:X}: {} "
                                    "times, {} failed polls, {:.1f} ms", key[0], key[1], key[2], key[3], key[4],
                                    stat.count, stat.misses, stat.ns / 1e6);
                        g_waitStats.clear();
                    }
                }
                break;
            }
            default:
                Fatal("GPU: PM4 type-3 opcode {:02X} ({} dwords) is not implemented", opcode, count);
            }
            if (s.pos > end)
                Fatal("GPU: opcode {:02X} read past its {}-dword payload", opcode, count);
            while (s.pos < end)
                s.Read();
        }

        void ExecutePackets(Stream& s)
        {
            while (s.Remaining())
            {
                const uint32_t header = s.Read();
                if (header == 0 || header == 0x0BADF00D)
                    continue;
                switch (header >> 30)
                {
                case 0:
                {
                    const uint32_t count = ((header >> 16) & 0x3FFF) + 1;
                    const uint32_t base = header & 0x7FFF;
                    if (count > s.Remaining())
                        { ReportTruncated(std::format("type-0 packet {:08X}", header), s); return; }
                    g_currentOpcode = 0x10000 | base; // type 0: register writes from `base`
                    g_currentPacket = s.Address() - 4;
                    for (uint32_t i = 0; i < count; i++)
                        WriteRegister((header & 0x8000) ? base : base + i, s.Read());
                    break;
                }
                case 1:
                {
                    if (s.Remaining() < 2)
                        { ReportTruncated(std::format("type-1 packet {:08X}", header), s); return; }
                    const uint32_t first = s.Read(), second = s.Read();
                    WriteRegister(header & 0x7FF, first);
                    WriteRegister((header >> 11) & 0x7FF, second);
                    break;
                }
                case 2:
                    break; // NOP
                case 3:
                {
                    const uint32_t opcode = (header >> 8) & 0x7F;
                    const uint32_t count = ((header >> 16) & 0x3FFF) + 1;
                    if (count > s.Remaining())
                        { ReportTruncated(std::format("type-3 opcode {:02X} ({} dwords)", opcode, count), s); return; }
                    const bool predicated = (header & 1) != 0;
                    if (predicated && ((g_binSelect & g_binMask) == 0 || opcode == kOpcodeRuntimeSwap))
                    {
                        for (uint32_t i = 0; i < count; i++)
                            s.Read();
                        break;
                    }
                    ExecuteType3(opcode, count, s);
                    break;
                }
                }
            }
        }

        void CommandProcessorThread()
        {
            g_cpThread = CreateSystemThread();
            g_cpContext = kernel::AttachHostThread(g_cpThread);
            uint32_t idleSpins = 0;
            for (;;)
            {
                const uint32_t size = g_ringSizeDwords.load();
                const uint32_t writeIndex = ReadRegister(reg::kCpRbWptr);
                if (size == 0 || writeIndex == g_readIndex || writeIndex == 0xBAADF00D)
                {
                    if (!g_deferred.empty())
                    {
                        // The title is likely waiting for one of these: wake with the GPU fence, not a timer.
                        RetireDeferredWrites(1);
                        idleSpins = 0;
                        continue;
                    }
                    // Idle: the title's write-pointer store wakes us (PpcMmioStore32). A short spin first (the next
                    // kick often follows at once); the 1 ms timeout covers writes that do not come through an MMIO
                    // store. This used to spin and yield up to 2000 times per idle period (~a third of a core).
                    if (++idleSpins < 64)
                        YieldProcessor();
                    else
                        WaitForSingleObject(g_kick, 1);
                    continue;
                }
                idleSpins = 0;
                Stream ring{ g_ringBase.load(), size, g_readIndex, (writeIndex - g_readIndex) & (size - 1) };
                ExecutePackets(ring);
                g_currentOpcode = 0x20000; // ring chunk done
                // The title may reuse ring space (and data referenced from it, e.g. DrawVerticesUP vertices) once the
                // read pointer has passed it: the GPU must be done with it first (a deferred write).
                g_readIndex = writeIndex & (size - 1);
                if (const uint32_t writeBack = g_readPointerWriteBack.load())
                    DeferWrite(Physical(writeBack), __builtin_bswap32(g_readIndex));
            }
        }
    }

    void Initialize()
    {
        auto& mem = guest::g_memory;
        mem.HostCommit(kMmioBase, kMmioSize);
        mem.HostCommit(kWritebackVirtualBase, kWritebackVirtualSize);
        // The low 16 MiB of physical memory is used by the system (GPU writeback); titles' allocations land above it.
        if (mem.physical.Allocate(0x1000000, 0x1000000, 0, 0xFFFFFF, false) != 0)
            Fatal("Unable to reserve the low 16 MiB of physical memory");
        mem.CommitPhysical(0, 0x1000000);
        WriteFixedRegisters();
    }

    void InitializeRingBuffer(uint32_t physical, uint32_t sizeLog2)
    {
        const uint32_t bytes = 1u << (sizeLog2 + 3);
        g_ringBase = physical & 0x1FFFFFFF;
        memset(Physical(physical), 0, bytes);
        g_readIndex = 0;
        g_ringSizeDwords = bytes / 4;
        Log("GPU: ring buffer at {:08X}, {} bytes", physical, bytes);
        if (!g_cpStarted)
        {
            g_cpStarted = true;
            kernel::StartHostThread(CommandProcessorThread);
        }
    }

    void WaitForProgress(uint32_t microseconds)
    {
        std::unique_lock lock(g_progressMutex);
        const uint64_t seen = g_progress.load();
        g_progressChanged.wait_for(lock, std::chrono::microseconds(microseconds),
                                   [&] { return g_progress.load() != seen; });
    }

    void EnableReadPointerWriteBack(uint32_t address, uint32_t blockSizeLog2)
    {
        // A physical address, or a CPU address in a physical window.
        g_readPointerWriteBack = address >= guest::kPhysicalWindow64k ? guest::WindowToPhysical(address) : address;
        Log("GPU: read pointer write-back to {:08X} (physical {:08X})", address, g_readPointerWriteBack.load());
    }

    void ClearInterruptCallbackIn(uint32_t start, uint32_t end)
    {
        const uint32_t callback = g_interruptCallback.load();
        if (callback >= start && callback < end)
        {
            std::lock_guard lock(g_interruptMutex);
            g_interruptCallback = 0;
            g_interruptUserData = 0;
            Log("GPU: the ended title's interrupt callback {:08X} removed", callback);
        }
    }

    void SetInterruptCallback(uint32_t callback, uint32_t userData)
    {
        g_interruptUserData = userData;
        g_interruptProcessType = kernel::CurrentProcessType();
        g_interruptCallback = callback;
        Log("GPU: interrupt callback {:08X} (user data {:08X}, process type {})", callback, userData,
            g_interruptProcessType.load());
        if (!g_vblankStarted && callback != 0)
        {
            g_vblankStarted = true;
            kernel::StartHostThread(VblankThread);
        }
    }

    void SetRegister(uint32_t index, uint32_t value)
    {
        StoreRegister(index, value);
    }

    void TestWriteRegister(uint32_t index, uint32_t value)
    {
        WriteRegister(index, value);
    }

    const DisplayGamma& CurrentDisplayGamma()
    {
        return g_gamma;
    }

    uint64_t VblankCount()
    {
        return g_vblanks;
    }

    uint32_t ReadRegister(uint32_t index)
    {
        if (index < 0x4000)
            return __builtin_bswap32(__atomic_load_n(RegisterSlot(index), __ATOMIC_SEQ_CST));
        if (index < reg::kCount)
            return g_hostRegisters[index];
        Fatal("GPU register read of index {:X} beyond the register file", index);
    }

    const uint32_t* HostRegisters(uint32_t first)
    {
        if (first < 0x4000 || first >= reg::kCount)
            Fatal("GPU: HostRegisters({:X}) outside the host register file", first);
        return g_hostRegisters.data() + first;
    }

    uint64_t ConstantVersion(ConstantBlock block)
    {
        return g_constantVersions[size_t(block)];
    }

    std::string DescribeCommandProcessor()
    {
        // Opcode: type-3 opcode, 0x1xxxx = type-0 writes from register xxxx, 0x20000 = renderer sync after a ring chunk.
        return std::format("current packet {:X} at {:08X}", g_currentOpcode.load(), g_currentPacket.load());
    }

    uint64_t SwapCount()
    {
        return g_swapCount.load();
    }

    namespace
    {
        uint32_t g_runtimeFront = 0, g_runtimeFrontSize = 0;
    }

    void PresentRuntimeFrame(uint32_t width, uint32_t height)
    {
        // A black linear front buffer, described by fetch constant 0 as VdSwap describes a title's (present.cpp).
        auto& mem = guest::g_memory;
        const uint32_t pitch = (width + 31) & ~31u;
        const uint32_t bytes = (pitch * height * 4 + 0xFFF) & ~0xFFFu;
        if (g_runtimeFrontSize != bytes)
        {
            if (g_runtimeFront)
                mem.physical.Free(g_runtimeFront);
            g_runtimeFront = mem.physical.Allocate(bytes, 0x1000, 0x1000000, 0x1FFFFFFF, true);
            if (g_runtimeFront == UINT32_MAX)
                Fatal("GPU: no physical memory for the runtime's {}x{} front buffer", width, height);
            mem.CommitPhysical(g_runtimeFront, bytes);
            std::memset(Physical(g_runtimeFront), 0, bytes);
            g_runtimeFrontSize = bytes;
        }
        StoreRegister(0x4800, 2 | (pitch / 32) << 22);              // type 2 (texture), linear, pitch
        StoreRegister(0x4801, 6 | g_runtimeFront);                  // k_8_8_8_8, no endian swap
        StoreRegister(0x4802, (width - 1) | (height - 1) << 13);
        StoreRegister(0x4803, (2u | 1u << 3) << 1);                 // LE_X8R8G8B8
        StoreRegister(0x4804, 0);
        StoreRegister(0x4805, 1u << 9);                             // 2D
        g_vblanks++; // no vblank thread before the kernel: these frames are the system UI's clock
        const uint64_t frame = ++g_swapCount;
        transition::FrameShown(Physical(g_runtimeFront), size_t(width) * height * 4, gpu::PresenterActive());
        RenderSwap(frame, g_runtimeFront, width, height, GuestOverlay{});
        input::OnFrame(frame);
    }

    void EndRuntimeFrames()
    {
        if (g_runtimeFront)
            guest::g_memory.physical.Free(g_runtimeFront);
        g_runtimeFront = g_runtimeFrontSize = 0;
    }
}

void PpcMmioStore32(uint32_t address) noexcept
{
    if (address == gpu::kMmioBase + gpu::reg::kCpRbWptr * 4)
    {
        gpu::g_kicks.fetch_add(1, std::memory_order_relaxed);
        SetEvent(gpu::g_kick);
    }
}
