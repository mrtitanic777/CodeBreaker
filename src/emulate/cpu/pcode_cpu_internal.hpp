// emulate/cpu/pcode_cpu_internal.hpp -- what the per-CPU adapters (pcode_cpu*.cpp) share.
#pragma once

#include <array>
#include <cstddef>
#include <map>
#include <string_view>
#include <utility>

#include "decompile/engine/pcode_emulator.hh"

namespace cb::emulate::pcode_cpu {

// The one emulator of this thread for a description (languages/<description>), made once.
decompile::sleigh::Emulator& emulator_named(const char* description);

// Whether a space of the description is the code/data space the bus answers ("ram").
bool is_ram(std::string_view space);

// A table of register names as direct references, found once per emulator of this thread: reading a register by name builds a string and searches for it.
template <std::size_t N>
const std::array<decompile::sleigh::Emulator::RegRef, N>& reg_refs(const decompile::sleigh::Emulator& emu, const char* const (&names)[N]) {
    thread_local std::map<std::pair<const void*, const void*>, std::array<decompile::sleigh::Emulator::RegRef, N>> known;
    const auto [slot, fresh] = known.try_emplace({&emu, static_cast<const void*>(names)});
    if (fresh)
        for (std::size_t i = 0; i < N; ++i) slot->second[i] = emu.regRef(names[i]);
    return slot->second;
}

}  // namespace cb::emulate::pcode_cpu
