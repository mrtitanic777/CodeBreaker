// Deferred procedure calls (KeInitializeDpc / KeInsertQueueDpc).
//
// KDPC (0x1C bytes): +0x00 Type (u16, 19), +0x02 SelectedCpu, +0x03 DesiredCpu, +0x04 ListEntry, +0x0C Routine,
// +0x10 Context, +0x14 Argument1, +0x18 Argument2. A queued DPC runs once, soon after insertion, at DISPATCH_LEVEL on
// a dedicated worker thread: Routine(Dpc, Context, Argument1, Argument2).

#include "kernel/kernel.h"

#include <condition_variable>
#include <deque>
#include <mutex>
#include <set>
#include <thread>

namespace
{
    constexpr uint16_t kDpcType = 19;
    constexpr uint32_t kDpcRoutine = 0x0C, kDpcContext = 0x10, kDpcArgument1 = 0x14, kDpcArgument2 = 0x18;

    std::mutex g_mutex;
    std::condition_variable g_queued;
    std::deque<uint32_t> g_queue;
    std::set<uint32_t> g_inQueue;
    bool g_workerStarted = false;

    void Worker()
    {
        kernel::ThreadParams params;
        params.stackSize = 0x10000;
        params.processType = kernel::kProcessTypeSystem; // serves whatever title runs; not ended with it
        kernel::GuestThread* thread = kernel::CreateThread(params);
        PPCContext* ctx = kernel::AttachHostThread(thread);
        auto& mem = guest::g_memory;
        for (;;)
        {
            uint32_t dpc;
            {
                std::unique_lock lock(g_mutex);
                g_queued.wait(lock, [] { return !g_queue.empty(); });
                dpc = g_queue.front();
                g_queue.pop_front();
                g_inQueue.erase(dpc);
            }
            kernel::CallGuestAtDispatchLevel(thread, *ctx, mem.Read32(dpc + kDpcRoutine),
                { dpc, mem.Read32(dpc + kDpcContext), mem.Read32(dpc + kDpcArgument1), mem.Read32(dpc + kDpcArgument2) },
                mem.Read8(dpc + 3));
        }
    }
}

// VOID KeInitializeDpc(PKDPC Dpc, PKDEFERRED_ROUTINE Routine, PVOID Context)
GUEST_IMPORT(KeInitializeDpc)
{
    auto& mem = guest::g_memory;
    const uint32_t dpc = ctx.r3.u32;
    memset(mem.Translate(dpc), 0, 0x1C);
    mem.Write16(dpc + 0x00, kDpcType);
    mem.Write32(dpc + kDpcRoutine, ctx.r4.u32);
    mem.Write32(dpc + kDpcContext, ctx.r5.u32);
}

bool kernel::QueueDpc(uint32_t dpc, uint32_t argument1, uint32_t argument2)
{
    auto& mem = guest::g_memory;
    std::lock_guard lock(g_mutex);
    if (g_inQueue.count(dpc))
        return false;
    mem.Write32(dpc + kDpcArgument1, argument1);
    mem.Write32(dpc + kDpcArgument2, argument2);
    g_inQueue.insert(dpc);
    g_queue.push_back(dpc);
    if (!g_workerStarted)
    {
        g_workerStarted = true;
        kernel::StartHostThread(Worker);
    }
    g_queued.notify_one();
    return true;
}

// BOOLEAN KeInsertQueueDpc(PKDPC Dpc, PVOID SystemArgument1, PVOID SystemArgument2)
// FALSE if the DPC is already queued (its arguments are then left unchanged), as on NT.
GUEST_IMPORT(KeInsertQueueDpc)
{
    ctx.r3.u64 = kernel::QueueDpc(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32) ? 1 : 0;
}
