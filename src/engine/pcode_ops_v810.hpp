// engine/pcode_ops_v810.hpp -- the Virtual Boy adapter's own semantics for what the description of the V810 leaves out (pcode_ops_v810.cpp).
#pragma once

#include <cstdint>
#include <optional>

#include "decompile/v810.hpp"
#include "emulate/v810.hpp"

namespace cb::engine::pcode_ops_v810 {

// Execute the instruction at g.pc on the registers `g` and the bus, as the processor does; `steps` counts it. nullopt: it ran. A Fault: it did not (nothing changed).
std::optional<emulate::v810::Fault> step(emulate::v810::Registers& g, emulate::v810::Bus& bus, std::uint64_t& steps);
// The same for an instruction the caller has fetched and decoded (nullopt: the bits are no instruction): the bus is not read for it again.
std::optional<emulate::v810::Fault> step(emulate::v810::Registers& g, emulate::v810::Bus& bus, std::uint64_t& steps, const std::optional<decompile::v810::Insn>& decoded);

}  // namespace cb::engine::pcode_ops_v810
