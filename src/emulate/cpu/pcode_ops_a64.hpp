// emulate/cpu/pcode_ops_a64.hpp -- the AArch64 description's named operations (NEON_*, SVE_*, ...) as host code over engine/pcode_vector.hpp.
#pragma once

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "emulate/cpu/pcode_ops.hpp"

namespace cb::emulate::pcode_ops {

// The table of the operations written so far (built once).
const Table& a64_table();

// Bind a64_table() to `emu` (an AARCH64.sla emulator): the operations of the description it serves now run by number.
Binding bind_a64(Emu& emu);

// Record the name of every bound operation that runs on this thread into `*into` (null: stop recording): what the tests use to say which operations a run reached.
void trace_a64(std::vector<std::string_view>* into);

// The instruction word about to run, for the few operations whose arguments do not tell a scalar form from a vector one (call before each step of an AARCH64.sla emulator).
void set_a64_word(std::uint32_t word);

// Count the calls of every operation the table serves, on every thread (on: the counts start again from zero), and read the counts: which named operations a program really runs.
void count_a64_operations(bool on);
std::map<std::string, std::uint64_t> a64_operation_counts();

// For each operation of the description that the table does not serve, the shapes of the calls it got on this thread ("result bytes:argument bytes=value ...").
std::map<std::string, std::set<std::string>> a64_recorded_shapes();

// FMAX, FMIN, FMAXNM, FMINNM and their pairwise and across forms give the results the lane's own core gives when an operand is a NaN (the host's: the default NaN, or the other operand),
// which differs from the manual (a NaN operand is the result, quieted). On by default; the tests that check the operations against Unicorn turn it off.
void set_a64_lane_compat(bool on);

// The operations of the description that are named for the architecture and that a64_table() does not serve (a name that is not of the NEON, SVE or crypto groups -- the
// barriers, cache and TLB operations, hints -- is not listed: the adapter owns them).
std::vector<std::string> a64_unserved(Emu& emu);

}  // namespace cb::emulate::pcode_ops
