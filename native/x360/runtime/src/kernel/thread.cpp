#include "kernel/kernel.h"
#include "kernel/objects.h"
#include "modules.h"

#include <windows.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <format>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

namespace kernel
{
    namespace
    {
        thread_local GuestThread* t_currentThread = nullptr;
        std::mutex g_threadsMutex;
        std::vector<GuestThread*> g_threads;
        std::atomic<uint32_t> g_nextThreadId = 1;

        // The title process's TLS (SetTitleTls); system-process threads get kDefaultTlsSlotCount slots, no data.
        uint32_t g_tlsSlotCount = 64;
        uint32_t g_tlsDataSize = 0;
        uint32_t g_tlsRawAddress = 0;
        uint32_t g_tlsRawSize = 0;
        uint32_t g_processObject = 0;       // the title's KPROCESS
        uint32_t g_systemProcessObject = 0; // the system's KPROCESS

        bool EnvFlag(const char* name)
        {
            const char* v = std::getenv(name);
            return v && *v && *v != '0';
        }

        constexpr uint32_t kGuardPageSize = 0x1000;
        constexpr uint32_t kDefaultTlsSlotCount = 1024;

        // Binds a guest thread to the calling host thread and gives it a fresh register context.
        PPCContext* Attach(GuestThread* thread)
        {
            t_currentThread = thread;
            auto* ctx = static_cast<PPCContext*>(_aligned_malloc(sizeof(PPCContext), alignof(PPCContext)));
            new (ctx) PPCContext{};
            thread->context = ctx;

            // Default floating-point environment: round to nearest, all exceptions masked, no flush-to-zero.
            _mm_setcsr(0x1F80);
            ctx->fpscr.csr = ctx->fpscr.getcsr();

            ctx->r1.u64 = thread->stackBase;
            ctx->r13.u64 = thread->pcr;
            ctx->lr = 0;
            return ctx;
        }

        DWORD WINAPI HostThreadMain(void* parameter)
        {
            auto* thread = static_cast<GuestThread*>(parameter);
            PPCContext* ctx = Attach(thread);

            try
            {
                // The title's main thread under the console's XAM (XexStartExecutable): XAM's title start routine
                // runs first (no arguments), then the title's entry point.
                if (thread->params.preStart)
                {
                    const uint64_t stack = ctx->r1.u64;
                    CallGuest(*ctx, thread->params.preStart);
                    ctx->r1.u64 = stack;
                }
                if (thread->params.xapiThreadStartup)
                {
                    ctx->r3.u64 = thread->params.startAddress;
                    ctx->r4.u64 = thread->params.startContext;
                    CallGuest(*ctx, thread->params.xapiThreadStartup);
                }
                else
                {
                    ctx->r3.u64 = thread->params.startContext;
                    CallGuest(*ctx, thread->params.startAddress);
                }
                thread->exitCode = ctx->r3.u32;
                Log("Thread {} returned from its start routine (r3={:08X})", thread->id, thread->exitCode);
            }
            catch (const ThreadExit& exit)
            {
                thread->exitCode = exit.status;
                Log("Thread {} terminated with status {:08X}", thread->id, exit.status);
            }
            guest::g_memory.Write32(thread->threadObject + kthread::kExitStatus, thread->exitCode);
            {
                // A thread object is signaled once the thread has terminated (waits on its handle complete).
                auto lock = LockDispatcher();
                guest::g_memory.Write32(thread->threadObject + header::kSignalState, 1);
                NotifyDispatcherChange(thread->threadObject);
            }
            thread->finished = true;
            return 0;
        }
    }

    thread_local const char* t_currentImport = nullptr;
    thread_local uint32_t t_currentImportCaller = 0;

    const char* CurrentImport(uint32_t* caller)
    {
        if (caller)
            *caller = t_currentImportCaller;
        return t_currentImport ? t_currentImport : "(none)";
    }

    void TraceImport(const char* name, const PPCContext& ctx)
    {
        t_currentImport = name;
        t_currentImportCaller = uint32_t(ctx.lr);
        if (GuestThread* thread = t_currentThread)
        {
            if (thread->terminating.load(std::memory_order_relaxed))
                throw ThreadExit{ 0 };
            thread->lastImport.store(name, std::memory_order_relaxed);
            thread->lastImportCaller.store(uint32_t(ctx.lr), std::memory_order_relaxed);
            thread->lastImportArgument.store(ctx.r3.u32, std::memory_order_relaxed);
        }
        // CB_TRACE_IMPORTS=1 logs every import; any other value is a comma-separated list of name substrings to log
        // ("-Name" entries exclude; a list of only exclusions logs everything else).
        static const bool enabled = EnvFlag("CB_TRACE_IMPORTS");
        static const auto filters = []
            {
                std::pair<std::vector<std::string>, std::vector<std::string>> lists; // include, exclude
                const char* v = std::getenv("CB_TRACE_IMPORTS");
                if (!v || std::string_view(v) == "1")
                    return lists;
                for (std::string_view rest = v; !rest.empty();)
                {
                    const size_t comma = rest.find(',');
                    const std::string_view item = rest.substr(0, comma);
                    if (!item.empty() && item[0] == '-')
                        lists.second.emplace_back(item.substr(1));
                    else if (!item.empty())
                        lists.first.emplace_back(item);
                    rest = comma == std::string_view::npos ? std::string_view() : rest.substr(comma + 1);
                }
                return lists;
            }();
        if (!enabled)
            return;
        auto matches = [name](const std::vector<std::string>& list) {
            return std::any_of(list.begin(), list.end(),
                [name](const std::string& f) { return std::string_view(name).find(f) != std::string_view::npos; });
        };
        if ((!filters.first.empty() && !matches(filters.first)) || matches(filters.second))
            return;
        static const bool withStack = EnvFlag("CB_TRACE_STACK");
        Log("[{}] {}(r3={:08X} r4={:08X} r5={:08X} r6={:08X}) lr={:08X}{}",
            CurrentThread() ? CurrentThread()->id : 0, name, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, uint32_t(ctx.lr),
            withStack ? " stack " + GuestBacktrace(ctx) : std::string());
    }

    namespace
    {
        // Every non-leaf function of this compiler saves LR at -8 of its incoming stack pointer before its stwu, so
        // the back chain at [r1] leads to each caller's return address. The first entry is LR itself.
        std::string Backtrace(const GuestThread* thread, uint32_t r1, uint32_t lr)
        {
            auto& mem = guest::g_memory;
            std::string text = std::format("{:08X}", lr);
            if (thread == nullptr)
                return text;
            uint32_t frame = r1;
            for (int depth = 0; depth < 24; depth++)
            {
                if (frame < thread->stackLimit || frame + 4 > thread->stackBase)
                    break;
                const uint32_t next = mem.Read32(frame);
                if (next <= frame || next > thread->stackBase || next - 8 < thread->stackLimit)
                    break;
                text += std::format(" {:08X}", mem.Read32(next - 8));
                frame = next;
            }
            return text;
        }
    }

    std::string GuestBacktrace(const PPCContext& ctx)
    {
        return Backtrace(CurrentThread(), ctx.r1.u32, uint32_t(ctx.lr));
    }

    void DumpThreads(const std::string& reason)
    {
        Log("Thread dump ({}):", reason);
        ForEachThreadImpl([](GuestThread* thread) {
            if (!thread->started || thread->finished)
                return;
            const char* import = thread->lastImport.load(std::memory_order_relaxed);
            const PPCContext* c = thread->context;
            Log("  thread {} (start {:08X}): last import {}(r3={:08X}) from {:08X}; stack {}", thread->id,
                thread->params.startAddress, import ? import : "(none)", thread->lastImportArgument.load(),
                thread->lastImportCaller.load(), c ? Backtrace(thread, c->r1.u32, uint32_t(c->lr)) : std::string("?"));
        });
    }

    void UnimplementedImport(const char* name, PPCContext& ctx)
    {
        // An import from dash.xex's own export table ("dash_Ordinal_<n>": the dash's plugins, Dash.Search.lex and the
        // other LIVE-delivered .lex modules, call the dash's exported functions): the dash's recompiled code.
        if (std::strncmp(name, "dash_Ordinal_", 13) == 0)
            if (const modules::LoadedModule* dash = modules::FindByName("dash.xex"))
                if (const uint32_t address = modules::ExportAddress(*dash, uint16_t(std::atoi(name + 13))))
                {
                    CallGuest(ctx, address);
                    return;
                }
        Log("UNIMPLEMENTED IMPORT {}(r3={:08X} r4={:08X} r5={:08X} r6={:08X} r7={:08X} r8={:08X}) called from {:08X} on thread {} (stack {})",
            name, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, uint32_t(ctx.lr),
            CurrentThread() ? CurrentThread()->id : 0, GuestBacktrace(ctx));
        static const bool keepGoing = EnvFlag("CB_CONTINUE_UNIMPLEMENTED");
        if (!keepGoing)
            Fatal("Stopping at unimplemented import {} (set CB_CONTINUE_UNIMPLEMENTED=1 to return 0 and continue)", name);
        ctx.r3.u64 = 0;
    }

    void CallGuest(PPCContext& ctx, uint32_t address)
    {
        uint8_t* base = guest::g_memory.base;
        if (!modules::FindByCode(address) && (address < kDynamicExportStart || address >= kDynamicExportEnd))
            Fatal("Guest call to {:08X} is outside every recompiled module", address);
        PPCFunc* func = PPC_LOOKUP_FUNC(base, address);
        if (func == nullptr)
            Fatal("Guest call to {:08X}: no recompiled function starts there", address);
        func(ctx, base);
    }

    void SetCurrentCpu(GuestThread* thread, uint8_t cpu)
    {
        auto& mem = guest::g_memory;
        mem.Write8(thread->pcr + pcr::kCurrentCpu, cpu);
        mem.Write8(thread->threadObject + kthread::kCurrentCpu, cpu);
    }

    uint8_t CurrentCpu(const GuestThread* thread)
    {
        return guest::g_memory.Read8(thread->pcr + pcr::kCurrentCpu);
    }

    GuestThread* CurrentThread()
    {
        return t_currentThread;
    }

    GuestThread* FindThread(uint32_t threadObject)
    {
        std::lock_guard lock(g_threadsMutex);
        for (GuestThread* t : g_threads)
            if (t->threadObject == threadObject)
                return t;
        return nullptr;
    }

    void ForEachThreadImpl(const std::function<void(GuestThread*)>& f)
    {
        std::lock_guard lock(g_threadsMutex);
        for (GuestThread* t : g_threads)
            f(t);
    }

    uint32_t TlsSlotCount()
    {
        return g_tlsSlotCount;
    }

    void InitializeThreading(uint32_t tlsSlotCount, uint32_t tlsDataSize, uint32_t tlsRawAddress, uint32_t tlsRawSize,
        uint32_t titleProcess, uint32_t systemProcess)
    {
        g_tlsSlotCount = tlsSlotCount ? tlsSlotCount : kDefaultTlsSlotCount;
        g_tlsDataSize = tlsDataSize;
        g_tlsRawAddress = tlsRawAddress;
        g_tlsRawSize = tlsRawSize;
        g_processObject = titleProcess;
        g_systemProcessObject = systemProcess;
    }

    void SetTitleTls(uint32_t tlsSlotCount, uint32_t tlsDataSize, uint32_t tlsRawAddress, uint32_t tlsRawSize)
    {
        g_tlsSlotCount = tlsSlotCount ? tlsSlotCount : kDefaultTlsSlotCount;
        g_tlsDataSize = tlsDataSize;
        g_tlsRawAddress = tlsRawAddress;
        g_tlsRawSize = tlsRawSize;
    }

    GuestThread* CreateThread(const ThreadParams& params)
    {
        auto& mem = guest::g_memory;
        auto* thread = new GuestThread;
        thread->params = params;
        thread->id = g_nextThreadId++;

        // Stack: lowest page is a no-access guard so overflows fault instead of corrupting memory.
        const uint32_t stackSize = (std::max<uint32_t>(params.stackSize, 0x4000) + 0xFFF) & ~0xFFFu;
        thread->stackAllocation = mem.stacks.Allocate(stackSize + kGuardPageSize, 0x1000, true, guest::kPageReadWrite, true);
        if (thread->stackAllocation == 0)
            Fatal("Out of guest stack space creating thread {}", thread->id);
        mem.stacks.Decommit(thread->stackAllocation, kGuardPageSize);
        thread->stackLimit = thread->stackAllocation + kGuardPageSize;
        thread->stackBase = thread->stackLimit + stackSize;

        thread->pcr = mem.SystemAlloc(pcr::kSize);
        thread->threadObject = mem.SystemAlloc(kthread::kSize);

        const uint32_t t = thread->threadObject;
        const uint32_t prcb = thread->pcr + pcr::kPrcb;
        mem.Write32(t + kthread::kMutantList, t + kthread::kMutantList); // empty list: flink = blink = self
        mem.Write32(t + kthread::kMutantList + 4, t + kthread::kMutantList);
        mem.Write32(t + kthread::kStackBase, thread->stackBase);
        mem.Write32(t + kthread::kStackLimit, thread->stackLimit);
        mem.Write32(t + kthread::kStackKernel, thread->stackBase - 240);
        uint8_t processType = params.processType;
        if (processType == 0)
            processType = (params.creationFlags & 0x2) ? kProcessTypeSystem
                : t_currentThread ? mem.Read8(t_currentThread->threadObject + kthread::kProcessType)
                : kProcessTypeTitle;
        thread->params.processType = processType;

        // TLS: [declspec(thread) data][slots]; PCR.TlsPointer -> data, KTHREAD.TlsAddress -> slots. Per process: the
        // title's from its XEX header, the system process's (bootanim, XAM) the default slots without data.
        const bool titleTls = processType != kProcessTypeSystem;
        const uint32_t tlsDataSize = titleTls ? g_tlsDataSize : 0;
        thread->tlsStatic = mem.SystemAlloc(tlsDataSize + (titleTls ? g_tlsSlotCount : kDefaultTlsSlotCount) * 4);
        thread->tlsSlots = thread->tlsStatic + tlsDataSize;
        if (titleTls && g_tlsRawSize)
            memcpy(mem.Translate(thread->tlsStatic), mem.Translate(g_tlsRawAddress), g_tlsRawSize);
        mem.Write32(t + kthread::kTlsAddress, thread->tlsSlots);

        mem.Write8(t + 0, kObjectThread);
        mem.Write32(t + kthread::kProcess, processType == kProcessTypeSystem ? g_systemProcessObject : g_processObject);
        mem.Write8(t + kthread::kProcessType, processType);
        mem.Write8(t + kthread::kProcessTypeCopy, processType);
        mem.Write32(t + kthread::kMsrMask, 0xFDFFD7FF);
        mem.Write32(t + kthread::kPrcbPointer, prcb);
        mem.Write32(t + kthread::kPrcbPointer2, prcb);
        mem.Write32(t + kthread::kStackAllocBase, thread->stackBase);
        mem.Write32(t + kthread::kThreadId, thread->id);
        mem.Write32(t + kthread::kStartAddress, params.startAddress);
        mem.Write32(t + kthread::kCreationFlags, params.creationFlags);
        mem.Write8(t + kthread::kSuspendCount, (params.creationFlags & 1) ? 1 : 0); // CREATE_SUSPENDED

        const uint32_t p = thread->pcr;
        uint8_t cpu = 0;
        if (const uint32_t mask = (params.creationFlags >> 24) & 0x3F)
            cpu = uint8_t(__builtin_ctz(mask));
        else if (t_currentThread)
            cpu = CurrentCpu(t_currentThread);
        mem.Write8(p + pcr::kCurrentCpu, cpu);
        mem.Write8(t + kthread::kCurrentCpu, cpu);
        mem.Write32(p + pcr::kTlsPointer, thread->tlsStatic);
        mem.Write64(p + pcr::kSelfPointer, p);
        mem.Write32(p + pcr::kStackBase, thread->stackBase);
        mem.Write32(p + pcr::kStackEnd, thread->stackLimit);
        mem.Write32(p + pcr::kCurrentThread, t);
        mem.Write32(p + pcr::kPrcbPointer, prcb);

        {
            std::lock_guard lock(g_threadsMutex);
            g_threads.push_back(thread);
        }
        Log("Created thread {}: start {:08X} ctx {:08X} stack {:08X}-{:08X} pcr {:08X} kthread {:08X}{}",
            thread->id, params.startAddress, params.startContext, thread->stackLimit, thread->stackBase, p, t,
            processType == kProcessTypeSystem ? " (system process)" : "");
        return thread;
    }

    uint8_t CurrentProcessType()
    {
        return t_currentThread ? guest::g_memory.Read8(t_currentThread->threadObject + kthread::kProcessType)
                               : kProcessTypeTitle;
    }

    void CallGuestAtDispatchLevel(GuestThread* thread, PPCContext& ctx, uint32_t address,
        std::initializer_list<uint32_t> args, uint32_t cpu, uint8_t processType)
    {
        auto& mem = guest::g_memory;
        const uint32_t p = thread->pcr;
        const uint8_t oldIrql = mem.Read8(p + pcr::kCurrentIrql);
        const uint32_t oldDpcActive = mem.Read32(p + pcr::kDpcActive);
        const uint8_t oldDpcType = mem.Read8(p + pcr::kDpcProcessType);
        const uint8_t oldCpu = mem.Read8(p + pcr::kCurrentCpu);
        mem.Write8(p + pcr::kCurrentIrql, 2); // DISPATCH_LEVEL
        mem.Write32(p + pcr::kDpcActive, 1);
        mem.Write8(p + pcr::kDpcProcessType, processType);
        mem.Write8(p + pcr::kCurrentCpu, uint8_t(cpu));

        ctx.r1.u64 = thread->stackBase;
        PPCRegister* registers[] = { &ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6, &ctx.r7, &ctx.r8, &ctx.r9, &ctx.r10 };
        size_t i = 0;
        for (uint32_t arg : args)
            registers[i++]->u64 = arg;
        CallGuest(ctx, address);

        mem.Write8(p + pcr::kCurrentIrql, oldIrql);
        mem.Write32(p + pcr::kDpcActive, oldDpcActive);
        mem.Write8(p + pcr::kDpcProcessType, oldDpcType);
        mem.Write8(p + pcr::kCurrentCpu, oldCpu);
    }

    void StartHostThread(std::function<void()> body)
    {
        auto* heapBody = new std::function<void()>(std::move(body));
        auto entry = [](void* parameter) -> DWORD
            {
                std::unique_ptr<std::function<void()>> f(static_cast<std::function<void()>*>(parameter));
                (*f)();
                return 0;
            };
        HANDLE handle = ::CreateThread(nullptr, 16 * 1024 * 1024, entry, heapBody, STACK_SIZE_PARAM_IS_A_RESERVATION, nullptr);
        if (handle == nullptr)
            Fatal("Unable to create a runtime host thread (error {})", GetLastError());
        ::CloseHandle(handle);
    }

    PPCContext* AttachHostThread(GuestThread* thread)
    {
        HANDLE self = nullptr;
        if (!DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &self, 0, FALSE,
                DUPLICATE_SAME_ACCESS))
            Fatal("AttachHostThread: DuplicateHandle failed ({})", GetLastError());
        thread->hostHandle = self;
        thread->started = true;
        return Attach(thread);
    }

    void StartThread(GuestThread* thread)
    {
        if (thread->started.exchange(true))
            return;
        // Recompiled code nests one host call per guest call, so give it a large host stack.
        constexpr SIZE_T kHostStackSize = 16 * 1024 * 1024;
        HANDLE handle = ::CreateThread(nullptr, kHostStackSize, HostThreadMain, thread,
            STACK_SIZE_PARAM_IS_A_RESERVATION | CREATE_SUSPENDED, nullptr);
        if (handle == nullptr)
            Fatal("Unable to create host thread (error {})", GetLastError());
        thread->hostHandle = handle;
        ArmWatchpoint(handle);
        ::ResumeThread(handle);
    }
}
