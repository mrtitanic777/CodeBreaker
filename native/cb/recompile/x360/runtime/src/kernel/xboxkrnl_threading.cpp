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

// Interlocked singly linked lists. SLIST_HEADER is 8 bytes, big-endian: Next (u32), Depth (u16), Sequence (u16); a list
// entry's first word is its Next. Titles inline the push and pop in most places (lwarx/stwcx on the header), so these
// serve the ones that import them (Catherine's demo pops 100,000 times in its first minute). The header is changed
// with one 64-bit compare-and-swap, as the console does with ldarx/stdcx.
namespace
{
    struct SListHeader
    {
        uint32_t next;
        uint16_t depth;
        uint16_t sequence;
    };

    uint64_t* SListWord(uint32_t header)
    {
        return reinterpret_cast<uint64_t*>(guest::g_memory.Translate(header));
    }

    SListHeader SListDecode(uint64_t raw)
    {
        const uint64_t v = __builtin_bswap64(raw);
        return { uint32_t(v >> 32), uint16_t(v >> 16), uint16_t(v) };
    }

    uint64_t SListEncode(const SListHeader& h)
    {
        return __builtin_bswap64(uint64_t(h.next) << 32 | uint64_t(h.depth) << 16 | h.sequence);
    }

    // Replaces the header with what `change` makes of it; returns the header it replaced.
    template <typename Change>
    SListHeader SListUpdate(uint32_t header, Change change)
    {
        uint64_t* word = SListWord(header);
        uint64_t old = __atomic_load_n(word, __ATOMIC_ACQUIRE);
        for (;;)
        {
            const SListHeader before = SListDecode(old);
            const uint64_t desired = SListEncode(change(before));
            if (desired == old || __atomic_compare_exchange_n(word, &old, desired, false, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE))
                return before;
        }
    }
}

// PSLIST_ENTRY InterlockedPushEntrySList(PSLIST_HEADER ListHead, PSLIST_ENTRY ListEntry): the previous first entry.
GUEST_IMPORT(InterlockedPushEntrySList)
{
    const uint32_t entry = ctx.r4.u32;
    const SListHeader before = SListUpdate(ctx.r3.u32, [&](SListHeader h) {
        guest::g_memory.Write32(entry, h.next);
        return SListHeader{ entry, uint16_t(h.depth + 1), uint16_t(h.sequence + 1) };
    });
    ctx.r3.u64 = before.next;
}

// PSLIST_ENTRY InterlockedPopEntrySList(PSLIST_HEADER ListHead): the first entry, 0 for an empty list.
GUEST_IMPORT(InterlockedPopEntrySList)
{
    const SListHeader before = SListUpdate(ctx.r3.u32, [&](SListHeader h) {
        if (h.next == 0)
            return h;
        return SListHeader{ guest::g_memory.Read32(h.next), uint16_t(h.depth - 1), h.sequence };
    });
    ctx.r3.u64 = before.next;
}

// PSLIST_ENTRY InterlockedFlushSList(PSLIST_HEADER ListHead): the whole chain, leaving the list empty.
GUEST_IMPORT(InterlockedFlushSList)
{
    const SListHeader before = SListUpdate(ctx.r3.u32, [](SListHeader) { return SListHeader{ 0, 0, 0 }; });
    ctx.r3.u64 = before.next;
}

// USHORT QueryDepthSList(PSLIST_HEADER ListHead)
GUEST_IMPORT(QueryDepthSList)
{
    ctx.r3.u64 = SListDecode(__atomic_load_n(SListWord(ctx.r3.u32), __ATOMIC_ACQUIRE)).depth;
}

// KeLockL2 / KeUnlockL2: lock part of the L2 cache for the title's own use (a streaming buffer the GPU or another core
// reads). The host's caches are coherent and need no locking: a lock is granted, an unlock has nothing to undo.
GUEST_IMPORT(KeLockL2)
{
    ctx.r3.u64 = 0;
}

GUEST_IMPORT(KeUnlockL2)
{
    ctx.r3.u64 = 0;
}
