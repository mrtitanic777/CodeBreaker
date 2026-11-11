// codebreaker/platforms/nes.hpp -- C++17 port of codebreaker.platforms.nes.
//
// The NES (Famicom): 6502 code (the 2A03, without decimal mode) on an iNES cartridge image.
// Its programs were written in assembly, which is what comes back (source/nes_writer.py); the
// cartridge built from that source is compared with the original, and its functions are run
// in CodeBreaker's own 6502 (emulate/m6502.py).
//
// PLATFORM() is the platforms::Platform that platforms::machine(...) makes (platforms/package.hpp), built on first use.
#pragma once

#include "py.hpp"
#include "codebreaker/platforms/package.hpp"

namespace codebreaker::platforms::nes {

bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const Platform& platform, const py::str& path);

const Platform& PLATFORM();

}  // namespace codebreaker::platforms::nes
