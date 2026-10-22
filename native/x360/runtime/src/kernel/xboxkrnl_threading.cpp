// xboxkrnl threading exports (Ke*/Ex*/Nt* thread and process state).

#include "kernel/kernel.h"

// UCHAR KeGetCurrentProcessType(): the current thread's process type, or the PCR's DPC value while a DPC is active.
GUEST_IMPORT(KeGetCurrentProcessType)
{
    auto& mem = guest::g_memory;
    const uint32_t pcr = ctx.r13.u32;
    if (mem.Read32(pcr + kernel::pcr::kDpcActive) == 0)
        ctx.r3.u64 = mem.Read8(mem.Read32(pcr + kernel::pcr::kCurrentThread) + kernel::kthread::kProcessType);
    else
        ctx.r3.u64 = mem.Read8(pcr + kernel::pcr::kDpcProcessType);
}

// VOID KeSetCurrentProcessType(UCHAR Type): only takes effect for the DPC process type while a DPC is active.
GUEST_IMPORT(KeSetCurrentProcessType)
{
    auto& mem = guest::g_memory;
    const uint32_t pcr = ctx.r13.u32;
    if (mem.Read32(pcr + kernel::pcr::kDpcActive) != 0)
        mem.Write8(pcr + kernel::pcr::kDpcProcessType, uint8_t(ctx.r3.u32));
}
