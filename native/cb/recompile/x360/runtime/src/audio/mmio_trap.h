#pragma once

#include <cstdint>

// Write traps for MMIO register pages that the title accesses with ordinary loads and stores (the recompiled code
// has no hook there). The page is kept read-only on the host: a guest store faults, the faulting instruction is
// completed with the page writable under the trap flag, and the handler then sees every 32-bit register the store
// wrote (and any other word that changed meanwhile). Reads need no trap: the runtime keeps register values in the
// page itself. Handlers run on the storing thread, before the store "returns", one at a time per page.
namespace audio
{
    // value: the register's 32 bits as guest memory holds them, read big-endian (a byte-reversed guest store of v
    // shows up as bswap(v)).
    using MmioWriteHandler = void (*)(uint32_t guestAddress, uint32_t value);

    void InstallMmioWriteTrap(uint32_t guestPage, MmioWriteHandler handler);
    // Writes a register from the runtime (inside a handler, or at any other time) without triggering the trap.
    void WriteMmio(uint32_t guestAddress, uint32_t value);
}
