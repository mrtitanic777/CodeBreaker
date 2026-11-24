// engine/pcode_ops_tlcs900.hpp -- the Neo Geo Pocket adapter's own semantics for what the description of the TLCS-900/H leaves out (pcode_ops_tlcs900.cpp).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <vector>

#include "decompile/tlcs900.hpp"
#include "emulate/tlcs900.hpp"

namespace cb::engine::pcode_ops_tlcs900 {

// What a run keeps besides the registers: the instructions counted, the BIOS calls made, and the hook that may supply what a call leaves in the registers.
struct Host {
    std::uint64_t steps = 0;
    std::vector<emulate::tlcs900::SoftwareInterrupt> swis;
    std::function<void(emulate::tlcs900::Registers&, const emulate::tlcs900::SoftwareInterrupt&)> on_swi;
};

// Execute `insn` (decoded at regs.pc) on `regs` and the bus. nullopt: it ran and `host.steps` counts it (a block instruction counts each of its repeats). A Fault: it did not.
std::optional<emulate::tlcs900::Fault> step(Host& host, emulate::tlcs900::Registers& regs, emulate::tlcs900::Bus& bus, const decompile::tlcs900::Insn& insn);

}  // namespace cb::engine::pcode_ops_tlcs900
