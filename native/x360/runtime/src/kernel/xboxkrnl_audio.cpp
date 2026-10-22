// xboxkrnl audio exports: the render-driver interface used by the title's XAudio2, and XMA context allocation.
// Behaviour cross-checked against Xenia Canary / Nexia (BSD-3) and the title's own code; the code is our own.
//
// Render driver: up to 8 clients. The console's audio hardware plays one frame of 256 samples at 48 kHz (5.333 ms)
// per tick. A frame is 6 channels x 256 samples of big-endian float, planar FL FR FC LFE BL BR (6144 bytes).
// Each tick of the emulated audio clock plays the oldest submitted frame of every client (summed; a client with
// nothing queued contributes silence) and then lets each client produce frames until it is four frames ahead: the client's callback runs
// with r3 = address of a cell holding its context and submits one frame with XAudioSubmitRenderDriverFrame. TU46's
// XAudio2 callback (CXboxRendererConnection::DriverCallback, 0x82A1E738) waits for its engine to finish a pass,
// de-interleaves it into one global buffer (0x82D02950) shared by all its engines and submits from there, so a
// submitted frame is copied at once. The callbacks therefore run one at a time, in client order, on one driver
// thread (as the console's audio driver and Xenia's worker do): TU46 registers three engines, and calling them from
// parallel threads let one engine's pass overwrite another's frame in the shared buffer while it was being submitted
// (music cut to zero, doubled or halved at random points: the user's pops).
//
// The clock ticks from a timer at exactly 48000/256 Hz; with a host device (audio/host_output.h) it is steered by
// the device's consumption so the host queue neither drains nor grows. CB_AUDIO_DUMP writes the mix to a WAV file.
//
// XMA: audio/apu.* (hardware contexts, register traps, decoding).

#include "modules.h"
#include "kernel/kernel.h"

#include "audio/apu.h"
#include "audio/host_output.h"
#include "audio/ui_sounds.h"
#include "audio/wav_dump.h"
#include "guest_clock.h"

#include <windows.h>
#include <timeapi.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

namespace
{
    using Clock = std::chrono::steady_clock;

    constexpr uint32_t kMaxClients = 8;
    constexpr uint32_t kMaxQueuedFrames = 64;    // submitted frames waiting to be played, per client
    // Frames each client keeps ahead of the hardware (queued + being produced). TU46's XAudio2 asks for 1024 samples
    // of output latency unless the speaker config has XAUDIOSPEAKERCONFIG_ANALOG_LOW_LATENCY (StartStreaming,
    // 0x82A1EB80), i.e. four frames.
    constexpr uint32_t kFramesAhead = 4;
    constexpr uint32_t kFrameSamples = audio::kFrameSamples;
    constexpr uint32_t kChannels = audio::kChannels;
    constexpr uint32_t kHandleTag = 0x41550000; // "AU" | client index
    constexpr uint32_t kHresultInvalidArg = 0x80070057;
    // Host queue the clock aims for when a device is present (samples): two frames beyond the device's own buffer.
    constexpr uint32_t kHostTargetSamples = 2 * kFrameSamples;

    using Frame = std::array<float, kFrameSamples * kChannels>; // interleaved FL FR FC LFE BL BR

    struct Client
    {
        bool inUse = false;
        uint32_t generation = 0;
        uint32_t callback = 0;
        uint32_t argumentCell = 0; // guest cell holding the client's context value
        uint32_t pendingCallbacks = 0;
        std::deque<Frame> queue;
        bool submitted = false; // a client is not late before its first frame
        uint64_t underruns = 0, overruns = 0;
    };

    std::mutex g_mutex;
    std::atomic<uint64_t> g_ticks = 0;
    std::condition_variable g_callbackReady;
    std::condition_variable g_callbackDone; // fixed clock: a callback returned
    std::array<Client, kMaxClients> g_clients;
    bool g_clockStarted = false;

    // The driver thread: runs the clients' callbacks one at a time, in client order (one per client per round).
    void DriverThread()
    {
        kernel::ThreadParams params;
        params.stackSize = 0x20000;
        params.processType = kernel::kProcessTypeSystem; // serves whatever title runs; not ended with it
        kernel::GuestThread* thread = kernel::CreateThread(params);
        // The console's audio driver calls its clients on hardware thread 4: XAudio in a system module (bootanim)
        // mixes inside the callback with per-CPU state it sets up for CPU 4 only (its processor table always says CPU
        // 4 for the system process, and CPU 4 is also a title's default), and finds that state through the PCR's
        // current CPU (bootanim 0x98050DB8).
        kernel::SetCurrentCpu(thread, 4);
        PPCContext* ctx = kernel::AttachHostThread(thread);
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
        std::unique_lock lock(g_mutex);
        for (;;)
        {
            g_callbackReady.wait(lock, [] {
                return std::any_of(g_clients.begin(), g_clients.end(),
                                   [](const Client& c) { return c.inUse && c.pendingCallbacks > 0; });
            });
            for (Client& c : g_clients)
            {
                if (!c.inUse || c.pendingCallbacks == 0)
                    continue;
                c.pendingCallbacks--;
                const uint32_t callback = c.callback, cell = c.argumentCell;
                if (!modules::FindByCode(callback)) // its title has ended (ExTerminateTitleProcess)
                {
                    c.inUse = false;
                    continue;
                }
                lock.unlock();
                ctx->r1.u64 = thread->stackBase;
                ctx->r3.u64 = cell;
                kernel::CallGuest(*ctx, callback);
                lock.lock();
                g_callbackDone.notify_all();
            }
        }
    }

    // One tick: play a frame of every client, then queue their callbacks.
    void Tick()
    {
        static uint64_t tick = 0;
        static std::array<uint64_t, kMaxClients> reported{};
        g_ticks++;
        Frame mix{};
        {
            std::lock_guard lock(g_mutex);
            // Every 10 s: clients that had no frame ready when the hardware needed one.
            if (++tick % (10 * audio::kSampleRate / kFrameSamples) == 0)
                for (uint32_t i = 0; i < kMaxClients; i++)
                    if (g_clients[i].inUse && g_clients[i].underruns != reported[i])
                    {
                        Log("Audio: client {}: {} late frame(s) in the last 10 s (played as silence)", i,
                            g_clients[i].underruns - reported[i]);
                        reported[i] = g_clients[i].underruns;
                    }
            for (Client& c : g_clients)
            {
                if (!c.inUse)
                    continue;
                if (c.queue.empty())
                    c.underruns += c.submitted ? 1 : 0;
                else
                {
                    const Frame& f = c.queue.front();
                    for (size_t i = 0; i < mix.size(); i++)
                        mix[i] += f[i];
                    c.queue.pop_front();
                }
                // Ask for frames until the client is kFramesAhead ahead again.
                const uint32_t ahead = uint32_t(c.queue.size()) + c.pendingCallbacks;
                if (ahead < kFramesAhead)
                    c.pendingCallbacks += kFramesAhead - ahead;
            }
        }
        g_callbackReady.notify_all();
        audio::MixUiSounds(mix.data(), kFrameSamples);
        audio::QueueHostSamples(mix.data(), kFrameSamples);
        audio::DumpSamples(mix.data(), kFrameSamples);
    }

    void ClockThread();

    // Starts the driver and clock threads once (g_mutex held).
    void StartClockLocked()
    {
        if (g_clockStarted)
            return;
        g_clockStarted = true;
        kernel::StartHostThread(DriverThread);
        if (!guest_clock::Fixed())
            std::thread(ClockThread).detach(); // fixed: the vblanks tick it (audio::AdvanceFixedClock)
    }

    // The audio clock: 48000 / 256 ticks per second from a timer; steered towards the host device's rate.
    void ClockThread()
    {
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
        timeBeginPeriod(1);
        const bool host = audio::HostOutputActive();
        const auto start = Clock::now();
        double elapsedNs = 0.0; // scheduled time of the next tick, from start
        constexpr double kPeriodNs = 1e9 * kFrameSamples / audio::kSampleRate;
        for (uint64_t tick = 0;; tick++)
        {
            double period = kPeriodNs;
            if (host)
            {
                // Proportional steering, at most +-0.5%: a fuller host queue slows the clock down.
                const double error = (double(audio::QueuedHostSamples()) - kHostTargetSamples) / kHostTargetSamples;
                period *= 1.0 + std::clamp(error * 0.002, -0.005, 0.005);
            }
            elapsedNs += period;
            const auto due = start + std::chrono::nanoseconds(int64_t(elapsedNs));
            // Coarse sleep, then a short spin for the last part.
            for (;;)
            {
                const auto now = Clock::now();
                if (now >= due)
                    break;
                const auto left = due - now;
                if (left > std::chrono::microseconds(1500))
                    std::this_thread::sleep_for(left - std::chrono::microseconds(1000));
                else
                    std::this_thread::yield();
            }
            // A late clock does not catch up with a burst: it restarts its schedule from now.
            if (Clock::now() - due > std::chrono::milliseconds(50))
                elapsedNs = double(std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - start).count());
            Tick();
        }
    }
}

// CB_FIXED_CLOCK=1 (src/guest_clock.h): the audio clock follows the guest clock instead of a host timer. Each vblank
// plays every frame due by the new guest time, and each frame's callbacks finish before the next one is played, so
// the clients produce the same frames at the same guest time in every run.
void audio::AdvanceFixedClock(uint64_t guestNs)
{
    constexpr uint64_t kPeriodNs = 1000000000ull * kFrameSamples / audio::kSampleRate; // 5.33 ms
    static uint64_t played = 0;
    {
        std::lock_guard lock(g_mutex);
        if (!g_clockStarted)
            return;
    }
    while ((played + 1) * kPeriodNs <= guestNs)
    {
        Tick();
        played++;
        // A callback that never returns (a stuck client) stops waiting after 250 ms, as the vblank's own wait.
        std::unique_lock lock(g_mutex);
        g_callbackDone.wait_for(lock, std::chrono::milliseconds(250), [] {
            return std::none_of(g_clients.begin(), g_clients.end(),
                                [](const Client& c) { return c.inUse && c.pendingCallbacks > 0; });
        });
    }
}

uint64_t audio::RenderTicks()
{
    return g_ticks;
}

// The system UI's sounds are mixed by the clock's ticks (audio/ui_sounds.h), with or without a guest client.
void audio::StartRenderClock()
{
    std::lock_guard lock(g_mutex);
    StartClockLocked();
}

namespace kernel
{
    // APU register block, XMA contexts and the host output; at startup, since the title's XAudio2 touches the
    // registers directly.
    void InitializeAudio()
    {
        audio::InitializeApu();
        audio::StartHostOutput();
        audio::InitializeUiSounds();
    }
}

// VOID XAudioGetSpeakerConfig(PDWORD Config): Dolby Digital | analog surround (the XConfig default), no low latency.
GUEST_IMPORT(XAudioGetSpeakerConfig)
{
    guest::g_memory.Write32(ctx.r3.u32, 0x00010001);
    ctx.r3.u64 = 0;
}

// HRESULT XAudioGetVoiceCategoryVolume(DWORD Category, PFLOAT Volume): full volume for every category.
GUEST_IMPORT(XAudioGetVoiceCategoryVolume)
{
    guest::g_memory.Write32(ctx.r4.u32, 0x3F800000);
    ctx.r3.u64 = 0;
}

// HRESULT XAudioEnableDucker(BOOL Enable)
GUEST_IMPORT(XAudioEnableDucker)
{
    ctx.r3.u64 = 0;
}

// HRESULT XAudioRegisterRenderDriverClient(PXAUDIO_DRIVER_CLIENT Client {Callback, Context}, PDWORD DriverHandle)
GUEST_IMPORT(XAudioRegisterRenderDriverClient)
{
    auto& mem = guest::g_memory;
    const uint32_t clientStruct = ctx.r3.u32;
    if (clientStruct == 0 || mem.Read32(clientStruct) == 0)
    {
        ctx.r3.u64 = kHresultInvalidArg;
        return;
    }
    uint32_t index = 0, generation = 0;
    {
        std::lock_guard lock(g_mutex);
        while (index < kMaxClients && g_clients[index].inUse)
            index++;
        if (index == kMaxClients)
            Fatal("XAudioRegisterRenderDriverClient: all {} render-driver client slots are in use", kMaxClients);
        Client& c = g_clients[index];
        generation = c.generation + 1;
        c = Client{};
        c.inUse = true;
        c.generation = generation;
        c.callback = mem.Read32(clientStruct);
        c.argumentCell = mem.SystemAlloc(4);
        mem.Write32(c.argumentCell, mem.Read32(clientStruct + 4));
        StartClockLocked();
    }
    mem.Write32(ctx.r4.u32, kHandleTag | index);
    Log("Audio: render driver client {} registered (callback {:08X})", index, mem.Read32(clientStruct));
    ctx.r3.u64 = 0;
}

// HRESULT XAudioUnregisterRenderDriverClient(DWORD DriverHandle)
GUEST_IMPORT(XAudioUnregisterRenderDriverClient)
{
    const uint32_t index = ctx.r3.u32 & 0xFFFF;
    {
        std::lock_guard lock(g_mutex);
        if (index < kMaxClients && g_clients[index].inUse)
        {
            guest::g_memory.SystemFree(g_clients[index].argumentCell);
            const uint32_t generation = g_clients[index].generation;
            g_clients[index] = Client{};
            g_clients[index].generation = generation;
        }
    }
    g_callbackReady.notify_all();
    ctx.r3.u64 = 0;
}

// HRESULT XAudioSubmitRenderDriverFrame(DWORD DriverHandle, PFLOAT Samples): queues a copy of the frame.
GUEST_IMPORT(XAudioSubmitRenderDriverFrame)
{
    const uint32_t index = ctx.r3.u32 & 0xFFFF;
    const uint8_t* src = guest::g_memory.Translate(ctx.r4.u32);
    Frame frame;
    for (uint32_t channel = 0; channel < kChannels; channel++)
        for (uint32_t i = 0; i < kFrameSamples; i++)
        {
            uint32_t raw;
            std::memcpy(&raw, src + (size_t(channel) * kFrameSamples + i) * 4, 4);
            raw = __builtin_bswap32(raw);
            std::memcpy(&frame[size_t(i) * kChannels + channel], &raw, 4);
        }
    std::lock_guard lock(g_mutex);
    if (index < kMaxClients && g_clients[index].inUse)
    {
        Client& c = g_clients[index];
        if (c.queue.size() >= kMaxQueuedFrames)
        {
            c.queue.pop_front();
            if (c.overruns++ == 0)
                Log("Audio: client {} submitted more than {} frames ahead; the oldest are dropped", index, kMaxQueuedFrames);
        }
        c.queue.push_back(frame);
        c.submitted = true;
    }
    ctx.r3.u64 = 0;
}

// NTSTATUS XMACreateContext(PXMA_CONTEXT* Context): the lowest free hardware context, or STATUS_NO_MEMORY.
GUEST_IMPORT(XMACreateContext)
{
    const uint32_t context = audio::AllocateXmaContext();
    guest::g_memory.Write32(ctx.r3.u32, context);
    ctx.r3.u64 = context ? kernel::kStatusSuccess : kernel::kStatusNoMemory;
}

// NTSTATUS XMAReleaseContext(PXMA_CONTEXT Context): zeroes and frees the context.
GUEST_IMPORT(XMAReleaseContext)
{
    audio::ReleaseXmaContext(ctx.r3.u32);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// DWORD XAudioGetVoiceCategoryVolumeChangeMask(PVOID Driver, PDWORD ChangeMask)
// Which voice categories' volumes changed since the last call (the dashboard's audio settings change them). The
// runtime never changes a category volume, so the mask is always 0.
GUEST_IMPORT(XAudioGetVoiceCategoryVolumeChangeMask)
{
    if (ctx.r4.u32)
        guest::g_memory.Write32(ctx.r4.u32, 0);
    ctx.r3.u64 = 0;
}

void kernel::UnregisterAudioClientsIn(uint32_t start, uint32_t end)
{
    std::lock_guard lock(g_mutex);
    for (Client& c : g_clients)
        if (c.inUse && c.callback >= start && c.callback < end)
        {
            c.inUse = false;
            c.queue.clear();
            c.pendingCallbacks = 0;
            Log("XAudio: the ended title's render client {:08X} removed", c.callback);
        }
}

// The kernel's XMA decoder API (XDK xmadecoder.h; the older dashboards' XAudio uses it where XAudio2 titles program
// the hardware themselves). Each function is a field of the context (audio/apu.cpp's layout) or a context state
// change; the context holds physical addresses (the buffers are physical allocations). An enabled context decodes
// after each change that gives it work (input valid, output space), as the hardware would on its own.
namespace
{
    uint32_t XmaPhysical(uint32_t address, const char* what)
    {
        if (address == 0)
            return 0;
        if (!guest::IsPhysicalWindow(address))
            Fatal("XMA: {} {:08X} is not in a physical memory window", what, address);
        return guest::WindowToPhysical(address);
    }
}

// VOID XMAInitializeContext(PXMACONTEXT Context, PXMACONTEXTINIT Init): the context from its XMACONTEXTINIT
// { pInputBuffer0, InputBuffer0Size (2 KB packets), pInputBuffer1, InputBuffer1Size, InputBufferReadOffset (bits),
// pOutputBuffer, OutputBufferSize (subframes), pWorkBuffer, NumSubframesToDecode, NumChannels (0 mono, 1 stereo:
// the 2863 dash stores channels - 1), DecoderTable (the sample rate: 0 24 kHz ... 3 48 kHz, as the 2863 dash picks
// it), LoopData { LoopStart, LoopEnd, LoopCount, LoopSubframeEnd, LoopSubframeSkip } }; nothing valid, offsets 0.
GUEST_IMPORT(XMAInitializeContext)
{
    auto& mem = guest::g_memory;
    const uint32_t context = ctx.r3.u32, init = ctx.r4.u32;
    auto field = [&](uint32_t offset) { return mem.Read32(init + offset); };
    const uint32_t w[10] = {
        (field(0x04) & 0xFFF) | uint32_t(mem.Read8(init + 0x34)) << 12 | (field(0x18) & 0x1F) << 22,
        (field(0x0C) & 0xFFF) | (uint32_t(mem.Read8(init + 0x35)) & 3) << 12 | (uint32_t(mem.Read8(init + 0x36)) & 7) << 17 |
            (field(0x20) & 0xF) << 20 | (field(0x28) & 3) << 27 | (field(0x24) & 1) << 29,
        field(0x10) & 0x3FFFFFF,
        field(0x2C) & 0x3FFFFFF,
        field(0x30) & 0x3FFFFFF,
        XmaPhysical(field(0x00), "input buffer 0"),
        XmaPhysical(field(0x08), "input buffer 1"),
        XmaPhysical(field(0x14), "output buffer"),
        XmaPhysical(field(0x1C), "work buffer"),
        0,
    };
    if (field(0x24) > 1)
        Fatal("XMAInitializeContext: NumChannels {} (0 mono / 1 stereo expected)", field(0x24));
    for (int i = 0; i < 10; i++)
        audio::SetXmaField(context, i, 0, 32, w[i], false);
}

GUEST_IMPORT(XMAEnableContext) { audio::EnableXmaContext(ctx.r3.u32); }

// HRESULT XMADisableContext(PXMACONTEXT Context, BOOL BlockIfBusy): decoding is done at each kick here, so it is
// never busy.
GUEST_IMPORT(XMADisableContext)
{
    audio::DisableXmaContext(ctx.r3.u32);
    ctx.r3.u64 = 0;
}

// VOID XMABlockWhileInUse(PXMACONTEXT Context): nothing to wait for (see XMADisableContext).
GUEST_IMPORT(XMABlockWhileInUse) {}

GUEST_IMPORT(XMAGetOutputBufferWriteOffset) { ctx.r3.u64 = audio::XmaField(ctx.r3.u32, 0, 27, 5); }
GUEST_IMPORT(XMAGetOutputBufferReadOffset) { ctx.r3.u64 = audio::XmaField(ctx.r3.u32, 9, 0, 5); }
GUEST_IMPORT(XMASetOutputBufferReadOffset) { audio::SetXmaField(ctx.r3.u32, 9, 0, 5, ctx.r4.u32, true); }
GUEST_IMPORT(XMASetOutputBufferValid) { audio::SetXmaField(ctx.r3.u32, 1, 31, 1, 1, true); }
GUEST_IMPORT(XMAIsOutputBufferValid) { ctx.r3.u64 = audio::XmaField(ctx.r3.u32, 1, 31, 1); }
GUEST_IMPORT(XMASetInputBuffer0Valid) { audio::SetXmaField(ctx.r3.u32, 0, 20, 1, 1, true); }
GUEST_IMPORT(XMAIsInputBuffer0Valid) { ctx.r3.u64 = audio::XmaField(ctx.r3.u32, 0, 20, 1); }
GUEST_IMPORT(XMASetInputBuffer1Valid) { audio::SetXmaField(ctx.r3.u32, 0, 21, 1, 1, true); }
GUEST_IMPORT(XMAIsInputBuffer1Valid) { ctx.r3.u64 = audio::XmaField(ctx.r3.u32, 0, 21, 1); }
GUEST_IMPORT(XMAGetPacketMetadata) { ctx.r3.u64 = audio::XmaField(ctx.r3.u32, 4, 26, 5); }
GUEST_IMPORT(XMAGetInputBufferReadOffset) { ctx.r3.u64 = audio::XmaField(ctx.r3.u32, 2, 0, 26); }
GUEST_IMPORT(XMASetInputBufferReadOffset) { audio::SetXmaField(ctx.r3.u32, 2, 0, 26, ctx.r4.u32, false); }

// VOID XMASetInputBuffer0 / 1(PXMACONTEXT Context, VOID* Buffer, DWORD NumBlocks): the buffer and its 2 KB packets.
GUEST_IMPORT(XMASetInputBuffer0)
{
    audio::SetXmaField(ctx.r3.u32, 5, 0, 32, XmaPhysical(ctx.r4.u32, "input buffer 0"), false);
    audio::SetXmaField(ctx.r3.u32, 0, 0, 12, ctx.r5.u32, false);
}

GUEST_IMPORT(XMASetInputBuffer1)
{
    audio::SetXmaField(ctx.r3.u32, 6, 0, 32, XmaPhysical(ctx.r4.u32, "input buffer 1"), false);
    audio::SetXmaField(ctx.r3.u32, 1, 0, 12, ctx.r5.u32, false);
}

// VOID XMASetLoopData(PXMACONTEXT Context, LPCXMALOOPDATA Loop): { LoopStart, LoopEnd (bits), LoopCount,
// LoopSubframeEnd, LoopSubframeSkip }.
GUEST_IMPORT(XMASetLoopData)
{
    auto& mem = guest::g_memory;
    const uint32_t context = ctx.r3.u32, loop = ctx.r4.u32;
    audio::SetXmaField(context, 3, 0, 26, mem.Read32(loop), false);
    audio::SetXmaField(context, 4, 0, 26, mem.Read32(loop + 4), false);
    audio::SetXmaField(context, 0, 12, 8, mem.Read8(loop + 8), false);
    audio::SetXmaField(context, 1, 12, 2, mem.Read8(loop + 9), false);
    audio::SetXmaField(context, 1, 17, 3, mem.Read8(loop + 10), false);
}
