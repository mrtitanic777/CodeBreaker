// engine/pcode_cpu_internal.hpp -- what the per-CPU adapters (pcode_cpu*.cpp) share.
#pragma once

#include <string_view>

#include "decompile/engine/pcode_emulator.hh"

namespace cb::engine::pcode_cpu {

// The one emulator of this thread for a description (languages/<description>), made once.
decompile::sleigh::Emulator& emulator_named(const char* description);

// Whether a space of the description is the code/data space the bus answers ("ram").
bool is_ram(std::string_view space);

}  // namespace cb::engine::pcode_cpu
