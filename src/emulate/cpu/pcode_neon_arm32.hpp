// emulate/cpu/pcode_neon_arm32.hpp -- the Advanced SIMD data-processing instructions and the core register transfers of 32-bit ARM (decompile/arm32_neon_dp.hpp) run over the lane
// library (engine/pcode_vector.hpp), for the one emulator's adapter (engine/pcode_cpu_arm.cpp). The ARMv7 descriptions name most of these as user operations with no behavior
// and leave out what an accumulating, inserting or saturating form needs (the destination as an input, the saturation of the result), so the adapter reads the registers the
// instruction names, runs it here and writes the result back: the one place the behavior is written for the emulator.
#pragma once

#include <cstdint>

#include "decompile/arm32_neon_dp.hpp"

namespace cb::emulate::pcode_neon_arm32 {

// The D registers (d0-d31, least significant byte first as they are held) and the core registers a transfer reads or writes.
struct Regs {
    std::uint64_t d[32] = {};
    std::uint32_t r[16] = {};
};

// The D registers the instruction may read or write (bit i: d<i>): the adapter loads these into Regs, runs, and stores back the ones that changed.
std::uint32_t reads(const decompile::arm::t2::NeonDp& op);

// Run the instruction on `regs`: the D registers it names (and r[op.rt] for a transfer). The result is the bits to OR into the floating point status register: QC (bit 27) when
// a result saturated, and the cumulative exception flags the lanes raised (IOC, DZC, OFC, UFC, IXC, IDC).
std::uint32_t run(const decompile::arm::t2::NeonDp& op, Regs& regs);

}  // namespace cb::emulate::pcode_neon_arm32
