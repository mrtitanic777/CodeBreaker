// emulate/cpu/pcode_cpu_a64_stops.hpp -- the table of the instructions the Switch lane stops at, as it has always stopped at them: the lane's own semantics for the encodings its first
// interpreter (emulate/a64) never ran. The adapter (engine/pcode_cpu_a64) ends the run at one of these with the reason below, and the tests keep it equal to the interpreter's.
#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "decompile/a64.hpp"

namespace cb::emulate::pcode_cpu::a64stops {

// Why the lane stops at the decoded instruction, up to " at <pc>" (the reason the run reports is that followed by the address); nullopt when it runs it.
std::optional<std::string> stop_reason(const decompile::a64::Insn& insn);

}  // namespace cb::emulate::pcode_cpu::a64stops
