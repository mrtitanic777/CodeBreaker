// rex/memory.h - SHIM for the vendored DXBC translator.
//
// The real rex/memory.h pulls the guest-runtime core (rex/ppc/context.h,
// rex/system/xmemory.h -> MMIO handler, simde SIMD) into every file that only
// wants a couple of host memory helpers. The DXBC shader translator uses just
// two symbols from this header: memory::Reinterpret<> and memory::copy_and_swap.
// Both live in the lightweight rex/memory/utils.h, so this shim includes only
// that and severs the heavy runtime branch.
#pragma once
#include <rex/memory/utils.h>

// The guest Memory manager. The DXBC translator only ever holds a Memory* (it
// never executes guest code), so a forward declaration satisfies the headers
// that pass it around without dragging in the MMIO/heap runtime.
namespace rex {
namespace memory {
class Memory;
}  // namespace memory
}  // namespace rex
