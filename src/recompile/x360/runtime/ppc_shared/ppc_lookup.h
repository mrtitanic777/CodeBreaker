#pragma once

// One indirect-call table for every recompiled module in the process (bootanim.xex, dash.xex, later dash plugins).
//
// XenonRecomp's default PPC_LOOKUP_FUNC puts a table after each image and indexes it from that image's code base, so
// an indirect call from one module into another (the dash calling a plugin's callback, a module calling back into
// itself from a kernel thread) would read the wrong table. Instead the runtime reserves one host table covering the
// whole 32-bit guest address space (one 8-byte slot per 4-byte guest instruction address: 32 GiB of address space,
// committed only for the code ranges of loaded modules and the runtime's dynamic exports), directly after the
// guest memory reservation, so it is reached from `base` like the original macro. A slot that is never committed
// faults; the fault handler reports it as an indirect call outside every recompiled module.
//
// This header is force-included into every module's generated code (each module's CMakeLists) and included by the
// runtime's ppc_config.h, so all of them agree. XenonRecomp's ppc_context.h only defines its own macro when this one
// is absent (native/xenonrecomp).

#include <cstdint>

// Host offset of the table from the guest memory base: after the 4 GiB guest space and the 64 KiB tail reserved past
// it (guest::kHostTailSize).
#define PPC_FUNC_TABLE_OFFSET 0x100010000ull
#define PPC_FUNC_TABLE_SIZE 0x800000000ull

#define PPC_LOOKUP_FUNC(x, y) *(PPCFunc**)((x) + PPC_FUNC_TABLE_OFFSET + uint64_t(uint32_t(y)) * 2)

// Stores the recompiler marks as MMIO (a store followed by eieio) also tell the runtime, so the GPU command processor
// can sleep until the title writes the ring buffer's write pointer instead of polling it (gpu/command_processor.cpp).
void PpcMmioStore32(uint32_t address) noexcept;
#define PPC_MM_STORE_U32(x, y) do { PPC_STORE_U32(x, y); PpcMmioStore32(uint32_t(x)); } while (0)
// mftb reads the runtime's guest clock (src/guest_clock.h: repeatable under CB_FIXED_CLOCK=1).
uint64_t PpcQueryTimebase() noexcept;
#define PPC_QUERY_TIMEBASE() PpcQueryTimebase()
