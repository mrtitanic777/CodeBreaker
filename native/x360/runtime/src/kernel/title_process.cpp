// The title process's end (ExTerminateTitleProcess), for the console's own XAM: it ends the running title (the
// dashboard after a sign-in, a game when another is launched) and its loader thread then loads the next executable
// (XexLoadExecutable / XexStartExecutable; MmResetLowestAvailablePages first).
//
// As the console's kernel does: every thread of the title process ends (here: at its next kernel call, or at once
// when it is blocked in a wait: kernel::ThreadExit unwinds it to its entry), the registered title terminate
// notifications run (EX_TITLE_TERMINATE_REGISTRATION: +0 routine(Registration)), the memory the title's threads
// allocated (virtual reservations, physical allocations) is freed and its executable unloaded (XexLoadExecutable loads
// it again from its file, with fresh data). Called on a title thread, the end runs on a system thread of its own and
// the caller ends too.

#include "gpu/gpu.h"
#include "kernel/kernel.h"
#include "kernel/objects.h"
#include "memory.h"
#include "modules.h"

#include <chrono>
#include <map>
#include <mutex>
#include <set>
#include <thread>

namespace
{
    std::mutex g_mutex;
    std::set<uint32_t> g_virtual, g_physical;   // the title process's allocations
    struct Unloaded
    {
        std::string path;
        const modules::RecompiledModule* recompiled = nullptr;
    };
    std::map<std::string, Unloaded> g_unloaded; // executables a termination unloaded, by lower-case file name

    std::string Lower(std::string s)
    {
        for (char& c : s)
            c = char(std::tolower(static_cast<unsigned char>(c)));
        if (const size_t slash = s.find_last_of("\\/:"); slash != std::string::npos)
            s = s.substr(slash + 1);
        return s;
    }

    void EndTitle(PPCContext& ctx)
    {
        const auto started = std::chrono::steady_clock::now();
        kernel::GuestThread* self = kernel::CurrentThread();
        std::vector<kernel::GuestThread*> title;
        kernel::ForEachThread([&](kernel::GuestThread* t) {
            if (t != self && t->params.processType == kernel::kProcessTypeTitle && t->started && !t->finished)
                title.push_back(t);
        });
        for (kernel::GuestThread* t : title)
            kernel::WakeForTermination(t);
        // Every thread at a kernel call ends at once; one that never makes one is reported, not waited on forever.
        for (int i = 0; i < 1000; i++)
        {
            bool all = true;
            for (kernel::GuestThread* t : title)
                all &= t->finished.load();
            if (all)
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        for (kernel::GuestThread* t : title)
            if (!t->finished)
                Log("ExTerminateTitleProcess: title thread {} has not ended (last kernel call {})", t->id,
                    t->lastImport.load() ? t->lastImport.load() : "none");
        for (uint32_t registration : kernel::TitleTerminateRegistrations())
            if (const uint32_t routine = guest::g_memory.Read32(registration))
            {
                PPCContext call = ctx;
                call.r3.u64 = registration;
                kernel::CallGuest(call, routine);
            }
        auto& mem = guest::g_memory;
        size_t freedVirtual = 0, freedPhysical = 0;
        {
            std::lock_guard lock(g_mutex);
            for (uint32_t base : g_virtual)
                if (guest::PageHeap* heap = mem.LookupVirtualHeap(base); heap && heap->Release(base))
                    freedVirtual++;
            for (uint32_t window : g_physical)
                if (mem.physical.Free(guest::WindowToPhysical(window)))
                    freedPhysical++;
            g_virtual.clear();
            g_physical.clear();
        }
        if (const modules::LoadedModule* executable = modules::FindByHandle(kernel::ExecutableModuleHandle()))
        {
            const std::string name = executable->recompiled->name;
            {
                std::lock_guard lock(g_mutex);
                g_unloaded[Lower(name)] = { executable->path, executable->recompiled };
            }
            const uint32_t start = executable->recompiled->codeBase, end = start + executable->recompiled->codeSize;
            gpu::ClearInterruptCallbackIn(start, end);
            kernel::UnregisterAudioClientsIn(start, end);
            modules::Unload(*executable);
        }
        Log("ExTerminateTitleProcess: {} title threads ended, {} reservations and {} physical allocations freed, "
            "the executable unloaded ({:.0f} ms)", title.size(), freedVirtual, freedPhysical,
            std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - started).count());
    }
}

namespace kernel
{
    void TrackTitleVirtual(uint32_t base) { std::lock_guard lock(g_mutex); g_virtual.insert(base); }
    void UntrackTitleVirtual(uint32_t base) { std::lock_guard lock(g_mutex); g_virtual.erase(base); }
    void TrackTitlePhysical(uint32_t window) { std::lock_guard lock(g_mutex); g_physical.insert(window); }
    void UntrackTitlePhysical(uint32_t window) { std::lock_guard lock(g_mutex); g_physical.erase(window); }

    const modules::LoadedModule* ReloadTerminatedExecutable(const std::string& name)
    {
        Unloaded u;
        {
            std::lock_guard lock(g_mutex);
            auto it = g_unloaded.find(Lower(name));
            if (it == g_unloaded.end())
                return nullptr;
            u = it->second;
            g_unloaded.erase(it);
        }
        const modules::LoadedModule& loaded = modules::Load(u.path, *u.recompiled);
        Log("XexLoadExecutable(\"{}\"): {} loaded again", name, u.recompiled->name);
        return &loaded;
    }
}

// VOID ExTerminateTitleProcess(DWORD ExitCode): ends the title process (see above). On a title thread it does not
// return.
GUEST_IMPORT(ExTerminateTitleProcess)
{
    kernel::GuestThread* self = kernel::CurrentThread();
    Log("ExTerminateTitleProcess({:08X}) from {:08X}", ctx.r3.u32, uint32_t(ctx.lr));
    if (self && self->params.processType == kernel::kProcessTypeTitle)
    {
        kernel::StartHostThread([] {
            kernel::ThreadParams params;
            params.stackSize = 0x10000;
            params.processType = kernel::kProcessTypeSystem;
            kernel::GuestThread* thread = kernel::CreateThread(params);
            PPCContext* c = kernel::AttachHostThread(thread);
            c->r1.u64 = thread->stackBase;
            EndTitle(*c);
        });
        throw kernel::ThreadExit{ ctx.r3.u32 };
    }
    EndTitle(ctx);
}

// VOID MmResetLowestAvailablePages(PDWORD, PDWORD, PDWORD, PDWORD): XAM's loader before it loads the next title
// (0x816A00E8): the title's lowest-page limits. The runtime keeps none: the four values are left as they are.
GUEST_IMPORT(MmResetLowestAvailablePages)
{
}

// VOID KePulseEvent(PKEVENT Event, KPRIORITY Increment, BOOLEAN Wait): sets the event (releasing what waits on it)
// and resets it.
GUEST_IMPORT(KePulseEvent)
{
    auto lock = kernel::LockDispatcher();
    auto& mem = guest::g_memory;
    const uint32_t event = ctx.r3.u32;
    const int32_t previous = int32_t(mem.Read32(event + kernel::header::kSignalState));
    mem.Write32(event + kernel::header::kSignalState, 1);
    kernel::NotifyDispatcherChange(event);
    mem.Write32(event + kernel::header::kSignalState, 0);
    ctx.r3.u64 = uint32_t(previous);
}
