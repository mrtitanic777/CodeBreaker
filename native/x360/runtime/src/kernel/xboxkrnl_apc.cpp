// Asynchronous procedure calls (KeInitializeApc / KeInsertQueueApc).
//
// KAPC (0x28 bytes; the dash allocates 0x28 and reads the thread back from +4): +0x00 Type (18), +0x01 ApcMode,
// +0x02 Inserted, +0x04 Thread (KTHREAD), +0x08 ApcListEntry, +0x10 KernelRoutine, +0x14 RundownRoutine,
// +0x18 NormalRoutine, +0x1C NormalContext, +0x20 SystemArgument1, +0x24 SystemArgument2 (Xenia's X_KAPC).
//
// User-mode APCs (ApcMode 1, what the dash queues: its XCAM overlapped completions and a UI thread call) run at the
// target thread's next alertable user-mode wait, as XAM's completion routines do (objects.cpp DeliverUserApcs): the
// KernelRoutine first, with pointers to the normal routine, its context and the two system arguments, then the
// NormalRoutine if the kernel routine left one. Kernel-mode APCs interrupt the thread wherever it is; nothing here can
// do that, so they stop with the details.

#include "kernel/objects.h"

using namespace kernel;

namespace
{
    constexpr uint8_t kApcType = 18;
    constexpr uint8_t kUserMode = 1;
    constexpr uint32_t kApcMode = 0x01, kApcInserted = 0x02, kApcThread = 0x04, kApcKernelRoutine = 0x10,
                       kApcRundownRoutine = 0x14, kApcNormalRoutine = 0x18, kApcNormalContext = 0x1C,
                       kApcArgument1 = 0x20, kApcArgument2 = 0x24, kApcSize = 0x28;
}

// VOID KeInitializeApc(PRKAPC Apc, PRKTHREAD Thread, PKKERNEL_ROUTINE KernelRoutine, PKRUNDOWN_ROUTINE RundownRoutine,
//                      PKNORMAL_ROUTINE NormalRoutine, KPROCESSOR_MODE ApcMode, PVOID NormalContext)
// Without a normal routine the APC is a kernel-mode one with no context (NT's rule).
GUEST_IMPORT(KeInitializeApc)
{
    auto& mem = guest::g_memory;
    const uint32_t apc = ctx.r3.u32;
    const uint32_t normal = ctx.r7.u32;
    memset(mem.Translate(apc), 0, kApcSize);
    mem.Write8(apc + 0, kApcType);
    mem.Write8(apc + kApcMode, normal ? uint8_t(ctx.r8.u32) : 0);
    mem.Write32(apc + kApcThread, ctx.r4.u32);
    mem.Write32(apc + kApcKernelRoutine, ctx.r5.u32);
    mem.Write32(apc + kApcRundownRoutine, ctx.r6.u32);
    mem.Write32(apc + kApcNormalRoutine, normal);
    mem.Write32(apc + kApcNormalContext, normal ? ctx.r9.u32 : 0);
}

// BOOLEAN KeInsertQueueApc(PRKAPC Apc, PVOID SystemArgument1, PVOID SystemArgument2, KPRIORITY Increment)
// FALSE if it is already queued or its thread is gone.
GUEST_IMPORT(KeInsertQueueApc)
{
    auto& mem = guest::g_memory;
    const uint32_t apc = ctx.r3.u32;
    if (mem.Read8(apc + kApcInserted))
    {
        ctx.r3.u64 = 0;
        return;
    }
    const uint32_t threadObject = mem.Read32(apc + kApcThread);
    GuestThread* thread = FindThread(threadObject);
    if (thread == nullptr || thread->finished)
    {
        ctx.r3.u64 = 0;
        return;
    }
    if (mem.Read8(apc + kApcMode) != kUserMode)
        Fatal("KeInsertQueueApc({:08X}): kernel-mode APC (kernel routine {:08X}, normal {:08X}) for thread {}: "
              "not implemented (it would interrupt the thread)", apc, mem.Read32(apc + kApcKernelRoutine),
              mem.Read32(apc + kApcNormalRoutine), thread->id);
    mem.Write32(apc + kApcArgument1, ctx.r4.u32);
    mem.Write32(apc + kApcArgument2, ctx.r5.u32);
    mem.Write8(apc + kApcInserted, 1);
    UserApc queued;
    queued.routine = mem.Read32(apc + kApcNormalRoutine);
    queued.context = mem.Read32(apc + kApcNormalContext);
    queued.argument1 = ctx.r4.u32;
    queued.argument2 = ctx.r5.u32;
    queued.kernelRoutine = mem.Read32(apc + kApcKernelRoutine);
    queued.apc = apc;
    QueueUserApc(thread, queued);
    ctx.r3.u64 = 1;
}
