// Data watchpoints and memory scans for diagnostics. Host debug registers on every guest thread trap accesses to
// guest addresses and log the accessing thread's guest call stack.
//
//   CB_WATCH=<guest address hex>[:<length 1|2|4|8>][:w|rw]   default length 4, writes only (debug register 0)
//   CB_WATCH_LIMIT=<n>                                        hits logged per watch (default 100)
//   CB_SCAN=<frame>:<text>[:watch]   when frame <frame> is presented, finds <text> in guest memory as UTF-16BE and
//                                    as ASCII and logs each occurrence with the bytes around it; ":watch" then arms
//                                    read/write watches (debug registers 1-3) on the 2 bytes after the first
//                                    UTF-16 occurrences that are not followed by a terminator.
//   CB_MEMORY_DUMP=<frame>:<start hex>:<length hex>:<file>   when frame <frame> is presented, writes that guest range
//                                    as it is then (unreadable pages as zeros): tables a title builds as it starts,
//                                    such as its XUI's class registrations (Dashboard/Versions' XUI tables).
//
// The address must be aligned to the length. A data breakpoint fires after the accessing instruction, so the guest
// LR in the log is the return address of the last call the accessing function made (the backtrace shows its callers).

#include "kernel/kernel.h"

#include <windows.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <mutex>
#include <string>

namespace
{
    struct Watch
    {
        bool enabled = false;
        uint32_t guest = 0;
        uint32_t length = 4;
        bool reads = false;
    };

    constexpr size_t kSlots = 4; // DR0-DR3
    std::array<Watch, kSlots> g_watches;
    std::array<std::atomic<uint64_t>, kSlots> g_hits{};
    std::mutex g_armMutex;

    uint64_t Limit()
    {
        static const uint64_t limit = [] {
            const char* l = std::getenv("CB_WATCH_LIMIT");
            return l ? std::strtoull(l, nullptr, 10) : 100ull;
        }();
        return limit;
    }

    // CB_WATCH goes to slot 0 before any thread starts.
    const bool g_configured = [] {
        const char* v = std::getenv("CB_WATCH");
        if (!v || !*v)
            return false;
        Watch w;
        char* end = nullptr;
        w.guest = uint32_t(std::strtoul(v, &end, 16));
        if (*end == ':')
        {
            w.length = uint32_t(std::strtoul(end + 1, &end, 10));
            if (*end == ':')
                w.reads = std::string_view(end + 1) == "rw";
        }
        if ((w.length != 1 && w.length != 2 && w.length != 4 && w.length != 8) || (w.guest % w.length) != 0)
            Fatal("CB_WATCH: length {} / address {:08X} is not a valid aligned watch", w.length, w.guest);
        w.enabled = true;
        g_watches[0] = w;
        return true;
    }();

    LONG CALLBACK WatchHandler(EXCEPTION_POINTERS* info)
    {
        const uint64_t dr6 = info->ContextRecord->Dr6;
        if (info->ExceptionRecord->ExceptionCode != EXCEPTION_SINGLE_STEP || !(dr6 & 0xF))
            return EXCEPTION_CONTINUE_SEARCH;
        info->ContextRecord->Dr6 = 0;
        for (size_t slot = 0; slot < kSlots; slot++)
        {
            if (!(dr6 & (1ull << slot)))
                continue;
            const Watch w = g_watches[slot];
            const uint64_t hit = ++g_hits[slot];
            if (!w.enabled || hit > Limit())
                continue;
            const kernel::GuestThread* thread = kernel::CurrentThread();
            // Read through the kernel so the watched bytes are not touched by this thread (that would trap again).
            uint64_t value = 0;
            ReadProcessMemory(GetCurrentProcess(), guest::g_memory.Translate(w.guest), &value, w.length, nullptr);
            const PPCContext* c = thread ? thread->context : nullptr;
            Log("Watch {:08X}: hit {} on thread {} (rip {:p}), value now {:0{}X}; r3-r6 {:08X} {:08X} {:08X} {:08X}; "
                "guest stack {}", w.guest, hit, thread ? thread->id : 0, info->ExceptionRecord->ExceptionAddress,
                __builtin_bswap64(value) >> (64 - w.length * 8), w.length * 2, c ? c->r3.u32 : 0, c ? c->r4.u32 : 0,
                c ? c->r5.u32 : 0, c ? c->r6.u32 : 0, c ? kernel::GuestBacktrace(*c) : std::string("?"));
        }
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    void InstallHandler()
    {
        static const bool installed = [] {
            AddVectoredExceptionHandler(1, WatchHandler);
            return true;
        }();
        (void)installed;
    }

    // Writes every enabled slot into a (suspended or not yet started) host thread's debug registers.
    void ApplyWatches(void* hostThread)
    {
        CONTEXT context{};
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        if (!GetThreadContext(hostThread, &context))
            Fatal("Watch: GetThreadContext failed ({})", GetLastError());
        DWORD64* address[kSlots] = { &context.Dr0, &context.Dr1, &context.Dr2, &context.Dr3 };
        for (size_t slot = 0; slot < kSlots; slot++)
        {
            const Watch& w = g_watches[slot];
            const uint64_t enableBit = 1ull << (slot * 2);
            const uint64_t controlShift = 16 + slot * 4;
            context.Dr7 &= ~(enableBit | (0xFull << controlShift));
            if (!w.enabled)
                continue;
            const uint64_t lengthBits = w.length == 1 ? 0 : w.length == 2 ? 1 : w.length == 8 ? 2 : 3;
            *address[slot] = reinterpret_cast<DWORD64>(guest::g_memory.Translate(w.guest));
            context.Dr7 |= enableBit | ((uint64_t(w.reads ? 3 : 1) | lengthBits << 2) << controlShift);
        }
        if (!SetThreadContext(hostThread, &context))
            Fatal("Watch: SetThreadContext failed ({})", GetLastError());
    }

    // Arms a watch at run time on every running guest thread (not the calling one, which must not be a guest thread).
    void ArmEverywhere(size_t slot, uint32_t guestAddress, uint32_t length, bool reads)
    {
        std::lock_guard lock(g_armMutex);
        InstallHandler();
        g_watches[slot] = Watch{ true, guestAddress, length, reads };
        Log("Watch: slot {}: {} of {} byte(s) at guest {:08X}", slot, reads ? "reads and writes" : "writes", length,
            guestAddress);
        const kernel::GuestThread* self = kernel::CurrentThread();
        kernel::ForEachThread([&](kernel::GuestThread* t) {
            if (t == self || !t->hostHandle || !t->started || t->finished)
                return;
            if (SuspendThread(t->hostHandle) == DWORD(-1))
                return;
            ApplyWatches(t->hostHandle);
            ResumeThread(t->hostHandle);
        });
    }

    struct ScanConfig
    {
        uint64_t frame = 0;
        std::u16string text;
        std::string ascii;
        bool watch = false;
    };

    const ScanConfig& Scan()
    {
        static const ScanConfig scan = [] {
            ScanConfig s;
            const char* v = std::getenv("CB_SCAN");
            if (!v || !*v)
                return s;
            char* end = nullptr;
            s.frame = std::strtoull(v, &end, 10);
            if (*end != ':')
                Fatal("CB_SCAN: expected <frame>:<text>[:watch]");
            std::string text = end + 1;
            if (text.size() > 6 && text.ends_with(":watch"))
            {
                s.watch = true;
                text.resize(text.size() - 6);
            }
            s.ascii = text;
            for (char ch : text)
                s.text.push_back(char16_t(uint8_t(ch)));
            return s;
        }();
        return scan;
    }

    std::string HexBytes(uint32_t guestAddress, uint32_t count)
    {
        std::string out;
        for (uint32_t i = 0; i < count; i++)
        {
            uint8_t b = 0;
            if (!ReadProcessMemory(GetCurrentProcess(), guest::g_memory.Translate(guestAddress + i), &b, 1, nullptr))
                return out + " ??";
            out += std::format("{}{:02X}", i && i % 4 == 0 ? " " : "", b);
        }
        return out;
    }

    void RunScan(const ScanConfig& s)
    {
        // Pattern bytes as guest memory holds them.
        std::string wide;
        for (char16_t ch : s.text)
        {
            wide.push_back(char(ch >> 8));
            wide.push_back(char(ch & 0xFF));
        }
        size_t hits = 0;
        size_t armed = 1;
        // Guest addresses 0x00000000-0xFFFFFFFF; physical windows alias each other, so scan below 0xA0000000 only
        // plus the A0 window (the C0/E0 windows are the same bytes).
        for (uint64_t address = 0; address < 0xC0000000ull;)
        {
            MEMORY_BASIC_INFORMATION mbi{};
            uint8_t* host = guest::g_memory.Translate(uint32_t(address));
            if (!VirtualQuery(host, &mbi, sizeof(mbi)))
                break;
            const uint64_t regionEnd = address + (reinterpret_cast<uint8_t*>(mbi.BaseAddress) + mbi.RegionSize - host);
            const bool readable = mbi.State == MEM_COMMIT && !(mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD));
            if (readable)
            {
                const std::string_view region(reinterpret_cast<const char*>(host), size_t(regionEnd - address));
                for (const std::string* pattern : std::array<const std::string*, 2>{ &wide, &s.ascii })
                {
                    const bool isWide = pattern == &wide;
                    for (size_t at = region.find(*pattern); at != std::string_view::npos; at = region.find(*pattern, at + 1))
                    {
                        const uint32_t guestAt = uint32_t(address + at);
                        const uint32_t after = guestAt + uint32_t(pattern->size());
                        const bool terminated = isWide ? guest::g_memory.Read16(after) == 0 : guest::g_memory.Read8(after) == 0;
                        Log("Scan: \"{}\" as {} at {:08X}{}; before: {}; after: {}", s.ascii, isWide ? "UTF-16BE" : "ASCII",
                            guestAt, terminated ? "" : " (NOT TERMINATED)", HexBytes(guestAt - 96, 96), HexBytes(after, 48));
                        hits++;
                        if (s.watch && isWide && !terminated && armed < kSlots && (after & 1) == 0)
                            ArmEverywhere(armed++, after, 2, true);
                    }
                }
            }
            address = regionEnd;
        }
        Log("Scan: {} occurrence(s) of \"{}\" at frame {}", hits, s.ascii, s.frame);
    }
}

namespace kernel
{
    // Arms the watches on a host thread created suspended (called before it first runs).
    void ArmWatchpoint(void* hostThread)
    {
        if (!g_configured && !Scan().watch)
            return;
        InstallHandler();
        static const bool logged = [] {
            if (g_configured)
                Log("Watch: {} of {} byte(s) at guest {:08X}", g_watches[0].reads ? "reads and writes" : "writes",
                    g_watches[0].length, g_watches[0].guest);
            return true;
        }();
        (void)logged;
        std::lock_guard lock(g_armMutex);
        ApplyWatches(hostThread);
    }

    // Called by the command processor after each presented frame (CB_SCAN).
    void DiagnosticsFrame(uint64_t frame)
    {
        ProfilerFrame(frame);
        struct MemoryDump
        {
            uint64_t frame = 0;
            uint32_t start = 0, length = 0;
            std::string file;
        };
        static const MemoryDump dump = [] {
            MemoryDump d;
            const char* v = std::getenv("CB_MEMORY_DUMP");
            char file[512] = {};
            unsigned long long f = 0;
            unsigned start = 0, length = 0;
            if (v && std::sscanf(v, "%llu:%x:%x:%511[^\n]", &f, &start, &length, file) == 4)
            {
                d.frame = f;
                d.start = start;
                d.length = length;
                d.file = file;
            }
            else if (v && *v)
                Fatal("CB_MEMORY_DUMP={}: expected <frame>:<start hex>:<length hex>:<file>", v);
            return d;
        }();
        if (dump.frame != 0 && frame == dump.frame)
        {
            std::ofstream out(dump.file, std::ios::binary);
            for (uint32_t page = 0; page < dump.length; page += 0x1000)
            {
                const uint32_t n = std::min<uint32_t>(0x1000, dump.length - page);
                const uint8_t* host = guest::g_memory.Translate(dump.start + page);
                MEMORY_BASIC_INFORMATION mbi{};
                const bool readable = VirtualQuery(host, &mbi, sizeof(mbi)) && mbi.State == MEM_COMMIT &&
                                      !(mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD));
                static const std::array<char, 0x1000> zeros{};
                out.write(readable ? reinterpret_cast<const char*>(host) : zeros.data(), n);
            }
            Log("CB_MEMORY_DUMP: {:08X}+{:X} written to {} at frame {}", dump.start, dump.length, dump.file, frame);
        }
        const ScanConfig& s = Scan();
        if (s.frame != 0 && frame == s.frame)
            RunScan(s);
    }
}
