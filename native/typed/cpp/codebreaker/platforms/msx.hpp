// codebreaker/platforms/msx.hpp -- C++17 port of codebreaker.platforms.msx.
//
// The MSX: Z80 code on a cartridge (.rom). Its programs were written in assembly, which is what
// comes back (source/msx_writer.py, sjasmplus / pasmo syntax); the cartridge built from that source
// is compared with the original, and its functions are run in CodeBreaker's own Z80
// (emulate/z80.py).
//
// PLATFORM() is the platforms::Platform that platforms::machine(...) makes (platforms/package.hpp), built on first use.
#pragma once

#include "py.hpp"
#include "codebreaker/platforms/package.hpp"

namespace codebreaker::platforms::msx {

bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const Platform& platform, const py::str& path);

const Platform& PLATFORM();

}  // namespace codebreaker::platforms::msx
