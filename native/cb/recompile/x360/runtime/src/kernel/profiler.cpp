// CB_PROFILE=<first frame>-<last frame>: a sampling profiler for the guest threads (diagnostics).
//
// Between the two presented frames a host thread wakes about every millisecond, suspends each running guest thread
// in turn, reads its host context and unwinds its host stack (x64 unwind data), then resumes it. Nothing is allocated
// while a thread is suspended (it may hold the heap lock). A frame inside a recompiled function is attributed to its
// guest address through the function table; other frames are runtime or system code, named from the runtime's PDB at
// report time. At the last frame the report goes to the log:
//   - per thread: samples and the share spent waiting (system wait functions on top);
//   - "self": where the top of the stack was (guest function, or host function plus its nearest guest caller);
//   - "inclusive": guest functions anywhere on the stack (counted once per sample).
// Guest addresses can be named with analysis/ghidra_functions.csv.
// CB_PROFILE_HOST=1 also samples the runtime's own host threads (GPU, audio, presenter, XUI, workers...): each is
// reported as "host <id>" and named after the deepest runtime function on its stack (its thread function).

#include "kernel/kernel.h"

#include "modules.h"

#include <windows.h>
#include <dbghelp.h>
#include <tlhelp32.h>
#include <timeapi.h>

#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <format>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#pragma comment(lib, "dbghelp.lib")

namespace
{
    constexpr int kMaxFrames = 48;

    struct Config
    {
        uint64_t first = 0, last = 0;
    };

    const Config& Settings()
    {
        static const Config config = [] {
            Config c;
            if (const char* s = std::getenv("CB_PROFILE"); s && *s)
            {
                char* end = nullptr;
                c.first = std::strtoull(s, &end, 10);
                if (*end == '-')
                    c.last = std::strtoull(end + 1, &end, 10);
                if (*end != 0 || c.first == 0 || c.last <= c.first)
                    Fatal("CB_PROFILE must look like <first frame>-<last frame>, got \"{}\"", s);
            }
            return c;
        }();
        return config;
    }

    // Host start address of each recompiled function -> guest address.
    const std::unordered_map<uint64_t, uint32_t>& HostToGuest()
    {
        static const std::unordered_map<uint64_t, uint32_t> map = [] {
            std::unordered_map<uint64_t, uint32_t> m;
            for (const modules::LoadedModule* module : modules::All())
                for (const PPCFuncMapping* f = module->recompiled->mappings; f->host != nullptr; f++)
                    m.emplace(reinterpret_cast<uint64_t>(f->host), uint32_t(f->guest));
            return m;
        }();
        return map;
    }

    // The start of the host function containing `pc` (following chained unwind data to the primary entry), or 0.
    uint64_t FunctionStart(uint64_t pc)
    {
        DWORD64 imageBase = 0;
        PRUNTIME_FUNCTION entry = RtlLookupFunctionEntry(pc, &imageBase, nullptr);
        if (!entry)
            return 0;
        for (int depth = 0; depth < 8; depth++)
        {
            const uint8_t* info = reinterpret_cast<const uint8_t*>(imageBase + entry->UnwindData);
            if (!(info[0] >> 3 & UNW_FLAG_CHAININFO))
                break;
            const uint8_t codes = info[2];
            entry = reinterpret_cast<PRUNTIME_FUNCTION>(const_cast<uint8_t*>(info) + 4 + ((codes + 1) & ~1) * 2);
        }
        return imageBase + entry->BeginAddress;
    }

    constexpr uint32_t kHostThreadFlag = 0x80000000; // Sample::thread of a host thread: flag | Windows thread id

    struct Sample
    {
        uint32_t thread = 0;
        int count = 0;
        uint64_t pcs[kMaxFrames];
    };

    struct Profile
    {
        std::map<uint32_t, uint64_t> threadSamples, threadWaiting;
        // (thread, top host function or 0, first runtime function on the stack or 0, nearest guest function) -> samples
        std::map<std::tuple<uint32_t, uint64_t, uint64_t, uint32_t>, uint64_t> self;
        std::map<uint32_t, uint64_t> inclusive;
        std::map<uint32_t, uint64_t> hostRoot; // host thread key -> deepest runtime function seen
        uint64_t samples = 0;
    };

    std::atomic<bool> g_running = false;
    std::mutex g_mutex;
    Profile g_profile;
    std::thread g_sampler;

    // Walks the stack of a suspended thread; no allocation.
    void Walk(HANDLE thread, Sample& sample)
    {
        CONTEXT context{};
        context.ContextFlags = CONTEXT_FULL;
        if (!GetThreadContext(thread, &context))
            return;
        while (sample.count < kMaxFrames && context.Rip != 0)
        {
            sample.pcs[sample.count++] = context.Rip;
            DWORD64 imageBase = 0;
            PRUNTIME_FUNCTION entry = RtlLookupFunctionEntry(context.Rip, &imageBase, nullptr);
            if (!entry)
            {
                // Leaf function without unwind data: the return address is on top of the stack.
                context.Rip = *reinterpret_cast<DWORD64*>(context.Rsp);
                context.Rsp += 8;
                continue;
            }
            void* handlerData = nullptr;
            DWORD64 establisher = 0;
            RtlVirtualUnwind(UNW_FLAG_NHANDLER, imageBase, context.Rip, entry, &context, &handlerData, &establisher,
                nullptr);
        }
    }

    std::string HostName(uint64_t address)
    {
        alignas(SYMBOL_INFO) char buffer[sizeof(SYMBOL_INFO) + 256] = {};
        auto* symbol = reinterpret_cast<SYMBOL_INFO*>(buffer);
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = 255;
        DWORD64 displacement = 0;
        if (SymFromAddr(GetCurrentProcess(), address, &displacement, symbol))
            return symbol->Name;
        HMODULE module = nullptr;
        char path[MAX_PATH] = {};
        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCSTR>(address), &module) && GetModuleFileNameA(module, path, MAX_PATH))
        {
            const std::string file(path);
            return std::format("{}+{:X}", file.substr(file.find_last_of("\\/") + 1),
                address - reinterpret_cast<uint64_t>(module));
        }
        return std::format("{:016X}", address);
    }

    // Whether `address` lies in the runtime executable (not a system DLL).
    bool InRuntime(uint64_t address)
    {
        static const auto [begin, end] = [] {
            const auto base = reinterpret_cast<uint64_t>(GetModuleHandleW(nullptr));
            const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
            const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
            return std::pair{ base, base + nt->OptionalHeader.SizeOfImage };
        }();
        return address >= begin && address < end;
    }

    bool IsWait(const std::string& name)
    {
        return name.starts_with("NtWaitFor") || name.starts_with("ZwWaitFor") || name.starts_with("NtDelayExecution") ||
            name.starts_with("ZwDelayExecution") || name.starts_with("NtRemoveIoCompletion") ||
            name.starts_with("NtSignalAndWait") || name.starts_with("ZwSignalAndWait") || name == "SwitchToThread" ||
            name.starts_with("NtUserMsgWaitForMultipleObjects") || name.starts_with("ZwRemoveIoCompletion") ||
            name.starts_with("NtWaitForWorkViaWorkerFactory") || name.starts_with("ZwWaitForWorkViaWorkerFactory") ||
            name.starts_with("NtYieldExecution") || name.starts_with("ZwYieldExecution");
    }

    void Record(const Sample& sample, std::unordered_map<uint64_t, std::string>& names)
    {
        const auto& map = HostToGuest();
        auto name = [&](uint64_t start) -> const std::string& {
            auto it = names.find(start);
            if (it == names.end())
                it = names.emplace(start, HostName(start)).first;
            return it->second;
        };
        std::lock_guard lock(g_mutex);
        Profile& p = g_profile;
        p.samples++;
        p.threadSamples[sample.thread]++;
        uint64_t topHost = 0, runtimeHost = 0;
        uint32_t topGuest = 0;
        std::unordered_set<uint32_t> seen;
        for (int i = 0; i < sample.count; i++)
        {
            const uint64_t start = FunctionStart(sample.pcs[i]);
            const auto it = map.find(start);
            if (it == map.end())
            {
                if (i == 0)
                {
                    topHost = start ? start : sample.pcs[i];
                    if (IsWait(name(topHost)))
                        p.threadWaiting[sample.thread]++;
                }
                if (runtimeHost == 0 && topGuest == 0 && start && InRuntime(start))
                    runtimeHost = start;
                continue;
            }
            if (topGuest == 0)
                topGuest = it->second;
            if (seen.insert(it->second).second)
                p.inclusive[it->second]++;
        }
        p.self[{ sample.thread, topHost, runtimeHost, topGuest }]++;
        if (sample.thread & kHostThreadFlag)
            for (int i = sample.count - 1; i >= 0; i--)
                if (const uint64_t start = FunctionStart(sample.pcs[i]); start && InRuntime(start))
                {
                    p.hostRoot[sample.thread] = start;
                    break;
                }
    }

    void Report(const std::unordered_map<uint64_t, std::string>& names)
    {
        std::lock_guard lock(g_mutex);
        const Profile& p = g_profile;
        Log("Profile: {} samples over frames {}-{}", p.samples, Settings().first, Settings().last);
        for (const auto& [thread, count] : p.threadSamples)
        {
            const uint64_t waiting = p.threadWaiting.count(thread) ? p.threadWaiting.at(thread) : 0;
            if (thread & kHostThreadFlag)
            {
                const uint64_t root = p.hostRoot.count(thread) ? p.hostRoot.at(thread) : 0;
                Log("Profile: host {} ({}): {} samples, {:.1f}% waiting", thread & ~kHostThreadFlag,
                    root ? HostName(root) : "no runtime frame", count, 100.0 * waiting / count);
                continue;
            }
            const kernel::GuestThread* t = nullptr;
            kernel::ForEachThread([&](kernel::GuestThread* g) { if (g->id == thread) t = g; });
            Log("Profile: thread {} (start {:08X}, host {}): {} samples, {:.1f}% waiting", thread,
                t ? t->params.startAddress : 0, t && t->hostHandle ? GetThreadId(t->hostHandle) : 0, count,
                100.0 * waiting / count);
        }
        auto top = [](const auto& counts, size_t n) {
            std::vector<std::pair<uint64_t, typename std::decay_t<decltype(counts)>::key_type>> v;
            for (const auto& [key, count] : counts)
                v.emplace_back(count, key);
            std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return a.first > b.first; });
            if (v.size() > n)
                v.resize(n);
            return v;
        };
        auto hostName = [&](uint64_t host) { return names.count(host) ? names.at(host) : HostName(host); };
        auto where = [&](uint64_t host, uint64_t runtime, uint32_t guest) {
            if (host == 0)
                return std::format("guest {:08X}", guest);
            std::string text = hostName(host);
            if (runtime && runtime != host)
                text += " in " + hostName(runtime);
            return text + std::format(" (guest caller {:08X})", guest);
        };
        std::map<std::tuple<uint64_t, uint64_t, uint32_t>, uint64_t> all;
        for (const auto& [key, count] : p.self)
            all[{ std::get<1>(key), std::get<2>(key), std::get<3>(key) }] += count;
        Log("Profile: self (top of stack, all threads):");
        for (const auto& [count, key] : top(all, 40))
            Log("Profile:   {:5.1f}% {}", 100.0 * count / p.samples,
                where(std::get<0>(key), std::get<1>(key), std::get<2>(key)));
        for (const auto& [thread, total] : p.threadSamples)
        {
            std::map<std::tuple<uint64_t, uint64_t, uint32_t>, uint64_t> mine;
            for (const auto& [key, count] : p.self)
                if (std::get<0>(key) == thread)
                    mine[{ std::get<1>(key), std::get<2>(key), std::get<3>(key) }] += count;
            if (thread & kHostThreadFlag)
                Log("Profile: host {}:", thread & ~kHostThreadFlag);
            else
                Log("Profile: thread {}:", thread);
            for (const auto& [count, key] : top(mine, 8))
                Log("Profile:   {:5.1f}% {}", 100.0 * count / total,
                    where(std::get<0>(key), std::get<1>(key), std::get<2>(key)));
        }
        Log("Profile: inclusive (guest functions on the stack):");
        for (const auto& [count, guest] : top(p.inclusive, 60))
            Log("Profile:   {:5.1f}% guest {:08X}", 100.0 * count / p.samples, guest);
    }

    void SamplerLoop()
    {
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
        std::unordered_map<uint64_t, std::string> names;
        std::vector<std::pair<uint32_t, HANDLE>> threads;
        std::vector<Sample> samples;
        const char* hostOption = std::getenv("CB_PROFILE_HOST");
        const bool host = hostOption && hostOption[0] == '1';
        std::map<DWORD, HANDLE> hostThreads; // Windows thread id -> handle (refreshed every ~100 ms)
        uint32_t iteration = 0;
        timeBeginPeriod(1);
        while (g_running)
        {
            threads.clear();
            std::unordered_set<DWORD> guestIds;
            kernel::ForEachThread([&](kernel::GuestThread* t) {
                if (t->hostHandle && t->started && !t->finished)
                {
                    threads.emplace_back(t->id, t->hostHandle);
                    guestIds.insert(GetThreadId(t->hostHandle));
                }
            });
            if (host)
            {
                if (iteration++ % 100 == 0)
                {
                    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
                    THREADENTRY32 entry{ sizeof(entry) };
                    std::unordered_set<DWORD> alive;
                    for (BOOL ok = Thread32First(snapshot, &entry); ok; ok = Thread32Next(snapshot, &entry))
                    {
                        if (entry.th32OwnerProcessID != GetCurrentProcessId() || entry.th32ThreadID == GetCurrentThreadId())
                            continue;
                        alive.insert(entry.th32ThreadID);
                        if (!hostThreads.count(entry.th32ThreadID))
                            if (HANDLE h = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION,
                                    FALSE, entry.th32ThreadID))
                                hostThreads[entry.th32ThreadID] = h;
                    }
                    CloseHandle(snapshot);
                    for (auto it = hostThreads.begin(); it != hostThreads.end();)
                        if (!alive.count(it->first))
                        {
                            CloseHandle(it->second);
                            it = hostThreads.erase(it);
                        }
                        else
                            ++it;
                }
                for (const auto& [id, handle] : hostThreads)
                    if (!guestIds.count(id))
                        threads.emplace_back(kHostThreadFlag | id, handle);
            }
            samples.assign(threads.size(), Sample{});
            for (size_t i = 0; i < threads.size(); i++)
            {
                samples[i].thread = threads[i].first;
                if (SuspendThread(threads[i].second) == DWORD(-1))
                    continue;
                Walk(threads[i].second, samples[i]);
                ResumeThread(threads[i].second);
            }
            for (const Sample& s : samples)
                if (s.count)
                    Record(s, names);
            Sleep(1);
        }
        timeEndPeriod(1);
        for (const auto& [id, handle] : hostThreads)
            CloseHandle(handle);
        Report(names);
    }
}

namespace kernel
{
    void ProfilerFrame(uint64_t frame)
    {
        const Config& c = Settings();
        if (c.first == 0)
            return;
        if (frame == c.first)
        {
            SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
            SymInitialize(GetCurrentProcess(), nullptr, TRUE);
            HostToGuest();
            Log("Profile: sampling from frame {}", frame);
            g_running = true;
            g_sampler = std::thread(SamplerLoop);
        }
        else if (frame == c.last && g_running)
        {
            g_running = false;
            g_sampler.join();
        }
    }
}
