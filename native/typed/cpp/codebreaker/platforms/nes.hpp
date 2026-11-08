// codebreaker/platforms/nes.hpp -- C++17 port of codebreaker.platforms.nes.
//
// The NES (Famicom): 6502 code (the 2A03, without decimal mode) on an iNES cartridge image.
// Its programs were written in assembly, which is what comes back (source/nes_writer.py); the
// cartridge built from that source is compared with the original, and its functions are run
// in CodeBreaker's own 6502 (emulate/m6502.py).
//
// The Python builds PLATFORM with platforms.machine(...). The platform record types are the ones
// platforms/msx.hpp holds (platforms has no typed port of its own yet); they carry the same
// values the Python's slot objects do.
#pragma once

#include "py.hpp"
#include "codebreaker/platforms/msx.hpp"

namespace codebreaker::platforms::nes {

using msx::Cpu;
using msx::Platform;
using msx::Ref;

bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const Platform& platform, const py::str& path);

extern const Platform PLATFORM;

}  // namespace codebreaker::platforms::nes
