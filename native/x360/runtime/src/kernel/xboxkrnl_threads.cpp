// xboxkrnl thread exports: thread-local storage, time, critical regions, spin locks, thread creation and control.

#include "kernel/kernel.h"
#include "kernel/objects.h"

#include <windows.h>

#include <mutex>
#include <vector>

// ---- Thread-local storage ----------------------------------------------------------------------------------------
// KeTls* slots are per-process indices into each thread's slot array (KTHREAD.TlsAddress).

namespace
{
    std::mutex g_tlsMutex;
    std::vector<bool> g_tlsUsed;

    uint32_t CurrentKThread(const PPCContext& ctx)
    {
        return guest::g_memory.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
    }

    uint32_t CurrentTlsSlots(const PPCContext& ctx)
    {
        return guest::g_memory.Read32(CurrentKThread(ctx) + kernel::kthread::kTlsAddress);
    }
}

// DWORD KeTlsAlloc(): a free slot index, or 0xFFFFFFFF.
GUEST_IMPORT(KeTlsAlloc)
{
    std::lock_guard lock(g_tlsMutex);
    if (g_tlsUsed.empty())
        g_tlsUsed.resize(kernel::TlsSlotCount(), false);
    uint32_t index = 0xFFFFFFFF;
    for (uint32_t i = 0; i < g_tlsUsed.size(); i++)
    {
        if (!g_tlsUsed[i])
        {
            g_tlsUsed[i] = true;
            index = i;
            break;
        }
    }
    ctx.r3.u64 = index;
}

// BOOL KeTlsFree(DWORD Index): frees the slot and clears it in every thread.
GUEST_IMPORT(KeTlsFree)
{
    const uint32_t index = ctx.r3.u32;
    std::lock_guard lock(g_tlsMutex);
    if (index >= g_tlsUsed.size() || !g_tlsUsed[index])
    {
        ctx.r3.u64 = 0;
        return;
    }
    g_tlsUsed[index] = false;
    kernel::ForEachThread([&](kernel::GuestThread* t) { guest::g_memory.Write32(t->tlsSlots + index * 4, 0); });
    ctx.r3.u64 = 1;
}

// PVOID KeTlsGetValue(DWORD Index)
GUEST_IMPORT(KeTlsGetValue)
{
    const uint32_t index = ctx.r3.u32;
    ctx.r3.u64 = index < kernel::TlsSlotCount() ? guest::g_memory.Read32(CurrentTlsSlots(ctx) + index * 4) : 0;
}

// BOOL KeTlsSetValue(DWORD Index, PVOID Value)
GUEST_IMPORT(KeTlsSetValue)
{
    const uint32_t index = ctx.r3.u32;
    if (index >= kernel::TlsSlotCount())
    {
        ctx.r3.u64 = 0;
        return;
    }
    guest::g_memory.Write32(CurrentTlsSlots(ctx) + index * 4, ctx.r4.u32);
    ctx.r3.u64 = 1;
}

// ---- Time --------------------------------------------------------------------------------------------------------

// LARGE_INTEGER KeQueryPerformanceFrequency(): the console's 50 MHz time base (matches mftb / PPC_QUERY_TIMEBASE).
GUEST_IMPORT(KeQueryPerformanceFrequency)
{
    ctx.r3.u64 = 50000000;
}

// VOID KeQuerySystemTime(PLARGE_INTEGER CurrentTime): FILETIME (100 ns since 1601, UTC).
GUEST_IMPORT(KeQuerySystemTime)
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    guest::g_memory.Write64(ctx.r3.u32, uint64_t(ft.dwHighDateTime) << 32 | ft.dwLowDateTime);
}

// ULONGLONG KeQueryInterruptTime(): 100 ns units since boot, the interrupt time the clock interrupt publishes in
// KeTimeStampBundle (read from there, so both agree).
GUEST_IMPORT(KeQueryInterruptTime)
{
    ctx.r3.u64 = kernel::InterruptTime();
}

// ---- Critical regions and spin locks -----------------------------------------------------------------------------

// VOID KeEnterCriticalRegion(): disables normal kernel APCs for the current thread (KTHREAD.KernelApcDisable--).
GUEST_IMPORT(KeEnterCriticalRegion)
{
    auto& mem = guest::g_memory;
    const uint32_t field = CurrentKThread(ctx) + kernel::kthread::kApcDisableCount;
    mem.Write32(field, mem.Read32(field) - 1);
}

// VOID KeLeaveCriticalRegion()
GUEST_IMPORT(KeLeaveCriticalRegion)
{
    auto& mem = guest::g_memory;
    const uint32_t field = CurrentKThread(ctx) + kernel::kthread::kApcDisableCount;
    mem.Write32(field, mem.Read32(field) + 1);
}

namespace
{
    // The lock word holds the owner's PRCB address. Holders run at DISPATCH_LEVEL and cannot be preempted on
    // their CPU on hardware; host threads can, so a long wait yields the host thread to let the holder finish.
    void AcquireSpinLock(const PPCContext& ctx, uint32_t lock)
    {
        auto& mem = guest::g_memory;
        const uint32_t prcb = ctx.r13.u32 + kernel::pcr::kPrcb;
        for (uint32_t spins = 0; !mem.CompareExchange32(lock, 0, prcb); spins++)
        {
            if (spins < 1000)
                YieldProcessor();
            else
                SwitchToThread();
        }
    }

    void ReleaseSpinLock(uint32_t lock)
    {
        __atomic_store_n(reinterpret_cast<uint32_t*>(guest::g_memory.Translate(lock)), 0u, __ATOMIC_SEQ_CST);
    }
}

// VOID KeAcquireSpinLockAtRaisedIrql(PKSPINLOCK SpinLock)
GUEST_IMPORT(KeAcquireSpinLockAtRaisedIrql)
{
    AcquireSpinLock(ctx, ctx.r3.u32);
}

// VOID KeReleaseSpinLockFromRaisedIrql(PKSPINLOCK SpinLock)
GUEST_IMPORT(KeReleaseSpinLockFromRaisedIrql)
{
    ReleaseSpinLock(ctx.r3.u32);
}

// KIRQL KfAcquireSpinLock(PKSPINLOCK SpinLock): raises the IRQL to DISPATCH_LEVEL, then acquires.
GUEST_IMPORT(KfAcquireSpinLock)
{
    auto& mem = guest::g_memory;
    const uint32_t irql = ctx.r13.u32 + kernel::pcr::kCurrentIrql;
    const uint8_t oldIrql = mem.Read8(irql);
    mem.Write8(irql, 2); // DISPATCH_LEVEL
    AcquireSpinLock(ctx, ctx.r3.u32);
    ctx.r3.u64 = oldIrql;
}

// VOID KfReleaseSpinLock(PKSPINLOCK SpinLock, KIRQL OldIrql): releases, then lowers the IRQL to OldIrql.
// Known deviation: lowering below DISPATCH_LEVEL does not run pending DPCs on this thread; ours run on their own
// host thread and are never held back by a raised IRQL (the spin lock itself still excludes them).
GUEST_IMPORT(KfReleaseSpinLock)
{
    ReleaseSpinLock(ctx.r3.u32);
    guest::g_memory.Write8(ctx.r13.u32 + kernel::pcr::kCurrentIrql, ctx.r4.u8);
}

// ---- Threads -----------------------------------------------------------------------------------------------------

// NTSTATUS ExCreateThread(PHANDLE Handle, DWORD StackSize, PDWORD ThreadId, PVOID XapiThreadStartup,
//                         PVOID StartAddress, PVOID StartContext, DWORD CreationFlags)
// CreationFlags: bit 0 = CREATE_SUSPENDED; bits 24+ = preferred hardware thread.
GUEST_IMPORT(ExCreateThread)
{
    auto& mem = guest::g_memory;
    kernel::ThreadParams params;
    params.stackSize = ctx.r4.u32 ? ctx.r4.u32 : kernel::CurrentThread()->params.stackSize;
    params.xapiThreadStartup = ctx.r6.u32;
    params.startAddress = ctx.r7.u32;
    params.startContext = ctx.r8.u32;
    params.creationFlags = ctx.r9.u32;
    kernel::GuestThread* thread = kernel::CreateThread(params);
    // The top byte is an affinity mask (AffinityCpu0 = 0x01000000 ... AffinityCpu5 = 0x20000000; callers build it as
    // (1 << cpu) << 24), not a CPU number.
    if (const uint32_t mask = (params.creationFlags >> 24) & 0x3F)
        thread->affinity = mask;
    // CreationFlags 0x80: return the KTHREAD pointer instead of a handle (the kernel's own callers, XAM).
    if (ctx.r3.u32)
        mem.Write32(ctx.r3.u32, (params.creationFlags & 0x80) ? thread->threadObject : kernel::CreateHandle(thread->threadObject));
    if (ctx.r5.u32)
        mem.Write32(ctx.r5.u32, thread->id);
    if (!(params.creationFlags & 1))
        kernel::StartThread(thread);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS ExTerminateThread(DWORD ExitStatus): ends the calling thread (unwinds to its host entry).
GUEST_IMPORT(ExTerminateThread)
{
    throw kernel::ThreadExit{ ctx.r3.u32 };
}

// NTSTATUS NtResumeThread(HANDLE Thread, PDWORD PreviousSuspendCount)
GUEST_IMPORT(NtResumeThread)
{
    auto& mem = guest::g_memory;
    const uint32_t object = kernel::LookupHandle(ctx.r3.u32);
    kernel::GuestThread* thread = object ? kernel::FindThread(object) : nullptr;
    if (thread == nullptr)
    {
        ctx.r3.u64 = kernel::kStatusInvalidHandle;
        return;
    }
    const uint8_t previous = mem.Read8(object + kernel::kthread::kSuspendCount);
    if (previous > 1)
        Fatal("NtResumeThread on a thread suspended {} times: nested suspension is not implemented", previous);
    if (previous == 1)
    {
        mem.Write8(object + kernel::kthread::kSuspendCount, 0);
        kernel::StartThread(thread);
    }
    if (ctx.r4.u32)
        mem.Write32(ctx.r4.u32, previous);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS KeSetAffinityThread(PKTHREAD Thread, KAFFINITY Affinity, PKAFFINITY PreviousAffinity)
// Unlike Windows NT the console returns a status and writes the previous affinity through the pointer.
GUEST_IMPORT(KeSetAffinityThread)
{
    kernel::GuestThread* thread = kernel::FindThread(ctx.r3.u32);
    if (ctx.r4.u32 == 0 || thread == nullptr)
    {
        ctx.r3.u64 = kernel::kStatusInvalidParameter;
        return;
    }
    if (ctx.r5.u32)
        guest::g_memory.Write32(ctx.r5.u32, thread->affinity);
    thread->affinity = ctx.r4.u32;
    // A thread outside its new mask moves to the mask's first hardware thread.
    if (const uint32_t mask = ctx.r4.u32 & 0x3F; mask && !((mask >> kernel::CurrentCpu(thread)) & 1))
        kernel::SetCurrentCpu(thread, uint8_t(__builtin_ctz(mask)));
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// LONG KeQueryBasePriorityThread(PKTHREAD Thread)
GUEST_IMPORT(KeQueryBasePriorityThread)
{
    ctx.r3.u64 = uint32_t(int32_t(int8_t(guest::g_memory.Read8(ctx.r3.u32 + kernel::kthread::kBasePriority))));
}

// LONG KeSetBasePriorityThread(PKTHREAD Thread, LONG Increment): returns the previous base priority.
// Host scheduling priority is not changed (the host has no equivalent of the console's fixed hardware threads).
GUEST_IMPORT(KeSetBasePriorityThread)
{
    auto& mem = guest::g_memory;
    const uint32_t thread = ctx.r3.u32;
    const int8_t previous = int8_t(mem.Read8(thread + kernel::kthread::kBasePriority));
    mem.Write8(thread + kernel::kthread::kBasePriority, uint8_t(int8_t(ctx.r4.u32)));
    ctx.r3.u64 = uint32_t(int32_t(previous));
}

// KPRIORITY KeSetPriorityThread(PKTHREAD Thread, KPRIORITY Priority): the thread's current priority (KTHREAD +0x70),
// returning the previous one (the older dashboards raise their threads with it). Host scheduling is not changed, as
// for KeSetBasePriorityThread.
GUEST_IMPORT(KeSetPriorityThread)
{
    auto& mem = guest::g_memory;
    const uint32_t thread = ctx.r3.u32;
    const int8_t previous = int8_t(mem.Read8(thread + kernel::kthread::kPriority));
    mem.Write8(thread + kernel::kthread::kPriority, uint8_t(int8_t(ctx.r4.u32)));
    ctx.r3.u64 = uint32_t(int32_t(previous));
}

// VOID KeEnableFpuExceptions(BOOLEAN Enable): records the flag on the thread. Host FP exceptions stay masked.
GUEST_IMPORT(KeEnableFpuExceptions)
{
    guest::g_memory.Write8(CurrentKThread(ctx) + kernel::kthread::kFpuExceptionsOn, ctx.r3.u32 != 0 ? 1 : 0);
}

// ---- IRQL --------------------------------------------------------------------------------------------------------

// KIRQL KeRaiseIrqlToDpcLevel(void)
GUEST_IMPORT(KeRaiseIrqlToDpcLevel)
{
    auto& mem = guest::g_memory;
    const uint32_t irql = ctx.r13.u32 + kernel::pcr::kCurrentIrql;
    const uint8_t oldIrql = mem.Read8(irql);
    if (oldIrql > 2)
        Fatal("KeRaiseIrqlToDpcLevel at IRQL {} (would lower it; the kernel bug-checks)", oldIrql);
    mem.Write8(irql, 2); // DISPATCH_LEVEL
    ctx.r3.u64 = oldIrql;
}

// KIRQL KfRaiseIrql(KIRQL NewIrql): the previous IRQL (xam.xex's network stack raises to DISPATCH_LEVEL).
GUEST_IMPORT(KfRaiseIrql)
{
    auto& mem = guest::g_memory;
    const uint32_t irql = ctx.r13.u32 + kernel::pcr::kCurrentIrql;
    const uint8_t oldIrql = mem.Read8(irql);
    if (ctx.r3.u8 < oldIrql)
        Fatal("KfRaiseIrql to {} from IRQL {} (would lower it; the kernel bug-checks)", ctx.r3.u8, oldIrql);
    mem.Write8(irql, ctx.r3.u8);
    ctx.r3.u64 = oldIrql;
}

// VOID KfLowerIrql(KIRQL NewIrql)
// Same known deviation as KfReleaseSpinLock: pending DPCs run on their own host thread, not when the IRQL drops.
GUEST_IMPORT(KfLowerIrql)
{
    auto& mem = guest::g_memory;
    const uint32_t irql = ctx.r13.u32 + kernel::pcr::kCurrentIrql;
    if (ctx.r3.u8 > mem.Read8(irql))
        Fatal("KfLowerIrql to {} from IRQL {} (would raise it; the kernel bug-checks)", ctx.r3.u8, mem.Read8(irql));
    mem.Write8(irql, ctx.r3.u8);
}

// LONG KeResumeThread(PKTHREAD Thread): returns the previous suspend count.
GUEST_IMPORT(KeResumeThread)
{
    auto& mem = guest::g_memory;
    const uint32_t object = ctx.r3.u32;
    kernel::GuestThread* thread = kernel::FindThread(object);
    if (thread == nullptr)
        Fatal("KeResumeThread({:08X}): not a thread object", object);
    const uint8_t previous = mem.Read8(object + kernel::kthread::kSuspendCount);
    if (previous > 1)
        Fatal("KeResumeThread on a thread suspended {} times: nested suspension is not implemented", previous);
    if (previous == 1)
    {
        mem.Write8(object + kernel::kthread::kSuspendCount, 0);
        kernel::StartThread(thread);
    }
    ctx.r3.u64 = previous;
}
